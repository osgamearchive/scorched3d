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

#include <weapons/WeaponDigger.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileHog.h>

REGISTER_ACCESSORY_SOURCE(WeaponDigger);

WeaponDigger::WeaponDigger() : warHeads_(0)
{

}

WeaponDigger::~WeaponDigger()
{

}

bool WeaponDigger::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory size
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

bool WeaponDigger::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(warHeads_);
	return true;
}

bool WeaponDigger::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(warHeads_)) return false;
	return true;
}

Action *WeaponDigger::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Action *action = new ShotProjectileHog(
			position, 
			velocity,
			this, playerId, warHeads_, false);
		return action;
	}

	return 0;
}

const char *WeaponDigger::getFiredSound()
{
	// TODO
	return PKGDIR "data/wav/shoot/flare.wav";
}

const char *WeaponDigger::getExplosionTexture()
{
	return "exp03";
}
