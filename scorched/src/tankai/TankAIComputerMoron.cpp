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
#include <tankai/TankAIComputerMoron.h>
#include <tank/Tank.h>

TankAIComputerMoron::TankAIComputerMoron(Tank *tank) :
	TankAIComputer(tank)
{
	tankBuyer_.addAccessory("Missile", 2);
}

TankAIComputerMoron::~TankAIComputerMoron()
{

}

void TankAIComputerMoron::playMove(const unsigned state, 
								   float frameTime, char *buffer,
								   unsigned int keyState)
{
	// Choose weapons
	selectWeapons();

	// Find the angle + power etc.. to use
	float angleXYDegs = RAND * 360.0f;
	float angleYZDegs = RAND * 70.0f + 10.0f;
	float power = RAND * 900.0f + 100.0f;

	// Set the parameters
	// Sets the angle of the gun and the power
	currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
	currentTank_->getPhysics().rotateGunYZ(angleYZDegs, false);
	currentTank_->getState().changePower(power, false);

	// Actualy fire the shot
	// Fires the shot and ends the turn
	fireShot();
}

void TankAIComputerMoron::selectWeapons()
{
	// Choose a weapon
	// Chooses a random weapon
	int weaponInc = int(RAND * 3.0f);
	for (int i=0; i<weaponInc; i++)
	{
		currentTank_->getAccessories().getWeapons().nextWeapon();
	}
}
