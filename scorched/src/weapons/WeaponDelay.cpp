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
#include <actions/ShotDelay.h>
#include <engine/ActionController.h>
#include <server/ScorchedServer.h>

REGISTER_ACCESSORY_SOURCE(WeaponDelay);

WeaponDelay::WeaponDelay() :
	delay_(0.0f)
{

}

WeaponDelay::~WeaponDelay()
{

}

bool WeaponDelay::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("delayedweapon", subNode)) return false;

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
	delayedWeapon_ = (Weapon*) accessory;

	if (!accessoryNode->getNamedChild("delay", delay_)) return false;

	return true;
}

bool WeaponDelay::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, delayedWeapon_)) return false;
	buffer.addToBuffer(delay_);
	return true;
}

bool WeaponDelay::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	delayedWeapon_ = Weapon::read(reader); if (!delayedWeapon_) return false;
	if (!reader.getFromBuffer(delay_)) return false;
	return true;
}

void WeaponDelay::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Action *action = new ShotDelay(
		playerId,
		delayedWeapon_,
		velocity,
		position,
		delay_,
		data);
	context.actionController->addAction(action);
}

