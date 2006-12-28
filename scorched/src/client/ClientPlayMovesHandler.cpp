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

#include <client/ClientPlayMovesHandler.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <engine/ActionController.h>
#include <coms/ComsPlayMovesMessage.h>

ClientPlayMovesHandler *ClientPlayMovesHandler::instance_ = 0;

ClientPlayMovesHandler *ClientPlayMovesHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientPlayMovesHandler;
	}
	return instance_;
}

ClientPlayMovesHandler::ClientPlayMovesHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsPlayMovesMessage",
		this);
}

ClientPlayMovesHandler::~ClientPlayMovesHandler()
{
}

bool ClientPlayMovesHandler::processMessage(
	NetMessage &message,
	const char *messageType,
	NetBufferReader &reader)
{
	// Read the new shots into the action controller
	ComsPlayMovesMessage playMovesMessage;
	if (!playMovesMessage.readMessage(reader)) return false;

	// Read the moves from the message
	readMessage(playMovesMessage);

	// Seed
	ScorchedClient::instance()->getActionController().getRandom().seed(playMovesMessage.getSeed());

	// Ensure and move to the shot state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
	return true;
}
