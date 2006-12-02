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

#include <client/ClientPlayerStatusHandler.h>
#include <client/ScorchedClient.h>
#include <coms/ComsPlayerStatusMessage.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>

ClientPlayerStatusHandler *ClientPlayerStatusHandler::instance_ = 0;

ClientPlayerStatusHandler *ClientPlayerStatusHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientPlayerStatusHandler;
	}
	return instance_;
}

ClientPlayerStatusHandler::ClientPlayerStatusHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsPlayerStatusMessage",
		this);
}

ClientPlayerStatusHandler::~ClientPlayerStatusHandler()
{
}

bool ClientPlayerStatusHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsPlayerStatusMessage message;
	if (!message.readMessage(reader)) return false;

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getAllTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *tank = (*itor).second;
		if (message.playerWaiting(tank->getPlayerId())) 
			tank->getState().setNotReady();
		else tank->getState().setReady();
	}

	return true;
}
