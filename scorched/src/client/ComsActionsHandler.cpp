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


#include <client/ComsActionsHandler.h>
#include <client/StateSetup.h>
#include <coms/ComsActionsMessage.h>
#include <engine/GameState.h>
#include <engine/ActionController.h>

ComsActionsHandler *ComsActionsHandler::instance_ = 0;

ComsActionsHandler *ComsActionsHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ComsActionsHandler;
	}
	return instance_;
}

ComsActionsHandler::ComsActionsHandler()
{
	ComsMessageHandler::instance()->addHandler(
		"ComsActionsMessage",
		this);
}

ComsActionsHandler::~ComsActionsHandler()
{
}

bool ComsActionsHandler::processMessage(NetPlayerID &id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Reset the action controller in anticipation of the new shots
	ActionController::instance()->resetTime();
	ActionController::instance()->getBuffer().reset();

	// Read the new shots into the action controller
	ComsActionsMessage actionsMessage;
	if (!actionsMessage.readMessage(reader)) return false;

	// Ensure we are in the shot state
	GameState::instance()->stimulate(MainGameState::StimShot);
	return true;
}