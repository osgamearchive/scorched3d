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

#include <server/ServerNextRoundState.h>
#include <server/ServerState.h>
#include <server/ScorchedServer.h>
#include <server/TurnController.h>
#include <scorched/ServerDialog.h>
#include <common/Logger.h>

ServerNextRoundState::ServerNextRoundState()
{
}

ServerNextRoundState::~ServerNextRoundState()
{
}

void ServerNextRoundState::enterState(const unsigned state)
{
	serverLog(0, "NextRound");

	// Move all tanks into the next round
	// Load the set of options for this next player
	ScorchedServer::instance()->getContext().optionsTransient.nextRound();

	// Set the wind for the next shot
	Vector wind;
	if (ScorchedServer::instance()->getOptionsTransient().getWindOn())
	{
		wind = ScorchedServer::instance()->getOptionsTransient().getWindDirection();
		wind *= ScorchedServer::instance()->getOptionsTransient().getWindSpeed() / 2.0f;
	}
	ScorchedServer::instance()->getActionController().getPhysics().setWind(wind);

	// Setup this list of players that need to move before this round is over
	TurnController::instance()->nextRound();

	// Move into the ready state
	ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusReady);
}
