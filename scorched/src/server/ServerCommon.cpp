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

#include <server/ServerCommon.h>
#include <server/ScorchedServer.h>
#include <server/ServerBanned.h>
#include <server/ServerMessageHandler.h>
#include <tank/TankContainer.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetInterface.h>

static FileLogger *serverFileLogger = 0;

void ServerCommon::startFileLogger()
{
	if (!serverFileLogger) 
	{
		char buffer[256];
		sprintf(buffer, "ServerLog-%i-", 
			ScorchedServer::instance()->getOptionsGame().getPortNo());

		serverFileLogger = new FileLogger(buffer);
		if (0 != strcmp(ScorchedServer::instance()->getOptionsGame().
			getServerFileLogger(), "none"))
		{
			Logger::addLogger(serverFileLogger);
			Logger::log(0, "Created file logger.");
		}
		else
		{
			Logger::log(0, "Not created file logger.");
		}
	}	
}

void ServerCommon::sendStringMessage(unsigned int dest, const char *fmt, ...)
{
	static char text[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	ComsTextMessage message(text, 0, true);
	if (dest == 0)
	{
		ComsMessageSender::sendToAllConnectedClients(message);
	}
	else
	{
		ComsMessageSender::sendToSingleClient(message, dest);
	}
}

void ServerCommon::sendString(unsigned int dest, const char *fmt, ...)
{
	static char text[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	ComsTextMessage message(text);
	if (dest == 0)
	{
		ComsMessageSender::sendToAllConnectedClients(message);
	}
	else
	{
		ComsMessageSender::sendToSingleClient(message, dest);
	}
}

void ServerCommon::slapPlayer(unsigned int playerId, float slap)
{
	Logger::log(0, "Slapping player \"%i\" %.0f", 
		playerId, slap);

	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		tank->getState().setLife(
			tank->getState().getLife() - slap);
		sendString(0,
			"Slapping player \"%s\" %.0f",
			tank->getName(), slap);
		Logger::log(0, "Slapping client \"%s\" \"%i\" %.0f", 
			tank->getName(), tank->getPlayerId(), slap);
	}
}

void ServerCommon::kickDestination(unsigned int destinationId, bool delayed)
{
	Logger::log(0, "Kicking destination \"%i\"", destinationId);

	bool kickedPlayers = false;
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getDestinationId() == destinationId)
		{
			kickedPlayers = true;
			kickPlayer(tank->getPlayerId(), delayed);
		}
	}
	
	// Make sure we disconnect even if a player has not been created yet
	if (!kickedPlayers)
	{
		ScorchedServer::instance()->getNetInterface().
			disconnectClient(destinationId, delayed);
	}
}

void ServerCommon::kickPlayer(unsigned int playerId, bool delayed)
{
	Logger::log(0, "Kicking player \"%i\"", playerId);

	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		sendString(0,
			"Player \"%s\" has been kicked from the server",
			tank->getName(), tank->getPlayerId());
		Logger::log(0, "Kicking client \"%s\" \"%i\"", 
			tank->getName(), tank->getPlayerId());

		if (tank->getDestinationId() == 0)
		{
			ServerMessageHandler::instance()->
				destroyPlayer(tank->getPlayerId());
		}
		else
		{
			ScorchedServer::instance()->getNetInterface().
				disconnectClient(tank->getDestinationId(), delayed);
		}
	}
}

void ServerCommon::banPlayer(unsigned int playerId)
{
	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		if (tank->getDestinationId() == 0)
		{
			Logger::log(0, "Cannot ban local player (bot)");
			return;
		}
		unsigned int ipAddress = tank->getIpAddress();
		if (ipAddress != 0)
		{
			Logger::log(0, "Banning player %i", playerId);
		
			ServerBanned::instance()->
				addBanned(ipAddress, tank->getName());
			kickPlayer(playerId);
		}
	}
}

void ServerCommon::killAll()
{
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *current = (*itor).second;
		current->getState().setState(TankState::sDead);
	}
}

void ServerCommon::serverLog(unsigned int playerId, const char *fmt, ...)
{
	if (OptionsParam::instance()->getDedicatedServer())
	{
		static char text[2048];

		// Add the actual log message
		va_list ap;
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);

		Logger::log(playerId, text);
	}
}
