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

#include <common/Defines.h>
#include <sound/Sound.h>
#include <sound/SoundBuffer.h>
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
	init_(false), defaultSource_(0)
{
}

Sound::~Sound()
{
	if (init_)
	{
		delete defaultSource_;
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
		{
			SourceList::iterator itor;
			for (itor = managedSources_.begin();
				itor != managedSources_.end();
				itor++)
			{
				SoundSource *source = (*itor);
				delete source;
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

bool Sound::init(int channels)
{
	const char *deviceName = 0;
	ALCdevice *soundDevice = alcOpenDevice((const ALubyte *) deviceName);
	if (!soundDevice)
	{
		dialogExit("Failed to find sound device %s",
			(deviceName?deviceName:"Null"));
		return false;
	}

	ALCcontext *soundContext = alcCreateContext(soundDevice, 0);
	if (!soundContext)
	{
		dialogExit("Scorched3D", "Failed to create sound context");
		return false;
	}

	alcMakeContextCurrent(soundContext); 
	alDistanceModel(AL_INVERSE_DISTANCE);

	init_ = true;
	return init_;
}

void Sound::checkManaged()
{
	if (managedSources_.empty()) return;

	SourceList::iterator itor;
	bool check = true;
	while (check)
	{
		check = false;
		for (itor = managedSources_.begin();
			itor != managedSources_.end();
			itor++)
		{
			SoundSource *source = (*itor);
			if (!source->getPlaying())
			{
				delete source;
				managedSources_.erase(itor);
				check = true;
				break;
			}
		}
	}
}

void Sound::manageSource(SoundSource *source)
{
	managedSources_.push_back(source);
}

SoundSource *Sound::createSource()
{
	checkManaged();

	SoundSource *source = new SoundSource;
	if (init_) source->create();
	return source;
}

SoundListener *Sound::getDefaultListener()
{
	return &listener_;
}

SoundSource *Sound::getDefaultSource()
{
	if (!defaultSource_)
	{
		defaultSource_ = createSource();
		defaultSource_->setRelative();
	}
	return defaultSource_;
}

SoundBuffer *Sound::createBuffer(char *fileName)
{
	// If sound is not init return an empty buffer
	// This will happen if the user turns sound off
	if (!init_) return new SoundBuffer;

	// Return a buffer full of sound :)
	SoundBuffer *buffer = new SoundBuffer;
	if (!buffer->createBuffer(fileName))
	{
		dialogExit("Failed to load sound",
			"%u:\"%s\"", buffer->getError(), fileName);

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
