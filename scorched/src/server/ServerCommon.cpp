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

void ServerCommon::kickDestination(unsigned int destinationId)
{
	if (destinationId == 0)
	{
		Logger::log(0, "Cannot kick local destination");
		return;
	}
	Logger::log(0, "Kicking destination \"%i\"", destinationId);

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getDestinationId() == destinationId)
		{
			sendString(0,
				"Player \"%s\" has been kicked from the server",
				tank->getName(), tank->getPlayerId());
			Logger::log(0, "Kicking client \"%s\" \"%i\"", 
				tank->getName(), tank->getPlayerId());
		}
	}

	ScorchedServer::instance()->getNetInterface().disconnectClient(destinationId);
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
