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
#include <server/ServerState.h>
#include <server/ScorchedServer.h>
#include <server/TurnController.h>
#include <scorched/ServerDialog.h>
#include <tank/TankColorGenerator.h>
#include <tankai/TankAIAdder.h>
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
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsConnectMessage",
		this);
}

ServerConnectHandler::~ServerConnectHandler()
{
}

bool ServerConnectHandler::processMessage(unsigned int destinationId,
	const char *messageType, NetBufferReader &reader)
{
	// First things, first
	// Check we can actually accept the connection
	if (ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() ==
		ScorchedServer::instance()->getTankContainer().getNoOfTanks())
	{
		sendString(destinationId, "Too many players");
		kickDestination(destinationId);
		return true;		
	}

	// Decode the connect message
	ComsConnectMessage message;
	if (!message.readMessage(reader)) return false;

	// Check the player protocol versions are the same (correct)
	if (0 != strcmp(message.getProtocolVersion(), ScorchedProtocolVersion))
	{
		sendString(destinationId, 			
			"The version of Scorched you are running\n"
			"does not match the server's version.\n"
			"This server is running Scorched build %s (%s).\n"
			"You are running Scorched build %s (%s).\n"
			"New versions can be downloaded from\n"
			"http://www.scorched3d.co.uk\n"
			"Connection failed.", 
			ScorchedVersion, ScorchedProtocolVersion,
			message.getVersion(), message.getProtocolVersion());
		Logger::log(0, 
			"ERROR: Player connected with out of date version \"%s(%s)\"",
			message.getVersion(),
			message.getProtocolVersion());

		kickDestination(destinationId);
		return true;
	}

	// Check for player numbers 
	if (message.getPlayers().empty())
	{
		sendString(destinationId, "Too little players");
		kickDestination(destinationId);
		return true;
	}

	// Check player availability
	if ((int) message.getPlayers().size() > 
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
		ScorchedServer::instance()->getTankContainer().getNoOfTanks())
	{
		sendString(destinationId, "Too many players");
		kickDestination(destinationId);
		return true;
	}

	// Check for a password (if any)
	if (ScorchedServer::instance()->getOptionsGame().getServerPassword()[0])
	{
		if (0 != strcmp(message.getPassword(),
						ScorchedServer::instance()->getOptionsGame().getServerPassword()))
		{
			sendString(destinationId, 			
					   "This server is running a password protected game.\n"
					   "Your supplied password does not match.\n"
					"Connection failed.");
			Logger::log(0, "ERROR: Player connected with an invalid password");
			
			kickDestination(destinationId);
			return true;
		}
	}

	// Check if a player from this destination has connected already
	bool found = false;
	std::map<unsigned int, Tank *> &playingTanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator playingItor;
	for (playingItor = playingTanks.begin();
		playingItor != playingTanks.end();
		playingItor++)
	{
		Tank *current = (*playingItor).second;
		if (current->getDestinationId() == destinationId)
		{
			found = true;
			break;
		}
	}
	if (found) // If so, dont allow more
	{
		Logger::log(0, "ERROR: Duplicate connection from destination \"%i\"", destinationId);
		kickDestination(destinationId);
		return true;
	}

	// Form the MOTD (Message of the day)
	std::string motd;
	FileLines motdFile;
	motdFile.readFile(PKGDIR "data/motd.txt");
	motdFile.getAsOneLine(motd);

	// Send the connection accepted message to the client
	ComsConnectAcceptMessage acceptMessage(
		destinationId,
		ScorchedServer::instance()->getOptionsGame().getServerName(),
		motd.c_str());
	if (!ComsMessageSender::sendToSingleClient(acceptMessage, destinationId))
	{
		Logger::log(0,
			"ERROR: Failed to send accept to client \"%i\"",
			destinationId);
		kickDestination(destinationId);
		return true;
	}

	// Send all current tanks to the new client
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tankList = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tankList.begin();
		itor != tankList.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		ComsAddPlayerMessage oldPlayerMessage(
			tank->getPlayerId(),
			tank->getName(),
			tank->getColor(),
			tank->getModel().getModelName(),
			tank->getDestinationId()); 
		ComsMessageSender::sendToSingleClient(oldPlayerMessage, destinationId);
	}

	// Add all the new tanks
	std::list<ComsConnectMessage::PlayerEntry> &newPlayers = 
		message.getPlayers();
	std::list<ComsConnectMessage::PlayerEntry>::iterator newPlayerItor;
	for (newPlayerItor = newPlayers.begin();
		newPlayerItor != newPlayers.end();
		newPlayerItor++)
	{
		ComsConnectMessage::PlayerEntry &newPlayer = (*newPlayerItor);
		addNextTank(destinationId,
			newPlayer.name.c_str(),
			newPlayer.model.c_str(),
			message.getUniqueId(),
			newPlayer.spectator);
	}

	return true;
}

void ServerConnectHandler::addNextTank(unsigned int destinationId,
	const char *sentPlayerName,
	const char *sentPlayerModel,
	const char *sentUniqueId,
	bool spectator)
{
	// Get the next available tankId
	unsigned int tankId = TankAIAdder::getNextTankId();
	while (ScorchedServer::instance()->getTankContainer().getTankById(tankId))
	{
		tankId = TankAIAdder::getNextTankId();
	}

	// Form the correct player name
	// Remove spaces from the front of the name and
	// unwanted characters from middle
	char *nameBeginning = (char *) sentPlayerName;
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

		// The player has connected
	Vector color = TankColorGenerator::instance()->getNextColor();
	TankModelId modelId(sentPlayerModel);

	// Create the new tank and add it to the tank container
	// Collections
	Tank *tank = new Tank(
		ScorchedServer::instance()->getContext(),
		tankId,
		destinationId,
		playerName.c_str(),
		color,
		modelId);
	tank->setUnqiueId(sentUniqueId);
	tank->getState().setSpectator(spectator);
	ScorchedServer::instance()->getTankContainer().addTank(tank);

	// Add to dialog
	Logger::log(tankId, "Player connected \"%i\" \"%s\"",
		tankId,
		playerName.c_str());

	// Tell the clients to create this tank
	ComsAddPlayerMessage addPlayerMessage(
		tank->getPlayerId(),
		tank->getName(),
		tank->getColor(),
		tank->getModel().getModelName(),
		tank->getDestinationId()); 
	ComsMessageSender::sendToAllConnectedClients(addPlayerMessage);
}
