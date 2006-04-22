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

#include <weapons/WeaponDelay.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>

REGISTER_ACCESSORY_SOURCE(WeaponDelay);

WeaponDelay::WeaponDelay() :
	delay_(0.0f)
{

}

WeaponDelay::~WeaponDelay()
{

}

bool WeaponDelay::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("delayedweapon", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	delayedWeapon_ = (Weapon*) accessory;

	if (!accessoryNode->getNamedChild("delay", delay_)) return false;

	return true;
}

void WeaponDelay::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Action *action = new CallbackWeapon(
		this, delay_, 0,
		playerId,
		position,
		velocity,
		data);
	context.actionController->addAction(action);
}

void WeaponDelay::weaponCallback(
			ScorchedContext &context,
			unsigned int playerId, Vector &position, Vector &velocity,
			unsigned int data,
			unsigned int userData)
{
	delayedWeapon_->fireWeapon(context, playerId, position, velocity, data);
}
