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
#include <server/ScorchedServerUtil.h>
#include <server/ServerMessageHandler.h>
#include <tank/TankContainer.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <common/FileLogger.h>
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
			Logger::log( "Created file logger.");
		}
		else
		{
			Logger::log( "Not created file logger.");
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

void ServerCommon::sendStringAdmin(const char *fmt, ...)
{
	static char text[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->	
			getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getAdmin())
		{
			ServerCommon::sendString(
				tank->getDestinationId(), 
				"(Admin) %s", 
				text);
		}
	}
}

void ServerCommon::poorPlayer(unsigned int playerId)
{
	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		ServerCommon::sendString(0,
			"admin poor \"%s\"",
			tank->getName());

		tank->getScore().setMoney(0);
	}
}

void ServerCommon::slapPlayer(unsigned int playerId, float slap)
{
	Logger::log( "Slapping player \"%i\" %.0f", 
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
		Logger::log( "Slapping client \"%s\" \"%i\" %.0f", 
			tank->getName(), tank->getPlayerId(), slap);
	}
}

void ServerCommon::kickDestination(unsigned int destinationId, bool delayed)
{
	Logger::log( "Kicking destination \"%i\"", destinationId);

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
	Logger::log( "Kicking player \"%i\"", playerId);

	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		sendString(0,
			"Player \"%s\" has been kicked from the server",
			tank->getName(), tank->getPlayerId());
		Logger::log( "Kicking client \"%s\" \"%i\"", 
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

void ServerCommon::banPlayer(unsigned int playerId,
	ServerBanned::BannedType type)
{
	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		if (tank->getDestinationId() == 0)
		{
			Logger::log( "Cannot ban local player/bot");
			return;
		}
		unsigned int ipAddress = tank->getIpAddress();
		if (ipAddress != 0)
		{
			switch (type)
			{
			case ServerBanned::Banned:
				Logger::log( "Banning player %i", playerId);
				break;
			case ServerBanned::Muted:
				Logger::log( "Perminantly muting player %i", playerId);
				break;
			case ServerBanned::NotBanned:
				Logger::log( "Unbanning player %i", playerId);
				break;
			case ServerBanned::Flagged:
				Logger::log( "Flagging player %i", playerId);
				break;				
			}
		
			ScorchedServerUtil::instance()->bannedPlayers.
				addBanned(ipAddress, tank->getName(), tank->getUniqueId(), type);
			if (type == ServerBanned::Banned)
			{
				kickPlayer(playerId);

				ServerAuthHandler *authHandler =
					ScorchedServerUtil::instance()->getAuthHandler();
				if (authHandler)
				{
					authHandler->banUser(tank->getUniqueId());
				}
			}
		}
	}
}

void ServerCommon::killAll()
{
	Logger::log("Killing all players");

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

void ServerCommon::startNewGame()
{
	killAll();

	Logger::log("Starting a new game");
	ScorchedServer::instance()->getOptionsTransient().startNewGame();
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

		Logger::log(text);
	}
}
