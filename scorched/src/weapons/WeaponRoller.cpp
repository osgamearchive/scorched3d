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

#include <weapons/WeaponRoller.h>
#include <actions/ShotProjectileRoller.h>

REGISTER_ACCESSORY_SOURCE(WeaponRoller);

WeaponRoller::WeaponRoller()
{

}

WeaponRoller::~WeaponRoller()
{

}

bool WeaponRoller::parseXML(XMLNode *accessoryNode)
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

	XMLNode *timeNode = accessoryNode->getNamedChild("rollertime");
	if (!timeNode)
	{
		dialogMessage("Accessory",
			"Failed to find rollertime node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	rollerTime_ = (float) atof(timeNode->getContent());

	XMLNode *heightNode = accessoryNode->getNamedChild("rollerheight");
	if (!heightNode)
	{
		dialogMessage("Accessory",
			"Failed to find rollerheight node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	rollerHeight_ = (float) atof(heightNode->getContent());

	XMLNode *stepNode = accessoryNode->getNamedChild("steptime");
	if (!stepNode)
	{
		dialogMessage("Accessory",
			"Failed to find steptime node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	stepTime_ = (float) atof(stepNode->getContent());

	XMLNode *rollersNode = accessoryNode->getNamedChild("numberrollers");
	if (!rollersNode)
	{
		dialogMessage("Accessory",
			"Failed to find numberrollers node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	numberRollers_ = atoi(rollersNode->getContent());	

	return true;
}

bool WeaponRoller::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(rollerTime_);
	buffer.addToBuffer(rollerHeight_);
	buffer.addToBuffer(stepTime_);
	buffer.addToBuffer(numberRollers_);
	buffer.addToBuffer(size_);
	return true;
}

bool WeaponRoller::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(rollerTime_)) return false;
	if (!reader.getFromBuffer(rollerHeight_)) return false;
	if (!reader.getFromBuffer(stepTime_)) return false;
	if (!reader.getFromBuffer(numberRollers_)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	return true;
}

Action *WeaponRoller::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new ShotProjectileRoller(
		position, 
		velocity,
		this, 
		playerId);

	return action;
}
