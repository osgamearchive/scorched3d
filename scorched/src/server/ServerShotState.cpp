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


#include <server/ServerShotState.h>
#include <server/ServerState.h>
#include <server/ServerShotHolder.h>
#include <tank/TankContainer.h>
#include <tank/TankStart.h>
#include <coms/ComsActionsMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <landscape/GlobalHMap.h>
#include <engine/ActionController.h>

ServerShotState::ServerShotState()
{
}

ServerShotState::~ServerShotState()
{
}

void ServerShotState::enterState(const unsigned state)
{
	Logger::log(0, "Playing Shots");

	// Reset the counts in the action controller
	ActionController::instance()->resetTime();
	ActionController::instance()->clear();

	// Add all shots to the action controller
	ServerShotHolder::instance()->playShots();

	// Reset the amount of time taken
	totalTime_ = 0.0f;
	sentMessage_ = false;
}

bool ServerShotState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	if (!ActionController::instance()->noReferencedActions())
	{
		// The action controller will now have shots to simulate
		// We continue simulation until there are no actions left
		// in the action controller
		const float maxSingleSimTime = 5.0f;
		stepActions(state, maxSingleSimTime);

		// All the actions have finished
		if (ActionController::instance()->noReferencedActions() &&
			TankContainer::instance()->aliveCount() < 2)
		{
			// The actual state transition for no tanks left is done
			// in the next round state however
			// score the last remaining tanks here as it is an action
			TankStart::scoreWinners();

			// Make sure all actions have been executed
			stepActions(state, 120.0f);
		}
	}
	else
	{
		if (!sentMessage_)
		{
			sentMessage_ = true;

			// We have finished all shots
			Logger::log(0, "Finished playing Shots (%.2f seconds)", totalTime_);

			// tell the clients of the shot outcomes
			ComsActionsMessage actionsMessage;
			ComsMessageSender::sendToAllPlayingClients(actionsMessage);
			Logger::log(0, "Sending actions message (%i bytes)", 
				NetBufferDefault::defaultBuffer.getBufferUsed());

			// Remove a few seconds from the wait time just to be sure
			totalTime_ -= 2.5f;
			if (totalTime_ <= 0.0f) 
			{
				return true;
			}
		}
		else
		{
			// Wait for the clients to finish the shot
			totalTime_ -= frameTime;
			if (totalTime_ <= 0.0f) 
			{
				return true;
			}
		}
	}
	return false;
}

void ServerShotState::stepActions(unsigned int state, float maxSingleSimTime)
{
	const float stepTime = 0.02f;
	float currentTime = 0.0f;
	while (!ActionController::instance()->noReferencedActions())
	{
		ActionController::instance()->simulate(state, stepTime);
		totalTime_ += stepTime;
		currentTime += stepTime;
		if (currentTime > maxSingleSimTime) break;
	}
}
