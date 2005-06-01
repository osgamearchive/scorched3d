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

#include <server/ServerScoreState.h>
#include <server/ScorchedServer.h>
#include <server/ServerCommon.h>
#include <common/OptionsGame.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <tank/TankSort.h>
#include <tank/TankContainer.h>
#include <coms/ComsScoreMessage.h>
#include <coms/ComsMessageSender.h>

ServerScoreState::ServerScoreState()
{
}

ServerScoreState::~ServerScoreState()
{
}

void ServerScoreState::enterState(const unsigned state)
{
	ComsScoreMessage scoreMessage;
	ComsMessageSender::sendToAllPlayingClients(scoreMessage);

	// Findout who is the winner
	if (ScorchedServer::instance()->getOptionsGame().getTeams() == 1)
	{
		std::list<Tank *> sortedTanks;
		std::map<unsigned int, Tank *> &tanks = 
			ScorchedServer::instance()->getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			sortedTanks.push_back((*itor).second);
		}

		
		TankSort::getSortedTanks(sortedTanks, ScorchedServer::instance()->getContext());
		std::list<Tank *>::iterator scoreitor = sortedTanks.begin();
		ServerCommon::serverLog((*scoreitor)->getPlayerId(), "\"%s\" is the overall winner!", (*scoreitor)->getName());
		ServerCommon::sendStringMessage(0, "\"%s\" is the overall winner!", (*scoreitor)->getName());
		
		// Score the winning tank as the overall winner
		StatsLogger::instance()->tankOverallWinner((*scoreitor));
	}
	else
	{
		int winningTeam = TankSort::getWinningTeam(
			ScorchedServer::instance()->getContext());
		if (winningTeam == 0)
		{
			ServerCommon::serverLog(0, "The game is a draw!");
			ServerCommon::sendStringMessage(0, "The game is a draw!");
		}
		else if (winningTeam == 1)
		{
			ServerCommon::serverLog(0, "Red team is the overall winner!");
			ServerCommon::sendStringMessage(0, "Red team is the overall winner!");
		}
		else if (winningTeam == 2)
		{
			ServerCommon::serverLog(0, "Blue team is the overall winner!");
			ServerCommon::sendStringMessage(0, "Blue team is the overall winner!");
		} 
		else if (winningTeam == 3)
		{
			ServerCommon::serverLog(0, "Green team is the overall winner!");
			ServerCommon::sendStringMessage(0, "Green team is the overall winner!");
		} 
		else if (winningTeam == 4)
		{
			ServerCommon::serverLog(0, "Yellow team is the overall winner!");
			ServerCommon::sendStringMessage(0, "Yellow team is the overall winner!");
		}
		
		// Score all the winning tanks as overall winners
		if (winningTeam != 0)
		{
			std::map<unsigned int, Tank *> &tanks = 
				ScorchedServer::instance()->getTankContainer().getPlayingTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *tank = (*itor).second;
				if (tank->getTeam() == winningTeam)
				{
					StatsLogger::instance()->tankOverallWinner(tank);
				}
			}
		}
	}

	// Reset the amount of time taken
	totalTime_ = 0.0f;
}

bool ServerScoreState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	totalTime_ += frameTime;
	return (totalTime_ > ScorchedServer::instance()->getOptionsGame().getScoreTime());
}
