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

#include <actions/ShotProjectileRoller.h>
#include <actions/Roller.h>
#include <weapons/WeaponRoller.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>

REGISTER_ACTION_SOURCE(ShotProjectileRoller);

ShotProjectileRoller::ShotProjectileRoller()
{

}

ShotProjectileRoller::ShotProjectileRoller(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId) :
	ShotProjectile(startPosition, velocity, weapon, playerId, 1)
{
}

ShotProjectileRoller::~ShotProjectileRoller()
{
}

void ShotProjectileRoller::collision(Vector &position)
{
	ShotProjectile::collision(position);

	WeaponRoller *weapon = (WeaponRoller *) weapon_;
	for (int i=0; i<weapon->getNumberRollers(); i++)
	{
		int x = int(position[0] + RAND * 4.0f - 2.0f);
		int y = int(position[1] + RAND * 4.0f - 2.0f);
		addRoller(x, y);
	}
}

void ShotProjectileRoller::addRoller(int x, int y)
{
	// Ensure that the Roller has not hit the walls
	// or anything outside the landscape
	if (x > 1 && y > 1 &&
		x < context_->landscapeMaps.getHMap().getWidth() - 1 &&
		y < context_->landscapeMaps.getHMap().getWidth() - 1)
	{
		context_->actionController.addAction(
			new Roller(x, y, weapon_, playerId_));
	}
}
