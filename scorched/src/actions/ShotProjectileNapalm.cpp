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

#include <actions/ShotProjectileNapalm.h>
#include <actions/Napalm.h>
#include <engine/ScorchedContext.h>
#include <weapons/WeaponNapalm.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <common/SoundStore.h>

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

	WeaponNapalm *weapon = (WeaponNapalm *) weapon_;
	for (int i=0; i<weapon->getNumberStreams(); i++)
	{
		int x = int(position[0] + RAND * 4.0f - 2.0f);
		int y = int(position[1] + RAND * 4.0f - 2.0f);
		addNapalm(x, y);
	}

	if (!context_->serverMode) 
	{
		if (weapon_->getExplosionSound())
		{
			static char soundBuffer[256];
			sprintf(soundBuffer, PKGDIR "data/wav/%s", weapon_->getExplosionSound());
			SoundBuffer *expSound = 
				SoundStore::instance()->fetchOrCreateBuffer(soundBuffer);
			expSound->play();
		}
	}
}

void ShotProjectileNapalm::addNapalm(int x, int y)
{
	// Ensure that the napalm has not hit the walls
	// or anything outside the landscape
	if (x > 1 && y > 1 &&
		x < context_->landscapeMaps.getHMap().getWidth() - 1 &&
		y < context_->landscapeMaps.getHMap().getWidth() - 1)
	{
		context_->actionController.addAction(
			new Napalm(x, y, weapon_, playerId_));
	}
}
