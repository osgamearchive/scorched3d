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


// WeaponMirv.cpp: implementation of the WeaponMirv class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/WeaponMirv.h>
#include <tank/TankContainer.h>
#include <actions/ShotProjectileMirv.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeaponMirv::WeaponMirv(char *name, int price, int bundle, 
					   int armsLevel, int size, 
					   int numberWarheads, bool spread)
	: Weapon(name, price, bundle, armsLevel), size_(size), 
	noWarheads_(numberWarheads), spread_(spread)
{

}

WeaponMirv::~WeaponMirv()
{

}

Action *WeaponMirv::fireWeapon(unsigned int playerId)
{
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank)
	{
		Vector velocity = tank->getPhysics().getVelocityVector() *
			tank->getState().getPower();
		Action *action = new ShotProjectileMirv(
			tank->getPhysics().getTankGunPosition(), 
			velocity,
			this, playerId, (float) size_, noWarheads_, spread_);

		return action;
	}

	return 0;
}

const char *WeaponMirv::getExplosionTexture()
{
	return "exp05";
}

const char *WeaponMirv::getFiredSound()
{
	return PKGDIR "data/wav/shoot/large.wav";
}

const char *WeaponMirv::getExplosionSound()
{
	return PKGDIR "data/wav/explosions/large.wav";
}
