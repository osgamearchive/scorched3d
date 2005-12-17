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

#include <weapons/WeaponTranslate.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>

REGISTER_ACCESSORY_SOURCE(WeaponTranslate);

WeaponTranslate::WeaponTranslate() :
	translateDist_(0.0f), nextAction_(0)
{

}

WeaponTranslate::~WeaponTranslate()
{

}

bool WeaponTranslate::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("translatedist", translateDist_)) return false;

	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("nextaction", subNode)) return false;
	
	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	nextAction_ = (Weapon*) accessory;

	return true;
}

void WeaponTranslate::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Vector newVelocity = velocity.Normalize() * translateDist_;
	Vector newPosition = position + newVelocity;
	
	nextAction_->fireWeapon(context, playerId, newPosition, velocity, data);
}

