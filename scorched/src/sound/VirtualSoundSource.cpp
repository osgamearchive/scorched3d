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

#include <sound/VirtualSoundSource.h>
#include <sound/Sound.h>

VirtualSoundSource::VirtualSoundSource(
	unsigned int priority, bool looping, bool managed) : 
	actualSource_(0),
	priority_(priority),
	gain_(1.0f),
	buffer_(0),
	relative_(false), 
	managed_(managed),
	looping_(looping)
{
	DIALOG_ASSERT(!(managed_ && looping_));
	if (managed_) Sound::instance()->addManaged(this);
}

VirtualSoundSource::~VirtualSoundSource()
{
	stop();
}

void VirtualSoundSource::play(SoundBuffer *buffer)
{
	if (!actualSource_)
	{
		actualSource_ = Sound::instance()->addPlaying(this, priority_);
	}
	if (actualSource_)
	{
		actualSource_->setGain(gain_);
		actualSource_->setRelative(relative_);
		actualSource_->setPosition(position_);
		actualSource_->setVelocity(velocity_);
		actualSource_->play(buffer, looping_);
		buffer_ = buffer;
	}
}

void VirtualSoundSource::simulate()
{
	if (actualSource_)
	{
		actualSource_->simulate(looping_);
	}
}

void VirtualSoundSource::stop()
{
	Sound::instance()->removePlaying(this, priority_);
	if (actualSource_)
	{
		actualSource_->stop();
		actualSource_ = 0;
	}
}

SoundSource *VirtualSoundSource::getSource()
{
	return actualSource_;
}

unsigned int VirtualSoundSource::getPriority()
{
	return priority_;
}

SoundBuffer *VirtualSoundSource::getBuffer()
{
	return buffer_;
}

bool VirtualSoundSource::getLooping()
{
	return looping_;
}

bool VirtualSoundSource::getPlaying()
{
	if (actualSource_) return actualSource_->getPlaying();
	return false;
}

bool VirtualSoundSource::getManaged()
{
	return managed_;
}

void VirtualSoundSource::setRelative()
{
	relative_ = true;
	if (actualSource_) actualSource_->setRelative(true);
}

void VirtualSoundSource::setPosition(Vector &position)
{
	position_ = position;
	if (actualSource_) actualSource_->setPosition(position);
}

void VirtualSoundSource::setVelocity(Vector &velocity)
{
	velocity_ = velocity;
	if (actualSource_) actualSource_->setVelocity(velocity);
}

void VirtualSoundSource::setGain(float gain)
{
	gain_ = gain;
	if (actualSource_) actualSource_->setGain(gain);
}
