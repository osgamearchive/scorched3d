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

#include <weapons/WeaponTracer.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileTracer.h>

REGISTER_ACCESSORY_SOURCE(WeaponTracer);

WeaponTracer::WeaponTracer() : showShotPath_(false)
{

}

WeaponTracer::~WeaponTracer()
{

}

bool WeaponTracer::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory smoke
	XMLNode *smokeNode = accessoryNode->getNamedChild("smoke");
	if (!smokeNode)
	{
		dialogMessage("Accessory",
			"Failed to find smoke node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	showShotPath_ = (strcmp(smokeNode->getContent(), "true") == 0);

	return true;
}

bool WeaponTracer::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(showShotPath_);
	return true;
}

bool WeaponTracer::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(showShotPath_)) return false;
	return true;
}

Action *WeaponTracer::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Action *action = new ShotProjectileTracer(
			position, 
			velocity,
			this, playerId, showShotPath_);
		return action;
	}

	return 0;
}
