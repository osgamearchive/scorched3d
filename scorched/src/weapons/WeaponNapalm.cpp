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

#include <weapons/WeaponNapalm.h>
#include <actions/ShotProjectileNapalm.h>

REGISTER_ACCESSORY_SOURCE(WeaponNapalm);

WeaponNapalm::WeaponNapalm()
{

}

WeaponNapalm::~WeaponNapalm()
{

}

bool WeaponNapalm::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	XMLNode *timeNode = accessoryNode->getNamedChild("napalmtime");
	if (!timeNode)
	{
		dialogMessage("Accessory",
			"Failed to find napalmtime node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	napalmTime_ = (float) atof(timeNode->getContent());

	XMLNode *heightNode = accessoryNode->getNamedChild("napalmheight");
	if (!heightNode)
	{
		dialogMessage("Accessory",
			"Failed to find napalmheight node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	napalmHeight_ = (float) atof(heightNode->getContent());

	XMLNode *stepNode = accessoryNode->getNamedChild("steptime");
	if (!stepNode)
	{
		dialogMessage("Accessory",
			"Failed to find steptime node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	stepTime_ = (float) atof(stepNode->getContent());

	XMLNode *hurtStepNode = accessoryNode->getNamedChild("hurtsteptime");
	if (!hurtStepNode)
	{
		dialogMessage("Accessory",
			"Failed to find hurtsteptime node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	hurtStepTime_ = (float) atof(hurtStepNode->getContent());

	XMLNode *hurtPerSecondNode = accessoryNode->getNamedChild("hurtpersecond");
	if (!hurtPerSecondNode)
	{
		dialogMessage("Accessory",
			"Failed to find hurtpersecond node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	hurtPerSecond_ = (float) atof(hurtPerSecondNode->getContent());

	XMLNode *streamsNode = accessoryNode->getNamedChild("numberstreams");
	if (!streamsNode)
	{
		dialogMessage("Accessory",
			"Failed to find numberstreams node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	numberStreams_ = atoi(streamsNode->getContent());	

	XMLNode *effectNode = accessoryNode->getNamedChild("effectradius");
	if (!effectNode)
	{
		dialogMessage("Accessory",
			"Failed to find effectradius node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	effectRadius_ = atoi(effectNode->getContent());

	return true;
}

bool WeaponNapalm::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(napalmTime_);
	buffer.addToBuffer(napalmHeight_);
	buffer.addToBuffer(stepTime_);
	buffer.addToBuffer(hurtStepTime_);
	buffer.addToBuffer(hurtPerSecond_);
	buffer.addToBuffer(numberStreams_);
	buffer.addToBuffer(effectRadius_);
	return true;
}

bool WeaponNapalm::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(napalmTime_)) return false;
	if (!reader.getFromBuffer(napalmHeight_)) return false;
	if (!reader.getFromBuffer(stepTime_)) return false;
	if (!reader.getFromBuffer(hurtStepTime_)) return false;
	if (!reader.getFromBuffer(hurtPerSecond_)) return false;
	if (!reader.getFromBuffer(numberStreams_)) return false;
	if (!reader.getFromBuffer(effectRadius_)) return false;
	return true;
}

Action *WeaponNapalm::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new ShotProjectileNapalm(
		position, 
		velocity,
		this, playerId);

	return action;
}
