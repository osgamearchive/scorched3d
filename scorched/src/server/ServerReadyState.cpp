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
#include <server/ServerCommon.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <common/OptionsTransient.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsPlayerStatusMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsLastChanceMessage.h>
#include <tank/TankContainer.h>

ServerReadyState::ServerReadyState(ServerShotState *shotState) : 
	time_(0.0f), shotState_(shotState)
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

	// Set the wait timer to the current time
	time_ = 0.0f;

	if (shotState_->getShotTime() == 0.0f)
	{
		idleTime_ = (float) ScorchedServer::instance()->
			getOptionsGame().getIdleKickTime();
	}
	else
	{
		idleTime_ = (float) ScorchedServer::instance()->
			getOptionsGame().getIdleShotKickTime();
	}

	// Add on the time the shots took to simulate
	// So we don't time clients out too quickly
	time_ -= shotState_->getShotTime();
	shotState_->getShotTime() = 0.0f;

	// Make all computer players ready
	// And send out the first status messages
	ComsPlayerStatusMessage statusMessage;
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
			// Set computer player ready
			tank->getState().setReady();
		}
		else
		{
			// Set all other players not-ready
			statusMessage.getWaitingPlayers().push_back(tank->getPlayerId());
		}
	}

	// Tell clients who we are waiting on
	ComsMessageSender::sendToAllPlayingClients(statusMessage);	
}

bool ServerReadyState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Send status messages every 5 seconds
	if (time_ > 0.0f)
	{
		if (((int) time_) / 5 != ((int) (time_ + frameTime)) / 5)
		{
			// Say who we are waiting on
			ComsPlayerStatusMessage statusMessage;
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
					statusMessage.getWaitingPlayers().push_back(tank->getPlayerId());
				}
			}
			ComsMessageSender::sendToAllPlayingClients(statusMessage);			

			// Send out a last chance message just before we kick
			if (OptionsParam::instance()->getDedicatedServer() &&
				(idleTime_ > 0) && (time_ > idleTime_ - 6))
			{
				for (itor = tanks.begin();
					itor != tanks.end();
					itor++)
				{
					Tank *tank = (*itor).second;
					if (tank->getState().getReadyState() == TankState::SNotReady)
					{
						Logger::log(0, "Sending last chance message to \"%s\" after %.0f seconds",
							tank->getName(), time_);

						// Tell client to hurry up
						ComsLastChanceMessage chanceMessage;
						ComsMessageSender::sendToSingleClient(
							chanceMessage,
							tank->getDestinationId());	
					}
				}
			}
		}
	}	

	time_ += frameTime;

	// Slow down the shots, when we are on a dedicated server
	// so if there are only bots playing the games don't end
	// too quickly
	if (!OptionsParam::instance()->getDedicatedServer() ||
		time_ > 0.0f) 
	{
		// Check all players returned ready
		if (ScorchedServer::instance()->getTankContainer().allReady())
		{
			//Logger::log(0, "All ready after %.2f seconds", time_);
			finished();
			return true;
		}
	}

	// Check if any players have timed out
	if (OptionsParam::instance()->getDedicatedServer() &&
		(idleTime_ > 0) && (time_ > idleTime_))
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
				if (OptionsParam::instance()->getDedicatedServer())
				{
					Logger::log(0, 
						"Player \"%s\" not returned ready for %0.f seconds", 
						tank->getName(),
						idleTime_);
				}

				ServerCommon::sendString(tank->getDestinationId(), 
					"Kicked for not responding for %.0f seconds", idleTime_);
				ServerCommon::kickDestination(tank->getDestinationId(), true);
			}
		}

		finished();

		// Stimulate into the next state
		return true;
	}
	return false;
}

void ServerReadyState::finished()
{
	// Update the stats for the players before sending out the
	// stats message
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Ensure stats are uptodate
		StatsLogger::instance()->updateStats(tank);

		// Get the new tanks rank
		char *rank = StatsLogger::instance()->tankRank(tank);
		tank->getScore().setStatsRank(rank);
	}

	// Say we are waiting on no one
	ComsPlayerStatusMessage statusMessage;
	ComsMessageSender::sendToAllPlayingClients(statusMessage);	

	// Make sure all clients have the correct game settings
	ComsGameStateMessage message;
	ComsMessageSender::sendToAllPlayingClients(message);	
}
