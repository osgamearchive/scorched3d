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

#include <actions/ShotProjectileMirv.h>
#include <weapons/WeaponMirv.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>

REGISTER_ACTION_SOURCE(ShotProjectileMirv);

ShotProjectileMirv::ShotProjectileMirv() : up_(false)
{

}

ShotProjectileMirv::ShotProjectileMirv(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId) :
	ShotProjectile(startPosition, velocity, weapon, playerId),
	up_(false)
{

}

ShotProjectileMirv::~ShotProjectileMirv()
{

}

void ShotProjectileMirv::simulate(float timepassed, bool &remove)
{
	if (getCurrentVelocity()[2] > 0.0f) up_ = true;
	else if (up_)
	{
		apex(getCurrentPosition(),
			getCurrentVelocity());
		remove = true;
	}

	ShotProjectile::simulate(timepassed, remove);
}

void ShotProjectileMirv::apex(Vector &position, Vector &currentVelocity)
{
	WeaponMirv *weaponM = (WeaponMirv *) weapon_;

	// Add a shot that will fall where the original was aimed
	Action *newShot = weaponM->getSubWeapon()->fireWeapon(
		playerId_, position, currentVelocity);
	context_->actionController.addAction(newShot);	

	// Add all of the sub warheads that have a random spread
	for (int i=0; i<weaponM->getNoWarHeads() - 1; i++)
	{
		Vector newDiff = currentVelocity;
		newDiff[2] = 0.0f;
		if (weaponM->getSpread())
		{
			Vector diff = newDiff;
			diff[2] -= 1.0f;
			Vector perp = newDiff * diff;

			newDiff += (perp * ((RAND * 1.0f) - 0.5f));
		}
		newDiff[2] += (float(i - (weaponM->getNoWarHeads() / 2)) / 
			float(weaponM->getNoWarHeads() / 2)) * 5.0f;

		Action *newShot = weaponM->getSubWeapon()->fireWeapon(
			playerId_, position, newDiff);
		context_->actionController.addAction(newShot);	
	}

	ShotProjectile::collision(position);
}
