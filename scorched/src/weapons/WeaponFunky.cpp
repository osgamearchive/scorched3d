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

#include <weapons/WeaponFunky.h>
#include <actions/ShotProjectileFunky.h>

REGISTER_ACCESSORY_SOURCE(WeaponFunky);

WeaponFunky::WeaponFunky() :
	size_(0), subsize_(0), warHeads_(0)
{

}

WeaponFunky::~WeaponFunky()
{

}

bool WeaponFunky::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory size
	XMLNode *sizeNode = accessoryNode->getNamedChild("size");
	if (!sizeNode)
	{
		dialogMessage("Accessory",
			"Failed to find size node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	size_ = atoi(sizeNode->getContent());

	// Get the accessory size
	XMLNode *subsizeNode = accessoryNode->getNamedChild("subsize");
	if (!subsizeNode)
	{
		dialogMessage("Accessory",
			"Failed to find subsizeNode node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	subsize_ = atoi(subsizeNode->getContent());

	// Get the accessory warheads
	XMLNode *warheadsNode = accessoryNode->getNamedChild("nowarheads");
	if (!warheadsNode)
	{
		dialogMessage("Accessory",
			"Failed to find nowarheads node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	warHeads_ = atoi(warheadsNode->getContent());

	return true;
}

bool WeaponFunky::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(size_);
	buffer.addToBuffer(subsize_);
	buffer.addToBuffer(warHeads_);
	return true;
}

bool WeaponFunky::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(subsize_)) return false;
	if (!reader.getFromBuffer(warHeads_)) return false;
	return true;
}

Action *WeaponFunky::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new ShotProjectileFunky(
		position, 
		velocity,
		this, playerId, (float) size_, (float) subsize_, warHeads_);
	return action;
}
