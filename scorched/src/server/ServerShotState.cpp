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

#include <server/ScorchedServer.h>
#include <server/ServerShotState.h>
#include <server/ServerState.h>
#include <server/ServerShotHolder.h>
#include <server/ServerCommon.h>
#include <actions/TankScored.h>
#include <actions/ShowScore.h>
#include <coms/ComsActionsMessage.h>
#include <coms/ComsMessageSender.h>
#include <engine/ActionController.h>
#include <common/StatsLogger.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>

ServerShotState::ServerShotState() 
	: totalTime_(0.0f), firstTime_(true)
{
}

ServerShotState::~ServerShotState()
{
}

void ServerShotState::enterState(const unsigned state)
{
	// Reset the counts in the action controller
	ScorchedServer::instance()->getActionController().resetTime();
	ScorchedServer::instance()->getActionController().clear();

	// Add all shots that should be run at the start of the round
	// to the action controller
	ServerShotHolder::instance()->playShots(true);

	// Reset the amount of time taken
	totalTime_ = 0.0f;
	firstTime_ = true;
}

bool ServerShotState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	if (!ScorchedServer::instance()->getActionController().noReferencedActions() ||
		firstTime_)
	{
		firstTime_ = false;

		// The action controller will now have shots to simulate
		// We continue simulation until there are no actions left
		// in the action controller
		const float maxSingleSimTime = 5.0f;
		stepActions(state, maxSingleSimTime);

		// All the actions have finished
		if (ScorchedServer::instance()->getActionController().noReferencedActions())
		{ 
			// Add all the shots that should be run at the end of the round
			// to the action controller
			ServerShotHolder::instance()->playShots(false);
			stepActions(state, 0.5f);

			// Check if any player or team has won the round
			if (ScorchedServer::instance()->getTankContainer().aliveCount() < 2 ||
				(ScorchedServer::instance()->getTankContainer().teamCount() == 1 &&
				ScorchedServer::instance()->getOptionsGame().getTeams() == 2))
			{
				// The actual state transition for no tanks left is done
				// in the next round state however
				// score the last remaining tanks here as it is an action
				scoreWinners();

				// Make sure all actions have been executed
				stepActions(state, 120.0f);
			}
		}
	}
	else
	{
		// We have finished all shots
		ServerCommon::serverLog(0, "Finished playing Shots (%.2f seconds)", totalTime_);

		// tell the clients of the shot outcomes
		ComsActionsMessage actionsMessage(totalTime_);
		ComsMessageSender::sendToAllPlayingClients(actionsMessage);
		ServerCommon::serverLog(0, "Sending actions message (%i bytes)", 
			NetBufferDefault::defaultBuffer.getBufferUsed());

		return true;
	}
	return false;
}

void ServerShotState::stepActions(unsigned int state, float maxSingleSimTime)
{
	const float stepTime = 0.02f;
	float currentTime = 0.0f;
	while (!ScorchedServer::instance()->getActionController().noReferencedActions())
	{
		ScorchedServer::instance()->getActionController().simulate(state, stepTime);
		totalTime_ += stepTime;
		currentTime += stepTime;
		if (currentTime > maxSingleSimTime) break;
	}
}

void ServerShotState::scoreWinners()
{
	// Calculate all the tanks interest
	float interest = float(ScorchedServer::instance()->
		getOptionsGame().getInterest()) / 100.0f;

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
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
			addMoney += ScorchedServer::instance()->
				getOptionsGame().getMoneyWonForRound();
			addRounds = 1;
			StatsLogger::instance()->tankWon(tank);
		}

		// Add the money to the tanks
		if (addRounds != 0 || addMoney != 0)
		{
			TankScored *scored = new TankScored(tank->getPlayerId(), 
				addMoney,
				0,
				addRounds);
			ScorchedServer::instance()->getActionController().addAction(scored);
		}
	}

	// Only show the score dialog if it is not the very last round
	// if it is the very last round then the end of game score dialog will
	// be shown instead
	if (ScorchedServer::instance()->getOptionsTransient().getNoRoundsLeft() > 0)
	{
		// Add an action to make all clients show the current
		// rankings
		ScorchedServer::instance()->getActionController().addAction(new ShowScore);
	}
}
