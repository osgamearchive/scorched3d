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

#include <client/ScorchedClient.h>
#include <client/ClientDefenseHandler.h>
#include <tankai/TankAILogic.h>
#include <coms/ComsDefenseMessage.h>

ClientDefenseHandler *ClientDefenseHandler::instance_ = 0;

ClientDefenseHandler *ClientDefenseHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientDefenseHandler;
	}
	return instance_;
}

ClientDefenseHandler::ClientDefenseHandler() : messageCount_(1)
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsDefenseMessage",
		this);
}

ClientDefenseHandler::~ClientDefenseHandler()
{
}

bool ClientDefenseHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Decode the connect message
	ComsDefenseMessage message;
	if (!message.readMessage(reader)) return false;

	// Check tank exists and is alive
	Tank *tank = ScorchedClient::instance()->getTankContainer().getTankById(message.getPlayerId());
	if (!tank || tank->getState().getState() != TankState::sNormal)
	{
		return true;
	}

	// Actually perform the action in the message
	TankAILogic::processDefenseMessage(
		ScorchedClient::instance()->getContext(), 
		message, tank);
	messageCount_++;
	return true;
}
