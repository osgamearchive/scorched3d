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
#include <tankai/TankAILogic.h>
#include <server/ServerShotHolder.h>
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <actions/TankSay.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <coms/ComsMessageSender.h>

TankAIComputer::TankAIComputer() : 
	primaryShot_(true), 
	availableForRandom_(true),
	useShields_(true),
	useParachutes_(true),
	useBatteries_(true)
{
}

TankAIComputer::~TankAIComputer()
{
}

void TankAIComputer::setTank(Tank *tank)
{
	tankBuyer_.setTank(tank);
	tankTarget_.setTank(tank);
	tankAim_.setTank(tank);
	TankAI::setTank(tank);
}

bool TankAIComputer::parseConfig(AccessoryStore &store, XMLNode *node)
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

	// Defenses
	bool noDefenses;
	if (!node->getNamedChild("nodefenses", noDefenses)) return false;
	if (noDefenses)
	{
		useShields_ = false;
		useParachutes_ = false;
		useBatteries_ = false;
	}

	// Available when random is chosen
	if (!node->getNamedChild("availableforrandom", 
		availableForRandom_)) return false;

	// Aiming info
	if (!tankAim_.parseConfig(node)) return false;

	// Weapons
	if (!tankBuyer_.parseConfig(store, node)) return false;

	// Targeting
	if (!tankTarget_.parseConfig(node)) return false;

	return true;
}

void TankAIComputer::reset()
{
	tankTarget_.reset();
}

void TankAIComputer::newGame()
{
	tankAim_.newGame();
}

void TankAIComputer::tankHurt(Weapon *weapon, unsigned int firer)
{
	tankTarget_.tankHurt(weapon, firer);
	if (currentTank_->getState().getState() == TankState::sDead)
	{
		const char *line = TankAIStrings::instance()->getDeathLine();
		if (line) say(line);
	}
}

void TankAIComputer::shotLanded(ParticleAction action,
								ScorchedCollisionInfo *collision,
								Weapon *weapon, unsigned int firer, 
								Vector &position,
								unsigned int landedCounter)
{
	tankTarget_.shotLanded(action, collision, weapon, firer,
		position, landedCounter);
	if (primaryShot_ && firer == currentTank_->getPlayerId())
	{
		tankAim_.ourShotLanded(weapon, position);
		primaryShot_ = false;
	}
}

void TankAIComputer::autoDefense()
{
	raiseDefenses();
}

void TankAIComputer::buyAccessories()
{
	tankBuyer_.buyAccessories(10);

	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eFinishedBuy);
	ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
}

void TankAIComputer::selectFirstShield()
{
	if (currentTank_->getAccessories().getShields().getAllShields().size())
	{
		std::list<Accessory *> shieldList = 
			currentTank_->getAccessories().getShields().getAllShields();
		if (!shieldList.empty())
		{
			Accessory *shield = shieldList.front();
			if (!currentTank_->getAccessories().getShields().getCurrentShield())
			{
				shieldsUpDown(shield->getAccessoryId());
			}
		}
	}
}

void TankAIComputer::raiseDefenses()
{
	// Try to enable parachutes (fails if we don't have any)
	if (currentTank_->getAccessories().getParachutes().getNoParachutes() != 0 &&
		useParachutes_)
	{
		if (!currentTank_->getAccessories().getParachutes().parachutesEnabled())
		{
			parachutesUpDown(true);
		}
	}

	// Try to raise shields (fails if we don't have any)
	if (!currentTank_->getAccessories().getShields().getCurrentShield() &&
		useShields_)
	{
		if (currentTank_->getAccessories().getShields().getAllShields().size())
		{
			selectFirstShield();
		}
	}
}

void TankAIComputer::say(const char *text)
{
	std::string newText(currentTank_->getName());
	newText += ":";
	newText += text;

	ScorchedServer::instance()->getActionController().addAction(
		new TankSay(currentTank_->getPlayerId(), newText.c_str()));
}

void TankAIComputer::playMove(const unsigned state, float frameTime, 
									char *buffer, unsigned int keyState)
{
	// Play move is called when the computer opponent must make there move
	// Chooses a random weapon
	int weaponInc = int(RAND * 3.0f);
	for (int i=0; i<weaponInc; i++)
	{
		currentTank_->getAccessories().getWeapons().nextWeapon();
	}

	// Make sure defenses are raised (if we don't have an autodefense)
	raiseDefenses();

	// Use batteries if we need to and have them
	while (useBatteries_ &&
		currentTank_->getState().getLife() < 100.0f &&
		currentTank_->getAccessories().getBatteries().getNoBatteries() > 0)
	{
		useBattery();
	}

	// Is there any point in making a move
	// Done after select weapons to allow batteries to be used
	if (currentTank_->getState().getLife() < 10) 
	{
		resign();
		return;
	}

	Tank *target = tankTarget_.findTankToShootAt();
	if (target)
	{
		if (tankAim_.aimAtTank(target))
		{
			fireShot();
			return;
		}
	}

	skipShot();
}

void TankAIComputer::fireShot()
{
	primaryShot_ = true;
	Weapon *currentWeapon = 
		currentTank_->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eShot);
		message->setShot(
			currentTank_->getAccessories().getWeapons().getCurrent()->getAccessoryId(),
			currentTank_->getPhysics().getRotationGunXY(),
			currentTank_->getPhysics().getRotationGunYZ(),
			currentTank_->getPhysics().getPower());
	
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

void TankAIComputer::move(int x, int y)
{
	ComsPlayedMoveMessage *message = 
		new ComsPlayedMoveMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eMove);
	message->setPosition(x, y);
	ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
}

void TankAIComputer::parachutesUpDown(bool on)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		on?ComsDefenseMessage::eParachutesUp:ComsDefenseMessage::eParachutesDown);

	if (TankAILogic::processDefenseMessage(
		ScorchedServer::instance()->getContext(), defenseMessage, currentTank_))
	{
		ComsMessageSender::sendToAllPlayingClients(defenseMessage);
	}
}

void TankAIComputer::shieldsUpDown(unsigned int shieldId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(shieldId!=0)?ComsDefenseMessage::eShieldUp:ComsDefenseMessage::eShieldDown,
		shieldId);

	if (TankAILogic::processDefenseMessage(
		ScorchedServer::instance()->getContext(), defenseMessage, currentTank_))
	{
		ComsMessageSender::sendToAllPlayingClients(defenseMessage);
	}
}

void TankAIComputer::useBattery()
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		ComsDefenseMessage::eBatteryUse);

	if (TankAILogic::processDefenseMessage(
		ScorchedServer::instance()->getContext(), defenseMessage, currentTank_))
	{
		ComsMessageSender::sendToAllPlayingClients(defenseMessage);
	}
}
