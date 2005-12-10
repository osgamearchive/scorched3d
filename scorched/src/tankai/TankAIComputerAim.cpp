////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <tankai/TankAIComputerAim.h>
#include <server/ScorchedServer.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <tank/TankContainer.h>
#include <tank/TankLib.h>
#include <math.h>
#include <float.h>

TankAIComputerAim::TankAIComputerAim() : lastShot_(0)
{
	newGame();
}

TankAIComputerAim::~TankAIComputerAim()
{
}

void TankAIComputerAim::newGame()
{
	// Called for each new round
	// We have been told that a new round has been started so we will clear
	// the list of shots and there distances
	lastShot_ = 0;
	madeShots_.clear();
}

bool TankAIComputerAim::parseConfig(XMLNode *node)
{
	if (!node->getNamedChild("aimsniper", sniperDist_)) return false;
	if (!node->getNamedChild("aimtype", aimType_)) return false;
	if (0 != strcmp(aimType_.c_str(), "newrefined") &&
		0 != strcmp(aimType_.c_str(), "refined") &&
		0 != strcmp(aimType_.c_str(), "guess") &&
		0 != strcmp(aimType_.c_str(), "random"))
	{
		return node->returnError("Unknown aimtype. "
			"Should be one of newrefined, refined, guess, random");
	}
	if (!node->getNamedChild("checknearcollision", checkNearCollision_)) return false;

	return true;
}

void TankAIComputerAim::ourShotLanded(Weapon *weapon, Vector &position)
{
	// Called when the shot we have fired has landed
	// Store the last position of this shot in the cache
	if (lastShot_ != 0)
	{
		// Find the tank the last shot has been made at
		Tank *targetTank = ScorchedServer::instance()->
			getTankContainer().getTankById(lastShot_);
		if (targetTank)
		{
			// Find the list of shots for this tank
			std::map<unsigned int, ShotListEntry>::iterator finditor = 
				madeShots_.find(targetTank->getPlayerId());
			DIALOG_ASSERT(finditor != madeShots_.end());

			// Add this position
			(*finditor).second.shotList.back().finalpos = position;
			(*finditor).second.lastShot = position;
		}

		lastShot_ = 0;
	}
}

void TankAIComputerAim::setTank(Tank *tank)
{
	currentTank_ = tank;
}

TankAIComputerAim::AimResult TankAIComputerAim::aimAtTank(
	Tank *tank, float &distance, int &noShots)
{
	if (!tank) return AimFailed;

	if (0 == strcmp(aimType_.c_str(), "refined") ||
		0 == strcmp(aimType_.c_str(), "newrefined"))
	{
		return refinedAim(tank, true, distance, noShots);
	}
	else if (0 == strcmp(aimType_.c_str(), "guess"))
	{
		return refinedAim(tank, false, distance, noShots);
	}
	else if (0 == strcmp(aimType_.c_str(), "random"))
	{
		return randomAim(distance, noShots);
	}
	else DIALOG_ASSERT(0);

	return AimFailed;
}

bool TankAIComputerAim::refineLastShot(Tank *tank, 
	float &angleXYDegs, float &angleYZDegs, float &power)
{
	if (0 == strcmp(aimType_.c_str(), "newrefined"))
	{
		return newRefineLastShot(tank, angleXYDegs, angleYZDegs, power);
	}
	return oldRefineLastShot(tank, angleXYDegs, angleYZDegs, power);
}

bool TankAIComputerAim::newRefineLastShot(Tank *tank, 
	float &angleXYDegs, float &angleYZDegs, float &power)
{
	// Hmm, this is actually a lot simpilier that I thought it would be
	// should have tried this before
	//
	// Basicially the aiming is done seperately in 2d (for both the x 
	// and y axis) and then combined at the end to make a truely 3d shot
	float nearestDistanceX = FLT_MAX, nearestDistanceY = FLT_MAX;
	MadeShot *nearestX = 0, *nearestY = 0;

	// We can use the shot position gained from other tanks
	// Find the closest x and closest y shot made so far
	Vector tankPosition = tank->getPosition().getTankPosition();
	std::map<unsigned int, ShotListEntry>::iterator outeritor;
	for (outeritor = madeShots_.begin();
		outeritor != madeShots_.end();
		outeritor++)
	{
		std::list<MadeShot>::iterator inneritor;
		for (inneritor = (*outeritor).second.shotList.begin();
			inneritor != (*outeritor).second.shotList.end();
			inneritor++)
		{
			MadeShot &shot = *inneritor;

			float distanceX = fabsf(shot.finalpos[0] - tankPosition[0]);
			if (distanceX < nearestDistanceX)
			{
				// Find a close enough shot or
				// the last shot we made at this player
				if (distanceX < 15.0f ||
					(*outeritor).first == tank->getPlayerId())
				{
					nearestX = &shot;
					nearestDistanceX = distanceX;
				}
			}
			float distanceY = fabsf(shot.finalpos[1] - tankPosition[1]);
			if (distanceY < nearestDistanceY)
			{
				if (distanceY < 15.0f ||
					(*outeritor).first == tank->getPlayerId())
				{
					nearestY = &shot;
					nearestDistanceY = distanceY;
				}
			}
		}
	}

	// Check if we have enough info to refine the shot
	if (!nearestX || !nearestY) return false;

	// Figure out the best old velocity we have
	Vector oldVelocityX = TankLib::getVelocityVector(
		nearestX->angleXYDegs, nearestX->angleYZDegs) *
		nearestX->power;
	Vector oldVelocityY = TankLib::getVelocityVector(
		nearestY->angleXYDegs, nearestY->angleYZDegs) *
		nearestY->power;
	Vector oldVelocity(
		oldVelocityX[0], oldVelocityY[1], oldVelocityX[0]);

	// Figure out the new best angle
	Vector newVelocity = oldVelocity;
	newVelocity[0] -= (nearestX->finalpos[0] - tankPosition[0]) * 0.1f;
	newVelocity[1] -= (nearestY->finalpos[1] - tankPosition[1]) * 0.1f;

	float angleXYRads = atan2f(newVelocity[1], newVelocity[0]);
	angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;
	angleYZDegs = 45.0f;

	// And the new best power
	Vector resultingVelocity = TankLib::getVelocityVector(
		angleXYDegs, angleYZDegs);
	float resultingDistance = 
		sqrtf(resultingVelocity[0] * resultingVelocity[0] +
		resultingVelocity[1] * resultingVelocity[1]);
	float wantedDistance = 
		sqrtf(newVelocity[0] * newVelocity[0] +
		newVelocity[1] * newVelocity[1]);
	power = wantedDistance / resultingDistance;

	// Remove the nearest shots so we don't get out of date info
	// when the ground has moved 
	// Only remove for this tank as it should only be a problem
	// very close to the target
	std::map<unsigned int, ShotListEntry>::iterator finditor = 
		madeShots_.find(tank->getPlayerId());
	if (finditor != madeShots_.end())
	{
		std::list<MadeShot> shots;
		std::list<MadeShot>::iterator inneritor;
		for (inneritor = (*finditor).second.shotList.begin();
			inneritor != (*finditor).second.shotList.end();
			inneritor++)
		{
			MadeShot &shot = *inneritor;
			if (&shot != nearestX &&
				&shot != nearestY)
			{
				shots.push_back(shot);
			}
		}
		(*finditor).second.shotList = shots;
	}

	return true;
}

bool TankAIComputerAim::oldRefineLastShot(Tank *tank, 
	float &angleXYDegs, float &angleYZDegs, float &power)
{
	// Find any previous shots made at this tank
	std::map<unsigned int, ShotListEntry>::iterator finditor = 
		madeShots_.find(tank->getPlayerId());
	if (finditor != madeShots_.end())
	{
		std::list<MadeShot> &shotList = (*finditor).second.shotList;

		if (shotList.empty())
		{
			// We have not made any shots at this target
			// Make a random shot
			return false;
		}

		// If we have made more than 5 shots at the same target something
		// has gone wrong, we will therefore ignore all previous data and start over
		if (shotList.size() > 5)
		{
			// Remove all shots at this tank
			madeShots_.erase(finditor);

			// Make a random shot
			return false;
		}

		// Get the current distance to tank
		float distToTank = (currentTank_->getPosition().getTankPosition() - 
			tank->getPosition().getTankPosition()).Magnitude();

		// Set the initial values to the initial entry
		float closestLessDistToTank = 0.0f;
		float closestMoreDistToTank = 500.0f;
		float closestLessPower = 0.0f;
		float closestMorePower = 1000.0f;

		float closestLessXY, closestMoreXY;
		float closestLessYZ, closestMoreYZ;
		Vector closestMorePos, closestLessPos;

		// Now check the other entries
		// Find the closest previous shot that missed short (if any)
		// and the closest previous shot that miessed long (if any)
		std::list<MadeShot>::iterator itor;
		for (itor = shotList.begin();
			itor != shotList.end();
			itor++)
		{
			float newDistToTank = (currentTank_->getPosition().getTankPosition() - 
				(*itor).finalpos).Magnitude();
			if (newDistToTank > closestLessDistToTank &&
				newDistToTank <= distToTank)
			{
				// We have found a new closest shot that was short
				// Store its power and distance
				closestLessDistToTank = newDistToTank;
				closestLessPower = (*itor).power;
				closestLessPos = (*itor).finalpos;
				closestLessXY = (*itor).angleXYDegs;
				closestLessYZ = (*itor).angleYZDegs;
			}

			if (newDistToTank < closestMoreDistToTank &&
				newDistToTank >= distToTank)
			{
				// We have found a new closest shot that was long
				// Store its power and distance
				closestMoreDistToTank = newDistToTank;
				closestMorePower = (*itor).power;
				closestMorePos = (*itor).finalpos;
				closestMoreXY = (*itor).angleXYDegs;
				closestMoreYZ = (*itor).angleYZDegs;
			}
		}

		// Use the values for the closest shots to determine a better shot
		if (closestLessDistToTank != 0.0f &&
			closestMoreDistToTank != 500.0f)
		{
			angleXYDegs = closestMoreXY;
			angleYZDegs = closestMoreYZ;

			// We have both a shot that was too short and a shot that was too long
			// Choose a power that is the correct percentage between the two
			float percentage = (distToTank - closestLessDistToTank) / (
				closestMoreDistToTank - closestLessDistToTank);
			power = closestLessPower + ((closestMorePower - closestLessPower) * percentage);

			// Make adjustments for the wind
			if(ScorchedServer::instance()->getOptionsTransient().getWindOn())
			{
				Vector dirToTank = tank->getPosition().getTankPosition() - 
					currentTank_->getPosition().getTankPosition();
				Vector dirToShot = tank->getPosition().getTankPosition() - 
					closestMorePos;	

				// Work only in 2D
				dirToShot[2] = 0.0f;
				dirToTank[2] = 0.0f;
				closestMorePos[2] = 0.0f;
				closestLessPos[2] = 0.0f;

				// We are close enough on the distance to try for the direction
				if ((closestMorePos - closestLessPos).Magnitude() < dirToShot.Magnitude())
				{
					// Create new shot towards tank  using wind
					dirToShot = dirToShot.Normalize();
					dirToTank = dirToTank.Normalize();
					Vector dirToTankP = dirToTank.get2DPerp();
					float offset = dirToTankP.dotP(dirToShot);
					float add = -offset * (2.5f + RAND);
					angleXYDegs += add;

					// Remove all shots at this tank
					madeShots_.erase(finditor);
				}
			}
		}
		else if (closestLessDistToTank != 0.0f)
		{
			// We have only a shot that was too short
			// Make a guess on how much more power to use
			power = closestLessPower * (distToTank / closestLessDistToTank);
			//power = (closestLessDistToTank + power) / 2.0f;
			angleXYDegs = closestLessXY;
			angleYZDegs = closestLessYZ;
		}
		else if (closestMoreDistToTank != 500.0f)
		{
			// We have only a shot that was too long
			// Make a guess on how much less power to use
			power = closestMorePower * (distToTank / closestMoreDistToTank);
			//power = (power) / 2.0f;
			angleXYDegs = closestMoreXY;
			angleYZDegs = closestMoreYZ;
		}
		else
		{
			return false;
		}

		return true;
	}

	return false;
}

TankAIComputerAim::AimResult TankAIComputerAim::refinedAim(
	Tank *targetTank, bool refine, float &distance, int &noShots)
{
	// Find the angle + power etc.. to use
	float angleXYDegs = 0.0f; 
	float angleYZDegs = 0.0f;
	float power = 0.0f;

	// Find closest shot and number of shots made at this target
	// do this before we add the new shot as we don't know where
	// it landed it yet.
	noShots = 0;
	distance = 512.0f;
	std::map<unsigned int, ShotListEntry>::iterator finditor = 
		madeShots_.find(targetTank->getPlayerId());
	if (finditor != madeShots_.end())
	{
		noShots = (*finditor).second.shotCount;
		distance = (targetTank->getPosition().getTankPosition()-
			(*finditor).second.lastShot).Magnitude();
	}

	// Check if we can make a sniper shot to the target
	// If not then try an ordinary shot
	if (!TankLib::getSniperShotTowardsPosition(
		ScorchedServer::instance()->getContext(),
		currentTank_->getPosition().getTankPosition(), 
		targetTank->getPosition().getTankPosition(), sniperDist_, 
		angleXYDegs, angleYZDegs, power,
		checkNearCollision_))
	{
		// Try to refine an already made shot
		// This happens if we have already made a shot at this target then
		// try to make the next shot even better
		if (!refine ||
			!refineLastShot(targetTank, angleXYDegs, angleYZDegs, power))
		{
			// Else make a new shot up
			// Makes a randow powered shot towards the target
			TankLib::getShotTowardsPosition(
				ScorchedServer::instance()->getContext(),
				currentTank_->getPosition().getTankPosition(), 
				targetTank->getPosition().getTankPosition(), 
				angleXYDegs, angleYZDegs, power);
		}

		// Set the parameters
		// Sets the angle of the gun and the power
		currentTank_->getPosition().rotateGunXY(angleXYDegs, false);
		currentTank_->getPosition().rotateGunYZ(angleYZDegs, false);
		currentTank_->getPosition().changePower(power, false);

		// Check we will not kill ourselves
		if (checkNearCollision_)
		{
			// Check we don't collide with ground within a near distance
			float distance = (currentTank_->getPosition().getTankPosition() - 
				targetTank->getPosition().getTankPosition()).Magnitude();
			int allowedIntersectDist = int(distance / 2.0f);
			while (TankLib::intersection(
				ScorchedServer::instance()->getContext(), 
				currentTank_->getPosition().getTankGunPosition() - Vector(0.0f, 0.0f, 0.5f), 
				angleXYDegs, angleYZDegs, power, allowedIntersectDist))
			{
				angleYZDegs += 5.0f;
				power *= 1.1f; if (power > 1000.0f) power = 1000.0f;
				if (angleYZDegs > 90.0f) return AimBurried;
			}
		}

		// Save the shot made in the list of shots
		// Cache all shots made at the targets
		// This will be used later to make a more educated shot next time
		// Only used if refined shots are being made
		MadeShot newMadeShot;
		newMadeShot.angleXYDegs = angleXYDegs;
		newMadeShot.angleYZDegs = angleYZDegs;
		newMadeShot.power = power;
		madeShots_[targetTank->getPlayerId()].shotList.push_back(newMadeShot);
		madeShots_[targetTank->getPlayerId()].shotCount++;
		lastShot_ = targetTank->getPlayerId();
	}

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPosition().rotateGunXY(angleXYDegs, false);
	currentTank_->getPosition().rotateGunYZ(angleYZDegs, false);
	currentTank_->getPosition().changePower(power, false);

	return AimOk;
}

TankAIComputerAim::AimResult TankAIComputerAim::randomAim(
	float &distance, int &noShots)
{
	// Find the angle + power etc.. to use
	float angleXYDegs = RAND * 360.0f;
	float angleYZDegs = RAND * 70.0f + 10.0f;
	float power = RAND * 900.0f + 100.0f;

	// No idea
	noShots = int(RAND * 10.0f);
	distance = RAND * 100.0f;

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPosition().rotateGunXY(angleXYDegs, false);
	currentTank_->getPosition().rotateGunYZ(angleYZDegs, false);
	currentTank_->getPosition().changePower(power, false);

	return AimOk;
}
