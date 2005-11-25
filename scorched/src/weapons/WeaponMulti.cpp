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
#include <common/Defines.h>

REGISTER_ACCESSORY_SOURCE(WeaponMulti);

WeaponMulti::WeaponMulti()
{

}

WeaponMulti::~WeaponMulti()
{

}

bool WeaponMulti::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	for (int i=1;;i++)
	{
		// Get the next weapon
		char buffer[128];
		snprintf(buffer, 128, "subweapon%i", i);
		XMLNode *subNode = 0;
		accessoryNode->getNamedChild(buffer, subNode, false);
		if (!subNode) break;
		
		// Check next weapon is correct type
		AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
		if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
		{
			dialogMessage("Accessory",
						  "Sub weapon of wrong type \"%s\"",
						  accessory->getAccessoryTypeName());
			return false;
		}
		subWeapons_.push_back((Weapon*) accessory);
	}

	return true;
}

void WeaponMulti::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	std::list<Weapon *>::iterator itor;
	for (itor = subWeapons_.begin();
		 itor != subWeapons_.end();
		 itor++)
	{
		Weapon *weapon = *itor;
		weapon->fireWeapon(context, playerId, position, velocity, data);
	}
}
