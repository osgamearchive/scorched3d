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

#include <actions/ShotProjectileAimedUnder.h>
#include <engine/ScorchedContext.h>
#include <weapons/WeaponAimedUnder.h>
#include <tank/TankLib.h>

REGISTER_ACTION_SOURCE(ShotProjectileAimedUnder);

ShotProjectileAimedUnder::ShotProjectileAimedUnder()
{

}

ShotProjectileAimedUnder::ShotProjectileAimedUnder(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId) :
	ShotProjectile(startPosition, velocity, weapon, playerId, 2)
{
}

ShotProjectileAimedUnder::~ShotProjectileAimedUnder()
{
}

void ShotProjectileAimedUnder::collision(Vector &position)
{
	ShotProjectile::collision(position);

	WeaponAimedUnder *under = (WeaponAimedUnder *) weapon_;

	// NOTE: This code is very similar to the funky bomb code
	// except it works under ground
	position[2] = context_->landscapeMaps.getHMap().
		getInterpHeight(position[0], position[1]) / 2.0f;

	// Get all of the distances of the tanks less than 50 away
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		*context_,
		position, 
		sortedTanks,
		0,
		75.0f);

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
	for (int i=0; i<under->getNoWarHeads(); i++)
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
				*context_,
				position, 
				shootAt->getPhysics().getTankPosition(), -1.0f, 
				angleXYDegs, angleYZDegs, power);

			angleXYDegs += (RAND * 30.0f) + -15.0f;
			angleYZDegs += (RAND * 30.0f) + -15.0f;
		}

		// Create the shot
		int flareType = int(RAND * 4.0f);
		Vector &velocity = TankLib::getVelocityVector(
			angleXYDegs, angleYZDegs);
		velocity *= power;

		Action *newShot = under->getSubWeapon()->fireWeapon(
			playerId_, position, velocity);
		context_->actionController.addAction(newShot);	
	}
}
