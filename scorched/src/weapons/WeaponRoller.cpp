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
#include <tank/TankContainer.h>
#include <landscape/LandscapeMaps.h>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponRoller);

WeaponRoller::WeaponRoller() : shieldHurtFactor_(0.0f)
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

	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("collisionaction", subNode)) return false;

	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			accessory->getAccessoryTypeName());
		return false;
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

	for (int i=0; i<numberRollers_; i++)
	{
		Vector position = oldposition;
		position[2] += 1.5f;
		
		// Make a slightly different starting position
		position[0] += RAND * 2.0f - 1.0f;
		position[1] += RAND * 2.0f - 1.0f;
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
			std::map<unsigned int, Tank *> &tanks = 
				context.tankContainer->getPlayingTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *current = (*itor).second;
				Vector &tankPos = 
					current->getPhysics().getTankPosition();
				Accessory *accessory = 
					current->getAccessories().getShields().getCurrentShield();
				if (accessory)
				{
					Shield *shield = (Shield *) accessory->getAction();
					float shieldSize = shield->getActualRadius();
					if ((position - tankPos).Magnitude() < shieldSize)
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
	if (position[0] > 1 && position[1] > 1 &&
		position[0] < context.landscapeMaps->getGroundMaps().getMapWidth() - 1 &&
		position[1] < context.landscapeMaps->getGroundMaps().getMapHeight() - 1)
	{
		Vector velocity(RAND - 0.5f, RAND - 0.5f, 2.0f * RAND);
		context.actionController->addAction(
			new ShotBounce(position, velocity, this, playerId, data));
	}
}
