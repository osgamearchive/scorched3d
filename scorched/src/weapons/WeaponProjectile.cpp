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


// WeaponProjectile.cpp: implementation of the WeaponProjectile class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/WeaponProjectile.h>
#include <actions/ShotProjectileExplosion.h>
#include <tank/TankContainer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponProjectile::WeaponProjectile(char *name, int price, 
								   int bundle, int armsLevel, int size)
	: Weapon(name, price, bundle, armsLevel), size_(size)
{

}

WeaponProjectile::~WeaponProjectile()
{

}

Action *WeaponProjectile::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileExplosion(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, (float) size_);

		return action;
	}

	return 0;
}

const char *WeaponProjectile::getFiredSound()
{
	if (size_ < 5)
	{
		return PKGDIR "data/wav/shoot/small.wav";
	}
	else if (size_ < 10)
	{
		return PKGDIR "data/wav/shoot/medium.wav";
	}
	else if (size_ < 15)
	{
		return PKGDIR "data/wav/shoot/large.wav";
	}
	return PKGDIR "data/wav/shoot/xlarge.wav";
}

const char *WeaponProjectile::getExplosionSound()
{
	if (size_ < 5)
	{
		return PKGDIR "data/wav/explosions/small.wav";
	}
	else if (size_ < 10)
	{
		return PKGDIR "data/wav/explosions/medium.wav";
	}
	else if (size_ < 15)
	{
		return PKGDIR "data/wav/explosions/large.wav";
	}
	return PKGDIR "data/wav/explosions/xlarge.wav";
}
