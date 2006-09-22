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

#include <server/ServerLinesHandler.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <coms/ComsLinesMessage.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankContainer.h>

#include <common/Logger.h>

ServerLinesHandler *ServerLinesHandler::instance()
{
	static ServerLinesHandler *instance = 
		new ServerLinesHandler;
	return instance;
}

ServerLinesHandler::ServerLinesHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsLinesMessage",
		this);
}

ServerLinesHandler::~ServerLinesHandler()
{
}

bool ServerLinesHandler::processMessage(unsigned int destinationId,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsLinesMessage message;
	if (!message.readMessage(reader)) return false;

	// Check the player id has been supplied
	unsigned int playerId = message.getPlayerId();
	if (playerId == 0) return true;

	// Check this is a tank, and comes from the correct destination
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(playerId);
	if (!tank || tank->getDestinationId() != destinationId) return true;

	// If this tank has been muted also don't allow lines
	if (tank->getState().getMuted()) return true;

	// Send all team messages to everyone in the team (or any admins)
	// Only send to the same destination once
	std::set<unsigned int> doneDests;
	doneDests.insert(destinationId); // Don't send to recieved dest
	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *currentTank = (*itor).second;
		if (tank->getTeam() == currentTank->getTeam() ||
			currentTank->getState().getAdmin())
		{
			if (doneDests.find(currentTank->getDestinationId()) ==
				doneDests.end())
			{
				doneDests.insert(currentTank->getDestinationId());
				ComsMessageSender::sendToSingleClient(message,
					currentTank->getDestinationId());
			}
		}
	}	

	return true;
}
