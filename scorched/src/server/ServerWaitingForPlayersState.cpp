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


#include <server/ServerWaitingForPlayersState.h>
#include <scorched/ServerDialog.h>
#include <tank/TankContainer.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>

ServerWaitingForPlayersState::ServerWaitingForPlayersState() : time_(0.0f)
{
}

ServerWaitingForPlayersState::~ServerWaitingForPlayersState()
{
}

void ServerWaitingForPlayersState::enterState(const unsigned state)
{
	time_ = 0.0f;
}

bool ServerWaitingForPlayersState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check how many players are waiting
	if (TankContainer::instance()->getNoOfTanks() >= 
		OptionsGame::instance()->getNoMinPlayers())
	{
		// Now we wait for more players to join
		// for a few seconds
		time_ += frameTime;
		int timeLeft = OptionsGame::instance()->getWaitTime() - int(time_);
		
		// Actually transition once no time is left
		if (timeLeft <= 0)
		{
			startGame();

			// stimulate to new game
			return true;
		}
		else
		{
			// Print how long they will wait for
			static int lastTimeLeft = 0;
			if (timeLeft % 5 == 0)
			{
				if (timeLeft != lastTimeLeft)
				{
					sendString(0, "Starting game in %i seconds", timeLeft);
					Logger::log(0, "Starting game in %i seconds", timeLeft);
					lastTimeLeft = timeLeft;
				}
			}
		}
	}
	else
	{
		time_ = 0.0f;
	}
	return false;
}

void ServerWaitingForPlayersState::startGame()
{
	// Reset all the tanks back to their original scores etc...
	TankContainer::instance()->resetTanks();
	OptionsTransient::instance()->reset();

	sendString(0, "Starting game...");
}
