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

#include <client/ClientConnectionAcceptHandler.h>
#include <client/ScorchedClient.h>
#include <coms/ComsConnectAcceptMessage.h>
#include <common/Logger.h>

ClientConnectionAcceptHandler *ClientConnectionAcceptHandler::instance_ = 0;

ClientConnectionAcceptHandler *ClientConnectionAcceptHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientConnectionAcceptHandler();
	}

	return instance_;
}

ClientConnectionAcceptHandler::ClientConnectionAcceptHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsConnectAcceptMessage",
		this);
}

ClientConnectionAcceptHandler::~ClientConnectionAcceptHandler()
{

}

bool ClientConnectionAcceptHandler::processMessage(unsigned int id,
												   const char *messageType,
												   NetBufferReader &reader)
{
	ComsConnectAcceptMessage message;
	if (!message.readMessage(reader)) return false;

	// need to store this client id somewhere
	ScorchedClient::instance()->getTankContainer().setCurrentPlayerId(message.getClientId());

	// Add connection info to the dialogs
	Logger::log(0,
		"Connection accepted by \"%s\"",
		message.getServerName());
	Logger::log(0, message.getMotd());

	return true;
}
