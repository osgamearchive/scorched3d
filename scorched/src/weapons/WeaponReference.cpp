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

#include <weapons/WeaponReference.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>

REGISTER_ACCESSORY_SOURCE(WeaponReference);

WeaponReference::WeaponReference()
{

}

WeaponReference::~WeaponReference()
{

}

bool WeaponReference::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the next weapon
	std::string subNode;
	if (!accessoryNode->getNamedChild("weapon", subNode)) return false;

	// Check next weapon is correct type
	Accessory *accessory = 
		ScorchedServer::instance()->getAccessoryStore().findByPrimaryAccessoryName(subNode.c_str());
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Failed to find weapon/wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	refWeapon_ = (Weapon*) accessory;

	return true;
}

bool WeaponReference::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, refWeapon_)) return false;
	return true;
}

bool WeaponReference::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	refWeapon_ = Weapon::read(reader); if (!refWeapon_) return false;
	return true;
}

void WeaponReference::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	refWeapon_->fireWeapon(context, playerId, position, velocity, data);
}

