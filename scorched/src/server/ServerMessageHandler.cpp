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
#include <scorched/ServerDialog.h>
#include <coms/ComsRmPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>

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
	Logger::log(0, "Client connected \"%i\"", message.getPlayerId());
}

void ServerMessageHandler::clientDisconnected(NetMessage &message)
{
	unsigned int tankId = (unsigned int) message.getPlayerId();
	destroyPlayer(tankId);
}

void ServerMessageHandler::destroyPlayer(unsigned int tankId)
{
	Tank *tank = TankContainer::instance()->removeTank(tankId);
	if (tank)
	{
		Logger::log(tankId, "Client disconnected \"%i\" \"%s\"", 
			tankId, tank->getName());

		// Tell all the clients to remove the tank
		ComsRmPlayerMessage rmPlayerMessage(
			tankId);
		ComsMessageSender::sendToAllConnectedClients(rmPlayerMessage);

		delete tank;
	}
	else
	{
		Logger::log(0, "Client disconnected \"%i\"", tankId);
	}
}


void ServerMessageHandler::clientError(NetMessage &message,
		const char *errorString)
{
	Logger::log(0, "Client \"%i\", ***Error*** \"%s\"", 
		message.getPlayerId(),
		errorString);
	kickPlayer(message.getPlayerId());
}
