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

#include <weapons/WeaponMirv.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileMirv.h>

REGISTER_ACCESSORY_SOURCE(WeaponMirv);

WeaponMirv::WeaponMirv() :
	noWarheads_(0), spread_(false)
{

}

WeaponMirv::~WeaponMirv()
{

}

bool WeaponMirv::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory spread
	XMLNode *spreadNode = accessoryNode->getNamedChild("spread");
	if (!spreadNode)
	{
		dialogMessage("Accessory",
			"Failed to find spread node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	spread_ = (strcmp(spreadNode->getContent(), "true") == 0);

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

	// Get the accessory warheads
	XMLNode *warheadsNode = accessoryNode->getNamedChild("nowarheads");
	if (!warheadsNode)
	{
		dialogMessage("Accessory",
			"Failed to find nowarheads node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	noWarheads_ = atoi(warheadsNode->getContent());

	return true;
}

bool WeaponMirv::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(spread_);
	buffer.addToBuffer(size_);
	buffer.addToBuffer(noWarheads_);
	return true;
}

bool WeaponMirv::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(spread_)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(noWarheads_)) return false;
	return true;
}

Action *WeaponMirv::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Action *action = new ShotProjectileMirv(
			position, 
			velocity,
			this, playerId, (float) size_, noWarheads_, spread_);

		return action;
	}

	return 0;
}

const char *WeaponMirv::getExplosionTexture()
{
	return "exp05";
}

const char *WeaponMirv::getFiredSound()
{
	return PKGDIR "data/wav/shoot/large.wav";
}

const char *WeaponMirv::getExplosionSound()
{
	return PKGDIR "data/wav/explosions/large.wav";
}
