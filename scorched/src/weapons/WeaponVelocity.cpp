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
	XMLNode *subNode = accessoryNode->getNamedChild("aimedweapon", false, true);
	if (!subNode)
	{
		dialogMessage("Accessory",
			"Failed to find aimedweapon node in accessory \"%s\"",
			name_.c_str());
		return false;
	}

	// Check next weapon is correct type
	Accessory *accessory = AccessoryStore::instance()->createAccessory(subNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	aimedWeapon_ = (Weapon*) accessory;

	XMLNode *velocityNode = accessoryNode->getNamedChild("velocitychange", false, true);
	if (!velocityNode)
	{
		dialogMessage("Accessory",
			"Failed to find velocitychange node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	velocityChange_ = (float) atof(velocityNode->getContent());

	return true;
}

bool WeaponVelocity::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, aimedWeapon_)) return false;
	buffer.addToBuffer(velocityChange_);
	return true;
}

bool WeaponVelocity::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	aimedWeapon_ = Weapon::read(reader); if (!aimedWeapon_) return false;
	if (!reader.getFromBuffer(velocityChange_)) return false;
	return true;
}

void WeaponVelocity::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	// Add a shot that will fall where the original was aimed
	// but with altered velocity
	Vector newVelocity = velocity * velocityChange_;
	aimedWeapon_->fireWeapon(context, playerId, position, newVelocity);
}

