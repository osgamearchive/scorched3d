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


// Sound.cpp: implementation of the Sound class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Defines.h>
#include <common/Sound.h>
#include <common/SoundBuffer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Sound *Sound::instance_ = 0;

Sound *Sound::instance()
{
	if (!instance_)
	{
		instance_ = new Sound;
	}

	return instance_;
}

Sound::Sound() : init_(false)
{

}

Sound::~Sound()
{
	if (init_)
	{
		std::list<SoundBuffer *>::iterator itor;
		for (itor = buffers_.begin();
			itor != buffers_.end();
			itor++)
		{
			SoundBuffer *buffer = (*itor);
			delete buffer;
		}		
		Mix_CloseAudio();
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
	if ( Mix_OpenAudio(11025, // frequency 
		AUDIO_S16SYS, // format (16 bit)
		1, // channels (mono) 
		512) // mixing buffer
		< 0 ) 
	{
		dialogMessage(SDL_GetError(), 
			"Warning: Couldn't set 11025 Hz 16-bit mono audio");
	}
	else
	{
		init_ = true;
	}

	Mix_AllocateChannels(channels);

	return init_;
}

void Sound::setVolume(int volume)
{
	if (init_) Mix_Volume(-1, volume);
}

int Sound::getVolume()
{
	if (init_) return Mix_Volume(-1, -1);
	return 0;
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
		delete buffer;
		return 0;
	}

	buffers_.push_back(buffer);
	return buffer;
}

