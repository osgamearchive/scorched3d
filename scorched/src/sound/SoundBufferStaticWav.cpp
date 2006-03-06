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

#include <sound/SoundBufferStaticWav.h>
#include <sound/Sound.h>
#include <AL/al.h>
#include <AL/alut.h>

SoundBufferStaticWav::SoundBufferStaticWav() : 
	buffer_(0)
{
}

SoundBufferStaticWav::~SoundBufferStaticWav()
{
	destroyBuffer();
}

void SoundBufferStaticWav::play(unsigned int source, bool repeat)
{
	if (!buffer_) return;

    alSourcei(source, AL_BUFFER, buffer_);
	alSourcei(source, AL_LOOPING, (repeat?AL_TRUE:AL_FALSE));
	alSourcePlay(source);
}

void SoundBufferStaticWav::stop(unsigned int source)
{
	if (!buffer_) return;

	alSourceStop(source);
}

void SoundBufferStaticWav::destroyBuffer()
{
	if (buffer_) alDeleteBuffers (1, &buffer_);
	buffer_ = 0;
}

bool SoundBufferStaticWav::createBuffer(const char *wavFileName)
{
	// Create a buffer
	alGetError();
	alGenBuffers(1, &buffer_);
	if ((error_ = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	// Load WAV
	void *data;
	ALenum format;
	ALsizei size;
	ALsizei freq;
	ALboolean loop;

#ifdef __DARWIN__
	alutLoadWAVFile((ALbyte*) wavFileName,&format,&data,&size,&freq);
#else
	alutLoadWAVFile((ALbyte*) wavFileName,&format,&data,&size,&freq,&loop);
#endif

	if ((error_ = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	// Load WAV into buffer
	alBufferData(buffer_,format,data,size,freq);
	if ((error_ = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	// Delete WAV memory
	alutUnloadWAV(format,data,size,freq);
	if ((error_ = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	fileName_ = wavFileName;
	return true;
}
