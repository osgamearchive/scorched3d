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

#include <server/TurnController.h>
#include <server/ScorchedServer.h>
#include <tank/TankSort.h>
#include <vector>

TurnController *TurnController::instance_ = 0;

TurnController *TurnController::instance()
{
	if (!instance_)
	{
		instance_ = new TurnController;
	}
	return instance_;
}

TurnController::TurnController()
{
}

TurnController::~TurnController()
{
}

const char *TurnController::getGameType(OptionsGame::TurnType type)
{
	const char *gameType = "Unknown";
	switch (type)
	{
	case OptionsGame::TurnSequentialLooserFirst:
	case OptionsGame::TurnSequentialRandom:
		gameType = "Sequential";
		break;
	case OptionsGame::TurnSimultaneous:
		gameType = "Simultaneous";
		break;
	}
	return gameType;
}

void TurnController::newGame()
{
	playerOrder_.clear();

	OptionsGame::TurnType turnType =
		ScorchedServer::instance()->getOptionsGame().getTurnType();

	// A current game number of zero means its it weapon and auto-defense 
	// choose time, all the players should do this at the same time
	if (ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() == 0)
	{
		turnType = OptionsGame::TurnSimultaneous;
	}

	// Standard player ordering is the reverse of the tank score
	TankSort::getSortedTanksIds(
		ScorchedServer::instance()->getTankContainer(), 
		playerOrder_);
	playerOrder_.reverse();

	// Check for a different ordering
	if (turnType == OptionsGame::TurnSequentialRandom)
	{
		// Create zero player vector
		std::vector<unsigned int> tmpPlayers;
		for (int i=0; i<(int) playerOrder_.size(); i++)
			tmpPlayers.push_back(0);

		// Randomize list order into vector
		while (!playerOrder_.empty())
		{
			unsigned int player = playerOrder_.front();
			playerOrder_.pop_front();
			bool done = false;
			while (!done)
			{
				int pos = int(RAND * float(tmpPlayers.size()));
				if (pos < int(tmpPlayers.size()) && tmpPlayers[pos] == 0) 
				{
					tmpPlayers[pos] = player;
					done = true;
				}
			}
		}

		// Copy vector back to list
		for (int i=0; i<(int) tmpPlayers.size(); i++)
			playerOrder_.push_back(tmpPlayers[i]);
	}
}

void TurnController::nextRound()
{
	// Initialize so all players are still to play
	playersLeftToMove_.clear();
	playersLeftToMove_ = playerOrder_;
}

void TurnController::nextShot()
{
	playersThisShot_.clear();

	OptionsGame::TurnType turnType =
		ScorchedServer::instance()->getOptionsGame().getTurnType();
	if (ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() == 0)
	{
		turnType = OptionsGame::TurnSimultaneous;
	}

	if (turnType == OptionsGame::TurnSequentialRandom ||
		turnType == OptionsGame::TurnSequentialLooserFirst)
	{
		// Get the first normal state player and add them to the shot list
		// only this player should make a shot
		Tank *nextTank = 0;
		while (!nextTank || 
			(nextTank->getState().getState() != TankState::sNormal))
		{
			if (playersLeftToMove_.empty()) return; // No valid players, next round
			unsigned int player = playersLeftToMove_.front();
			playersLeftToMove_.pop_front();
			nextTank = ScorchedServer::instance()->getTankContainer().getTankById(player);
		}

		// Make it this players turn
		playersThisShot_.push_back(nextTank->getPlayerId());
	}
	else if (turnType == OptionsGame::TurnSimultaneous)
	{
		// Move all normal state players into the shot list
		// as all players should make a shot the same time
		std::list<unsigned int>::iterator itor;
		for (itor = playersLeftToMove_.begin();
			itor != playersLeftToMove_.end();
			itor++)
		{
			unsigned int player = *itor;

			Tank *nextTank = 
				ScorchedServer::instance()->getTankContainer().getTankById(player);
			if (nextTank &&
				(nextTank->getState().getState() == TankState::sNormal))
			{
				// Player is ready and it is their turn
				playersThisShot_.push_back(nextTank->getPlayerId());
			}
		}

		// All players should play this shot
		playersLeftToMove_.clear();
	}
	else
	{
		dialogMessage("TurnController::ready - ",
			"Unknown type %i", turnType);
	}
}

void TurnController::nextTurn()
{
	playersThisTurn_.clear();

	std::set<unsigned int> usedDestinations;
	std::list<unsigned int> removePlayers;
	std::list<unsigned int>::iterator itor;
	for (itor = playersThisShot_.begin();
		itor != playersThisShot_.end();
		itor++)
	{
		unsigned int player = *itor;
		Tank *nextTank = 
			ScorchedServer::instance()->getTankContainer().getTankById(player);		
		if (nextTank &&
			(nextTank->getState().getState() == TankState::sNormal))
		{
			if (nextTank->getDestinationId() == 0)
			{
				removePlayers.push_back(player);
				playersThisTurn_.push_back(player);
			}
			else if (usedDestinations.find(nextTank->getDestinationId()) == usedDestinations.end())
			{
				removePlayers.push_back(player);
				playersThisTurn_.push_back(player);
				usedDestinations.insert(nextTank->getDestinationId());
			}
		}
	}

	for (itor = removePlayers.begin();
		itor != removePlayers.end();
		itor++)
	{
		unsigned int player = *itor;
		playersThisShot_.remove(player);
	}
}

bool TurnController::playerThisTurn(unsigned int playerId)
{
	std::list<unsigned int>::iterator itor;
	for (itor = playersThisTurn_.begin();
		itor != playersThisTurn_.end();
		itor++)
	{
		unsigned int player = *itor;
		if (playerId == player) return true;
	}
	return false;
}
