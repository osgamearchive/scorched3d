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

#include <server/ServerAdminHandler.h>
#include <server/ScorchedServer.h>
#include <server/ServerCommon.h>
#include <common/OptionsGame.h>
#include <coms/ComsAdminMessage.h>
#include <tank/TankContainer.h>
#include <stdlib.h>

ServerAdminHandler *ServerAdminHandler::instance()
{
	static ServerAdminHandler *instance = 
		new ServerAdminHandler;
	return instance;
}

ServerAdminHandler::ServerAdminHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsAdminMessage",
		this);
}

ServerAdminHandler::~ServerAdminHandler()
{
}

bool ServerAdminHandler::processMessage(unsigned int destinationId,
	const char *messageType,
									NetBufferReader &reader)
{
	ComsAdminMessage message;
	if (!message.readMessage(reader)) return false;

	// Find the tank for this destination
	Tank *adminTank = 0;
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->
			getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getDestinationId() == destinationId)
			adminTank = tank;
	}
	if (!adminTank) return true;

	// Login if that is what is happening
	if (message.getType() == ComsAdminMessage::AdminLogin)
	{
		if (ScorchedServer::instance()->getOptionsGame().
			getServerAdminPassword()[0])
		{
			if (strcmp(message.getParam1(), 
					ScorchedServer::instance()->getOptionsGame().
					getServerAdminPassword()) == 0)
			{
				ServerCommon::sendString(destinationId,
					"Admin logged in");
				adminTank->getState().setAdmin(true);
			}
			else
			{
				ServerCommon::sendString(destinationId,
					"Incorrect admin password");
			}
		}
		else
		{
			ServerCommon::sendString(destinationId,
				"Admin functionality not enabled");
		}
		return true;
	}
	// Else only allow logged in tanks
	else if (!adminTank->getState().getAdmin())
	{
		ServerCommon::sendString(destinationId,
			"You are not logged in as admin");
		return true;
	}

	// Do admin fn (we are logged in at this point)
	switch (message.getType())
	{
	case ComsAdminMessage::AdminBan:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank) ServerCommon::banDestination(
				targetTank->getDestinationId());
		}
		break;
	case ComsAdminMessage::AdminKick:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank) ServerCommon::kickDestination(
				targetTank->getDestinationId());
		}
		break;
	case ComsAdminMessage::AdminKillAll:
		ServerCommon::killAll();
		break;
	case ComsAdminMessage::AdminSlap:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank) ServerCommon::slapDestination(
				targetTank->getDestinationId(),
				(float) atof(message.getParam2()));
		}
		break;
	}

	return true;
}
