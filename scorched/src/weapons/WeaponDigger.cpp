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

#include <weapons/WeaponDigger.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileHog.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponDigger::WeaponDigger(char *name, int price, int bundle, 
						   int armsLevel, int warHeads) :
	Weapon(name, price, bundle, armsLevel), 
	warHeads_(warHeads)
{

}

WeaponDigger::~WeaponDigger()
{

}


Action *WeaponDigger::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileHog(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, warHeads_, false);
		return action;
	}

	return 0;
}

const char *WeaponDigger::getFiredSound()
{
	// TODO
	return PKGDIR "data/wav/shoot/flare.wav";
}
