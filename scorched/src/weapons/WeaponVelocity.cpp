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

#include <weapons/WeaponVelocity.h>
#include <weapons/AccessoryStore.h>

REGISTER_ACCESSORY_SOURCE(WeaponVelocity);

WeaponVelocity::WeaponVelocity() :
	velocityChange_(0.0f)
{

}

WeaponVelocity::~WeaponVelocity()
{

}

bool WeaponVelocity::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("aimedweapon", subNode)) return false;

	// Check next weapon is correct type
	Accessory *accessory = store_->createAccessory(subNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	aimedWeapon_ = (Weapon*) accessory;

	if (!accessoryNode->getNamedChild("velocitychange", velocityChange_)) return false;

	return true;
}

bool WeaponVelocity::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!store_->writeWeapon(buffer, aimedWeapon_)) return false;
	buffer.addToBuffer(velocityChange_);
	return true;
}

bool WeaponVelocity::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	aimedWeapon_ = store_->readWeapon(reader); if (!aimedWeapon_) return false;
	if (!reader.getFromBuffer(velocityChange_)) return false;
	return true;
}

void WeaponVelocity::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	// Add a shot that will fall where the original was aimed
	// but with altered velocity
	Vector newVelocity = velocity * velocityChange_;
	aimedWeapon_->fireWeapon(context, playerId, position, newVelocity, data);
}

