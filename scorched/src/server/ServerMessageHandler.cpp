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

#include <server/ServerMessageHandler.h>
#include <server/ScorchedServer.h>
#include <server/ServerCommon.h>
#include <server/ServerBanned.h>
#include <tank/TankDeadContainer.h>
#include <tank/TankContainer.h>
#include <coms/ComsRmPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetInterface.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/StatsLogger.h>

ServerMessageHandler *ServerMessageHandler::instance_ = 0;

ServerMessageHandler *ServerMessageHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerMessageHandler;
	}
	return instance_;
}

ServerMessageHandler::ServerMessageHandler()
{
}

ServerMessageHandler::~ServerMessageHandler()
{
}

void ServerMessageHandler::clientConnected(NetMessage &message)
{
	if (message.getIpAddress() != 0 &&
		ServerBanned::instance()->getBanned(message.getIpAddress(), "") == 
		ServerBanned::Banned)
	{
		Logger::log( "Banned client connected dest=\"%i\" ip=\"%s\"", 
			message.getDestinationId(),
			NetInterface::getIpName(message.getIpAddress()));
		ScorchedServer::instance()->getNetInterface().
			disconnectClient(message.getDestinationId());
	}
	else
	{
		Logger::log( "Client connected dest=\"%i\" ip=\"%s\"", 
			message.getDestinationId(),
			NetInterface::getIpName(message.getIpAddress()));
	}
}

void ServerMessageHandler::clientDisconnected(NetMessage &message)
{
	Logger::log( "Client disconnected dest=\"%i\" ip=\"%s\"", 
		message.getDestinationId(),
		NetInterface::getIpName(message.getIpAddress()));

	// Build up a list of players at this destination
	std::list<unsigned int> removePlayers;
	unsigned int destinationId = message.getDestinationId();
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
			removePlayers.push_back(tank->getPlayerId());
		}
	}

	// Remove all players for this destination
	std::list<unsigned int>::iterator remItor;
	for (remItor = removePlayers.begin();
		 remItor != removePlayers.end();
		 remItor++)
	{
		unsigned int playerId = *remItor;
		destroyPlayer(playerId);
	}
}

void ServerMessageHandler::destroyPlayer(unsigned int tankId)
{
	// Try to remove this player
	Tank *tank = ScorchedServer::instance()->getTankContainer().removeTank(tankId);
	if (tank)
	{
		Logger::log( 
			"Player disconnected dest=\"%i\" id=\"%i\" name=\"%s\"", 
			tank->getDestinationId(),
			tankId, tank->getName());

		StatsLogger::instance()->tankDisconnected(tank);

		// Tell all the clients to remove this player
		ComsRmPlayerMessage rmPlayerMessage(
			tankId);
		ComsMessageSender::sendToAllConnectedClients(rmPlayerMessage);

		// Tidy player
		if (ScorchedServer::instance()->getOptionsGame().getResidualPlayers())
		{
			ScorchedServer::instance()->getTankDeadContainer().addTank(tank);
		}
		else
		{
			delete tank;
		}
	}
	else
	{
		Logger::log( "Unknown player disconnected id=\"%i\"", tankId);
	}
}

void ServerMessageHandler::clientError(NetMessage &message,
		const char *errorString)
{
	Logger::log( "Client \"%i\", ***Server Error*** \"%s\"", 
		message.getDestinationId(),
		errorString);
	ServerCommon::kickDestination(message.getDestinationId());
}
