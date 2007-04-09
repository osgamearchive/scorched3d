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

#include <engine/ShotState.h>
#include <engine/ActionController.h>
#include <actions/Resurrection.h>
#include <placement/PlacementTankPosition.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsPlayerStateMessage.h>
#include <common/Logger.h>
#include <common/RandomGenerator.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <tank/TankAccessories.h>
#include <tank/TankPosition.h>
#include <target/TargetLife.h>

ShotState::ShotState(ScorchedContext &context,
	PlayShots &playShots) :
	context_(context), playShots_(playShots),
	firstTime_(true)
{
}

ShotState::~ShotState()
{
}

void ShotState::enterState(const unsigned state)
{
	// Set all player kills this turn to 0 (used for multikill)
	{
		std::map<unsigned int, Tank *> &tanks = 
			context_.tankContainer->getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			tank->getScore().setTurnKills(0);
		}
	}

	// Reset the counts in the action controller
	context_.actionController->resetTime();
	context_.actionController->clear();

	// Add all shots that should be run at the start of the round
	// to the action controller
	playShots_.playShots(context_, true);

	// Reset the amount of time taken
	firstTime_ = true;
	lastTime_ = false;

	// Add all of the new events
	context_.actionController->getEvents().initialize(context_);
}

bool ShotState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	if (!context_.actionController->noReferencedActions() ||
		firstTime_)
	{
		// The action controller will now have shots to simulate
		// We continue simulation until there are no actions left
		// in the action controller
		firstTime_ = false;
	}
	else
	{
		if (!lastTime_)
		{
			lastTime_ = true;

			// Add all the shots that should be run at the end of the round
			// to the action controller
			playShots_.playShots(context_, false);
			
			// Resurect any tanks that have more lives
			resurectTanks();
		}
		else
		{
			// We have finished all shots
			//Logger::log(formatString(
			//	"Finished playing Shots"));
			context_.actionController->getEvents().clear();
			context_.actionController->logProfiledActions();

			return true;
		}
	}
	return false;
}

void ShotState::resurectTanks()
{
	std::map<unsigned int, Tank *> &tanks =
		context_.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Check for any dead tanks that can be rezed
		if (!tank->getState().getSpectator() &&
			tank->getState().getState() == TankState::sDead &&
			(tank->getState().getLives() > 0 ||
			tank->getState().getMaxLives() == 0))
		{
			Vector tankPos = PlacementTankPosition::placeTank(
				tank->getPlayerId(), tank->getTeam(),
				context_,
				context_.actionController->getRandom());

			Resurrection *rez = new Resurrection(
				tank->getPlayerId(), tankPos);
			context_.actionController->addAction(rez);
		}
	}	
}
