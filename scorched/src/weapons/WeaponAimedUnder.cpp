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

#include <weapons/WeaponAimedUnder.h>
#include <weapons/AccessoryStore.h>
#include <landscape/LandscapeMaps.h>
#include <tank/TankLib.h>
#include <tank/TankContainer.h>
#include <common/Defines.h>
#include <server/ScorchedServer.h>
#include <list>
#include <math.h>

REGISTER_ACCESSORY_SOURCE(WeaponAimedUnder);

WeaponAimedUnder::WeaponAimedUnder() : warHeads_(0)
{

}

WeaponAimedUnder::~WeaponAimedUnder()
{

}

bool WeaponAimedUnder::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

	// Get the accessory size
	XMLNode *warHeadsNode = accessoryNode->getNamedChild("nowarheads", false, true);
	if (!warHeadsNode)
	{
		dialogMessage("Accessory",
			"Failed to find nowarheads node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	warHeads_ = atoi(warHeadsNode->getContent());

	// Get the next weapon
	XMLNode *subNode = accessoryNode->getNamedChild("aimedweapon", false, true);
	if (!subNode)
	{
		dialogMessage("Accessory",
			"Failed to find aimedweapon node in accessory \"%s\"",
			name_.c_str());
		return false;
	}

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
	aimedWeapon_ = (Weapon*) accessory;
	
	// Get the accessory aimed distance
	XMLNode *aimNode = accessoryNode->getNamedChild("maxaimdistance", false, true);
	if (!aimNode)
	{
		dialogMessage("Accessory",
			"Failed to find maxaimdistance node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	maxAimedDistance_ = (float) atof(aimNode->getContent());
	
	// Get the accessory percentage miss chance
	XMLNode *percentageNode = accessoryNode->getNamedChild("percentagemiss", false, true);
	if (!percentageNode)
	{
		dialogMessage("Accessory",
			"Failed to find percentagemiss node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	percentageMissChance_ = (float) atof(percentageNode->getContent());

	// Get the accessory percentage miss chance
	XMLNode *inaccuracyNode = accessoryNode->getNamedChild("inaccuracy", false, true);
	if (!inaccuracyNode)
	{
		dialogMessage("Accessory",
			"Failed to find inaccuracy node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	maxInacuracy_ = (float) atof(inaccuracyNode->getContent());

	return true;
}

bool WeaponAimedUnder::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	buffer.addToBuffer(warHeads_);
	if (!Weapon::write(buffer, aimedWeapon_)) return false;
	buffer.addToBuffer(maxAimedDistance_);
	buffer.addToBuffer(percentageMissChance_);
	buffer.addToBuffer(maxInacuracy_);
	return true;
}

bool WeaponAimedUnder::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	if (!reader.getFromBuffer(warHeads_)) return false;
	aimedWeapon_ = Weapon::read(reader); if (!aimedWeapon_) return false;
	if (!reader.getFromBuffer(maxAimedDistance_)) return false;
	if (!reader.getFromBuffer(percentageMissChance_)) return false;
	if (!reader.getFromBuffer(maxInacuracy_)) return false;
	return true;
}

void WeaponAimedUnder::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &oldvelocity)
{
	// NOTE: This code is very similar to the funky bomb code
	// except it works under ground
	position[2] = context.landscapeMaps->getHMap().
		getInterpHeight(position[0], position[1]) / 2.0f;

	// Get all of the distances of the tanks less than 50 away
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
		float power = 1000.0f;
		if (shootAt)
		{
			// We have a tank to aim at
			// Aim a shot towards it
			TankLib::getShotTowardsPosition(
				context,
				position, 
				shootAt->getPhysics().getTankPosition(), -1.0f, 
				angleXYDegs, angleYZDegs, power);

			angleXYDegs += (RAND * maxInacuracy_) - 
				(maxInacuracy_ / 2.0f);
			angleYZDegs += (RAND * maxInacuracy_) - 
				(maxInacuracy_ / 2.0f);
		}

		// Create the shot
		Vector &velocity = TankLib::getVelocityVector(
			angleXYDegs, angleYZDegs);
		velocity *= power;

		aimedWeapon_->fireWeapon(context, playerId, position, velocity);
	}
}
