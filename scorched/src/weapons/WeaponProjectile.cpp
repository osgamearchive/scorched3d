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

REGISTER_ACCESSORY_SOURCE(WeaponProjectile);

WeaponProjectile::WeaponProjectile() : size_(0), under_(false), multiColored_(false)
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

	// Get the accessory under
	XMLNode *underNode = accessoryNode->getNamedChild("under");
	if (underNode) under_ = true;

	// Get the accessory colored
	XMLNode *colorNode = accessoryNode->getNamedChild("multicolor");
	if (colorNode) multiColored_ = true;

	return true;
}

bool WeaponProjectile::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(size_);
	buffer.addToBuffer(under_);
	return true;
}

bool WeaponProjectile::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(size_)) return false;
	if (!reader.getFromBuffer(under_)) return false;
	return true;
}

Vector &WeaponProjectile::getExplosionColor()
{
	if (!multiColored_) return Weapon::getExplosionColor();

	static Vector red(1.0f, 0.0f, 0.0f);
	static Vector green(0.0f, 1.0f, 0.0f);
	static Vector blue(0.0f, 0.0f, 1.0f);
	static Vector yellow(1.0f, 1.0f, 0.0f);

	int color = int(RAND * 4.0f);
	switch (color)
	{
	case 0:
		return red;
	case 1:
		return green;
	case 2:
		return blue;
	case 3:
		return yellow;
	}
	return Weapon::getExplosionColor();
}

Action *WeaponProjectile::fireWeapon(unsigned int playerId, Vector &position, Vector &velocity)
{
	Action *action = new ShotProjectileExplosion(
		position, 
		velocity,
		this, playerId, (float) size_, 0, under_);

	return action;
}
