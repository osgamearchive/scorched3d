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
#include <server/ScorchedServer.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>
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
	TankStart::nextRound(ScorchedServer::instance()->getContext());

	// Set the wind for the next shot
	Vector wind;
	if (ScorchedServer::instance()->getOptionsTransient().getWindOn())
	{
		wind = ScorchedServer::instance()->getOptionsTransient().getWindDirection();
		wind *= ScorchedServer::instance()->getOptionsTransient().getWindSpeed() / 2.0f;
	}
	ScorchedServer::instance()->getActionController().getPhysics().setWind(wind);

	// Set the wait timer to the current time
	time_ = 0.0f;
}

bool ServerNextRoundState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check all players returned ready
	if (ScorchedServer::instance()->getTankContainer().allReady())
	{
		if (ScorchedServer::instance()->getTankContainer().aliveCount() < 2 ||
			ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() > 
			ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns())
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
	if ((ScorchedServer::instance()->getOptionsGame().getIdleKickTime() > 0) &&
		(time_ > ScorchedServer::instance()->getOptionsGame().getIdleKickTime()))
	{
		// Kick all not returned players
		std::map<unsigned int, Tank *> &tanks = 
			ScorchedServer::instance()->getTankContainer().getPlayingTanks();
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
					ScorchedServer::instance()->getOptionsGame().getIdleKickTime());

				kickPlayer(tank->getPlayerId());
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
