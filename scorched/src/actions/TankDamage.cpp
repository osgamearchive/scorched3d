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
#include <actions/TankDead.h>
#include <actions/TankFalling.h>
#include <actions/TankScored.h>
#include <actions/CameraPositionAction.h>
#include <common/OptionsGame.h>
#include <weapons/AccessoryStore.h>
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
		float damage, bool useShieldDamage,
		unsigned int data) :
	weapon_(weapon), firstTime_(true),
	damagedPlayerId_(damagedPlayerId), firedPlayerId_(firedPlayerId),
	damage_(damage), useShieldDamage_(useShieldDamage),
	data_(data)
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

		// Find the tank that has been damaged
		Tank *damagedTank = 
			context_->tankContainer->getTankById(damagedPlayerId_);
		if (damagedTank)
		{
			if (damagedTank->getState().getState() == TankState::sNormal)
			{
				// Remove any damage from shield first
				if (damage_ > 0.0f)
				{
					float shieldDamage = 0.0f;
					Accessory *sh = damagedTank->getAccessories().getShields().getCurrentShield();
					if (sh && useShieldDamage_)
					{
						// TODO: Hmm should different shield types and sizes take damage
						// at different rates
						float power = damagedTank->getAccessories().getShields().getShieldPower();
						if (power < damage_)
						{
							power = 0.0f;
							damage_ -= power;
						}
						else
						{
							power -= damage_;
							damage_ = 0.0f;
						}

						damagedTank->getAccessories().getShields().setShieldPower(power);
					}
				}

				// Remove the remaining damage from the tank
				if (damage_ > 0.0f)
				{
					// Remove the life
					if (damage_ > damagedTank->getState().getLife()) damage_ = 
						damagedTank->getState().getLife();
					damagedTank->getState().setLife(damagedTank->getState().getLife() - damage_);

					// Check if the tank is dead
					bool killedTank = false;
					if (damagedTank->getState().getLife() == 0.0f)
					{
						// The tank has died, make it blow up etc...
						TankDead *deadTank = 
							new TankDead(weapon_, damagedPlayerId_, firedPlayerId_, data_);
						context_->actionController->addAction(deadTank);

						// The tank is now dead
						damagedTank->getState().setState(TankState::sDead);
						killedTank = true;
					}

					// Add any score got from this endevour
					Tank *firedTank = 
						context_->tankContainer->getTankById(firedPlayerId_);
					if (firedTank)
					{	
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
				TankAI *ai = damagedTank->getTankAI();
				if (ai) ai->tankHurt(weapon_, firedPlayerId_);

				// Check if the tank needs to fall
				if (damagedTank->getState().getState() != TankState::sDead)
				{
					// The tank is not dead check if it needs to fall
					Vector &position = damagedTank->getPhysics().getTankPosition();
					if (context_->landscapeMaps->getHMap().
						getInterpHeight(position[0], position[1]) < position[2])
					{
						// Check this tank is not already falling
						std::set<unsigned int>::iterator findItor =
							TankFalling::fallingTanks.find(damagedPlayerId_);
						if (findItor == TankFalling::fallingTanks.end())
						{
							TankFalling::fallingTanks.insert(damagedPlayerId_);
							
							// Tank falling
							context_->actionController->addAction(
								new TankFalling(weapon_, damagedPlayerId_, firedPlayerId_, data_));
						}
					}
				}
			}
		}
	}

	remove = true;
	Action::simulate(frameTime, remove);
}

bool TankDamage::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(damagedPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(damage_);
	buffer.addToBuffer(useShieldDamage_);
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
	if (!reader.getFromBuffer(data_)) return false;
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;

	Tank *damagedTank = 
		context_->tankContainer->getTankById(damagedPlayerId_);
	if (damagedTank)
	{
		if (damagedTank->getState().getState() == TankState::sNormal)
		{
			const float ShowTime = 4.0f;
			ActionMeta *pos = new CameraPositionAction(
				damagedTank->getPhysics().getTankPosition(), ShowTime,
				15);
			context_->actionController->getBuffer().clientAdd(-3.0f, pos);
		}
	}

	return true;
}
