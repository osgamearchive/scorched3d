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

#include <server/ServerNextShotState.h>
#include <server/TurnController.h>
#include <server/ScorchedServer.h>
#include <server/ServerState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ServerCommon.h>
#include <common/Logger.h>

ServerNextShotState::ServerNextShotState()
{
}

ServerNextShotState::~ServerNextShotState()
{
}

void ServerNextShotState::enterState(const unsigned state)
{
	// Check there are enough players for this shot
	if (ServerTooFewPlayersStimulus::instance()->acceptStateChange(state, 
		ServerState::ServerStateTooFewPlayers, 0.0f))
	{
		ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusTooFewPlayers);
		return;
	}

	// Check why this round has finished
	bool roundFinished = false;
	if (ScorchedServer::instance()->getOptionsGame().getTeams() == 2 &&
		ScorchedServer::instance()->getTankContainer().teamCount() == 1)
	{
		// Only one team left
		roundFinished = true;
	}
	else if (ScorchedServer::instance()->getTankContainer().aliveCount() < 2) 
	{
		// Only one person left
		roundFinished = true;
	}
	else if (ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() > 
			 ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns() && 
			 ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns() > 0)
	{
		roundFinished = true;
		ServerCommon::serverLog(0, "Skipping round due to turn limit");
		ServerCommon::sendString(0, "Skipping round due to turn limit");
	}

	// Check if this round has finished
	if (roundFinished)
	{
		// We have finished with this round
		// check for all rounds completely finished
		if (ScorchedServer::instance()->getOptionsTransient().getNoRoundsLeft() <= 0)
		{
			// We have finished with all rounds show the score
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusScore);
		}
		else
		{
			// We have finished with this round, go onto the next round
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNewGame);
		}
	}
	else
	{
		TurnController::instance()->nextShot();
		if (TurnController::instance()->getPlayersThisShot().empty())
		{
			// There are no players still to have a shot
			// The round must have finished
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNextRound);
		}
		else
		{
			// We have shots to make, lets make them
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNextTurn);
		}
	}
}

