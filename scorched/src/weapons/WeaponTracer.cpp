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


// WeaponTracer.cpp: implementation of the WeaponTracer class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/WeaponTracer.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileTracer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponTracer::WeaponTracer(char *name, int price, int bundle, 
						   int armsLevel, bool showShotPath) :
	Weapon(name, price, bundle, armsLevel), 
	showShotPath_(showShotPath)
{

}

WeaponTracer::~WeaponTracer()
{

}


Action *WeaponTracer::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileTracer(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, showShotPath_);
		return action;
	}

	return 0;
}

const char *WeaponTracer::getFiredSound()
{
	return PKGDIR "data/wav/shoot/flare.wav";
}
