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
#include <scorched/ServerDialog.h>
#include <common/OptionsParam.h>
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

bool ServerAddPlayerHandler::processMessage(unsigned int id,
	const char *messageType, NetBufferReader &reader)
{
	ComsAddPlayerMessage message;
	if (!message.readMessage(reader)) return false;

	// Validate player
	unsigned int playerId = message.getPlayerId();
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(playerId);
	if (!tank || !tank->getState().getSpectator()) return true;

	// Add a computer player (if chosen and a single player match)
	if (0 != strcmp(message.getPlayerType(), "Human"))
	{
		if (OptionsParam::instance()->getDedicatedServer()) return true;

		TankAIComputer *ai = 
			TankAIStore::instance()->getAIByName(message.getPlayerType());
		if (ai)
		{
			tank->setTankAI(ai->getCopy(tank, &ScorchedServer::instance()->getContext()));
			tank->setDestinationId(0);

			// A hacky case check for when we have added all bots
			// and NO human players
			// In this case we need to add a spectator, otherwise no messages
			// will be sent to the client
			if (ScorchedServer::instance()->getTankContainer().getNoOfTanks() >=
				ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers())
			{
				bool allBots = true;
				std::map<unsigned int, Tank *> &tanks = 
					ScorchedServer::instance()->getTankContainer().getPlayingTanks();
				std::map<unsigned int, Tank *>::iterator itor;
				for (itor = tanks.begin();
					itor != tanks.end();
					itor++)
				{
					Tank *tank = (*itor).second;
					if (tank->getDestinationId() != 0) allBots = false;
				}
				if (allBots)
				{
					ScorchedServer::instance()->getOptionsGame().setNoMaxPlayers(
						ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers()+1);
					ServerConnectHandler::instance()->addNextTank(
						NetLoopBack::ClientLoopBackID,
						"Spectator",
						"Random",
						"",
						true);
				}
			}
		}
	}

	// Setup the new player
	std::string name(message.getPlayerName());
	getUniqueName(name);
	TankModelId modelId(message.getModelName());
	tank->setName(name.c_str());
	tank->setModel(modelId);
	tank->getState().setSpectator(false);

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

	// Tell this computer that a new tank has connected
	sendString(0, "Player playing \"%s\"->\"%s\"",
		tank->getName(), name.c_str());

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
	}

	// Check the client provides a name with a least 1 char in it
	// and the name is less than 16 chars
	std::string playerName;
	int nameLen = strlen(nameBeginning);
	if (nameLen == 0) playerName = "NoName";
	else if (nameLen < 16) playerName = nameBeginning;
	else playerName.append(nameBeginning, 16);

	// Make sure no-one has the same name
	while (ScorchedServer::instance()->getTankContainer().getTankByName(playerName.c_str()))
	{
		playerName += "(2)";
	}
	
	sentname = playerName;
}
