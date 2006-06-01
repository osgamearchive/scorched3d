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
#include <server/ServerNewGameState.h>
#include <engine/ActionController.h>
#include <actions/Resurrection.h>
#include <coms/ComsActionsMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsPlayerStateMessage.h>
#include <common/Logger.h>
#include <common/RandomGenerator.h>

ServerShotState::ServerShotState() 
	: totalTime_(0.0f), firstTime_(true)
{
}

ServerShotState::~ServerShotState()
{
}

void ServerShotState::enterState(const unsigned state)
{
	// Send the player state to all players to ensure that the playing field
	// is consistent before the shots start
	// This should be done before the actual shots are fired or
	// any play is made
	ComsPlayerStateMessage playerState;
	ComsMessageSender::sendToAllPlayingClients(playerState);

	// Reset the counts in the action controller
	ScorchedServer::instance()->getActionController().resetTime();
	ScorchedServer::instance()->getActionController().clear();

	// Add all shots that should be run at the start of the round
	// to the action controller
	ServerShotHolder::instance()->playShots(true);

	// Reset the amount of time taken
	totalTime_ = 0.0f;
	firstTime_ = true;

	events_.initialize();
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
		stepActions(state, maxSingleSimTime, true);
	}
	else
	{
		// Add all the shots that should be run at the end of the round
		// to the action controller
		ServerShotHolder::instance()->playShots(false);
		
		// Resurect any tanks that have more lives
		resurectTanks();

		// Make sure that the new actions are run
		stepActions(state, 0.5f, false);

		// We have finished all shots
		ServerCommon::serverLog(formatString(
			"Finished playing Shots (%.2f seconds)", totalTime_));
		ScorchedServer::instance()->getActionController().logProfiledActions();

		// tell the clients of the shot outcomes
		ComsActionsMessage actionsMessage(totalTime_);
		ComsMessageSender::sendToAllPlayingClients(actionsMessage);
		ServerCommon::serverLog(formatString("Sending actions message (%i bytes)", 
			NetBufferDefault::defaultBuffer.getBufferUsed()));

		return true;
	}
	return false;
}

void ServerShotState::resurectTanks()
{
	RandomGenerator generator;
	generator.seed(rand());

	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Check for any dead tanks that can be rezed
		if (!tank->getState().getSpectator() &&
			!tank->getState().getInitializing() &&
			!tank->getState().getLoading() &&
			tank->getState().getState() == TankState::sDead &&
			(tank->getState().getLives() > 0 ||
			tank->getState().getMaxLives() == 0))
		{
			Vector tankPos = ServerNewGameState::placeTank(
				tank->getPlayerId(), tank->getTeam(),
				ScorchedServer::instance()->getContext(),
				generator);

			Resurrection *rez = new Resurrection(
				tank->getPlayerId(), tankPos);
			ScorchedServer::instance()->getActionController().addAction(rez);
		}
	}	
}

void ServerShotState::stepActions(unsigned int state, 
	float maxSingleSimTime,
	bool allowEvents)
{
	const float stepTime = 0.02f;
	float currentTime = 0.0f;

	bool allowEvents2 = !ScorchedServer::instance()->getActionController().allEvents();
	while (!ScorchedServer::instance()->getActionController().noReferencedActions() &&
		currentTime <= maxSingleSimTime)
	{
		ScorchedServer::instance()->getActionController().simulate(state, stepTime);
		if (allowEvents && allowEvents2 && totalTime_ < 10.0f)
		{
			ScorchedServer::instance()->getActionController().setActionEvent(true);
			events_.simulate(stepTime, ScorchedServer::instance()->getContext());
			ScorchedServer::instance()->getActionController().setActionEvent(false);
		}

		totalTime_ += stepTime;
		currentTime += stepTime;
	}
}

