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

#include <list>
#include <math.h>
#include <tank/TankLib.h>
#include <actions/ShotProjectileAimedOver.h>
#include <actions/Explosion.h>
#include <engine/ScorchedContext.h>
#include <weapons/WeaponAimedOver.h>
#include <common/Defines.h>

REGISTER_ACTION_SOURCE(ShotProjectileAimedOver);

ShotProjectileAimedOver::ShotProjectileAimedOver()
{

}

ShotProjectileAimedOver::ShotProjectileAimedOver(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId) : 
		ShotProjectile(startPosition, velocity, 
			weapon, playerId)
{

}

ShotProjectileAimedOver::~ShotProjectileAimedOver()
{

}

void ShotProjectileAimedOver::collision(Vector &position)
{
	ShotProjectile::collision(position);

	WeaponAimedOver *over = (WeaponAimedOver *) weapon_;

	if (over->getMainSize() > 0.0f)
	{
		context_->actionController.addAction(
			new Explosion(position, (float) over->getMainSize(), weapon_, playerId_));
	}

	position[2] += 0.2f;

	// Get all of the distances of the tanks less than 50 away
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		*context_,
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
	for (int i=0; i<over->getNoWarHeads(); i++)
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
				*context_,
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

		Action *newShot = over->getSubWeapon()->fireWeapon(
			playerId_, position, velocity);
		context_->actionController.addAction(newShot);	
	}
}
