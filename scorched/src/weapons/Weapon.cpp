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

#include <weapons/Weapon.h>
#include <weapons/AccessoryStore.h>

Weapon::Weapon() : deathAnimationWeight_(0)
{

}

Weapon::~Weapon()
{

}

Accessory::AccessoryType Weapon::getType()
{
	return Accessory::AccessoryWeapon;
}

bool Weapon::parseXML(XMLNode *accessoryNode)
{
	if (!Accessory::parseXML(accessoryNode)) return false;

	// Get the accessory name
	XMLNode *deathWeight = accessoryNode->getNamedChild("deathanimationweight");
	if (deathWeight)
	{
		deathAnimationWeight_ = atoi(deathWeight->getContent());
	}

	return true;
}

bool Weapon::write(NetBuffer &buffer, Weapon *weapon)
{
	buffer.addToBuffer(weapon->getName());
	return true;
}

Weapon *Weapon::read(NetBufferReader &reader)
{
	std::string weapon;
	if (!reader.getFromBuffer(weapon)) return 0;
	Accessory *accessory = 
		AccessoryStore::instance()->findByAccessoryName(weapon.c_str());
	if (!accessory || (accessory->getType() != Accessory::AccessoryWeapon)) 
	{
		return 0;
	}
	return ((Weapon *) accessory);
}

const char *Weapon::getExplosionTexture()
{
	return "exp00";
}

const char *Weapon::getFiredSound()
{
	return 0;
}

const char *Weapon::getExplosionSound()
{
	return 0;
}
