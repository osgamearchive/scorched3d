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

#include <server/ServerAddPlayerHandler.h>
#include <server/ScorchedServer.h>
#include <server/ServerConnectHandler.h>
#include <server/ServerState.h>
#include <server/ServerCommon.h>
#include <common/OptionsParam.h>
#include <common/StatsLogger.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetLoopBack.h>
#include <tankai/TankAIStore.h>

ServerAddPlayerHandler *ServerAddPlayerHandler::instance_ = 0;

ServerAddPlayerHandler *ServerAddPlayerHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerAddPlayerHandler;
	}
	return instance_;
}

ServerAddPlayerHandler::ServerAddPlayerHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsAddPlayerMessage",
		this);
}

ServerAddPlayerHandler::~ServerAddPlayerHandler()
{
}

bool ServerAddPlayerHandler::processMessage(unsigned int destinationId,
	const char *messageType, NetBufferReader &reader)
{
	ComsAddPlayerMessage message;
	if (!message.readMessage(reader)) return false;

	// Validate player
	unsigned int playerId = message.getPlayerId();
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(playerId);
	if (!tank || (tank->getState().getState() != TankState::sDead))
	{
		ServerCommon::sendString(destinationId, "Can only change tank when dead.");
		return true;
	}

	// Add a computer player (if chosen and a single player match)
	if (0 != strcmp(message.getPlayerType(), "Human"))
	{
		if (ScorchedServer::instance()->getGameState().getState() !=
			ServerState::ServerStateTooFewPlayers)
		{
			ServerCommon::sendString(destinationId, "Can only change type before game starts.");
			return true;
		}

		// Only allow this on a single player game
		if (OptionsParam::instance()->getDedicatedServer()) return true;

		// Check tank ai is valid
		TankAIComputer *ai = 
			TankAIStore::instance()->getAIByName(message.getPlayerType());
		if (!ai) return true;

		// Set the tank to have the ai
		tank->setTankAI(ai->getCopy(tank, &ScorchedServer::instance()->getContext()));
		tank->setDestinationId(0);
	}
	else
	{
		tank->setDestinationId(destinationId);
		tank->setTankAI(0);
	}

	// Setup the new player
	std::string name(message.getPlayerName());
	if (name != tank->getName()) getUniqueName(name);

	// Tell this computer that a new tank has connected
	ServerCommon::sendString(0, "Player playing \"%s\"->\"%s\"",
		tank->getName(), name.c_str());

	TankModelId modelId(message.getModelName());
	tank->setName(name.c_str());
	tank->setModel(modelId);
	tank->getState().setSpectator(false);
	StatsLogger::instance()->tankJoined(tank);

	// Choose a team (if applicable)
	if (ScorchedServer::instance()->getOptionsGame().getTeams() > 1)
	{
		if (message.getPlayerTeam() > 0 && message.getPlayerTeam() <=
			(unsigned int) ScorchedServer::instance()->getOptionsGame().getTeams())
		{
			tank->setTeam(message.getPlayerTeam());
		}
		else
		{
			tank->setTeam(ScorchedServer::instance()->getOptionsTransient().getLeastUsedTeam(
				ScorchedServer::instance()->getTankContainer()));
		}
	}

	// If we are in a waiting for players state then we can
	// send the state of these new players
	if (ScorchedServer::instance()->getGameState().getState() == 
		ServerState::ServerStateTooFewPlayers)
	{
		ComsGameStateMessage message;
		ComsMessageSender::sendToAllPlayingClients(message);
	}
	return true;
}

void ServerAddPlayerHandler::getUniqueName(std::string &sentname)
{
	// Form the correct player name
	// Remove spaces from the front of the name and
	// unwanted characters from middle
	char *nameBeginning = (char *) sentname.c_str();
	while (*nameBeginning == ' ') nameBeginning++;
	char *nameMiddle = nameBeginning;
	for (;*nameMiddle; nameMiddle++)
	{
		if (*nameMiddle == '\"') *nameMiddle = '\'';
		else if (*nameMiddle == ']') *nameMiddle = ')';
		else if (*nameMiddle == '[') *nameMiddle = '(';
		else if (*nameMiddle == '%') *nameMiddle = ' '; // Save problems with special chars
	}

	// Check the client provides a name with a least 1 char in it
	// and the name is less than 16 chars
	std::string playerName;
	int nameLen = strlen(nameBeginning);
	if (nameLen == 0) playerName = "NoName";
	else if (nameLen < 22) playerName = nameBeginning;
	else playerName.append(nameBeginning, 22);

	// Make sure no-one has the same name
	while (ScorchedServer::instance()->getTankContainer().getTankByName(playerName.c_str()))
	{
		playerName += "(2)";
	}
	
	sentname = playerName;
}
