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

#include <server/ServerTextHandler.h>
#include <server/ScorchedServer.h>
#include <tank/TankContainer.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>

ServerTextHandler *ServerTextHandler::instance()
{
	static ServerTextHandler *instance = 
		new ServerTextHandler;
	return instance;
}

ServerTextHandler::ServerTextHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsTextMessage",
		this);
}

ServerTextHandler::~ServerTextHandler()
{
}

bool ServerTextHandler::processMessage(unsigned int id,
	const char *messageType,
									NetBufferReader &reader)
{
	ComsTextMessage message;
	if (!message.readMessage(reader)) return false;

	unsigned int tankId = (unsigned int) id;
	Tank *tank = ScorchedServer::instance()->getTankContainer().getTankById(tankId);
	if (tank)
	{
		// Construct the message to send to all the clients
		std::string newText(tank->getName());
		if (tank->getState().getState() == TankState::sDead)
		{
			newText += " (DEAD)";
		}
		else if (tank->getState().getState() == TankState::sPending)
		{
			newText += " (PENDING)";
		}

		newText += ": ";
		newText += message.getText();

		// Update the server console with the say text
		Logger::log(tankId, "Says \"%s\"", newText.c_str());

		ComsTextMessage newMessage(newText.c_str(), 
			tank->getPlayerId(), tank->getColor());
		ComsMessageSender::sendToAllConnectedClients(newMessage);
	}
	else
	{
		Logger::log(0, "Says \"%s\"",	message.getText());
	}

	return true;
}
