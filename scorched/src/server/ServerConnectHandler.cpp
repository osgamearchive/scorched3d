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


#include <server/ServerConnectHandler.h>
#include <scorched/ServerDialog.h>
#include <server/ServerState.h>
#include <engine/GameState.h>
#include <tank/TankContainer.h>
#include <tank/TankColorGenerator.h>
#include <common/Defines.h>
#include <common/FileLines.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsConnectAcceptMessage.h>
#include <coms/ComsConnectMessage.h>
#include <coms/ComsMessageSender.h>

ServerConnectHandler *ServerConnectHandler::instance_ = 0;

ServerConnectHandler *ServerConnectHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerConnectHandler;
	}
	return instance_;
}

ServerConnectHandler::ServerConnectHandler()
{
	ComsMessageHandler::instance()->addHandler(
		"ComsConnectMessage",
		this);
}

ServerConnectHandler::~ServerConnectHandler()
{
}

bool ServerConnectHandler::processMessage(NetPlayerID &id,
	const char *messageType,
										  NetBufferReader &reader)
{
	// Decode the connect message
	ComsConnectMessage message;
	if (!message.readMessage(reader)) return false;

	// Check this client has not tried to add a tank before
	unsigned int tankId = (unsigned int) id;
	if (TankContainer::instance()->getTankById(tankId))
	{
		Logger::log(0, "Duplicate tank creation attempted \"%i\"", tankId);
		return true;
	}


	// Form the correct player name
	// Remove spaces from the front of the name and
	// unwanted characters from middle
	char *nameBeginning = (char *) message.getPlayerName();
	while (*nameBeginning == ' ') nameBeginning++;
	char *nameMiddle = nameBeginning;
	for (;*nameMiddle; nameMiddle++)
	{
		if (*nameMiddle == '\"') *nameMiddle = '\'';
		else if (*nameMiddle == ']') *nameMiddle = ')';
		else if (*nameMiddle == '[') *nameMiddle = '(';
	}

	// Check the client provides a name with a least 1 char in it
	int nameLen = strlen(nameBeginning);
	if (nameLen == 0)
	{
		sendString(id, "A player name must be provided");
		Logger::log(0, "Player connected with out a name");
		kickPlayer(id);
		return true;
	}
	std::string playerName;
	if (nameLen < 16) playerName = nameBeginning;
	else playerName.append(nameBeginning, 16);

	// Check the player protocol versions are the same (correct)
	if (0 != strcmp(message.getProtocolVersion(), ScorchedProtocolVersion))
	{
		sendString(id, 			
			"The version of Scorched you are running\n"
			"does not match the server's version.\n"
			"This server is running Scorched build %s (%s).\n"
			"You are running Scorched build %s (%s).\n"
			"New versions can be downloaded from\n"
			"http://www.scorched3d.co.uk", 
			ScorchedVersion, ScorchedProtocolVersion,
			message.getVersion(), message.getProtocolVersion());
		Logger::log(0, 
			"Player \"%s\" connected with out of date version \"%s(%s)\"",
			playerName.c_str(),
			message.getVersion(),
			message.getProtocolVersion());

		kickPlayer(id);
		return true;
	}

	// Check for a password (if any)
	if (OptionsGame::instance()->getServerPassword()[0])
	{
		if (0 != strcmp(message.getPassword(),
						OptionsGame::instance()->getServerPassword()))
		{
			sendString(id, 			
					   "This server is running a password protected game."
					   "Your supplied password does not match");
			Logger::log(0, 
						"Player \"%s\" connected with an invalid password",
						playerName.c_str());
			
			kickPlayer(id);
			return true;
		}
	}

	// Make sure no-one has the same name
	while (TankContainer::instance()->getTankByName(playerName.c_str()))
	{
		playerName += "(2)";
	}

	// Form the MOTD (Message of the day)
	std::string motd;
	FileLines motdFile;
	motdFile.readFile(PKGDIR "data/motd.txt");
	motdFile.getAsOneLine(motd);

	// Send the connection accepted message to the client
	ComsConnectAcceptMessage acceptMessage(
		tankId,
		OptionsGame::instance()->getServerName(),
		motd.c_str());
	if (!ComsMessageSender::sendToSingleClient(acceptMessage, id))
	{
		Logger::log(0,
				  "Failed to send accept to client \"%i\"",
				  id);
		kickPlayer(id);
		return true;
	}

	// Tell the client about the current state of play
	// (e.g.) how many players how long to wait etc...
	if (GameState::instance()->getState() == ServerState::ServerStateWaitingForPlayers &&
		(OptionsGame::instance()->getNoMinPlayers() > TankContainer::instance()->getNoOfTanks() + 1))
	{
		sendString(id,
			"--------------------------------------\n"
			"This server currently has %i players out\n"
			"of a maximum of %i players.\n"
			"This server will only start a game when\n"
			"there are %i or more players.\n"
			"%i more player(s) are required before a \n"
			"game will start.\n"
			"Waiting for more players to join...\n"
			"--------------------------------------\n",
			TankContainer::instance()->getNoOfTanks() + 1,
			OptionsGame::instance()->getNoMaxPlayers(),
			OptionsGame::instance()->getNoMinPlayers(),
			OptionsGame::instance()->getNoMinPlayers() - 
			TankContainer::instance()->getNoOfTanks() - 1);
	}
	else
	{
		sendString(id,
			"--------------------------------------\n"
			"This server currently has %i players out\n"
			"of a maximum of %i players.\n"
			"You will join the game within %i seconds...\n"
			"--------------------------------------\n",
			TankContainer::instance()->getNoOfTanks() + 1,
			OptionsGame::instance()->getNoMaxPlayers(),
			OptionsGame::instance()->getShotTime());
	}

	// The player has connected
	Vector color = TankColorGenerator::instance()->getNextColor();
	TankModelId modelId(message.getModelName());

	// Send all current tanks to the new client
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tankList = 
		TankContainer::instance()->getPlayingTanks();
	for (itor = tankList.begin();
		itor != tankList.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		ComsAddPlayerMessage oldPlayerMessage(
			tank->getPlayerId(),
			tank->getName(),
			tank->getColor(),
			tank->getModel().getModelName());
		ComsMessageSender::sendToSingleClient(oldPlayerMessage, id);
	}

	// Create the new tank and add it to the tank container
	// Collections
	Tank *tank = new Tank(
		tankId,
		playerName.c_str(),
		color,
		modelId);
	tank->setUnqiueId(message.getUniqueId());
	TankContainer::instance()->addTank(tank);

	// Add to dialog
	Logger::log(tankId, "Player connected \"%i\" \"%s\"",
		id,
		playerName.c_str());

	// Tell the clients to create this tank
	ComsAddPlayerMessage addPlayerMessage(
		tankId,
		playerName.c_str(),
		color,
		modelId.getModelName());
	ComsMessageSender::sendToAllConnectedClients(addPlayerMessage);

	return true;
}
