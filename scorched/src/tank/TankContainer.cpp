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

#include <tank/TankContainer.h>

TankContainer::TankContainer() : playerId_(0), destinationId_(0)
{

}

TankContainer::~TankContainer()
{

}

void TankContainer::addTank(Tank *tank)
{
	playingTanks_[tank->getPlayerId()] = tank;
}

Tank *TankContainer::removeTank(unsigned int playerId)
{
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = playingTanks_.begin();
		itor != playingTanks_.end();
		itor++)
	{
		Tank *current = (*itor).second;
		if (current->getPlayerId() == playerId)
		{
			playingTanks_.erase(itor);
			return current;
		}
	}

	return 0;
}

Tank *TankContainer::getTankByPos(unsigned int pos)
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		mainitor != playingTanks_.end();
		mainitor++, pos--)
	{
		if (pos <=0) return (*mainitor).second;
	}
	return 0;
}

Tank *TankContainer::getTankById(unsigned int id)
{
	std::map<unsigned int, Tank *>::iterator mainitor =
		playingTanks_.find(id);
	if (mainitor != playingTanks_.end())
	{
		return (*mainitor).second;
	}
	return 0;
}

Tank *TankContainer::getTankByName(const char *name)
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		if (0 == strcmp((*mainitor).second->getName(),name)) return (*mainitor).second;
	}
	return 0;
}

Tank *TankContainer::getCurrentTank()
{
	if (!playingTanks_.empty())
	{
		if (playerId_)
		{
			static Tank *currentPlayer = 0;
			if (!currentPlayer || currentPlayer->getPlayerId() != playerId_)
			{
				currentPlayer = getTankById(playerId_);
			}
			return currentPlayer;
		}
	}

	return 0;
}

void TankContainer::newGame()
{
	// Tell each tank a new game has started
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		mainitor != playingTanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		current->newGame();
	}
}

void TankContainer::clientNewGame()
{
	// Tell each tank a new game has started
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		mainitor != playingTanks_.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		current->clientNewGame();
	}
}

void TankContainer::resetTanks()
{
	// Reset all tanks
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		current->reset();
	}
}

void TankContainer::removeAllTanks()
{
	// Remove all tanks
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		delete current;
	}
	playingTanks_.clear();
}

int TankContainer::teamCount()
{
	int team1 = 0;
	int team2 = 0;
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getState() == TankState::sNormal)
		{
			if (current->getTeam() == 1) team1=1;
			if (current->getTeam() == 2) team2=1;
		}
	}
	return team1 + team2;
}

int TankContainer::aliveCount()
{
	int alive = 0;

	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getState() == TankState::sNormal)
		{
			alive++;
		}
	}
	return alive;
}

void TankContainer::setAllDead()
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (current->getState().getState() != TankState::sPending)
		{
			current->getState().setState(TankState::sDead);
		}
	}
}

bool TankContainer::allReady()
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;

		// current check tanks that are not pending
		if (current->getState().getState() != TankState::sPending)
		{
			if (current->getState().getReadyState() == 
				TankState::SNotReady) return false;
		}
	}
	return true;
}

void TankContainer::setAllNotReady()
{
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;

		// current check tanks that are not pending
		if (current->getState().getState() != TankState::sPending)
		{
			current->getState().setNotReady();
		}
	}
}

int TankContainer::getNoOfNonSpectatorTanks()
{
	int count = 0;
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks_.begin();
		 mainitor != playingTanks_.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (!current->getState().getSpectator()) count++;
	}
	return count;
}
