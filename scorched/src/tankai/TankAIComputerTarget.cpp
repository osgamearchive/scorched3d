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
#include <server/ScorchedServer.h>

TankAIComputerTarget::TankAIComputerTarget()
{

}

TankAIComputerTarget::~TankAIComputerTarget()
{

}

void TankAIComputerTarget::setTank(Tank *tank)
{
	currentTank_ = tank;
}

Tank *TankAIComputerTarget::findTankToShootAt()
{
	// Get the list of all tanks left in the game
	// Sorted by distance
	std::list<std::pair<float, Tank *> > sortedTanks;
	TankLib::getTanksSortedByDistance(
		ScorchedServer::instance()->getContext(),
		currentTank_->getPhysics().getTankPosition(), 
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

void TankAIComputerTarget::shotLanded(
		ParticleAction action,
		ScorchedCollisionInfo *collision,
		Weapon *weapon, unsigned int firer, 
		Vector &position,
		unsigned int landedCounter)
{
	// Check we did not fire this shot
	if (firer == currentTank_->getPlayerId()) return;

	// Check this is the first time this shot landed
	//if (landedCounter > 1) return;

	// Find the closest tank to this shot
	Tank *currentTank = 0;
	unsigned int currentDist = UINT_MAX;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		unsigned int dist = (unsigned int)
			(position - tank->getPhysics().getTankPosition()).Magnitude();
		if (dist < currentDist)
		{
			currentTank = tank;
			currentDist = dist;
		}
	}

	// Did the shot land close enough to be considered harmful
	if (currentDist > 75) return;

	// Are we the closest tank
	if (currentTank != currentTank) return;

	// We now hate this tank :)
	addTankToHitList(firer);
}

void TankAIComputerTarget::tankHurt(Weapon *weapon, unsigned int firer)
{
	// Don't take a grudge against ourselves
	if (firer != currentTank_->getPlayerId())
	{
		// Add tanks we take a grudge against
		addTankToHitList(firer);
	}
}

void TankAIComputerTarget::addTankToHitList(unsigned int firer)
{
	std::map<unsigned int, unsigned int>::iterator findItor = 
		hitlist_.find(firer);
	if (findItor == hitlist_.end())
	{
		hitlist_[firer] = 1;
	}
	else
	{
		hitlist_[firer] ++;
	}
}

void TankAIComputerTarget::reset()
{
	hitlist_.clear();
}
