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
#include <tank/TankLib.h>

TankAIComputerAim::TankAIComputerAim()
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
	if (0 != strcmp(aimType_.c_str(), "refined") &&
		0 != strcmp(aimType_.c_str(), "guess") &&
		0 != strcmp(aimType_.c_str(), "random"))
	{
		return node->returnError("Unknown aimtype. "
			"Should be one of refined, guess, random");
	}
	if (!node->getNamedChild("checknearcollision", checkNearCollision_)) return false;

	return true;
}

void TankAIComputerAim::ourShotLanded(Weapon *weapon, Vector &position)
{
	// Called when the shot we have fired has landed
	// Store the last position of this shot in the cache
	if (lastShot_)
	{
		lastShot_->finalpos = position;
		lastShot_ = 0;
	}
}

void TankAIComputerAim::setTank(Tank *tank)
{
	currentTank_ = tank;
}

TankAIComputerAim::AimResult TankAIComputerAim::aimAtTank(Tank *tank)
{
	if (!tank) return AimFailed;

	if (0 == strcmp(aimType_.c_str(), "refined"))
	{
		return refinedAim(tank, true);
	}
	else if (0 == strcmp(aimType_.c_str(), "guess"))
	{
		return refinedAim(tank, false);
	}
	else if (0 == strcmp(aimType_.c_str(), "random"))
	{
		return randomAim();
	}
	else DIALOG_ASSERT(0);

	return AimFailed;
}

bool TankAIComputerAim::refineLastShot(Tank *tank, 
	float &angleXYDegs, float &angleYZDegs, float &power)
{
	// Find any previous shots made at this tank
	std::map<unsigned int, std::list<MadeShot> >::iterator finditor = 
		madeShots_.find(tank->getPlayerId());
	if (finditor != madeShots_.end())
	{
		std::list<MadeShot> &shotList = (*finditor).second;

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
		float distToTank = (currentTank_->getPhysics().getTankPosition() - 
			tank->getPhysics().getTankPosition()).Magnitude();

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
			float newDistToTank = (currentTank_->getPhysics().getTankPosition() - 
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
				Vector dirToTank = tank->getPhysics().getTankPosition() - 
					currentTank_->getPhysics().getTankPosition();
				Vector dirToShot = tank->getPhysics().getTankPosition() - 
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
			angleXYDegs = closestLessXY;
			angleYZDegs = closestLessYZ;
		}
		else if (closestMoreDistToTank != 500.0f)
		{
			// We have only a shot that was too long
			// Make a guess on how much less power to use
			power = closestMorePower * (distToTank / closestMoreDistToTank);
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

TankAIComputerAim::AimResult TankAIComputerAim::refinedAim(Tank *targetTank, bool refine)
{
	// Find the angle + power etc.. to use
	float angleXYDegs = 0.0f; 
	float angleYZDegs = 0.0f;
	float power = 0.0f;

	// Try to refine an already made shot
	// This happens if we have already made a shot at this target then
	// try to make the next shot even better
	if (!refineLastShot(targetTank, angleXYDegs, angleYZDegs, power))
	{
		// Else make a new shot up
		// Makes a randow powered shot towards the target
		TankLib::getShotTowardsPosition(
			ScorchedServer::instance()->getContext(),
			currentTank_->getPhysics().getTankPosition(), 
			targetTank->getPhysics().getTankPosition(), sniperDist_, 
			angleXYDegs, angleYZDegs, power,
			checkNearCollision_);
	}

	// Check we will not kill ourselves
	if (checkNearCollision_)
	{
		// Check we don't collide with ground within a near distance
		float distance = (currentTank_->getPhysics().getTankPosition() - 
			targetTank->getPhysics().getTankPosition()).Magnitude();
		int allowedIntersectDist = int(distance / 2.0f);
		while (TankLib::intersection(
			ScorchedServer::instance()->getContext(), 
			currentTank_->getPhysics().getTankPosition(), 
			angleXYDegs, angleYZDegs, allowedIntersectDist))
		{
			angleYZDegs += 10.0f;
			power *= 1.1f; if (power > 1000.0f) power = 1000.0f;
			if (angleYZDegs > 90.0f) return AimBurried;
		}
	}

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
	currentTank_->getPhysics().rotateGunYZ(angleYZDegs, false);
	currentTank_->getPhysics().changePower(power, false);

	// Only save the shot if this ai refines
	if (refine)
	{
		// Save the shot made in the list of shots
		// Cache all shots made at the targets
		// This will be used later to make a more educated shot next time
		MadeShot newMadeShot;
		newMadeShot.angleXYDegs = angleXYDegs;
		newMadeShot.angleYZDegs = angleYZDegs;
		newMadeShot.power = power;
		madeShots_[targetTank->getPlayerId()].push_back(newMadeShot);
		lastShot_ = &madeShots_[targetTank->getPlayerId()].back();
	}

	return AimOk;
}

TankAIComputerAim::AimResult TankAIComputerAim::randomAim()
{
	// Find the angle + power etc.. to use
	float angleXYDegs = RAND * 360.0f;
	float angleYZDegs = RAND * 70.0f + 10.0f;
	float power = RAND * 900.0f + 100.0f;

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
	currentTank_->getPhysics().rotateGunYZ(angleYZDegs, false);
	currentTank_->getPhysics().changePower(power, false);

	return AimOk;
}
