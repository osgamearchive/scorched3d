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


#include <server/ServerBuyAccessoryHandler.h>
#include <server/ServerShotHolder.h>
#include <server/ServerState.h>
#include <tank/TankContainer.h>
#include <coms/ComsBuyAccessoryMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <engine/GameState.h>
#include <weapons/AccessoryStore.h>

ServerBuyAccessoryHandler *ServerBuyAccessoryHandler::instance_ = 0;

ServerBuyAccessoryHandler *ServerBuyAccessoryHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerBuyAccessoryHandler;
	}
	return instance_;
}

ServerBuyAccessoryHandler::ServerBuyAccessoryHandler()
{
	ComsMessageHandler::instance()->addHandler(
		"ComsBuyAccessoryMessage",
		this);
}

ServerBuyAccessoryHandler::~ServerBuyAccessoryHandler()
{
}

bool ServerBuyAccessoryHandler::processMessage(NetPlayerID &id,
	const char *messageType,
										  NetBufferReader &reader)
{
	ComsBuyAccessoryMessage message;
	if (!message.readMessage(reader)) return false;

	// Check we are at the correct time to buy anything
	if (GameState::instance()->getState() != ServerState::ServerStatePlaying ||
		OptionsTransient::instance()->getCurrentGameNo() != 1)
	{
		return true;
	}

	// Check we are in the correct round no to buy anything
	int roundsPlayed = OptionsGame::instance()->getNoRounds() -
		OptionsTransient::instance()->getNoRoundsLeft();
	if (OptionsGame::instance()->getBuyOnRound() - 1 >= roundsPlayed)
	{
		return true;
	}

	// Check that is player still exists
	unsigned int playerId = (unsigned int) id;
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (!tank) return true;

	// Check this player is alive
	if (tank->getState().getState() != TankState::sNormal) return true;

	// Check this player has not already given a move
	if (ServerShotHolder::instance()->haveShot(playerId)) return true;

	// Check that the accessory is valid
	Accessory *accessory = 
		AccessoryStore::instance()->findByAccessoryName(message.getAccessoryName());
	if (!accessory) return true;
	if (10 - accessory->getArmsLevel() > 
		OptionsGame::instance()->getMaxArmsLevel()) return true;

	// The game state and everything is correct
	// Perform the actual add or remove of accessory
	if (message.getBuy())
	{
		if (tank->getScore().getMoney() < accessory->getPrice()) return true;

		// Add the accessory
		tank->getAccessories().add(accessory);
		tank->getScore().setMoney(
			tank->getScore().getMoney() - accessory->getPrice());
	}
	else
	{
		if (tank->getAccessories().getAccessoryCount(accessory) <= 0) return true;

		// Remove the accessory
		tank->getAccessories().rm(accessory);
		tank->getScore().setMoney(
			tank->getScore().getMoney() + accessory->getSellPrice());
	}

	ComsMessageSender::sendToAllPlayingClients(message);
	return true;
}
