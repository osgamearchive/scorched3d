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

#include <actions/CallbackWeapon.h>
#include <weapons/AccessoryStore.h>

REGISTER_ACTION_SOURCE(CallbackWeapon);

CallbackWeapon::CallbackWeapon() :
	totalTime_(0.0f)
{
}

CallbackWeapon::CallbackWeapon(
	WeaponCallback *callback,
	float delay, unsigned int callbackData,
	unsigned int playerId, Vector &position, Vector &velocity,
    unsigned int data) :
	callback_(callback),
	delay_(delay),
	callbackData_(callbackData),
	position_(position),
	velocity_(velocity),
	playerId_(playerId),
	data_(data),
	totalTime_(0.0f)
{

}

CallbackWeapon::~CallbackWeapon()
{
}

void CallbackWeapon::init()
{
}

void CallbackWeapon::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (totalTime_ > delay_)
	{
		callback_->weaponCallback(
			*context_, playerId_, position_, velocity_, data_, callbackData_);
		remove = true;
	}

	Action::simulate(frameTime, remove);
}

bool CallbackWeapon::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(velocity_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(data_);
	buffer.addToBuffer(callbackData_);
	buffer.addToBuffer(delay_);
	context_->accessoryStore->writeWeapon(buffer, callback_);
	return true;
}

bool CallbackWeapon::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(velocity_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	if (!reader.getFromBuffer(callbackData_)) return false;
	if (!reader.getFromBuffer(delay_)) return false;
	callback_ = (WeaponCallback *) context_->accessoryStore->readWeapon(reader); 
	if (!callback_) return false;
	return true;
}
