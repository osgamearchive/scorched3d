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

TankContainer::TankContainer(TargetContainer &targets) : 
	targets_(targets),
	playerId_(0), 
	destinationId_(0)
{

}

TankContainer::~TankContainer()
{

}

void TankContainer::addTank(Tank *tank)
{
	targets_.addTarget(tank);
}

Tank *TankContainer::removeTank(unsigned int playerId)
{
	Target *target = targets_.removeTarget(playerId);
	if (target) DIALOG_ASSERT(!target->isTarget());
	return (Tank *) target;
}

Tank *TankContainer::getTankByPos(unsigned int pos)
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *tank = (Tank *) target;
			if (pos-- <= 0) return tank;
		}
	}
	return 0;
}

Tank *TankContainer::getTankById(unsigned int id)
{
	Target *target = targets_.getTargetById(id);
	if (target) DIALOG_ASSERT(!target->isTarget());
	return (Tank *) target;
}

Tank *TankContainer::getTankByName(const char *name)
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *tank = (Tank *) target;
			if (0 == strcmp(tank->getName(),name)) return tank;
		}
	}
	return 0;
}

Tank *TankContainer::getCurrentTank()
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

	return 0;
}

void TankContainer::clientNewGame()
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *tank = (Tank *) target;
			tank->clientNewGame();
		}
	}
}

void TankContainer::newMatch()
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *tank = (Tank *) target;
			tank->newMatch();
		}
	}
}

int TankContainer::teamCount()
{
	int team1 = 0;
	int team2 = 0;
	int team3 = 0;
	int team4 = 0;

	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;
			if (current->getState().getState() == TankState::sNormal)
			{
				if (current->getTeam() == 1) team1=1;
				if (current->getTeam() == 2) team2=1;
				if (current->getTeam() == 3) team3=1;
				if (current->getTeam() == 4) team4=1;
			}
		}
	}
	return team1 + team2 + team3 + team4;
}

int TankContainer::aliveCount()
{
	int alive = 0;
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;
			if (current->getState().getState() == TankState::sNormal)
			{
				alive++;
			}
		}
	}
	return alive;
}

void TankContainer::setAllDead()
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;
			if (current->getState().getState() != TankState::sPending)
			{
				current->getState().setState(TankState::sDead);
			}
		}
	}
}

bool TankContainer::allReady()
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;

			// current check tanks that are not pending
			if (current->getState().getState() != TankState::sPending)
			{
				if (current->getState().getReadyState() == 
					TankState::SNotReady) return false;
			}
		}
	}
	return true;
}

void TankContainer::setAllNotReady()
{
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;

			// current check tanks that are not pending
			if (current->getState().getState() != TankState::sPending)
			{
				current->getState().setNotReady();
			}
		}
	}
}

int TankContainer::getNoOfNonSpectatorTanks()
{
	int count = 0;
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;
			if (!current->getState().getSpectator()) count++;
		}
	}
	return count;
}

std::map<unsigned int, Tank *> &TankContainer::getPlayingTanks()
{
	static std::map<unsigned int, Tank *> tanks;
	tanks.clear();

	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			Tank *current = (Tank *) target;
			tanks[current->getPlayerId()] = current;
		}
	}	

	return tanks;
}

std::map<unsigned int, Tank *> &TankContainer::getAllTanks()
{
	static std::map<unsigned int, Tank *> tanks;
	tanks.clear();

	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTarget())
		{
			Tank *current = (Tank *) target;
			tanks[current->getPlayerId()] = current;
		}
	}	

	return tanks;
}

int TankContainer::getNoOfTanks()
{
	int count = 0;
	std::map<unsigned int, Target *>::iterator mainitor;
	for (mainitor = targets_.getTargets().begin();
		mainitor != targets_.getTargets().end();
		mainitor++)
	{
		Target *target = (*mainitor).second;
		if (!target->isTemp())
		{
			count++;
		}
	}	
	return count;
}
