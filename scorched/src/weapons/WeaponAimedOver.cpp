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

#include <weapons/WeaponAimedOver.h>
#include <weapons/AccessoryStore.h>
#include <landscape/LandscapeMaps.h>
#include <tank/Tank.h>
#include <tank/TankLib.h>
#include <common/Defines.h>
#include <list>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponAimedOver);

WeaponAimedOver::WeaponAimedOver() :
	warHeads_(0)
{

}

WeaponAimedOver::~WeaponAimedOver()
{

}

bool WeaponAimedOver::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	// Get the next weapon
	XMLNode *subNode = 0;
	if (!accessoryNode->getNamedChild("aimedweapon", subNode)) return false;

	// Check next weapon is correct type
	AccessoryPart *accessory = store->createAccessoryPart(context, parent_, subNode);
	if (!accessory || accessory->getType() != AccessoryPart::AccessoryWeapon)
	{
		return subNode->returnError("Failed to find sub weapon, not a weapon");
	}
	aimedWeapon_ = (Weapon*) accessory;

	// Get the accessory warheads
	if (!accessoryNode->getNamedChild("nowarheads", warHeads_)) return false;
	
	// Get the accessory aimed distance
	if (!accessoryNode->getNamedChild("maxaimdistance", maxAimedDistance_)) return false;
	
	// Get the accessory percentage miss chance
	if (!accessoryNode->getNamedChild("percentagemiss", percentageMissChance_)) return false;

	// Get the accessory percentage miss chance
	if (!accessoryNode->getNamedChild("inaccuracy", maxInacuracy_)) return false;

	return true;
}

void WeaponAimedOver::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &sentPosition, Vector &oldvelocity,
	unsigned int data)
{
	Vector position = sentPosition;
	if (position[0] < 6.0f) position[0] = 6.0f;
	else if (position[0] > context.landscapeMaps->getGroundMaps().getMapWidth() - 6) 
		position[0] = context.landscapeMaps->getGroundMaps().getMapWidth() - 6;
	if (position[1] < 6.0f) position[1] = 6.0f;
	else if (position[1] > context.landscapeMaps->getGroundMaps().getMapHeight() - 6)
	        position[1] = context.landscapeMaps->getGroundMaps().getMapHeight() - 6;
	float minHeight = context.landscapeMaps->getGroundMaps().getInterpHeight(
		position[0], position[1]);
	if (position[2] < minHeight + 0.5f)
	{
		position[2] = minHeight + 0.5f;
	}

	bool ceiling = false;
	{
		// This will return MAX_FLT when there is no roof
		float maxHeight = context.landscapeMaps->getRoofMaps().getInterpRoofHeight(
			position[0] / 4.0f, position[1] / 4.0f);
		if (position[2] > maxHeight - 1.0f)
		{
			ceiling = true;
			position[2] = maxHeight - 1.0f;
		}
	}

	// Get all of the distances of the tanks less than maxAimedDistance_ away
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		context,
		position, 
		sortedTanks,
		0,
		maxAimedDistance_);

	// Add all of these distances together
	float totalDist = 0.0f;
	std::list<std::pair<float, Tank *> >::iterator itor;
	for (itor = sortedTanks.begin();
		itor != sortedTanks.end();
		itor++)
	{
		totalDist += (*itor).first;
	}

	// Turn distance into a probablity that we will fire a the tank
	float maxDist = 0.0f;
	if (sortedTanks.size() == 1)
	{
		maxDist = totalDist;
	}
	else
	{
		for (itor = sortedTanks.begin();
			itor != sortedTanks.end();
			itor++)
		{
			(*itor).first = totalDist - (*itor).first;
			maxDist += (*itor).first;
		}
	}
	
	// Add a percetage that we will not fire at any tank
	maxDist *= 1.0f + (percentageMissChance_ / 100.0f);

	// For each war head
	for (int i=0; i<warHeads_; i++)
	{
		// Random probablity
		float dist = maxDist * RAND;

		// Find which tank fits this probability
		Tank *shootAt = 0;
		float distC = 0.0f;
		for (itor = sortedTanks.begin();
			itor != sortedTanks.end();
			itor++)
		{
			distC += (*itor).first;
			if (dist < distC)
			{
				shootAt = (*itor).second;
				break;
			}
		}			

		// Calcuate the angle for the shot
		float angleXYDegs = 360.0f * RAND;
		float angleYZDegs = 30.0f * RAND + 50.0f;
		float power = 300.0f * RAND + 150.0f;
		if (shootAt)
		{
			// We have a tank to aim at
			// Aim a shot towards it
			TankLib::getShotTowardsPosition(
				context,
				position, 
				shootAt->getPosition().getTankPosition(), 
				angleXYDegs, angleYZDegs, power);

			angleXYDegs += (RAND * maxInacuracy_) - 
				(maxInacuracy_ / 2.0f);
			angleYZDegs += (RAND * maxInacuracy_) - 
				(maxInacuracy_ / 2.0f);
		}
		if (ceiling) angleYZDegs += 180.0f;

		// Create the shot
		Vector &velocity = TankLib::getVelocityVector(
			angleXYDegs, angleYZDegs);
		velocity *= power;

		aimedWeapon_->fireWeapon(
			context, 
			playerId, position, velocity,
			data);	
	}
}
