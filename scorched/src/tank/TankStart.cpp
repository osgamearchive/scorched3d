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

#include <landscape/GlobalHMap.h>
#include <tank/TankStart.h>
#include <tank/TankContainer.h>
#include <engine/ActionController.h>
#include <actions/TankScored.h>
#include <actions/ShowScore.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>

void TankStart::scoreWinners()
{
	// Calculate all the tanks interest
	float interest = float(OptionsGame::instance()->getInterest()) / 100.0f;

	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Calculate how much money each tank should get
		int addMoney = int(float(tank->getScore().getMoney()) * interest);
		int addRounds = 0;
		if (tank->getState().getState() == TankState::sNormal)
		{
			addMoney += OptionsGame::instance()->getMoneyWonForRound();
			addRounds = 1;
		}

		// Add the money to the tanks
		if (addRounds != 0 || addMoney != 0)
		{
			TankScored *scored = new TankScored(tank->getPlayerId(), 
				addMoney,
				0,
				addRounds);
			ActionController::instance()->addAction(scored);
		}
	}

	// Add an action to make all clients show the current
	// rankings
	ActionController::instance()->addAction(new ShowScore);
}

void TankStart::newGame()
{
	// Set all options (wind etc..)
	OptionsTransient::instance()->newGame();

	// Move all pending and dead tanks into the fray
	// Set them all to not ready
	TankContainer::instance()->newGame();

	// Generate the new level
	unsigned long seed = rand();
	GlobalHMap::instance()->generateHMap(seed);

	// Set the start positions for the tanks
	// Must be generated after the level as it alters the
	// level
	TankStart::calculateStartPosition();
}

void TankStart::nextRound()
{
	// Load the set of options for this next player
	OptionsTransient::instance()->nextRound();

	// Tell all tanks it is the next round
	// Set the all the tanks to wait state
	// Once all tanks are ready the next state will be chosen
	TankContainer::instance()->nextRound();	
}

void TankStart::flattenArea(Vector &tankPos)
{
	// Flatten a small area around the tank,
	// before the game starts
	for (int x=-2; x<=2; x++)
	{
		for (int y=-2; y<=2; y++)
		{
			int ix = (int) tankPos[0] + x;
			int iy = (int) tankPos[1] + y;
			if (ix >= 0 && iy >= 0 &&
				ix < GlobalHMap::instance()->getHMap().getWidth() &&
				iy < GlobalHMap::instance()->getHMap().getWidth())
			{
				GlobalHMap::instance()->getHMap().getHeight(ix, iy) = tankPos[2];
			}
		}
	}
}

void TankStart::calculateStartPosition()
{
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	const int tankBorder = 10;
	for (mainitor = tanks.begin();
		 mainitor != tanks.end();
		 mainitor++)
	{
		Vector tankPos;
		bool tooClose = true;
		float closeness = (float) 
			OptionsGame::instance()->getTankStartCloseness();
		while (tooClose)
		{
			// Find a new position for the tank
			tooClose = false;
			float posX = float (GlobalHMap::instance()->getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float posY = float (GlobalHMap::instance()->getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float height = GlobalHMap::instance()->getHMap().
				getHeight((int) posX, (int) posY);
			tankPos = Vector(posX, posY, height);

			// Make sure not lower than water line
			// And that the tank is not too close to other tanks
			if (tankPos[2] < 5.5f) 
			{
				tooClose = true;
			}
			else
			{
				std::map<unsigned int, Tank *>::iterator itor;
				for (itor = tanks.begin();
					itor != mainitor;
					itor++)
				{
					if ((tankPos - (*itor).second->getPhysics().getTankPosition()).Magnitude() < 
						closeness) 
					{
						tooClose = true;
						break;
					}
				}
			}

			closeness -= 1.0f;
		}

		// Get the height of the tank
		tankPos[2] = GlobalHMap::instance()->getHMap().getInterpHeight(
			tankPos[0], tankPos[1]);
	
		// Set the starting position of the tank
		flattenArea(tankPos);
		(*mainitor).second->getPhysics().setTankPosition(tankPos);
	}
}


