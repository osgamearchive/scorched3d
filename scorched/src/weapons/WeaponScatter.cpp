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

#include <weapons/WeaponScatter.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponScatter);

WeaponScatter::WeaponScatter()
{

}

WeaponScatter::~WeaponScatter()
{

}

bool WeaponScatter::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("scatteredweapon", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			accessory->getAccessoryTypeName());
		return false;
	}
	aimedWeapon_ = (Weapon*) accessory;

	if (!accessoryNode->getNamedChild("position", position_)) return false;
	if (!accessoryNode->getNamedChild("positionOffset", positionOffset_)) return false;
	if (!accessoryNode->getNamedChild("direction", direction_)) return false;
	if (!accessoryNode->getNamedChild("directionOffset", directionOffset_)) return false;

	return true;
}

void WeaponScatter::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Vector pos, vel;
	pos[0] += position[0] - positionOffset_[0] + 
		positionOffset_[0] * 2.0f * RAND;
	pos[1] += position[1] - positionOffset_[1] + 
		positionOffset_[1] * 2.0f * RAND;
	pos[2] += position[2] - positionOffset_[2] + 
		positionOffset_[2] * 2.0f * RAND;
	vel[0] += direction_[0] - directionOffset_[0] + 
		directionOffset_[0] * 2.0f * RAND;
	vel[1] += direction_[1] - directionOffset_[1] + 
		directionOffset_[1] * 2.0f * RAND;
	vel[2] += direction_[2] - directionOffset_[2] + 
		directionOffset_[2] * 2.0f * RAND;

	aimedWeapon_->fireWeapon(context, playerId, pos, vel, data);
}

