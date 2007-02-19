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
#include <server/ServerBanned.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/TurnController.h>
#include <server/ServerCommon.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankContainer.h>
#include <tank/TankModelContainer.h>
#include <tank/TankAvatar.h>
#include <tank/TankState.h>
#include <tankai/TankAIAdder.h>
#include <tankai/TankAIStrings.h>
#include <common/Defines.h>
#include <common/FileLines.h>
#include <common/OptionsScorched.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <net/NetInterface.h>
#include <coms/ComsPlayerStateMessage.h>
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

bool ServerConnectHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType, NetBufferReader &reader)
{
	unsigned int destinationId = netMessage.getDestinationId();
	unsigned int ipAddress = netMessage.getIpAddress();
	// Only do this on the server, the client can have all bots
#ifdef S3D_SERVER
	{
		// First things, first
		// Check we can actually accept the connection
		if (ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() <=
			ScorchedServer::instance()->getTankContainer().getNoOfTanks())
		{
			const char *kickMessage =
				"--------------------------------------------------\n"
				"This server is full, you cannot join!\n"
				"--------------------------------------------------";
			ServerCommon::serverLog("Server full, kicking");
			ServerCommon::kickDestination(destinationId, kickMessage);
			return true;		
		}
	}
#endif // S3D_SERVER

	// Decode the connect message
	ComsConnectMessage message;

	// Check the player protocol versions are the same (correct)
	if (!message.readMessage(reader) ||
		(0 != strcmp(message.getProtocolVersion(), ScorchedProtocolVersion)))
	{
		const char *kickMessage = 
			formatString(
			"--------------------------------------------------\n"
			"The version of Scorched you are running\n"
			"does not match the server's version.\n"
			"This server is running Scorched build %s (%s).\n"
			"You are running Scorched build %s (%s).\n"
			"New versions can be downloaded from\n"
			"http://www.scorched3d.co.uk\n"
			"Connection failed.\n"
			"--------------------------------------------------", 
			ScorchedVersion, ScorchedProtocolVersion,
			message.getVersion(), message.getProtocolVersion());
		Logger::log( 
			formatString(
			"Player connected with out of date version \"%s(%s)\"",
			message.getVersion(),
			message.getProtocolVersion()));

		ServerCommon::kickDestination(destinationId, kickMessage);
		return true;
	}

#ifdef S3D_SERVER
	// Check for no players when a dedicated server is being used
	if (message.getNoPlayers() > 1)
	{
		ServerCommon::serverLog(
			formatString(
			"Client connecting with %u players",
			message.getNoPlayers()));
		ServerCommon::kickDestination(destinationId);
		return true;
	}
#endif // S3D_SERVER

	// Check player availability
	if (message.getNoPlayers() > 
		(unsigned int)
		(ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers() -
		ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
	{
		const char *kickMessage = 
			formatString(
			"--------------------------------------------------\n"
			"This server is full, you cannot join!\n"
			"--------------------------------------------------");
		ServerCommon::serverLog("Server full, kicking");
		ServerCommon::kickDestination(destinationId, kickMessage);
		return true;
	}

	// Check for a password (if any)
	if (ScorchedServer::instance()->getOptionsGame().getServerPassword()[0])
	{
		if (0 != strcmp(message.getPassword(),
						ScorchedServer::instance()->getOptionsGame().getServerPassword()))
		{
			const char *kickMessage = 
				"--------------------------------------------------\n"
				"This server is running a password protected game.\n"
				"Your supplied password does not match.\n"
				"Connection failed.\n"
				"--------------------------------------------------";
			Logger::log( "Player connected with an invalid password");
			
			ServerCommon::kickDestination(destinationId, kickMessage);
			return true;
		}
	}
	
	std::string uniqueId = message.getUniqueId();
	if (!uniqueId.c_str()[0]) // No ID
	{
		// Generate the players unique id (if we need to)
		//
		// This will only actualy generate an id when using a persistent
		// stats logger as this is the only time we can be sure of not
		// giving out duplicate ids.
		//
		uniqueId = StatsLogger::instance()->allocateId();
	}

	// Auth handler, make sure that only prefered players can connect
	ServerAuthHandler *authHandler =
		ScorchedServerUtil::instance()->getAuthHandler();
	if (authHandler)
	{
		std::string resultMessage;
		if (!authHandler->authenticateUser(uniqueId, 
			message.getUserName(), message.getPassword(), resultMessage))
		{
			const char *kickMessage = 
				formatString(
				"--------------------------------------------------\n"
				"%s"
				"Connection failed.\n"
				"--------------------------------------------------",
				resultMessage.c_str());
			Logger::log(formatString("User failed authentication \"%s\" [%s]",
				message.getUserName(), uniqueId.c_str()));

			ServerCommon::kickDestination(destinationId, kickMessage);			
			return true;
		}
	}

	// Check if this unique id has been banned
	if (uniqueId.c_str()[0])
	{
		ServerBanned::BannedType type = 
			ScorchedServerUtil::instance()->bannedPlayers.getBanned(ipAddress, uniqueId.c_str());
		if (type == ServerBanned::Banned)
		{
			Logger::log(formatString("Banned uniqueid connection from destination \"%i\"", 
				destinationId));
			ServerCommon::kickDestination(destinationId);
			return true;
		}
	}

	// Check that this unique id has not already connected (if unique ids are in use)
	if (uniqueId.c_str()[0] &&
		!ScorchedServer::instance()->getOptionsGame().getAllowSameUniqueId())
	{
		std::map<unsigned int, Tank *> &playingTanks = 
			ScorchedServer::instance()->getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator playingItor;
		for (playingItor = playingTanks.begin();
			playingItor != playingTanks.end();
			playingItor++)
		{
			Tank *current = (*playingItor).second;
			if (0 == strcmp(current->getUniqueId(), uniqueId.c_str()))
			{
				Logger::log(formatString("Duplicate uniqueid connection from destination \"%i\"", 
					destinationId));
				ServerCommon::kickDestination(destinationId);
				return true;
			}
		}
	}

	// Send the connection accepted message to the client
	ComsConnectAcceptMessage acceptMessage(
		destinationId,
		ScorchedServer::instance()->getOptionsGame().getServerName(),
		ScorchedServer::instance()->getOptionsGame().getPublishAddress(),
		uniqueId.c_str());
	// Add the connection png
#ifdef S3D_SERVER
	{
		const char *fileName = 
			getSettingsFile(formatString("icon-%i.png",
				ScorchedServer::instance()->getOptionsGame().getPortNo()));
		FILE *in = fopen(fileName, "rb");
		if (in)
		{
			acceptMessage.getServerPng().reset();
			unsigned char readBuf[512];
			while (unsigned int size = fread(readBuf, sizeof(unsigned char), 512, in))
			{
				acceptMessage.getServerPng().addDataToBuffer(readBuf, size);
			}
			fclose(in);
		}
	}
#endif
	if (!ComsMessageSender::sendToSingleClient(acceptMessage, destinationId))
	{
		Logger::log(formatString(
			"Failed to send accept to client \"%i\"",
			destinationId));
		ServerCommon::kickDestination(destinationId);
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
			tank->getModelContainer().getTankModelName(),
			tank->getModelContainer().getTankTypeName(),
			tank->getDestinationId(),
			tank->getTeam(),
			""); 
		oldPlayerMessage.setPlayerIconName(tank->getAvatar().getName());
		oldPlayerMessage.getPlayerIcon().addDataToBuffer(
			tank->getAvatar().getFile().getBuffer(),
			tank->getAvatar().getFile().getBufferUsed());
		ComsMessageSender::sendToSingleClient(oldPlayerMessage, destinationId);
	}

	// Add all the new tanks
	for (unsigned int i=0; i<message.getNoPlayers(); i++)
	{
		addNextTank(destinationId,
			ipAddress,	
			uniqueId.c_str(),
			message.getHostDesc(),
			false);
	}

	// For the single player game
	// Add a spectator that will always remain a spectator
	// this is so if we only have computer players we still
	// send messages to them
#ifndef S3D_SERVER
	{
		addNextTank(destinationId,
			ipAddress,
			uniqueId.c_str(),
			message.getHostDesc(),
			true);
	}
#endif

	// Send the state of all the currently connect clients
	ComsPlayerStateMessage comsPlayerStateMessage(ComsPlayerStateMessage::eTankNoAccessories);
	if (!ComsMessageSender::sendToSingleClient(
		comsPlayerStateMessage, destinationId)) return false;

	return true;
}

void ServerConnectHandler::addNextTank(unsigned int destinationId,
	unsigned int ipAddress,
	const char *sentUniqueId,
	const char *sentHostDesc,
	bool extraSpectator)
{
	// The player has connected
	Vector color;
	unsigned int tankId = 0;
	const char *playerName = "";

	if (extraSpectator)
	{
		tankId = 1;
		playerName = "Spectator";
		color = Vector(0.7f, 0.7f, 0.7f);
	}
	else
	{
		playerName = TankAIStrings::instance()->getPlayerName();
		color = TankColorGenerator::instance()->getNextColor(
			ScorchedServer::instance()->getTankContainer().getPlayingTanks());
		tankId = TankAIAdder::getNextTankId(
			ScorchedServer::instance()->getContext());
	}

	// Make sure host desc does not contain \"
	for (char *h=(char *)sentHostDesc; *h; h++) if (*h == '\"') *h=' ';
	
	// Create the new tank 
	Tank *tank = new Tank(
		ScorchedServer::instance()->getContext(),
		tankId,
		destinationId,
		playerName,
		color,
		"Random",
		"none"); // The model, this will be turned into a "proper" model in the player choice dialog
	tank->setUniqueId(sentUniqueId);
	tank->setIpAddress(ipAddress);
	tank->setHostDesc(sentHostDesc);
	tank->getState().setSpectator(true);

	// Use the stats name if stats are enabled and the player has one
	std::list<std::string> aliases  = 
		StatsLogger::instance()->getAliases(tank);
	if (!aliases.empty())
	{
		std::string alias = aliases.front();
		tank->setName(alias.c_str());
	}

	// Add the tank to the list of tanks
	ScorchedServer::instance()->getTankContainer().addTank(tank);

	// Tell the clients to create this tank
	ComsAddPlayerMessage addPlayerMessage(
		tank->getPlayerId(),
		tank->getName(),
		tank->getColor(),
		tank->getModelContainer().getTankModelName(),
		tank->getModelContainer().getTankTypeName(),
		tank->getDestinationId(),
		tank->getTeam(),
		"");
	ComsMessageSender::sendToAllConnectedClients(addPlayerMessage);

	// Tell this computer that a new tank has connected
#ifdef S3D_SERVER
	{
		// Add to dialog
		Logger::log(formatString("Player connected dest=\"%i\" id=\"%i\" name=\"%s\" unique=[%s]",
			tank->getDestinationId(),
			tank->getPlayerId(),
			tank->getName(),
			tank->getUniqueId()));

		ServerCommon::sendString(0, formatString("Player connected \"%s\"",
			tank->getName()));
	}
#endif

	// Add this tank to stats
	StatsLogger::instance()->tankConnected(tank);
	
	// Check if admin muted
	if (ipAddress != 0)
	{
		ServerBanned::BannedType type = 
			ScorchedServerUtil::instance()->bannedPlayers.getBanned(ipAddress, tank->getUniqueId());
		if (type == ServerBanned::Muted)	
		{
			tank->getState().setMuted(true);
			ServerCommon::sendStringAdmin(formatString("Player admin muted \"%s\"",
				tank->getName()));
		}
		else if (type == ServerBanned::Flagged)
		{
			ServerCommon::sendStringAdmin(formatString("Player admin flagged \"%s\"",
				tank->getName()));
		}
	}
}
