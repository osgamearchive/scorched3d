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

#include <actions/ShotDelay.h>
#include <engine/ScorchedContext.h>
#include <weapons/Weapon.h>

REGISTER_ACTION_SOURCE(ShotDelay);

ShotDelay::ShotDelay() : totalTime_(0.0f)
{
}

ShotDelay::ShotDelay(unsigned int playerId,
	Weapon *weapon,
	Vector &velocity,
	Vector &position,
	float delay,
	unsigned int data) :
	totalTime_(0.0f), 
	playerId_(playerId),
	weapon_(weapon),
	velocity_(velocity),
	position_(position),
	delay_(delay),
	data_(data)
{

}

ShotDelay::~ShotDelay()
{
}

void ShotDelay::init()
{

}

void ShotDelay::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (totalTime_ > delay_)
	{
		weapon_->fireWeapon(*context_, playerId_, position_, velocity_, data_);
		remove = true;
	}
	Action::simulate(frameTime, remove);
}

bool ShotDelay::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(delay_);
	buffer.addToBuffer(velocity_);
	buffer.addToBuffer(position_);
	buffer.addToBuffer(data_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool ShotDelay::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(delay_)) return false;
	if (!reader.getFromBuffer(velocity_)) return false;
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}

