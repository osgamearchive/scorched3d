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

#include <sound/SoundBuffer.h>
#include <sound/Sound.h>
#include <al/al.h>
#include <al/alut.h>

SoundBuffer::SoundBuffer() : 
	error_(0), buffer_(0)
{
}

SoundBuffer::~SoundBuffer()
{
	destroyBuffer();
}

void SoundBuffer::destroyBuffer()
{
	if (buffer_) alGenBuffers(1, &buffer_);
	buffer_ = 0;
}

bool SoundBuffer::createBuffer(char *wavFileName)
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
	alutLoadWAVFile(wavFileName,&format,&data,&size,&freq,&loop);
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

	return true;
}
