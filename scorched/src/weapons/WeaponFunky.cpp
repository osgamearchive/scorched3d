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


// WeaponFunky.cpp: implementation of the WeaponFunky class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/WeaponFunky.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileFunky.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponFunky::WeaponFunky(char *name, int price, int bundle, int armsLevel, 
						 int size, int subsize, int warheads) :
	Weapon(name, price, bundle, armsLevel), 
	size_(size), subsize_(subsize), warheads_(warheads)
{

}

WeaponFunky::~WeaponFunky()
{

}

Action *WeaponFunky::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileFunky(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, (float) size_, (float) subsize_, warheads_);
		return action;
	}

	return 0;
}

const char *WeaponFunky::getExplosionTexture()
{
	return "exp04";
}

const char *WeaponFunky::getFiredSound()
{
	return PKGDIR "data/wav/shoot/large.wav";
}

const char *WeaponFunky::getExplosionSound()
{
	return PKGDIR "data/wav/explosions/large.wav";
}
