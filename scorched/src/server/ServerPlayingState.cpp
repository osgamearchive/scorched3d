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

#include <server/ServerPlayingState.h>
#include <server/ServerShotHolder.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <coms/ComsStartGameMessage.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankContainer.h>

ServerPlayingState::ServerPlayingState() : time_(0.0f)
{
}

ServerPlayingState::~ServerPlayingState()
{
}

void ServerPlayingState::enterState(const unsigned state)
{
	Logger::log(0, "Players playing game");

	// Make sure that there are no shots from the last rounds
	ServerShotHolder::instance()->clearShots();

	// Tell the clients to start the game
	ComsStartGameMessage startMessage;
	ComsMessageSender::sendToAllPlayingClients(startMessage);

	// Set the wait timer to the current time
	time_ = 0.0f;
}

bool ServerPlayingState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check if the time to make the shots has expired
	time_ += frameTime;
	if (OptionsGame::instance()->getShotTime() > 0)
	{
		if (time_ > OptionsGame::instance()->getShotTime())
		{
			// For each alive tank
			// Check if the tank has missed its go
			// If so increment the missed counter
			// Once missed counter exceeds it threshold then kick the player
			std::map<unsigned int, Tank *> &tanks =
				TankContainer::instance()->getPlayingTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *tank = (*itor).second;
				if (tank->getState().getState() == TankState::sNormal)
				{
					if (!ServerShotHolder::instance()->haveShot(
						tank->getPlayerId()))
					{
						int movesMissed = tank->getScore().getMissedMoves() + 1;
						tank->getScore().setMissedMoves(movesMissed);

						if (movesMissed == 1)
						{
							Logger::log(tank->getPlayerId(), "Player \"%s\" failed to make a move",
								tank->getName());
							sendString(0, "Player \"%s\" failed to make a move",
								tank->getName());
						}
						else
						{
							Logger::log(tank->getPlayerId(), "Player \"%s\" failed to make %i moves in a row",
								tank->getName(), movesMissed);
							sendString(0, "Player \"%s\" failed to make %i moves in a row",
								tank->getName(), movesMissed);
						}

						// If the allowed missed moves has been specified
						if (OptionsGame::instance()->getAllowedMissedMoves() > 0)
						{
							// And this player has exceeded them
							if (movesMissed >= OptionsGame::instance()->getAllowedMissedMoves())
							{
								// Then kick this player
								NetPlayerID id = (NetPlayerID) tank->getPlayerId();
								kickPlayer(id);
							}
						}
					}
				}
			}

			return true;
		}
	}

	// Or we already have all shots
	if (ServerShotHolder::instance()->haveAllShots())
	{
		return true;
	}

	return false;
}
