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

#include <client/ClientPlayerAimHandler.h>
#include <client/ScorchedClient.h>
#include <coms/ComsPlayerAimMessage.h>

ClientPlayerAimHandler *ClientPlayerAimHandler::instance_ = 0;

ClientPlayerAimHandler *ClientPlayerAimHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientPlayerAimHandler;
	}
	return instance_;
}

ClientPlayerAimHandler::ClientPlayerAimHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsPlayerAimMessage",
		this);
}

ClientPlayerAimHandler::~ClientPlayerAimHandler()
{
}

bool ClientPlayerAimHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsPlayerAimMessage message;
	if (!message.readMessage(reader)) return false;

	return true;
}
