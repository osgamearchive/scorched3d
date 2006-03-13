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

#include <actions/TankDamage.h>
#include <actions/TankFalling.h>
#include <actions/TankScored.h>
#include <actions/CameraPositionAction.h>
#include <sprites/TextActionRenderer.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Shield.h>
#include <landscape/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>

REGISTER_ACTION_SOURCE(TankDamage);

TankDamage::TankDamage() : firstTime_(true)
{
}

TankDamage::TankDamage(Weapon *weapon, 
		unsigned int damagedPlayerId, unsigned int firedPlayerId,
		float damage, bool useShieldDamage, bool checkFall,
		bool shieldOnlyDamage,
		unsigned int data) :
	weapon_(weapon), firstTime_(true),
	damagedPlayerId_(damagedPlayerId), firedPlayerId_(firedPlayerId),
	damage_(damage), useShieldDamage_(useShieldDamage), checkFall_(checkFall),
	data_(data), shieldOnlyDamage_(shieldOnlyDamage)
{
}

TankDamage::~TankDamage()
{
}

void TankDamage::init()
{
}

void TankDamage::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		calculateDamage();
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

void TankDamage::calculateDamage()
{
	// Find the tank that has been damaged
	Target *damagedTarget = 
		context_->targetContainer->getTargetById(damagedPlayerId_);
	if (!damagedTarget || !damagedTarget->getAlive()) return;

	// Remove any damage from shield first
	if (damage_ > 0.0f)
	{
		float shieldDamage = 0.0f;
		Accessory *sh = damagedTarget->getShield().getCurrentShield();
		if (sh && useShieldDamage_)
		{
			Shield *shield = (Shield *) sh->getAction();
			float shieldPowerRequired = 
				damage_ * shield->getHitPenetration();
			float shieldPower = 
				damagedTarget->getShield().getShieldPower();
			if (shieldPower > shieldPowerRequired)
			{
				shieldPower -= shieldPowerRequired;
				damage_ = 0.0f;
			}
			else
			{
				float p = (shieldPowerRequired - shieldPower) / shield->getHitPenetration();
				shieldPower = 0.0f;
				damage_ -= p;
			}

			damagedTarget->getShield().setShieldPower(shieldPower);
		}
	}

	// Remove the remaining damage from the tank
	if (damage_ > 0.0f && !shieldOnlyDamage_)
	{
		if (!context_->serverMode)
		{
			Vector position = damagedTarget->getTargetPosition();
			position[0] += RAND * 5.0f - 2.5f;
			position[1] += RAND * 5.0f - 2.5f;
			position[2] += RAND * 5.0f - 2.5f;

			Vector redColor(0.75f, 0.0f, 0.0f);
			context_->actionController->addAction(
				new SpriteAction(
					new TextActionRenderer(
						position,
						redColor,
						formatString("%.0f", damage_))));
		}

		// Remove the life
		if (damage_ > damagedTarget->getLife().getLife()) damage_ = 
			damagedTarget->getLife().getLife();
		damagedTarget->getLife().setLife(damagedTarget->getLife().getLife() - damage_);
		if (context_->optionsGame->getLimitPowerByHealth() &&
			!damagedTarget->isTarget())
		{
			Tank *damagedTank = (Tank *) damagedTarget;
			damagedTank->getPosition().changePower(0.0f, true);
		}

		// Check if the tank is dead
		bool killedTank = false;
		if (damagedTarget->getLife().getLife() == 0.0f)
		{
			// The tank has died, make it blow up etc.
			calculateDeath();

			if (!damagedTarget->isTarget())
			{
				// The tank is now dead
				Tank *damagedTank = (Tank *) damagedTarget;
				damagedTank->getState().setState(TankState::sDead);
			}

			killedTank = true;
		}

		// Add any score got from this endevour
		// Should always be a tank that has fired
		Tank *firedTank = 
			context_->tankContainer->getTankById(firedPlayerId_);
		if (firedTank && !damagedTarget->isTarget())
		{	
			Tank *damagedTank = (Tank *) damagedTarget;

			// Calculate the score (more for more damage and the most if you kill them)
			int score = 0;
			if (killedTank) score = context_->optionsGame->getMoneyWonPerKillPoint() *
					weapon_->getArmsLevel();
			else score = context_->optionsGame->getMoneyWonPerHitPoint() *
					weapon_->getArmsLevel();

			// Make this a percentage if you want
			if (context_->optionsGame->getMoneyPerHealthPoint()) 
				score = (score * int(damage_)) / 100;

			// Remove score for self kills
			if (damagedPlayerId_ ==  firedPlayerId_) score *= -1;
			else
			{
				if ((context_->optionsGame->getTeams() > 1) &&
					(firedTank->getTeam() == damagedTank->getTeam()))
				{
					score *= -1;
				}
			}

			// Calculate the wins
			int wins = 0;
			if (killedTank) wins = (damagedPlayerId_ ==  firedPlayerId_)?-1:1;

			// Add the new score (if any)
			if (wins != 0 || score > 0)
			{
				TankScored *scored = new TankScored(firedPlayerId_, 
					score,
					wins,
					0);
				context_->actionController->addAction(scored);
			}
		}
	}

	// Tell this tanks ai that is has been hurt by another tank
	if (!damagedTarget->isTarget())
	{
		Tank *damagedTank = (Tank *) damagedTarget;
		TankAI *ai = damagedTank->getTankAI();
		if (ai) ai->tankHurt(weapon_, firedPlayerId_);
	}

	// Check if the tank needs to fall
	if (checkFall_ && damagedTarget->getAlive())
	{
		// The tank is not dead check if it needs to fall
		Vector &position = damagedTarget->getTargetPosition();
		if (context_->landscapeMaps->getGroundMaps().
			getInterpHeight(position[0], position[1]) < position[2])
		{
			// Check this tank is not already falling
			std::map<unsigned int, TankFalling *>::iterator findItor =
				TankFalling::fallingTanks.find(damagedPlayerId_);
			if (findItor == TankFalling::fallingTanks.end())
			{
				// Tank falling
				context_->actionController->addAction(
					new TankFalling(weapon_, damagedPlayerId_, firedPlayerId_,
						damagedTarget->getParachute().parachutesEnabled(),
						data_));
			}
		}
	}

	// DO LAST
	// If the tank is a target, remove the target
	if (!damagedTarget->getAlive() &&
		damagedTarget->isTarget())
	{
		Target *removedTarget = 
			context_->targetContainer->
				removeTarget(damagedTarget->getPlayerId());
		delete removedTarget;
	}
}

void TankDamage::calculateDeath()
{
	Target *killedTarget = 
		context_->targetContainer->getTargetById(damagedPlayerId_);
	if (!killedTarget) return;

	// Log the death
	logDeath();

	// Add the tank death explosion
	// Make the tank explode in one of many ways
	Weapon *weapon = killedTarget->getDeathAction();
	if (weapon)
	{
		Vector position = killedTarget->getTargetPosition();
		Vector velocity;
		weapon->fireWeapon(*context_, firedPlayerId_, 
			position, velocity, Weapon::eDataDeathAnimation);
		StatsLogger::instance()->weaponFired(weapon, true);
	}
}

void TankDamage::logDeath()
{
	Target *killedTarget = 
		context_->targetContainer->getTargetById(damagedPlayerId_);
	if (killedTarget->isTarget()) return;

	// Print the banner on who killed who
	GLTexture *weaponTexture = 0;
	if (!context_->serverMode)
	{
		Accessory *accessory = 
			context_->accessoryStore->
			findByAccessoryId(weapon_->getParent()->getAccessoryId());
		if (accessory)
		{
			weaponTexture = accessory->getTexture();
		}
	}

	Tank *killedTank = (Tank *) killedTarget;
	Tank *firedTank = 
		context_->tankContainer->getTankById(firedPlayerId_);
	if (firedTank)
	{
		if (!context_->serverMode ||
			OptionsParam::instance()->getDedicatedServer())
		{
			if (damagedPlayerId_ == firedPlayerId_)
			{
				StatsLogger::instance()->
					tankSelfKilled(firedTank, weapon_);
				StatsLogger::instance()->
					weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));
				
				LoggerInfo info(LoggerInfo::TypeDeath,
					formatString("\"%s\" killed self with a %s",
						killedTank->getName(),
						weapon_->getParent()->getName()));
				info.setPlayerId(firedPlayerId_);
				info.setOtherPlayerId(damagedPlayerId_);
				info.setIcon(weaponTexture);
				Logger::log(info);
			}
			else if ((context_->optionsGame->getTeams() > 1) &&
					(firedTank->getTeam() == killedTank->getTeam())) 
			{
				StatsLogger::instance()->
					tankTeamKilled(firedTank, killedTank, weapon_);
				StatsLogger::instance()->
					weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));

				LoggerInfo info(LoggerInfo::TypeDeath,
					formatString("\"%s\" team killed \"%s\" with a %s",
						firedTank->getName(),
						killedTank->getName(),
						weapon_->getParent()->getName()));
				info.setPlayerId(firedPlayerId_);
				info.setOtherPlayerId(damagedPlayerId_);
				info.setIcon(weaponTexture);
				Logger::log(info);
			}
			else
			{
				StatsLogger::instance()->
					tankKilled(firedTank, killedTank, weapon_);
				StatsLogger::instance()->
					weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));

				LoggerInfo info(LoggerInfo::TypeDeath,
					formatString("\"%s\" killed \"%s\" with a %s",
					firedTank->getName(),
					killedTank->getName(),
					weapon_->getParent()->getName()));
				info.setPlayerId(firedPlayerId_);
				info.setOtherPlayerId(damagedPlayerId_);
				info.setIcon(weaponTexture);
				Logger::log(info);
			}
		}
	}
	else if (firedPlayerId_ == 0)
	{
		if (!context_->serverMode ||
			OptionsParam::instance()->getDedicatedServer())
		{
			static Tank firedTank(*context_, 0, 0, "Environment", 
				Vector::nullVector, "");
			firedTank.setUniqueId("Environment");
			StatsLogger::instance()->
				tankKilled(&firedTank, killedTank, weapon_); 
			StatsLogger::instance()->
				weaponKilled(weapon_, (data_ & Weapon::eDataDeathAnimation));

			LoggerInfo info(LoggerInfo::TypeDeath,
					formatString("\"%s\" killed \"%s\" with a \"%s\"",
				firedTank.getName(),
				killedTank->getName(),
				weapon_->getParent()->getName()));
			info.setPlayerId(firedPlayerId_);
			info.setOtherPlayerId(damagedPlayerId_);
			info.setIcon(weaponTexture);
			Logger::log(info);
		}
	}
}

bool TankDamage::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(damagedPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(damage_);
	buffer.addToBuffer(useShieldDamage_);
	buffer.addToBuffer(shieldOnlyDamage_);
	buffer.addToBuffer(checkFall_);
	buffer.addToBuffer(data_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool TankDamage::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(damagedPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	if (!reader.getFromBuffer(damage_)) return false;
	if (!reader.getFromBuffer(useShieldDamage_)) return false;
	if (!reader.getFromBuffer(shieldOnlyDamage_)) return false;
	if (!reader.getFromBuffer(checkFall_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;

	Target *damagedTarget = 
		context_->targetContainer->getTargetById(damagedPlayerId_);
	if (damagedTarget)
	{
		if (damagedTarget->getAlive())
		{
			const float ShowTime = 4.0f;
			ActionMeta *pos = new CameraPositionAction(
				damagedTarget->getTargetPosition(), ShowTime,
				15);
			context_->actionController->getBuffer().clientAdd(-3.0f, pos);
		}
	}

	return true;
}
