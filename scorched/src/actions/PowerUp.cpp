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

#include <actions/PowerUp.h>
#include <weapons/AccessoryStore.h>

REGISTER_ACTION_SOURCE(PowerUp);

PowerUp::PowerUp()
{
}

PowerUp::PowerUp(unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data, WeaponPowerUp *powerUp) :
	position_(position),
	velocity_(velocity),
	playerId_(playerId),
	data_(data),
	powerUp_(powerUp)
{

}

PowerUp::~PowerUp()
{
}

void PowerUp::init()
{
}

void PowerUp::simulate(float frameTime, bool &remove)
{
	powerUp_->invokePowerUp(*context_, playerId_, position_, velocity_, data_);

	remove = true;
	Action::simulate(frameTime, remove);
}

bool PowerUp::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(velocity_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	context_->accessoryStore->writeWeapon(buffer, powerUp_);
	return true;
}

bool PowerUp::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(velocity_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	powerUp_ = (WeaponPowerUp *) context_->accessoryStore->readWeapon(reader); 
	if (!powerUp_) return false;
	return true;
}
