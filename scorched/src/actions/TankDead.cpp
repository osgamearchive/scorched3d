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


#include <actions/TankDead.h>
#include <actions/TankScored.h>
#include <actions/Explosion.h>
#include <actions/ShotProjectileFunky.h>
#include <sprites/TankDeadRenderer.h>
#include <tank/TankContainer.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <common/Logger.h>

REGISTER_ACTION_SOURCE(TankDead);

TankDead::TankDead() : firstTime_(true)
{
}

TankDead::TankDead(Weapon *weapon, unsigned int killedPlayerId,
				   unsigned int firedPlayerId) :
	weapon_(weapon), firstTime_(true),
	killedPlayerId_(killedPlayerId), firedPlayerId_(firedPlayerId)
{
}

TankDead::~TankDead()
{
}

void TankDead::init()
{

}

void TankDead::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;

		Tank *killedTank = 
			TankContainer::instance()->getTankById(killedPlayerId_);
		Tank *firedTank = 
			TankContainer::instance()->getTankById(firedPlayerId_);

		if (killedTank && 
			killedTank->getState().getState() == TankState::sNormal)
		{
			// Print the banner on who killed who
			if (killedTank && firedTank)
			{
				if (killedPlayerId_ == firedPlayerId_)
				{
					Logger::log(firedTank,
						"\"%s\" killed self with a \"%s\"",
						killedTank->getName(),
						weapon_->getName());
				}
				else 
				{
					Logger::log(firedTank,
							"\"%s\" killed \"%s\" with a \"%s\"",
							firedTank->getName(),
							killedTank->getName(),
							weapon_->getName());
				}
			}

			if (!OptionsParam::instance()->getOnServer()) 
			{
				setActionRender(
					new TankDeadRenderer(
						weapon_, killedPlayerId_, firedPlayerId_));
			}

			// The tank is now dead
			killedTank->getState().setState(TankState::sDead);

			// Add / Remove the scores
			if (firedTank)
			{
				if (killedPlayerId_ ==  firedPlayerId_)
				{
					// Kiled self
					TankScored *scored = new TankScored(firedPlayerId_, 
						-OptionsGame::instance()->getMoneyWonPerHitPoint() *
						weapon_->getArmsLevel(),
						-1,
						0);
					ActionController::instance()->addAction(scored);
				}
				else
				{
					// Kiled other
					TankScored *scored = new TankScored(firedPlayerId_, 
						+OptionsGame::instance()->getMoneyWonPerHitPoint() *
						weapon_->getArmsLevel(),
						+1,
						0);
					ActionController::instance()->addAction(scored);
				}
			}

			// Add the tank death explosion
			// Make the tank explode in one of many ways
			float explosionType = RAND;
			Action *firedAction = 0;
			if (explosionType > 0.9)
			{
				Vector nullVelocity;
				Weapon *newWeapon = (Weapon *)
					AccessoryStore::instance()->findByName("Funky Bomb");
				firedAction = new ShotProjectileFunky(
					killedTank->getPhysics().getTankPosition(),
					nullVelocity,
					newWeapon,
					firedPlayerId_, 10, 8, 12);
			}
			else if (explosionType > 0.8) 
			{
				Weapon *newWeapon = (Weapon *)
					AccessoryStore::instance()->findByName("Nuke");
				firedAction = new Explosion(killedTank->getPhysics().getTankPosition(),
					18, newWeapon, firedPlayerId_, 
					true, Explosion::DeformDown);
			}
			else if (explosionType > 0.6)
			{
				Weapon *newWeapon = (Weapon *)
					AccessoryStore::instance()->findByName("Ton Of Dirt");
				firedAction = new Explosion(killedTank->getPhysics().getTankPosition(),
					10, newWeapon, firedPlayerId_, 
					false, Explosion::DeformUp);
			}
			else
			{
				Weapon *newWeapon = (Weapon *)
					AccessoryStore::instance()->findByName("Baby Nuke");
				firedAction = new Explosion(killedTank->getPhysics().getTankPosition(),
					10, newWeapon, firedPlayerId_, 
					true, Explosion::DeformDown);
			}

			ActionController::instance()->addAction(firedAction);
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool TankDead::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(killedPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool TankDead::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(killedPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}
