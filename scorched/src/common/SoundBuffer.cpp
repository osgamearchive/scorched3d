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

#include <common/SoundBuffer.h>

SoundBuffer::SoundBuffer() : chunk_(NULL) , repeats_(0) , channel_(-1)
{

}

SoundBuffer::~SoundBuffer()
{
	destroyBuffer();
}

void SoundBuffer::destroyBuffer()
{
	stop();
	if (chunk_) Mix_FreeChunk(chunk_);
	chunk_ = NULL;
	channel_ = -1;
	repeats_ = 0;
}

bool SoundBuffer::createBuffer(char *wavFileName)
{
	chunk_ = Mix_LoadWAV(wavFileName);
	return (chunk_ != NULL);
}

bool SoundBuffer::play()
{
	if (!chunk_) return false;

	if (repeats_ == -1)
	{
		// If we are set to repeat, ensure that this sound
		// is not already playing.
		stop();
		// If it was playing we would loose the channel number
		// and would be unable to stop it.
	}

	channel_=Mix_PlayChannel(-1,chunk_,repeats_);
	return (channel_<0 ? false : true );
}

bool SoundBuffer::stop()
{
	if ((!chunk_) || (channel_<0)) return false;
	if (Mix_HaltChannel(channel_) < 0) return false;

	channel_ = -1;
	return true;
}

bool SoundBuffer::setRepeats()
{
	if (!chunk_) return false;

	repeats_=-1;
	return true;
}
