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

#include <weapons/WeaponMulti.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>

REGISTER_ACCESSORY_SOURCE(WeaponMulti);

WeaponMulti::WeaponMulti()
{

}

WeaponMulti::~WeaponMulti()
{

}

bool WeaponMulti::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	for (int i=1;;i++)
	{
		// Get the next weapon
		char buffer[128];
		sprintf(buffer, "subweapon%i", i);
		XMLNode *subNode = 0;
		accessoryNode->getNamedChild(buffer, subNode, false);
		if (!subNode) break;
		
		// Check next weapon is correct type
		Accessory *accessory = 
			ScorchedServer::instance()->getAccessoryStore().createAccessory(subNode);
		if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
		{
			dialogMessage("Accessory",
						  "Sub weapon of wrong type \"%s\"",
						  name_.c_str());
			return false;
		}
		subWeapons_.push_back((Weapon*) accessory);
	}

	return true;
}

bool WeaponMulti::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer((int) subWeapons_.size());
	std::list<Weapon *>::iterator itor;
	for (itor = subWeapons_.begin();
		 itor != subWeapons_.end();
		 itor++)
	{
		Weapon *weapon = *itor;
		if (!Weapon::write(buffer, weapon)) return false;
	}
	return true;
}

bool WeaponMulti::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	int size = 0;
	if (!reader.getFromBuffer(size)) return false;
	for (int i=0; i<size; i++)
	{
		Weapon *weapon = Weapon::read(reader); 
		if (!weapon) return false;
		subWeapons_.push_back(weapon);
	}

	return true;
}

void WeaponMulti::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity)
{
	std::list<Weapon *>::iterator itor;
	for (itor = subWeapons_.begin();
		 itor != subWeapons_.end();
		 itor++)
	{
		Weapon *weapon = *itor;
		weapon->fireWeapon(context, playerId, position, velocity);
	}
}
