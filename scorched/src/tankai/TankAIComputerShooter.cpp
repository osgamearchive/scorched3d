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

#include <weapons/AccessoryStore.h>
#include <tankai/TankAIComputerShooter.h>
#include <engine/ScorchedContext.h>
#include <tank/Tank.h>
#include <tank/TankLib.h>

TankAIComputerShooter::TankAIComputerShooter()
{

}

TankAIComputerShooter::~TankAIComputerShooter()
{

}

Tank *TankAIComputerShooter::findTankToShootAt()
{
	// Get the list of all tanks left in the game
	// Sorted by distance
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		*context_,
		currentTank_->getPhysics().getTankPosition(), 
		sortedTanks);

	// Choose a tank based on a probablity
	// The nearest tanks are more likely to be choosen
	Tank *returnTank = 0;
	if (!sortedTanks.empty())
	{
		// The number of tanks to skip before returning
		int noTanks = int(RAND * 1.2f);

		// Always skip the current tank as it is the first tank
		std::list<std::pair<float, Tank *> >::iterator itor = sortedTanks.begin();
		itor++;

		// Count the number of tanks to skip
		for (int i=0;itor != sortedTanks.end() && i<=noTanks; itor++, i++)
		{
			returnTank = (*itor).second;
		}
	}

	return returnTank;
}

void TankAIComputerShooter::playMove(const unsigned state, 
	float frameTime, char *buffer,
	unsigned int keyState)
{
	// Choose weapons
	autoDefense();
	selectWeapons();

	// Find the angle + power etc.. to use
	float angleXYDegs = 0.0f;
	float angleYZDegs = 0.0f;
	float power = 0.0f;
	Tank *targetTank = findTankToShootAt();
	if (!targetTank) return;

	TankLib::getShotTowardsPosition(
		*context_,
		currentTank_->getPhysics().getTankPosition(), 
		targetTank->getPhysics().getTankPosition(), 0.0f, 
		angleXYDegs, angleYZDegs, power);

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
	currentTank_->getPhysics().rotateGunYZ(angleYZDegs, false);
	currentTank_->getState().changePower(power, false);

	// Actualy fire the shot
	// Fires the shot and ends the turn
	fireShot();
}

void TankAIComputerShooter::autoDefense()
{
	// Try to enable parachutes (fails if we don't have any)
	if (currentTank_->getAccessories().getParachutes().getNoParachutes() > 0)
	{
		parachutesUpDown(true);
	}

	// Try to raise shields (fails if we don't have any)
	if (!currentTank_->getAccessories().getShields().getCurrentShield())
	{
		if (currentTank_->getAccessories().getShields().getAllShields().size())
		{
			selectFirstShield();
		}
	}
}

void TankAIComputerShooter::selectWeapons()
{
	// Make sure defenses are raised (if we don't have an autodefense)
	autoDefense();

	// Choose a weapon
	// Chooses a random weapon
	int weaponInc = int(RAND * 3.0f);
	for (int i=0; i<weaponInc; i++)
	{
		currentTank_->getAccessories().getWeapons().nextWeapon();
	}
}

