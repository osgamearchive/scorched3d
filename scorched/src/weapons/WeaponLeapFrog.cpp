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

#include <weapons/WeaponLeapFrog.h>
#include <actions/ShotProjectileLeapFrog.h>
#include <tank/TankContainer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponLeapFrog::WeaponLeapFrog(char *name, int price, 
		int bundle, int armsLevel, int size)
	: Weapon(name, price, bundle, armsLevel), size_(size)
{

}

WeaponLeapFrog::~WeaponLeapFrog()
{

}

Action *WeaponLeapFrog::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileLeapFrog(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, (float) size_, 2);

		return action;
	}

	return 0;
}

const char *WeaponLeapFrog::getFiredSound()
{
	return PKGDIR "data/wav/shoot/medium.wav";
}

const char *WeaponLeapFrog::getExplosionSound()
{
	return PKGDIR "data/wav/explosions/medium.wav";
}
