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


#include <server/ServerNewGameState.h>
#include <scorched/ServerDialog.h>
#include <server/ServerState.h>
#include <common/OptionsGame.h>
#include <common/Timer.h>
#include <common/Logger.h>
#include <coms/ComsNewGameMessage.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankContainer.h>
#include <tank/TankStart.h>
#include <landscape/GlobalHMap.h>

extern Timer serverTimer;

ServerNewGameState::ServerNewGameState()
{
}

ServerNewGameState::~ServerNewGameState()
{
}

void ServerNewGameState::enterState(const unsigned state)
{
	// Tell clients a new game is starting
	Logger::log(0, "Starting new game");
	sendString(0, "Starting a new game.");

	// Setup landscape and tank start pos
	Logger::log(0, "Generating landscape");
	TankStart::newGame();

	// Add pending tanks (all tanks should be pending) into the game
	addTanksToGame(state);

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	serverTimer.getTimeDifference();

	// Move into the state that waits for players to become ready
	GameState::instance()->stimulate(ServerState::ServerStimulusNextRound);
}

void ServerNewGameState::addTanksToGame(const unsigned state)
{
	// Check if there are any pending tanks
	// An optimization that is only for the
	// next round state case
	bool pending = false;
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		// Check to see if any tanks are pending being added
		if (tank->getState().getState() == TankState::sPending)
		{
			pending = true;
		}
	}
	if (!pending) return;

	// Generate the level message
	ComsNewGameMessage newGameMessage;
	if (!GlobalHMap::instance()->generateHMapDiff(
		newGameMessage.getLevelMessage()))
	{
		Logger::log(0, "ERROR: Failed to generate diff");
	}
	Logger::log(0, "Finished generating landscape (%i bytes)", 
		newGameMessage.getLevelMessage().getLevelLen());

	// Check if the generated landscape is too large to send to the clients
	if (newGameMessage.getLevelMessage().getLevelLen() >
		(unsigned int) OptionsGame::instance()->getMaxLandscapeSize())
	{
		return;
	}

	// Tell any pending tanks to join the game
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		// Check to see if any tanks are pending being added
		if (tank->getState().getState() == TankState::sPending)
		{
			if (state == ServerState::ServerStateNewGame)
			{
				// This tank is now playing
				tank->getState().setState(TankState::sNormal);
			}
			else
			{
				// This tank is now playing (but dead)
				tank->getState().setState(TankState::sDead);
			}

			// Send new game message to clients
			ComsMessageSender::sendToSingleClient(newGameMessage,
				(NetPlayerID) tank->getPlayerId());
		}
	}
}
