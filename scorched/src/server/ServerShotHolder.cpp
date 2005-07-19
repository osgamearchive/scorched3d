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

#include <server/ScorchedServer.h>
#include <server/ServerShotHolder.h>
#include <server/TurnController.h>
#include <tank/TankContainer.h>
#include <tankai/TankAIStrings.h>
#include <tankai/TankAIComputer.h>
#include <engine/ActionController.h>
#include <actions/TankMovement.h>
#include <actions/TankResign.h>
#include <actions/TankFired.h>
#include <landscape/LandscapeMaps.h>
#include <weapons/AccessoryStore.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsPlayerStatusMessage.h>

ServerShotHolder *ServerShotHolder::instance_ = 0;

ServerShotHolder *ServerShotHolder::instance()
{
	if (!instance_)
	{
		instance_ = new ServerShotHolder;
	}
	return instance_;
}

ServerShotHolder::ServerShotHolder()
{
}

ServerShotHolder::~ServerShotHolder()
{
}

void ServerShotHolder::clearShots()
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor;
	for (itor = messages_.begin();
		itor != messages_.end();
		itor++)
	{
		delete (*itor).second;
	}
	messages_.clear();
}

void ServerShotHolder::addShot(unsigned int playerId,
							   ComsPlayedMoveMessage *message)
{
	// Ensure each player can only add one message
	if (!haveShot(playerId))
	{
		messages_[playerId] = message;

		// Tell the client who we are currently waiting on
		ComsPlayerStatusMessage statusMessage;
		std::list<unsigned int> &tanks = 
			TurnController::instance()->getPlayersThisTurn();
		std::list<unsigned int>::iterator itor;
		for (itor = tanks.begin();
			 itor != tanks.end();
			 itor++)
		{
			unsigned int playerId = (*itor);
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().getTankById(playerId);
			if (tank && tank->getState().getState() == TankState::sNormal)
			{
				if (!haveShot(tank->getPlayerId())) 
					statusMessage.getWaitingPlayers().push_back(playerId);
			}
		}
		ComsMessageSender::sendToAllPlayingClients(statusMessage);
	}
}

bool ServerShotHolder::haveShot(unsigned int playerId)
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor =
		messages_.find(playerId);
	return (itor != messages_.end());
}

bool ServerShotHolder::haveAllTurnShots()
{
	std::list<unsigned int> &tanks = 
		TurnController::instance()->getPlayersThisTurn();
	std::list<unsigned int>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		unsigned int playerId = (*itor);
		Tank *tank = 
			ScorchedServer::instance()->getTankContainer().getTankById(playerId);
		if (tank && tank->getState().getState() == TankState::sNormal)
		{
			if (!haveShot(tank->getPlayerId())) return false;
		}
	}
	return true;
}

void ServerShotHolder::playShots(bool roundStart)
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor;
	for (itor = messages_.begin();
		itor != messages_.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		ComsPlayedMoveMessage *message = (*itor).second;

		// Check the tank exists for this player
		// It may not if the player has left the game after firing.
		Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(playerId);
		if (tank)
		{
			// This tank has now made a move, reset its missed move counter
			tank->getScore().setMissedMoves(0);

			// Actually play the move
			processPlayedMoveMessage(
				ScorchedServer::instance()->getContext(),
				*message, tank, roundStart);
		}
	}
}

void ServerShotHolder::processPlayedMoveMessage(ScorchedContext &context, 
	ComsPlayedMoveMessage &message, Tank *tank, bool roundStart)
{
	if (roundStart)
	{
		// All actions that are done at the very START of a new round
		switch (message.getType())
		{
			case ComsPlayedMoveMessage::eShot:
				processFiredMessage(context, message, tank);
				break;
			case ComsPlayedMoveMessage::eSkip:
				// Just do nothing as the player has requested
				// That they skip their move
				break;
			case ComsPlayedMoveMessage::eFinishedBuy:
				// Just used as a notification that the player
				// has finished buying, do nothing
				break;
			case ComsPlayedMoveMessage::eResign:
				if (context.optionsGame->getResignMode() == OptionsGame::ResignStart)
				{
					processResignMessage(context, message, tank);
				}
				else if (context.optionsGame->getResignMode() == OptionsGame::ResignDueToHealth)
				{
					if (RAND * 100.0f <= tank->getState().getLife())
					{
						processResignMessage(context, message, tank);
					}
				}
				break;
			case ComsPlayedMoveMessage::eMove:
				processMoveMessage(context, message, tank);
				break;
			default:
				// add other START round types
				break;
		}
	}
	else
	{
		// All actions that are done at the very END of a round
		switch (message.getType())
		{
			case ComsPlayedMoveMessage::eResign:
				if (context.optionsGame->getResignMode() == OptionsGame::ResignEnd ||
					context.optionsGame->getResignMode() == OptionsGame::ResignDueToHealth)
				{
					processResignMessage(context, message, tank);
				}
				break;
			default:
				// Add other END round types
				break;
		}
	}
}

void ServerShotHolder::processMoveMessage(ScorchedContext &context, 
	ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the is alive
	if (tank->getState().getState() != TankState::sNormal) return;

	int posX = message.getPositionX();
	int posY = message.getPositionY();
	if (posX > 0 && posX < context.landscapeMaps->getHMap().getWidth() &&
		posY > 0 && posY < context.landscapeMaps->getHMap().getWidth())
	{
		context.landscapeMaps->getMMap().calculateForTank(tank, context);
		MovementMap::MovementMapEntry entry =
			context.landscapeMaps->getMMap().getEntry(posX, posY);
		if (entry.type == MovementMap::eMovement &&
			entry.dist < tank->getAccessories().getFuel().getNoFuel())
		{
			TankMovement *move = 
				new TankMovement(tank->getPlayerId(), posX, posY);
			context.actionController->addAction(move);
		}
	}
}

void ServerShotHolder::processResignMessage(ScorchedContext &context, 
	ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the tank is alive
	if (tank->getState().getState() == TankState::sNormal)
	{
		// Tank resign action
		TankResign *resign = new TankResign(tank->getPlayerId());
		context.actionController->addAction(resign);

		StatsLogger::instance()->tankResigned(tank);
	}
}

void ServerShotHolder::processFiredMessage(ScorchedContext &context, 
	ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the tank is alive
	if (tank->getState().getState() == TankState::sNormal)
	{
		// Check the weapon name exists and is a weapon
		Accessory *accessory = 
			context.accessoryStore->findByAccessoryId(
			message.getWeaponId());
		if (accessory && accessory->getType() == AccessoryPart::AccessoryWeapon)
		{
			// Check this tank has these weapons
			int count = 
				tank->getAccessories().getWeapons().getWeaponCount(accessory);
			if (count > 0 || count == -1)
			{
				if ((10 - accessory->getArmsLevel()) <=
					context.optionsTransient->getArmsLevel() ||
					context.optionsGame->getGiveAllWeapons())
				{
					// Actually use up one of the weapons
					tank->getAccessories().getWeapons().rmWeapon(accessory, 1);

					// shot fired action
					Weapon *weapon = (Weapon *) accessory->getAction();
					TankFired *fired = new TankFired(tank->getPlayerId(), 
						weapon,
						message.getRotationXY(), message.getRotationYZ());
					context.actionController->addAction(fired);

					// Set the tank to have the correct rotation etc..
					tank->getPhysics().rotateGunXY(
						message.getRotationXY(), false);
					tank->getPhysics().rotateGunYZ(
						message.getRotationYZ(), false);
					tank->getPhysics().changePower(
						message.getPower(), false);

					// Create the action for the weapon and
					// add it to the action controller
					Vector velocity = tank->getPhysics().getVelocityVector() *
						(tank->getPhysics().getPower() + 1.0f);
					Vector position = tank->getPhysics().getTankGunPosition();

					weapon->fireWeapon(context, tank->getPlayerId(), position, velocity, 0);
					StatsLogger::instance()->tankFired(tank, weapon);
					StatsLogger::instance()->weaponFired(weapon, false);

					// Does a computer tank want to say something?
					if (tank->getDestinationId() == 0 && tank->getTankAI())
					{
						const char *line = TankAIStrings::instance()->getAttackLine();
						if (line)
						{
							((TankAIComputer*)tank->getTankAI())->say(line);
						}
					}
				}
			}
		}
	}
}

