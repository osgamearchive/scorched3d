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

#include <server/ServerReadyState.h>
#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <server/ServerShotHolder.h>
#include <server/ScorchedServer.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankContainer.h>

ServerReadyState::ServerReadyState() : time_(0.0f)
{
}

ServerReadyState::~ServerReadyState()
{
}

void ServerReadyState::enterState(const unsigned state)
{
	// Make sure that there are no shots from the last turns
	ServerShotHolder::instance()->clearShots();

	// Add any pending tanks into the game
	ServerNewGameState::addTanksToGame(state);

	// Set all the tanks to not ready
	ScorchedServer::instance()->getTankContainer().setAllNotReady();

	// Make all computer players ready
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		if (tank->getDestinationId() == 0)
		{
			tank->getState().setReady();
		}
	}

	// Set the wait timer to the current time
	time_ = 0.0f;
}

bool ServerReadyState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check all players returned ready
	if (ScorchedServer::instance()->getTankContainer().allReady())
	{
		// Make sure all clients have the correct game settings
		ComsGameStateMessage message;
		ComsMessageSender::sendToAllPlayingClients(message);	

		return true;
	}

	// Check if any players have timed out
	time_ += frameTime;
	if ((ScorchedServer::instance()->getOptionsGame().getIdleKickTime() > 0) &&
		(time_ > ScorchedServer::instance()->getOptionsGame().getIdleKickTime()))
	{
		// Kick all not returned players
		std::map<unsigned int, Tank *> &tanks = 
			ScorchedServer::instance()->getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getReadyState() == TankState::SNotReady)
			{
				Logger::log(tank->getPlayerId(), "Player \"%s\" not returned ready for %i seconds", 
					tank->getName(),
					ScorchedServer::instance()->getOptionsGame().getIdleKickTime());

				kickDestination(tank->getDestinationId());
			}
		}

		// Make sure all clients have the correct game settings
		ComsGameStateMessage message;
		ComsMessageSender::sendToAllPlayingClients(message);	

		// Stimulate into the next state
		return true;
	}
	return false;
}

