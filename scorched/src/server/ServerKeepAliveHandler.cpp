////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <server/ServerKeepAliveHandler.h>
#include <server/ServerCommon.h>
#include <server/ScorchedServer.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <tank/TankContainer.h>
#include <coms/ComsKeepAliveMessage.h>
#include <time.h>

ServerKeepAliveHandler *ServerKeepAliveHandler::instance_ = 0;

ServerKeepAliveHandler *ServerKeepAliveHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ServerKeepAliveHandler;
	}
	return instance_;
}

ServerKeepAliveHandler::ServerKeepAliveHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsKeepAliveMessage",
		this);
}

ServerKeepAliveHandler::~ServerKeepAliveHandler()
{
}

bool ServerKeepAliveHandler::processMessage(unsigned int destinationId,
	const char *messageType, NetBufferReader &reader)
{
	// Decode the connect message
	ComsKeepAliveMessage message;
	if (!message.readMessage(reader)) return false;

	unsigned int theTime = (unsigned int) time(0);

	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *current = (*itor).second;
		if (current->getDestinationId() == destinationId)
		{
			current->setKeepAlive(theTime);
		}
	}

	return true;
}

void ServerKeepAliveHandler::checkKeepAlives()
{
	unsigned int allowedTime = (unsigned int)
		ScorchedServer::instance()->getOptionsGame().getKeepAliveTimeoutTime();
	if (allowedTime == 0) return;

	unsigned int theTime = (unsigned int) time(0);

	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *current = (*itor).second;

		if (current->getKeepAlive() != 0 &&
			theTime - current->getKeepAlive()  > allowedTime)
		{
			ServerCommon::serverLog(0, 
					"\"%s\" Kicked for exceeding keep alive timeout (%u seconds)",
					current->getName(),
					theTime - current->getKeepAlive());
			ServerCommon::sendString(0,
					"\"%s\" kicked for exceeding keep alive timeout",
					current->getName());

			ServerCommon::kickDestination(current->getDestinationId());
			break; // As now the tank container may be out of date
		}
	}
}
