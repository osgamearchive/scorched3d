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

#include <tankai/TankAIComputerTarget.h>
#include <tank/TankLib.h>
#include <tank/TankContainer.h>
#include <tank/TankPosition.h>
#include <tank/TankState.h>
#include <server/ScorchedServer.h>
#include <common/DefinesMath.h>

TankAIComputerTarget::TankAIComputerTarget()
{

}

TankAIComputerTarget::~TankAIComputerTarget()
{

}

bool TankAIComputerTarget::parseConfig(XMLNode *node)
{
	if (!node->getNamedChild("targettype", targetType_)) return false;
	if (0 != strcmp(targetType_.c_str(), "nearest") &&
		0 != strcmp(targetType_.c_str(), "agressor") &&
		0 != strcmp(targetType_.c_str(), "none"))
	{
		return node->returnError("Unknown targettype. "
			"Should be one of none, nearest, agressor");
	}

	return true;
}

void TankAIComputerTarget::setTank(Tank *tank)
{
	currentTank_ = tank;
}

Tank *TankAIComputerTarget::findTankToShootAt()
{
	if (0 == strcmp(targetType_.c_str(), "none")) 
		return 0;
	if (0 == strcmp(targetType_.c_str(), "nearest")) 
		return findNearTankToShootAt();
	if (0 == strcmp(targetType_.c_str(), "agressor"))
		return findShotAtTankToShootAt();

	return findNearTankToShootAt();
}

Tank *TankAIComputerTarget::findNearTankToShootAt()
{
	// Get the list of all tanks left in the game
	// Sorted by distance
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		ScorchedServer::instance()->getContext(),
		currentTank_->getPosition().getTankPosition(), 
		sortedTanks,
		currentTank_->getTeam());

	// Choose a tank based on a probablity
	// The nearest tanks are more likely to be choosen
	Tank *returnTank = 0;
	if (!sortedTanks.empty())
	{
		// The number of tanks to skip before returning
		int noTanks = int(RAND * 1.5f);

		std::list<std::pair<float, Tank *> >::iterator itor = sortedTanks.begin();
		// Skip the current tank if it is the first tank
		if ((*itor).second->getPlayerId() == currentTank_->getPlayerId()) itor++;

		// Count the number of tanks to skip
		for (int i=0;itor != sortedTanks.end() && i<=noTanks; itor++, i++)
		{
			returnTank = (*itor).second;
		}
	}

	return returnTank;
}

Tank *TankAIComputerTarget::findShotAtTankToShootAt()
{
	// For each tank build a probability of choosing based
	// on the number of shots it fires at us
	int count = 0;
	std::list<std::pair<Tank *, int> > passed;
	{
		std::map<unsigned int, int>::iterator itor;
		for (itor = hitlist_.begin();
			itor != hitlist_.end();
			itor++)
		{
			unsigned int tankId = (*itor).first;
			unsigned int tankCount = (*itor).second;
			Tank *tank = ScorchedServer::instance()->
				getTankContainer().getTankById(tankId);

			// Check the tank is alive
			if (tank && 
				tank->getState().getState() == TankState::sNormal)
			{
				// Check the tank is not in the same team
				if (currentTank_->getTeam() == 0 ||
					currentTank_->getTeam() != tank->getTeam())
				{
					count += tankCount;
					passed.push_back(
						std::pair<Tank *, int>(tank, tankCount));
				}
			}
		}
	}

	// Did we find any tanks
	if (passed.empty()) return findNearTankToShootAt();

	// Choose a tank based on the number of times they 
	// have shot at us
	Tank *tank = 0;
	{
		int randCount = int(RAND * float(count));
		std::list<std::pair<Tank *, int> >::iterator itor;
		for (itor = passed.begin();
			itor != passed.end();
			itor++)
		{
			tank = (*itor).first;
			unsigned int tankCount = (*itor).second;

			randCount -= tankCount;
			if (randCount < 0) break;
		}
	}
	return tank;
}

void TankAIComputerTarget::shotLanded(
		ScorchedCollisionId collision,
		Weapon *weapon, unsigned int firer, 
		Vector &position)
{
	// Check we did not fire this shot
	if (firer == currentTank_->getPlayerId()) return;

	// Find the closest tank to this shot
	Tank *currentTank = 0;
	unsigned int currentDist = UINT_MAX;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getAllTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		unsigned int dist = (unsigned int)
			(position - tank->getPosition().getTankPosition()).Magnitude();
		if (dist < currentDist)
		{
			currentTank = tank;
			currentDist = dist;
		}
	}

	// Did the shot land close enough to be considered harmful
	if (currentDist > 75) return;

	// Are we the closest tank
	if (currentTank != currentTank_) return;

	// We now hate this tank :)
	addTankToHitList(firer, 1);
}

void TankAIComputerTarget::tankHurt(Weapon *weapon, unsigned int firer)
{
	// Don't take a grudge against ourselves
	if (firer != currentTank_->getPlayerId())
	{
		// Add tanks we take a grudge against
		addTankToHitList(firer, 2);
	}
}

void TankAIComputerTarget::addTankToHitList(unsigned int firer, int count)
{
	std::map<unsigned int, int>::iterator findItor = 
		hitlist_.find(firer);
	if (findItor == hitlist_.end())
	{
		hitlist_[firer] = count;
	}
	else
	{
		hitlist_[firer] += count;
	}
}

void TankAIComputerTarget::newMatch()
{
	hitlist_.clear();
}
