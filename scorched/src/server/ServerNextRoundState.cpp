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
#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>
#include <engine/ActionController.h>
#include <engine/GameState.h>
#include <tank/TankContainer.h>
#include <tank/TankStart.h>

ServerNextRoundState::ServerNextRoundState() : time_(0.0f)
{
}

ServerNextRoundState::~ServerNextRoundState()
{
}

void ServerNextRoundState::enterState(const unsigned state)
{
	Logger::log(0, "Next round");

	// Add any pending tanks into the game
	ServerNewGameState::addTanksToGame(state);

	// Move all tanks into the next round
	TankStart::nextRound();

	// Set the wind for the next shot
	Vector wind;
	if (OptionsTransient::instance()->getWindOn())
	{
		wind = OptionsTransient::instance()->getWindDirection();
		wind *= OptionsTransient::instance()->getWindSpeed() / 2.0f;
	}
	ActionController::instance()->getPhysics().setWind(wind);

	// Set the wait timer to the current time
	time_ = 0.0f;
}

bool ServerNextRoundState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check all players returned ready
	if (TankContainer::instance()->allReady())
	{
		if (TankContainer::instance()->aliveCount() < 2 ||
			OptionsTransient::instance()->getCurrentGameNo() > 
			OptionsGame::instance()->getNoMaxRoundTurns())
		{
			// We have finished with this round
			roundFinished();
		}
		else
		{
			// Stimulate into the next state
			return true;
		}
	}

	// Check if any players have timed out
	time_ += frameTime;
	if ((OptionsGame::instance()->getIdleKickTime() > 0) &&
		(time_ > OptionsGame::instance()->getIdleKickTime()))
	{
		// Kick all not returned players
		std::map<unsigned int, Tank *> &tanks = 
			TankContainer::instance()->getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getReadyState() == TankState::SNotReady)
			{
				Logger::log(tank->getPlayerId(), "Player \"%s\" not returned ready for %i seconds", 
					tank->getName(),
					OptionsGame::instance()->getIdleKickTime());

				kickPlayer((NetPlayerID) tank->getPlayerId());
			}
		}

		// Stimulate into the next state
		return true;
	}
	return false;
}

void ServerNextRoundState::roundFinished()
{
	// check for all rounds completely finished
	if (OptionsTransient::instance()->getNoRoundsLeft() <= 0)
	{
		// We have finished with all rounds show the score
		GameState::instance()->stimulate(ServerState::ServerStimulusScore);
	}
	else
	{
		// We have finished with this round, go onto the next round
		GameState::instance()->stimulate(ServerState::ServerStimulusNewGame);
	}
}
