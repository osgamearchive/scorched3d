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


#include <server/ServerDefenseHandler.h>
#include <server/ServerState.h>
#include <server/ServerShotHolder.h>
#include <server/ScorchedServer.h>
#include <tank/TankContainer.h>
#include <tank/TankAILogic.h>
#include <coms/ComsDefenseMessage.h>
#include <coms/ComsMessageSender.h>
#include <weapons/AccessoryStore.h>

ServerDefenseHandler *ServerDefenseHandler::instance_ = 0;

ServerDefenseHandler *ServerDefenseHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerDefenseHandler;
	}
	return instance_;
}

ServerDefenseHandler::ServerDefenseHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsDefenseMessage",
		this);
}

ServerDefenseHandler::~ServerDefenseHandler()
{
}

bool ServerDefenseHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Decode the connect message
	ComsDefenseMessage message;
	if (!message.readMessage(reader)) return false;

	// Check we are in the correct state
	if (ScorchedServer::instance()->getGameState().getState() != ServerState::ServerStatePlaying)
	{
		return false;
	}

	// Ensure the correct player id is in the message
	unsigned int playerId = (unsigned int) id;
	message.setPlayerId(playerId);

	// Check tank exists and is alive
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(playerId);
	if (tank && tank->getState().getState() != TankState::sNormal)
	{
		return true;
	}

	// Check tank has not made move yet
	if (ServerShotHolder::instance()->haveShot(playerId))
	{
		return true;
	}

	// Check tank can perform this defense 
	// And if so actually perform the defense
	if (!TankAILogic::processDefenseMessage(
		ScorchedServer::instance()->getContext(), message, tank))
	{
		return true;
	}

	// Send this action to all clients
	ComsMessageSender::sendToAllPlayingClients(message);
	return true;
}
