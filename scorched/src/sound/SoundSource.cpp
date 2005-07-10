////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <sound/SoundSource.h>
#include <sound/SoundBuffer.h>
#include <AL/al.h>

SoundSource::SoundSource() : source_(0)
{
}

SoundSource::~SoundSource()
{
	stop();
	destroy();
}

bool SoundSource::getPlaying()
{
	if (!source_) return false;
	ALint state = AL_STOPPED;
	alGetSourcei(source_, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void SoundSource::setRelative(bool relative)
{
	if (!source_) return;
	alSourcei(source_, AL_SOURCE_RELATIVE, (relative?AL_TRUE:AL_FALSE));
}

void SoundSource::setPosition(Vector &position)
{
	if (!source_) return;
	alSourcefv(source_, AL_POSITION, position);
}

void SoundSource::setVelocity(Vector &velocity)
{
	if (!source_) return;
	alSourcefv(source_, AL_VELOCITY, velocity);
}

void SoundSource::setGain(float gain)
{
	if (!source_) return;
	alSourcef(source_, AL_GAIN, gain);
}

void SoundSource::play(SoundBuffer *buffer, bool repeat)
{
	if (!buffer || !source_ || !buffer->getBuffer()) return;

	stop();
    alSourcei(source_, AL_BUFFER, buffer->getBuffer());
	alSourcei(source_, AL_LOOPING, (repeat?AL_TRUE:AL_FALSE));
	alSourcePlay(source_);
}

void SoundSource::stop()
{
	if (!source_) return;
	alSourceStop(source_);
}

void SoundSource::destroy()
{
	if (!source_) return;
	alDeleteSources(1, &source_);
}

bool SoundSource::create()
{
	unsigned int error;

	// Generate Sources
	alGetError();
	alGenSources(1, &source_);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		return false;
	}

	alSourcef(source_, AL_REFERENCE_DISTANCE, 75.0f);
	return true;
}
