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

#include <weapons/WeaponProjectile.h>
#include <actions/ShotProjectileExplosion.h>
#include <tank/TankContainer.h>

REGISTER_ACCESSORY_SOURCE(WeaponProjectile);

WeaponProjectile::WeaponProjectile() : size_(0)
{

}

WeaponProjectile::~WeaponProjectile()
{

}

bool WeaponProjectile::parseXML(XMLNode *accessoryNode)
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

	return true;
}

bool WeaponProjectile::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(size_);
	return true;
}

bool WeaponProjectile::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	return true;
}

Action *WeaponProjectile::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Action *action = new ShotProjectileExplosion(
			position, 
			velocity,
			this, playerId, (float) size_);

		return action;
	}

	return 0;
}

const char *WeaponProjectile::getFiredSound()
{
	if (size_ < 5)
	{
		return PKGDIR "data/wav/shoot/small.wav";
	}
	else if (size_ < 10)
	{
		return PKGDIR "data/wav/shoot/medium.wav";
	}
	else if (size_ < 15)
	{
		return PKGDIR "data/wav/shoot/large.wav";
	}
	return PKGDIR "data/wav/shoot/xlarge.wav";
}

const char *WeaponProjectile::getExplosionSound()
{
	if (size_ < 5)
	{
		return PKGDIR "data/wav/explosions/small.wav";
	}
	else if (size_ < 10)
	{
		return PKGDIR "data/wav/explosions/medium.wav";
	}
	else if (size_ < 15)
	{
		return PKGDIR "data/wav/explosions/large.wav";
	}
	return PKGDIR "data/wav/explosions/xlarge.wav";
}
