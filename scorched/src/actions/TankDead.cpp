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
#include <actions/CameraPositionAction.h>
#include <actions/Explosion.h>
#include <sprites/TankDeadRenderer.h>
#include <weapons/AccessoryStore.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <tank/TankContainer.h>

REGISTER_ACTION_SOURCE(TankDead);

TankDead::TankDead() : firstTime_(true)
{
}

TankDead::TankDead(Weapon *weapon, unsigned int killedPlayerId,
				   unsigned int firedPlayerId,
				   unsigned int data) :
	weapon_(weapon), firstTime_(true),
	killedPlayerId_(killedPlayerId), firedPlayerId_(firedPlayerId),
	data_(data)
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
			context_->tankContainer->getTankById(killedPlayerId_);
		Tank *firedTank = 
			context_->tankContainer->getTankById(firedPlayerId_);

		if (killedTank)
		{
			// Point the action camera at this event
			if (context_->serverMode)
			{
				const float ShowTime = 5.0f;
				ActionMeta *pos = new CameraPositionAction(
					killedTank->getPhysics().getTankPosition(), ShowTime,
					20);
				context_->actionController->getBuffer().serverAdd(
					context_->actionController->getActionTime() - 4.0f,
					pos);
				delete pos;
			}

			// Print the banner on who killed who
			if (firedTank)
			{
				if (!context_->serverMode ||
					OptionsParam::instance()->getDedicatedServer())
				{
					if (killedPlayerId_ == firedPlayerId_)
					{
						StatsLogger::instance()->
							tankSelfKilled(firedTank, weapon_);
						StatsLogger::instance()->
							weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));
						Logger::log(0,
							"\"%s\" killed self with a \"%s\"",
							killedTank->getName(),
							weapon_->getName());
					}
					else if ((context_->optionsGame->getTeams() > 1) &&
							(firedTank->getTeam() == killedTank->getTeam())) 
					{
						StatsLogger::instance()->
							tankTeamKilled(firedTank, killedTank, weapon_);
						StatsLogger::instance()->
							weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));
						Logger::log(0,
								"\"%s\" team killed \"%s\" with a \"%s\"",
								firedTank->getName(),
								killedTank->getName(),
								weapon_->getName());
					}
					else
					{
						StatsLogger::instance()->
							tankKilled(firedTank, killedTank, weapon_);
						StatsLogger::instance()->
							weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));
						Logger::log(0,
								"\"%s\" killed \"%s\" with a \"%s\"",
								firedTank->getName(),
								killedTank->getName(),
								weapon_->getName());
					}
				}
			}
			else if (firedPlayerId_ == 0)
			{
				if (!context_->serverMode ||
					OptionsParam::instance()->getDedicatedServer())
				{
					Tank firedTank(*context_, 0, 0, "Environment", 
						Vector::nullVector, killedTank->getModel());
					firedTank.setUnqiueId("Environment");
					StatsLogger::instance()->
						tankKilled(&firedTank, killedTank, weapon_); 
					StatsLogger::instance()->
						weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));
					Logger::log(0,
							"\"%s\" killed \"%s\" with a \"%s\"",
							firedTank.getName(),
							killedTank->getName(),
							weapon_->getName());
				}
			}

			if (!context_->serverMode) 
			{
				context_->actionController->addAction(
					new SpriteAction(
						new TankDeadRenderer(
							weapon_, killedPlayerId_, firedPlayerId_)));
			}

			// Add the tank death explosion
			// Make the tank explode in one of many ways
			Weapon *weapon = context_->accessoryStore->getDeathAnimation();
			if (weapon)
			{
				Vector position = killedTank->getPhysics().getTankPosition();
				Vector velocity;
				weapon->fireWeapon(*context_, firedPlayerId_, 
					position, velocity, Weapon::eDataDeathAnimation);
				StatsLogger::instance()->weaponFired(weapon, true);
			}
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool TankDead::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(killedPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(data_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool TankDead::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(killedPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	return true;
}
