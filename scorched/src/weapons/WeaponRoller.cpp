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

#include <weapons/WeaponRoller.h>
#include <weapons/AccessoryStore.h>
#include <actions/ShotBounce.h>
#include <common/Defines.h>
#include <engine/ActionController.h>
#include <landscape/LandscapeMaps.h>
#include <server/ScorchedServer.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponRoller);

WeaponRoller::WeaponRoller()
{

}

WeaponRoller::~WeaponRoller()
{

}

bool WeaponRoller::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("numberrollers", numberRollers_)) return false;

	if (!accessoryNode->getNamedChild("time", time_)) return false;

	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("collisionaction", subNode)) return false;

	Accessory *accessory = 
		ScorchedServer::instance()->getAccessoryStore().createAccessory(subNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	collisionAction_ = (Weapon*) accessory;

	// Get the weapon model
	XMLNode *modelNode = 0;
	if (!accessoryNode->getNamedChild("rollermodel", modelNode)) return false;
	if (!rollerModelId_.initFromNode("data/accessories", modelNode)) return false;

	return true;
}

bool WeaponRoller::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, collisionAction_)) return false;
	buffer.addToBuffer(numberRollers_);
	buffer.addToBuffer(time_);
	if (!rollerModelId_.writeModelID(buffer)) return false;
	return true;
}

bool WeaponRoller::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	collisionAction_ = Weapon::read(reader); if (!collisionAction_) return false;
	if (!reader.getFromBuffer(numberRollers_)) return false;
	if (!reader.getFromBuffer(time_)) return false;
	if (!rollerModelId_.readModelID(reader)) return false;
	return true;
}

void WeaponRoller::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &oldposition, Vector &velocity,
	unsigned int data)
{
	for (int i=0; i<numberRollers_; i++)
	{
	//for (float i=0.0f; i<360.0f; i+= 360.0f / float(numberRollers_))
	//{
		Vector position = oldposition;
		position[0] += RAND * 2.0f - 1.0f;
		position[1] += RAND * 2.0f - 1.0f;
		position[2] = context.landscapeMaps->getHMap().getInterpHeight(
			position[0], position[1]) + 1.0f;
		addRoller(context, playerId, position, data);
	}
}

void WeaponRoller::addRoller(ScorchedContext &context,
	unsigned int playerId,
	Vector &position, unsigned int data)
{
	// Ensure that the Roller has not hit the walls
	// or anything outside the landscape
	if (position[0] > 1 && position[1] > 1 &&
		position[0] < context.landscapeMaps->getHMap().getWidth() - 1 &&
		position[1] < context.landscapeMaps->getHMap().getWidth() - 1)
	{
		Vector velocity(RAND - 0.5f, RAND - 0.5f, 2.0f * RAND);
		context.actionController->addAction(
			new ShotBounce(position, velocity, this, playerId, data));
	}
}
