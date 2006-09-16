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

#include <server/ServerStateTooFewPlayersState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ScorchedServer.h>
#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <server/ServerCommon.h>
#include <tank/TankContainer.h>
#include <coms/ComsGameStoppedMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>

ServerStateTooFewPlayersState::ServerStateTooFewPlayersState() :
	GameStateI("ServerStateTooFewPlayersState"),
	totalTime_(0.0f)
	
{
}

ServerStateTooFewPlayersState::~ServerStateTooFewPlayersState()
{
}

void ServerStateTooFewPlayersState::enterState(const unsigned state)
{
	// Make sure all tanks are dead
	ScorchedServer::instance()->getTankContainer().setAllDead();

	// Tell any clients why the game has been abandonded
	ComsGameStoppedMessage gameStopped;
	ComsMessageSender::sendToAllPlayingClients(gameStopped);

	ServerCommon::sendString(0, "Too few players, stopping play");
}

bool ServerStateTooFewPlayersState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	bool readyToPlay = !ServerTooFewPlayersStimulus::instance()->
		acceptStateChange(state, nextState, frameTime);
	if (readyToPlay) return true;

	// Check if the game options have changed
	totalTime_ += frameTime;
	if (totalTime_ > 1.0f)
	{
		totalTime_ = 0.0f;
		if (ScorchedServer::instance()->getOptionsGame().commitChanges())
		{
			ServerCommon::sendString(0, "Game options have been changed!");
		}	
	}

	// Check settings
	if (!OptionsParam::instance()->getDedicatedServer())
	{
		if (ScorchedServer::instance()->getTankContainer().getNoOfNonSpectatorTanks() ==
			ScorchedServer::instance()->getOptionsGame().getNoMinPlayers())
		{
			// Something has gone wrong
			// We are playing a single player game.
			// We have enough players but have not started
			dialogExit("Scorched3D",
				"Incorrect players added for current game settings."
				"Is this a team game with un-even teams?");
		}
	}

	// Check if we need to add any new bots
	ServerNewGameState::checkBots();

	return false;
}
