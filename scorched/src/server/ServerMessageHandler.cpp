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
#include <scorched/ServerDialog.h>
#include <coms/ComsRmPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>

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
	Logger::log(0, "Client connected \"%i\"", message.getDestinationId());
}

void ServerMessageHandler::clientDisconnected(NetMessage &message)
{
	Logger::log(0, "Client disconnected \"%i\"", message.getDestinationId());

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
		Logger::log(tankId, "Player disconnected \"%i\" \"%s\"", 
			tankId, tank->getName());

		// Tell all the clients to remove this player
		ComsRmPlayerMessage rmPlayerMessage(
			tankId);
		ComsMessageSender::sendToAllConnectedClients(rmPlayerMessage);

		// Tidy player
		delete tank;
	}
	else
	{
		Logger::log(0, "Unknown player disconnected \"%i\"", tankId);
	}
}

void ServerMessageHandler::clientError(NetMessage &message,
		const char *errorString)
{
	Logger::log(0, "Client \"%i\", ***Server Error*** \"%s\"", 
		message.getDestinationId(),
		errorString);
	kickDestination(message.getDestinationId());
}
