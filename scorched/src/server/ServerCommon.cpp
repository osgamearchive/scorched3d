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
#include <server/ServerTextHandler.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <target/TargetLife.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <common/FileLogger.h>
#include <common/Defines.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <net/NetInterface.h>

static FileLogger *serverFileLogger = 0;

void ServerCommon::startFileLogger()
{
	if (!serverFileLogger) 
	{
		char buffer[256];
		snprintf(buffer, 256, "ServerLog-%i-", 
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

void ServerCommon::sendStringMessage(unsigned int dest, const char *text, bool blogMessage)
{
	// Remove any bad characters
	for (char *r = (char *) text; *r; r++)
	{
		if (*r == '%') *r = ' ';
		if (*r < 0) *r = ' ';
	}

	if (blogMessage)
	{
		const char *logMessage = formatString("Messages : %s", text);
		ServerTextHandler::instance()->addMessage(logMessage);
		ServerCommon::serverLog(logMessage);
	}

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

void ServerCommon::sendString(unsigned int dest, const char *text, bool blogMessage)
{
	// Remove any bad characters
	for (char *r = (char *) text; *r; r++)
	{
		if (*r == '%') *r = ' ';
		if (*r < 0) *r = ' ';
	}

	if (blogMessage)
	{
		const char *logMessage = formatString("Says : %s", text);
		ServerTextHandler::instance()->addMessage(logMessage);
		ServerCommon::serverLog(logMessage);
	}

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

void ServerCommon::sendStringAdmin(const char *text)
{
	// Remove any bad characters
	for (char *r = (char *) text; *r; r++)
	{
		if (*r == '%') *r = ' ';
		if (*r < 0) *r = ' ';
	}

	const char *logMessage = formatString("Admin Says : %s", text);
	ServerTextHandler::instance()->addMessage(logMessage);
	ServerCommon::serverLog(logMessage);

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
				formatString("(Admin) %s", text), false);
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
			formatString("admin poor \"%s\"",
			tank->getName()));

		tank->getScore().setMoney(0);
	}
}

void ServerCommon::slapPlayer(unsigned int playerId, float slap)
{
	Logger::log(formatString("Slapping player \"%i\" %.0f", 
		playerId, slap));

	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		tank->getLife().setLife(
			tank->getLife().getLife() - slap);
		sendString(0,
			formatString("Slapping player \"%s\" %.0f",
			tank->getName(), slap));
	}
}

void ServerCommon::kickDestination(unsigned int destinationId)
{
	Logger::log(formatString("Kicking destination \"%i\"", destinationId));

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
			kickPlayer(tank->getPlayerId());
		}
	}
	
	// Make sure we disconnect even if a player has not been created yet
	if (!kickedPlayers)
	{
		ScorchedServer::instance()->getNetInterface().
			disconnectClient(destinationId);
	}
}

void ServerCommon::kickPlayer(unsigned int playerId)
{
	Logger::log(formatString("Kicking player \"%i\"", playerId));

	Tank *tank = ScorchedServer::instance()->
		getTankContainer().getTankById(playerId);
	if (tank)
	{
		sendString(0, formatString(
			"Player \"%s\" has been kicked from the server",
			tank->getName(), tank->getPlayerId()));
		Logger::log(formatString("Kicking client \"%s\" \"%i\"", 
			tank->getName(), tank->getPlayerId()));

		if (tank->getDestinationId() == 0)
		{
			ServerMessageHandler::instance()->
				destroyPlayer(tank->getPlayerId());
		}
		else
		{
			// Cannot delay kick a player as the rest of the code
			// may expect him to have gone (when he is still there delayed)
			// and try to send messages to him
			ScorchedServer::instance()->getNetInterface().
				disconnectClient(tank->getDestinationId());
			ScorchedServer::instance()->getNetInterface().processMessages();
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
				Logger::log(formatString("Banning player %i", playerId));
				break;
			case ServerBanned::Muted:
				Logger::log(formatString("Perminantly muting player %i", playerId));
				break;
			case ServerBanned::NotBanned:
				Logger::log(formatString("Unbanning player %i", playerId));
				break;
			case ServerBanned::Flagged:
				Logger::log(formatString("Flagging player %i", playerId));
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
		ScorchedServer::instance()->getTankContainer().getAllTanks();
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *current = (*itor).second;
		if (current->getState().getState() == TankState::sNormal)
		{
			current->getState().setState(TankState::sDead);
			current->getState().setLives(0);
		}
	}
}

void ServerCommon::startNewGame()
{
	killAll();

	Logger::log("Starting a new game");
	ScorchedServer::instance()->getOptionsTransient().startNewGame();
}

bool &ServerCommon::getExitEmpty()
{
	static bool exitEmpty = false;
	return exitEmpty;
}

void ServerCommon::serverLog(const char *text)
{
#ifdef S3D_SERVER
	{
		Logger::log(text);
	}
#endif
}
