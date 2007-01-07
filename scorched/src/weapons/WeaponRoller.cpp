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
#include <weapons/Shield.h>
#include <actions/ShotBounce.h>
#include <common/Defines.h>
#include <engine/ActionController.h>
#include <target/TargetContainer.h>
#include <target/TargetShield.h>
#include <target/TargetLife.h>
#include <landscapemap/LandscapeMaps.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponRoller);

WeaponRoller::WeaponRoller() : 
	shieldHurtFactor_(0.0f), windFactor_(1.0f)
{

}

WeaponRoller::~WeaponRoller()
{

}

bool WeaponRoller::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get number of rollers
	if (!accessoryNode->getNamedChild("numberrollers", numberRollers_)) return false;

	// Get life time
	if (!accessoryNode->getNamedChild("time", time_)) return false;

    // Get the hurt factor (if any)
    accessoryNode->getNamedChild("shieldhurtfactor", shieldHurtFactor_, false);

	// Get the wind factor (if any)
	accessoryNode->getNamedChild("windfactor", windFactor_, false);

	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("collisionaction", subNode)) return false;

	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	collisionAction_ = (Weapon*) accessory;

	// Get the weapon model
	XMLNode *modelNode = 0;
	if (!accessoryNode->getNamedChild("rollermodel", modelNode)) return false;
	if (!rollerModelId_.initFromNode("data/accessories", modelNode)) return false;

	return true;
}

void WeaponRoller::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &oldposition, Vector &velocity,
	unsigned int data)
{
	float minHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
		oldposition[0], oldposition[1]);

	// Make sure position is not underground
	if (oldposition[2] < minHeight)
	{
		if (minHeight - oldposition[2] > 10.0f) // Give room for shields as well
		{
			return;
		}
	}

	RandomGenerator &random = context.actionController->getRandom();
	for (int i=0; i<numberRollers_; i++)
	{
		Vector position = oldposition;
		position[2] += 1.5f;
		
		// Make a slightly different starting position
		position[0] += random.getRandFloat() * 2.0f - 1.0f;
		position[1] += random.getRandFloat() * 2.0f - 1.0f;
		float minHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
			position[0], position[1]) + 1.0f;
		if (position[2] < minHeight) position[2] = minHeight;
				
		// Check if we have hit the roof (quite litteraly)
		{
			float maxHeight = context.landscapeMaps->getRoofMaps().getInterpRoofHeight(
				position[0] / 4.0f, position[1] / 4.0f);
			if (position[2] > maxHeight - 1.0f)
			{
				position[2] = maxHeight - 1.0f;
			}
		}

		// Make sure this new position is not inside a tank's shields
		bool ok = false;
		while (!ok)
		{
			ok = true;
			std::map<unsigned int, Target *> &targets = 
				context.targetContainer->getTargets();
			std::map<unsigned int, Target *>::iterator itor;
			for (itor = targets.begin();
				itor != targets.end();
				itor++)
			{
				Target *current = (*itor).second;
				Vector &tankPos = 
					current->getLife().getTargetPosition();
				Accessory *accessory = 
					current->getShield().getCurrentShield();
				if (accessory)
				{
					Shield *shield = (Shield *) accessory->getAction();
					Vector offset = position - tankPos;
					if (shield->inShield(offset))
					{
						ok = false;
						position[2] += 1.0f;
					}
				}
			}
		}
		
		addRoller(context, playerId, position, data);
	}
}

void WeaponRoller::addRoller(ScorchedContext &context,
	unsigned int playerId,
	Vector &position, unsigned int data)
{
	// Ensure that the Roller has not hit the walls
	// or anything outside the landscape
	RandomGenerator &random = context.actionController->getRandom();
	if (position[0] > 1 && position[1] > 1 &&
		position[0] < context.landscapeMaps->getGroundMaps().getMapWidth() - 1 &&
		position[1] < context.landscapeMaps->getGroundMaps().getMapHeight() - 1)
	{
		float velx = random.getRandFloat() - 0.5f;
		float vely = random.getRandFloat() - 0.5f;
		float velz = random.getRandFloat() * 2.0f;
		Vector velocity(velx, vely, velz);
		context.actionController->addAction(
			new ShotBounce(position, velocity, this, playerId, data));
	}
}
