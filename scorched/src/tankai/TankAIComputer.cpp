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

#include <tankai/TankAIComputer.h>
#include <tankai/TankAIStrings.h>
#include <server/ServerShotHolder.h>
#include <server/ScorchedServer.h>
#include <server/ServerDefenseHandler.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankState.h>
#include <tank/TankPosition.h>
#include <tank/TankAccessories.h>
#include <target/TargetShield.h>
#include <target/TargetParachute.h>
#include <target/TargetLife.h>
#include <stdlib.h>

TankAIComputer::TankAIComputer() : 
	primaryShot_(true), 
	availableForRandom_(true),
	availableForPlayers_(true),
	useShields_(true),
	useParachutes_(true),
	useBatteries_(true),
	useResign_(true),
	tankBuyer_(0)
{
}

TankAIComputer::~TankAIComputer()
{
}

TankAIComputerBuyer *TankAIComputer::getTankBuyer()
{
	if (!tankBuyer_)
	{
		if (!tankBuyers_.empty())
		{
			tankBuyer_ = &tankBuyers_[rand() % tankBuyers_.size()];
		}
		else DIALOG_ASSERT(0);
	}
	return tankBuyer_;
}

void TankAIComputer::setTank(Tank *tank)
{
	std::vector<TankAIComputerBuyer>::iterator itor;
	for (itor = tankBuyers_.begin();
		itor != tankBuyers_.end();
		itor++)
	{
		(*itor).setTank(tank);
	}
	tankTarget_.setTank(tank);
	tankAim_.setTank(tank);
	TankAI::setTank(tank);
}

bool TankAIComputer::parseConfig(AccessoryStore *store, XMLNode *node)
{
	// Name
	std::string name;
	if (!node->getNamedChild("name", name)) return false;

	// Description
	std::string description;
	if (!node->getNamedChild("description", description)) return false;
	description_.setText(
		name.c_str(),
		description.c_str());
	
	node->getNamedChild("availableforplayers", availableForPlayers_, false);

	if (!store) return true; ////// Shallow parse

	// Defenses
	bool noDefenses;
	if (!node->getNamedChild("nodefenses", noDefenses)) return false;
	if (noDefenses)
	{
		useShields_ = false;
		useParachutes_ = false;
		useBatteries_ = false;
	}

	// No resign
	bool noResign;
	if (!node->getNamedChild("noresign", noResign)) return false;
	useResign_ = !noResign;

	// Available when random is chosen
	if (!node->getNamedChild("availableforrandom", 
		availableForRandom_)) return false;
	if (!availableForPlayers_) availableForRandom_ = false;

	// Aiming info
	if (!tankAim_.parseConfig(node)) return false;

	// Weapons
	XMLNode *weaponsNode = 0;
	while (node->getNamedChild("weaponset", weaponsNode, false))
	{
		TankAIComputerBuyer buyer;
		tankBuyers_.push_back(buyer);
		if (!tankBuyers_.back().parseConfig(*store, weaponsNode)) return false;
	}
	if (tankBuyers_.empty())
	{
		dialogExit("TankAIComputer",
			"Must have at least one weapons set defined for an ai.\n"
			"(Even if it is empty)");
	}

	// Targeting
	if (!tankTarget_.parseConfig(node)) return false;

	return node->failChildren();
}

void TankAIComputer::newMatch()
{
	//getTankBuyer()->dumpAccessories();

	tankTarget_.newMatch();
}

void TankAIComputer::newGame()
{
	tankAim_.newGame();
}

void TankAIComputer::tankHurt(Weapon *weapon, unsigned int firer)
{
	tankTarget_.tankHurt(weapon, firer);
}

void TankAIComputer::shotLanded(ScorchedCollisionId collision,
								Weapon *weapon, unsigned int firer, 
								Vector &position)
{
	tankTarget_.shotLanded(collision, weapon, firer,
		position);
	if (primaryShot_ && firer == currentTank_->getPlayerId())
	{
		Vector newPosition = position;
		if (collision == CollisionIdWallLeft ||
			collision == CollisionIdWallRight ||
			collision == CollisionIdWallTop ||
			collision == CollisionIdWallBottom)
		{
			// Pretend the shot went through the wall so 
			// the ai gets the length it would have travelled and thus
			// knows to back of the power
			newPosition =  (position - currentTank_->getPosition().getTankPosition()) / 4.0f + position;
		}
		
		tankAim_.ourShotLanded(weapon, newPosition);
		primaryShot_ = false;
	}
}

void TankAIComputer::autoDefense()
{
	raiseDefenses();
}

void TankAIComputer::buyAccessories()
{
	getTankBuyer()->buyAccessories(10);

	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eFinishedBuy);
	ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
}

void TankAIComputer::selectFirstShield()
{
	if (currentTank_->getShield().getCurrentShield()) return;

	std::list<Accessory *> &shields =
		currentTank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryShield);
	if (!shields.empty())
	{
		Accessory *shield = shields.front();
		shieldsUpDown(shield->getAccessoryId());
	}
}

void TankAIComputer::selectFirstParachute()
{
	if (currentTank_->getParachute().getCurrentParachute()) return;

	std::list<Accessory *> &parachutes =
		currentTank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryParachute);
	if (!parachutes.empty())
	{
		Accessory *parachute = parachutes.front();
		parachutesUpDown(parachute->getAccessoryId());
	}
}

void TankAIComputer::raiseDefenses()
{
	// Try to enable parachutes (fails if we don't have any)
	if (useParachutes_) selectFirstParachute();

	// Try to raise shields (fails if we don't have any)
	if (useShields_) selectFirstShield();
}

void TankAIComputer::playMove(const unsigned state, float frameTime, 
									char *buffer, unsigned int keyState)
{
	// Play move is called when the computer opponent must make there move

	// Make sure defenses are raised (if we don't have an autodefense)
	raiseDefenses();

	// Use batteries if we need to and have them
	while (useBatteries_ &&
		currentTank_->getLife().getLife() < 
		currentTank_->getLife().getMaxLife() &&
		currentTank_->getAccessories().getBatteries().getNoBatteries() != 0)
	{
		std::list<Accessory *> &entries =
			currentTank_->getAccessories().getAllAccessoriesByType(
				AccessoryPart::AccessoryBattery);			
		if (!entries.empty())
		{
			useBattery(entries.front()->getAccessoryId());
		}
	}

	// Is there any point in making a move
	// Done after select weapons to allow batteries to be used
	if (useResign_ &&
		currentTank_->getLife().getLife() < 10) 
	{
		resign();
		return;
	}

	Tank *target = tankTarget_.findTankToShootAt();
	if (target)
	{
		int noShots;
		float distance;
		bool sniper = false;

		TankAIComputerAim::AimResult 
			aimResult = tankAim_.aimAtTank(target, getTankBuyer(), 
				distance, noShots, sniper);

		if (aimResult == TankAIComputerAim::AimOk)
		{
			std::vector<Accessory *> weapons;
			if (sniper)
			{
				weapons = getTankBuyer()->getWeaponType("sniper");
			}
			if (weapons.empty())
			{
				if (distance < 15.0f && noShots > 1)
				{
					weapons = getTankBuyer()->getWeaponType("explosionlarge");
				}
			}
			if (weapons.empty())
			{
				weapons = getTankBuyer()->getWeaponType("explosionsmall");
			}

			if (!weapons.empty())
			{
				int pos = rand() % weapons.size();
				Accessory *accessory = weapons[pos];
				currentTank_->getAccessories().getWeapons().setWeapon(accessory);
			}

			if (currentTank_->getAccessories().getWeapons().getCurrent())
			{
				fireShot();
				return;
			}
		}
		else if (aimResult == TankAIComputerAim::AimBurried)
		{
			std::vector<Accessory *> digs = 
				getTankBuyer()->getWeaponType("dig");
			if (!digs.empty())
			{
				int pos = rand() % digs.size();
				Accessory *accessory = digs[pos];
				currentTank_->getAccessories().getWeapons().setWeapon(accessory);
				fireShot();
				return;
			}
		}
	}

	skipShot();
}

void TankAIComputer::fireShot()
{
	primaryShot_ = true;
	Accessory *currentWeapon = 
		currentTank_->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eShot);
		message->setShot(
			currentWeapon->getAccessoryId(),
			currentTank_->getPosition().getRotationGunXY(),
			currentTank_->getPosition().getRotationGunYZ(),
			currentTank_->getPosition().getPower(),
			currentTank_->getPosition().getSelectPositionX(),
			currentTank_->getPosition().getSelectPositionY());
	
		ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
	}
}

void TankAIComputer::skipShot()
{
	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eSkip);
	ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
}

void TankAIComputer::resign()
{
	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eResign);
	ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
}

void TankAIComputer::parachutesUpDown(unsigned int paraId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(paraId!=0)?ComsDefenseMessage::eParachutesUp:ComsDefenseMessage::eParachutesDown,
		paraId);

	ServerDefenseHandler::instance()->processDefenseMessage(defenseMessage, currentTank_);
}

void TankAIComputer::shieldsUpDown(unsigned int shieldId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(shieldId!=0)?ComsDefenseMessage::eShieldUp:ComsDefenseMessage::eShieldDown,
		shieldId);

	ServerDefenseHandler::instance()->processDefenseMessage(defenseMessage, currentTank_);
}

void TankAIComputer::useBattery(unsigned int batteryId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		ComsDefenseMessage::eBatteryUse,
		batteryId);

	ServerDefenseHandler::instance()->processDefenseMessage(defenseMessage, currentTank_);
}
