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

#include <math.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankLib.h>
#include <tankai/TankAIComputerTosser.h>
#include <common/OptionsTransient.h>

TankAIComputerTosser::TankAIComputerTosser() : 
	lastShot_(0), sniperDist_(0.0f)
{

}

TankAIComputerTosser::~TankAIComputerTosser()
{

}

bool TankAIComputerTosser::parseConfig(XMLNode *node)
{
	if (!TankAIComputerShooter::parseConfig(node)) return false;

	XMLNode *sniperNode = node->getNamedChild("sniper");
	if (!sniperNode)
	{
		dialogMessage("TankAIComputer",
			"Failed to find sniper node in tank ai \"%s\"",
			name_.c_str());
		return false;
	}
	sniperDist_ = (float) atof(sniperNode->getContent());

	return true;
}

void TankAIComputerTosser::newGame()
{
	// Called for each new round
	// We have been told that a new round has been started so we will clear
	// the list of shots and there distances
	lastShot_ = 0;
	madeShots_.clear();

	TankAIComputerShooter::newGame();
}

void TankAIComputerTosser::ourShotLanded(Weapon *weapon, Vector &position)
{
	// Called when the shot we have fired has landed
	// Store the last position of this shot in the cache
	if (lastShot_)
	{
		lastShot_->finalpos = position;
		lastShot_ = 0;
	}
}

bool TankAIComputerTosser::refineLastShot(Tank *tank, float &angleXYDegs, float &angleYZDegs, float &power)
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
			if(context_->optionsTransient->getWindOn())
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

void TankAIComputerTosser::playMove(const unsigned state, float frameTime, 
									char *buffer, unsigned int keyState)
{
	// Play move is called when the computer opponent must make there move

	// Is there any point in making a move
	if (currentTank_->getState().getLife() < 10) 
	{
		resign();
		return;
	}

	// Choose weapons
	selectWeapons();

	// Find the tank to shoot at
	Tank *targetTank = findTankToShootAt();
	if (!targetTank) 
	{
		skipShot();
		return;
	}

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
			*context_,
			currentTank_->getPhysics().getTankPosition(), 
			targetTank->getPhysics().getTankPosition(), sniperDist_, 
			angleXYDegs, angleYZDegs, power);
	}

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
	currentTank_->getPhysics().rotateGunYZ(angleYZDegs, false);
	currentTank_->getPhysics().changePower(power, false);

	// Save the shot made in the list of shots
	// Cache all shots made at the targets
	// This will be used later to make a more educated shot next time
	MadeShot newMadeShot;
	newMadeShot.angleXYDegs = angleXYDegs;
	newMadeShot.angleYZDegs = angleYZDegs;
	newMadeShot.power = power;
	madeShots_[targetTank->getPlayerId()].push_back(newMadeShot);
	lastShot_ = &madeShots_[targetTank->getPlayerId()].back();

	// Actualy fire the shot
	// Fires the shot and ends the turn
	fireShot();
}
