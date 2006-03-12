////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <GLEXT/GLConsole.h>
#include <GLEXT/GLConsoleRuleMethodIAdapter.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <common/Logger.h>
#include <sound/Sound.h>
#include <sound/SoundBufferFactory.h>
#include <AL/al.h>
#include <AL/alc.h>

Sound *Sound::instance_ = 0;

Sound *Sound::instance()
{
	if (!instance_)
	{
		instance_ = new Sound;
	}

	return instance_;
}

Sound::Sound() : 
	init_(false), totalTime_(0.0f)
{
	new GLConsoleRuleMethodIAdapter<Sound>(
		this, &Sound::showSoundBuffers, "SoundBuffers");
}

Sound::~Sound()
{
	if (init_)
	{
		{
			SourceList::iterator itor;
			for (itor = totalSources_.begin();
				itor != totalSources_.end();
				itor++)
			{
				SoundSource *source = (*itor);
				delete source;
			}
		}
		{
			BufferMap::iterator itor;
			for (itor = bufferMap_.begin();
				itor != bufferMap_.end();
				itor++)
			{
				SoundBuffer *buffer = (*itor).second;
				delete buffer;
			}
		}

		ALCcontext *context = alcGetCurrentContext();
		ALCdevice *device = alcGetContextsDevice(context);
        alcDestroyContext(context);
        alcCloseDevice(device);
	}
	init_ = false;
}

void Sound::destroy()
{
	delete this;
	instance_ = 0;
}

static char *checkString(char *x) 
{
	return (char *)(x?x:"null");
}

bool Sound::init(int channels)
{
	ALCdevice *soundDevice = alcOpenDevice(0);
	if (!soundDevice)
	{
		dialogExit("Scorched3D", "Failed to open sound device");
		return false;
	}

	// Setting the frequency seems to cause screeching and
	// loss of stereo under linux!!
	/*int attrlist[] = 
	{ 
		ALC_FREQUENCY, 11025,
		ALC_INVALID
	};*/
	ALCcontext *soundContext = alcCreateContext(soundDevice, 0);
	if (!soundContext)
	{
		dialogExit("Scorched3D", "Failed to create sound context");
		return false;
	}

	alcMakeContextCurrent(soundContext); 
	alDistanceModel(AL_INVERSE_DISTANCE);

	GLConsole::instance()->addLine(false, "AL_VENDOR:");
	GLConsole::instance()->addLine(false, 
		checkString((char *) alGetString(AL_VENDOR)));
	GLConsole::instance()->addLine(false, "AL_VERSION:");
	GLConsole::instance()->addLine(false,
		checkString((char *) alGetString(AL_VERSION)));
	GLConsole::instance()->addLine(false, "AL_RENDERER:");
	GLConsole::instance()->addLine(false, 
		checkString((char *) alGetString(AL_RENDERER)));
	GLConsole::instance()->addLine(false, "AL_EXTENSIONS:");
	GLConsole::instance()->addLine(false,
		checkString((char *) alGetString(AL_EXTENSIONS)));
	GLConsole::instance()->addLine(false, "ALC_DEVICE_SPECIFIER:");
	GLConsole::instance()->addLine(false,
		checkString((char *) alcGetString(soundDevice, ALC_DEVICE_SPECIFIER)));

	// Create all sound channels
	for (int i=1; i<=OptionsDisplay::instance()->getSoundChannels(); i++)
	{
		SoundSource *source = new SoundSource;
		if (!source->create())
		{
			dialogExit("Scorched3D", 
				formatString("Failed to create sound channel number %i", i));
			return false;
		}
		totalSources_.push_back(source);
		availableSources_.push_back(source);
	}

	init_ = true;
	return init_;
}

void Sound::showSoundBuffers()
{
	// Show some debug of the current playing sounds
	int i = 1;
	Logger::log(formatString("%i sounds playing, %i sources free",
		getPlayingChannels(),
		getAvailableChannels()));
	VirtualSourceMap::iterator itor;
	for (itor = playingSources_.begin();
		itor != playingSources_.end();
		itor++, i++)
	{
		unsigned int p = (*itor).first;
		VirtualSoundSource *source = (*itor).second;
		Logger::log(formatString("%i,%u - %s:%s",
			i, p, 
			(source->getPlaying()?"Playing":"Stopped"),
			source->getBuffer()->getFileName()));
	}
}

void Sound::simulate(const unsigned state, float frameTime)
{
	// Simulate all the current sources
	// This is only applicable for streams
	VirtualSourceMap::iterator playingitor;
	for (playingitor = playingSources_.begin();
		playingitor != playingSources_.end();
		playingitor++)
	{
		VirtualSoundSource *source = (*playingitor).second;
		if (source->getPlaying())
		{
			source->simulate();
		}
	}

	// Check the buffers every so often
	totalTime_ += frameTime;
	if (totalTime_ < 0.2f) return;
	totalTime_ = 0.0f;

	tidyBuffers();
}

void Sound::tidyBuffers()
{
	// Check any sources that are looping and should be restarted
	// Playing a stopped source will remove an available source
	while (!loopingSources_.empty() &&
		!availableSources_.empty())
	{
		VirtualSoundSource *stopedSource = loopingSources_.back();
		loopingSources_.pop_back();
		DIALOG_ASSERT(stopedSource->getBuffer());
		stopedSource->play(stopedSource->getBuffer());
	}

	// Tidy any playing sources that have stopped playing
	// This is the list of sources that are currently actualy playing
	static VirtualSourceList finishedList;
	VirtualSourceMap::iterator playingitor;
	for (playingitor = playingSources_.begin();
		playingitor != playingSources_.end();
		playingitor++)
	{
		VirtualSoundSource *source = (*playingitor).second;
		if (!source->getPlaying())
		{
			finishedList.push_back(source);
		}
	}
	while (!finishedList.empty())
	{
		VirtualSoundSource *source = finishedList.back();
		finishedList.pop_back();
		source->stop();
	}

	// Tidy any managed sources that have stopped playing
	// Managed sources are virtualsources that are not kept by the user
	// and should be deleted if they stop playing
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		VirtualSourceList::iterator manitor;
		for (manitor = managedSources_.begin();
			manitor != managedSources_.end();
			manitor++)
		{
			VirtualSoundSource *source = (*manitor);
			if (!source->getPlaying())
			{
				managedSources_.erase(manitor);
				delete source;
				repeat = true;
				break;
			}
		}
	}
}

SoundSource *Sound::addPlaying(VirtualSoundSource *virt, unsigned int priority)
{
	// If there are no available sources and there are some sounds playing
	// check if we can re-use a playing sound with a lower priority than us
	if (!playingSources_.empty() && 
		availableSources_.empty())
	{
		VirtualSourceMap::iterator itor = 
			playingSources_.begin();
		VirtualSoundSource *playingSource = 
			(*itor).second;
		if (playingSource->getPriority() < priority)
		{
			playingSource->stop();
			if (playingSource->getLooping())
			{
				loopingSources_.push_back(playingSource);
			}
		}
	}

	// Now check to see if there is an available channel
	// (or perhaps we just freed one up)
	if (!availableSources_.empty())
	{
		SoundSource *source = availableSources_.back();
		availableSources_.pop_back();

		std::pair<unsigned int, VirtualSoundSource *> p(priority, virt);
		playingSources_.insert(p);
		return source;
	}

	return 0;
}

void Sound::removePlaying(VirtualSoundSource *virt, unsigned int priority)
{
	// Remove the virtual source from the list of playing
	std::pair<VirtualSourceMap::iterator,  VirtualSourceMap::iterator> p =
		playingSources_.equal_range(priority);
	VirtualSourceMap::iterator itor;
	for (itor = p.first; itor != p.second; itor++)
	{
		VirtualSoundSource *v = (*itor).second;
		if (v == virt)
		{
			playingSources_.erase(itor);
			break;
		}
	}

	// Remove from looping list
	if (virt->getLooping())
	{
		VirtualSourceList::iterator itor;
		for (itor = loopingSources_.begin();
			itor != loopingSources_.end();
			itor++)
		{
			if (*itor == virt)
			{
				loopingSources_.erase(itor);
				break;
			}
		}
	}

	// Return the source to the list of available
	if (virt->getSource())
	{
		availableSources_.push_back(virt->getSource());
	}
}

void Sound::addManaged(VirtualSoundSource *source)
{
	managedSources_.push_back(source);
}

int Sound::getAvailableChannels()
{
	return availableSources_.size();
}

int Sound::getPlayingChannels()
{
	return playingSources_.size();
}

SoundListener *Sound::getDefaultListener()
{
	return &listener_;
}

SoundBuffer *Sound::createBuffer(char *fileName)
{
	// Return a buffer full of sound :)
	SoundBuffer *buffer = SoundBufferFactory::createBuffer(
		(const char *) fileName);
	if (!buffer)
	{
		dialogExit("Failed to load sound",
			formatString("\"%s\"", fileName));

		delete buffer;
		return 0;
	}
	return buffer;
}

SoundBuffer *Sound::fetchOrCreateBuffer(char *fn)
{
	std::string filename(fn);
	BufferMap::iterator itor = bufferMap_.find(filename);
	if (itor != bufferMap_.end())
	{
		return (*itor).second;
	}

	SoundBuffer *buffer = createBuffer(fn);
	bufferMap_[filename] = buffer;
	return buffer;
}
