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

#include <server/ServerStartingState.h>
#include <server/ScorchedServer.h>
#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <server/ServerCommon.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>

ServerStartingState::ServerStartingState()
{
}

ServerStartingState::~ServerStartingState()
{
}

void ServerStartingState::enterState(const unsigned state)
{
	timeLeft_ = (float) ScorchedServer::instance()->getOptionsGame().getStartTime();
}

bool ServerStartingState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	float startTime = timeLeft_;
	timeLeft_ -= frameTime;

	if (frameTime > 0.0f && int(timeLeft_) != int(startTime))
	{
		ServerCommon::serverLog(0, "Game starting in %.0f seconds...", timeLeft_);
		ServerCommon::sendString(0, "Game starting in %.0f seconds...", timeLeft_);
	}

	// Check if we need to add any new bots
	ServerNewGameState::checkBots();

	// Add any new clients
	if (ServerNewGameState::addTanksToGame(state))
	{
		// If we add any tell the others
		ComsGameStateMessage stateMessage;
		ComsMessageSender::sendToAllPlayingClients(stateMessage);

		// Give more time
		timeLeft_ = (float) ScorchedServer::instance()->getOptionsGame().getStartTime();
	}

	return (timeLeft_ < 0.0f);
}

