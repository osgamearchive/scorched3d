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

#include <client/ClientLastChanceHandler.h>
#include <client/ScorchedClient.h>
#include <client/ClientReadyState.h>
#include <client/ClientState.h>
#include <engine/ActionController.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsLastChanceMessage.h>

ClientLastChanceHandler *ClientLastChanceHandler::instance_ = 0;

ClientLastChanceHandler *ClientLastChanceHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientLastChanceHandler;
	}
	return instance_;
}

ClientLastChanceHandler::ClientLastChanceHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsLastChanceMessage",
		this);
}

ClientLastChanceHandler::~ClientLastChanceHandler()
{
}

bool ClientLastChanceHandler::processMessage(unsigned int destinationId,
	const char *messageType, NetBufferReader &reader)
{
	// Decode the connect message
	ComsLastChanceMessage message;
	if (!message.readMessage(reader)) return false;

	// We are lagging in a state we should not be in!
	// This is the last chance so be drastic
	ScorchedClient::instance()->getActionController().clear(true);

	// Move to the correct state
	ClientReadyState::instance()->enterState(ClientState::StateReady);

	return true;
}
