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

#include <weapons/WeaponLeapFrog.h>
#include <weapons/AccessoryStore.h>
#include <actions/ShotProjectileLeapFrog.h>

REGISTER_ACCESSORY_SOURCE(WeaponLeapFrog);

WeaponLeapFrog::WeaponLeapFrog():  size_(0)
{

}

WeaponLeapFrog::~WeaponLeapFrog()
{

}

bool WeaponLeapFrog::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get this weapon size
	XMLNode *sizeNode = accessoryNode->getNamedChild("size");
	if (!sizeNode)
	{
		dialogMessage("Accessory",
			"Failed to find size node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	size_ = atoi(sizeNode->getContent());

	// Get the next weapon
	XMLNode *subNode = accessoryNode->getNamedChild("subweapon");
	if (!subNode)
	{
		dialogMessage("Accessory",
			"Failed to find subweapon node in accessory \"%s\"",
			name_.c_str());
		return false;
	}

	// Check next weapon is correct type
	Accessory *accessory = AccessoryStore::instance()->createAccessory(subNode);
	if (accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	subWeapon_ = (Weapon*) accessory;

	return true;
}

bool WeaponLeapFrog::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(size_);
	if (!Weapon::write(buffer, subWeapon_)) return false;
	return true;
}

bool WeaponLeapFrog::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	subWeapon_ = Weapon::read(reader); if (!subWeapon_) return false;
	return true;
}

Action *WeaponLeapFrog::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new ShotProjectileLeapFrog(
		position, 
		velocity,
		this, playerId, (float) size_);

	return action;
}
