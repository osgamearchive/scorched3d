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

#include <client/ClientActionsHandler.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <engine/ActionController.h>
#include <coms/ComsActionsMessage.h>

ClientActionsHandler *ClientActionsHandler::instance_ = 0;

ClientActionsHandler *ClientActionsHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientActionsHandler;
	}
	return instance_;
}

ClientActionsHandler::ClientActionsHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsActionsMessage",
		this);
}

ClientActionsHandler::~ClientActionsHandler()
{
}

bool ClientActionsHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Reset the action controller in anticipation of the new shots
	ScorchedClient::instance()->getActionController().resetTime();
	ScorchedClient::instance()->getActionController().getBuffer().clear();

	// Read the new shots into the action controller
	ComsActionsMessage actionsMessage;
	if (!actionsMessage.readMessage(reader)) return false;

	// Ensure we are in the shot state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
	return true;
}