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

#include <landscape/GlobalHMap.h>
#include <actions/ShotProjectileNapalm.h>
#include <actions/Napalm.h>
#include <engine/ActionController.h>
#include <weapons/WeaponNapalm.h>

static const float NapalmBurnTime = 8.0f;

REGISTER_ACTION_SOURCE(ShotProjectileNapalm);

ShotProjectileNapalm::ShotProjectileNapalm()
{

}

ShotProjectileNapalm::ShotProjectileNapalm(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId) :
	ShotProjectile(startPosition, velocity, weapon, playerId, 1)
{
}

ShotProjectileNapalm::~ShotProjectileNapalm()
{
}

void ShotProjectileNapalm::collision(Vector &position)
{
	ShotProjectile::collision(position);

	addNapalm((int) position[0], (int) position[1]);
	addNapalm((int) position[0] + 2, (int) position[1] + 2);
	if (((WeaponNapalm *) weapon_)->getHot()) addNapalm((int) position[0] - 2, (int) position[1] + 2);
}

void ShotProjectileNapalm::addNapalm(int x, int y)
{
	// Ensure that the napalm has not hit the walls
	// or anything outside the landscape
	if (x > 1 && y > 1 &&
		x < GlobalHMap::instance()->getHMap().getWidth() - 1 &&
		y < GlobalHMap::instance()->getHMap().getWidth() - 1)
	{
		ActionController::instance()->addAction(
			new Napalm(x, y, NapalmBurnTime, weapon_, playerId_));
	}
}