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

bool WeaponAimedOver::parseXML(XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(accessoryNode)) return false;

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
	Accessory *accessory = AccessoryStore::instance()->createAccessory(subNode);
	if (!accessory || accessory->getType() != Accessory::AccessoryWeapon)
	{
		dialogMessage("Accessory",
			"Sub weapon of wrong type \"%s\"",
			name_.c_str());
		return false;
	}
	aimedWeapon_ = (Weapon*) accessory;

	// Get the accessory warheads
	XMLNode *warheadsNode = accessoryNode->getNamedChild("nowarheads", false, true);
	if (!warheadsNode)
	{
		dialogMessage("Accessory",
			"Failed to find nowarheads node in accessory \"%s\"",
			name_.c_str());
		return false;
	}
	warHeads_ = atoi(warheadsNode->getContent());

	return true;
}

bool WeaponAimedOver::writeAccessory(NetBuffer &buffer)
{
	if (!Weapon::writeAccessory(buffer)) return false;
	if (!Weapon::write(buffer, aimedWeapon_)) return false;
	buffer.addToBuffer(warHeads_);
	return true;
}

bool WeaponAimedOver::readAccessory(NetBufferReader &reader)
{
	if (!Weapon::readAccessory(reader)) return false;
	aimedWeapon_ = Weapon::read(reader); if (!aimedWeapon_) return false;
	if (!reader.getFromBuffer(warHeads_)) return false;
	return true;
}

void WeaponAimedOver::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &oldvelocity)
{
	position[2] += 0.2f;

	// Get all of the distances of the tanks less than 50 away
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		context,
		position, 
		sortedTanks,
		0,
		50.0f);

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
	maxDist *= 1.20f; 

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
				shootAt->getPhysics().getTankPosition(), 20.0f, 
				angleXYDegs, angleYZDegs, power);

			angleXYDegs += (RAND * 10.0f) + -5.0f;
			angleYZDegs += (RAND * 10.0f) + -5.0f;
		}

		// Create the shot
		int flareType = int(RAND * 4.0f);
		Vector &velocity = TankLib::getVelocityVector(
			angleXYDegs, angleYZDegs);
		velocity *= power;

		aimedWeapon_->fireWeapon(
			context, 
			playerId, position, velocity);	
	}
}
