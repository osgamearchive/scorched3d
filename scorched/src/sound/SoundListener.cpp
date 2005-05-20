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

#include <sound/SoundListener.h>
#include <sound/Sound.h>
#include <al/al.h>

SoundListener::SoundListener()
{
}

SoundListener::~SoundListener()
{
}

void SoundListener::setPosition(Vector &position)
{
	if (!Sound::instance()->getInit()) return;

	alListenerfv(AL_POSITION, position);
}

void SoundListener::setVelocity(Vector &velocity)
{
	if (!Sound::instance()->getInit()) return;

	alListenerfv(AL_VELOCITY, velocity);
}

void SoundListener::setOrientation(Vector &o)
{
	if (!Sound::instance()->getInit()) return;

	float orientation[6];
	orientation[0] = o[0];
	orientation[1] = o[1];
	orientation[2] = o[2];
	orientation[3] = 0.0f;
	orientation[4] = 0.0f;
	orientation[5] = 1.0f;

	alListenerfv(AL_ORIENTATION, orientation);
}

void SoundListener::setGain(float gain)
{
	if (!Sound::instance()->getInit()) return;

	alListenerf(AL_GAIN, gain);
}
