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

#include <server/ServerShotFinishedState.h>
#include <server/ScorchedServer.h>
#include <server/ServerState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ServerNextShotState.h>
#include <server/ServerCommon.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <coms/ComsScoreMessage.h>
#include <coms/ComsPlayerStateMessage.h>
#include <coms/ComsMessageSender.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankContainer.h>
#include <tank/TankTeamScore.h>
#include <tank/TankSort.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>

float ServerShotFinishedState::speed_(1.0f);

ServerShotFinishedState::ServerShotFinishedState() :
	GameStateI("ServerShotFinishedState")
{
}

ServerShotFinishedState::~ServerShotFinishedState()
{
}

void ServerShotFinishedState::enterState(const unsigned state)
{
	totalTime_ = 0.0f;
	waitTime_ = 0.0f;

	// Check if any player or team has won the round
	if (ServerNextShotState::getRoundFinished() ||
		((ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() >
		ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns()) &&
		ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns() > 0))
	{
		// The actual state transition for no tanks left is done
		// in the next round state however
		// score the last remaining tanks here
		bool finalScore = scoreWinners();
		if (finalScore)
		{
			waitTime_ = (float) ScorchedServer::instance()->
				getOptionsGame().getScoreTime();
		}
		else
		{
			waitTime_ = (float) ScorchedServer::instance()->
				getOptionsGame().getRoundScoreTime();
		}

		ComsPlayerStateMessage playerMessage(true);
		ComsMessageSender::sendToAllPlayingClients(playerMessage);
		ComsScoreMessage message(finalScore);
		ComsMessageSender::sendToAllPlayingClients(message);
	}
}

bool ServerShotFinishedState::acceptStateChange(const unsigned state, 
	const unsigned nextState,
	float frameTime)
{
	totalTime_ += frameTime * speed_;
	if (totalTime_ > waitTime_)
	{
		if (ServerTooFewPlayersStimulus::instance()->acceptStateChange(state, 
			ServerState::ServerStateTooFewPlayers, 0.0f))
		{
			ScorchedServer::instance()->getGameState().stimulate(
				ServerState::ServerStimulusTooFewPlayers);
		}
		else return true;
	}
	return false;
}

bool ServerShotFinishedState::scoreWinners()
{
	// Calculate all the tanks interest
	float interest = float(ScorchedServer::instance()->
		getOptionsGame().getInterest()) / 100.0f;
	std::map<unsigned int, Tank *> &playingTank = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();

	int moneyWonForRound = 
		ScorchedServer::instance()->getOptionsGame().getMoneyWonForRound();
	int moneyWonForLives =
	        ScorchedServer::instance()->getOptionsGame().getMoneyWonForLives();
	int scoreWonForRound = 
		ScorchedServer::instance()->getOptionsGame().getScoreWonForRound();
	int scoreWonForLives = 
		ScorchedServer::instance()->getOptionsGame().getScoreWonForLives();

	// Add score for each life left
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = playingTank.begin();
	        itor != playingTank.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getSpectator()) continue;
		if (tank->getAlive())
			tank->getScore().setScore(
	                        tank->getScore().getScore() + (scoreWonForLives * tank->getState().getLives()));
	}
				
	// Is it a team game?
	if (ScorchedServer::instance()->getOptionsGame().getTeams() > 1)
	{
		// Yes, check which team has won and give points accordingly
		// Add score for round wins
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = playingTank.begin();
		        itor != playingTank.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getSpectator()) continue;

			if (tank->getAlive())
			{
				ScorchedServer::instance()->getContext().tankTeamScore->addScore(
				        scoreWonForLives * tank->getState().getLives(), tank->getTeam());
			}
		}

		std::set<int> winningTeams;
		int winningTeam = 
			ScorchedServer::instance()->getContext().tankTeamScore->getWonGame();
		if (winningTeam != 0)
		{
			winningTeams.insert(winningTeam);
			ScorchedServer::instance()->getContext().tankTeamScore->addScore(
				scoreWonForRound, winningTeam);
		}
		else
		{
			for (itor = playingTank.begin();
				itor != playingTank.end();
				itor++)
			{
				Tank *tank = (*itor).second;
				if (tank->getState().getSpectator()) continue;

				if (tank->getAlive())
				{
					if (winningTeams.find(tank->getTeam()) == winningTeams.end())
					{
						ScorchedServer::instance()->getContext().tankTeamScore->addScore(
							scoreWonForRound, tank->getTeam());
						winningTeams.insert(tank->getTeam());

					}
				}
			}
		}

		for (itor = playingTank.begin();
			itor != playingTank.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getSpectator()) continue;

			if (winningTeams.find(tank->getTeam()) != winningTeams.end())
			{
				StatsLogger::instance()->tankWon(tank);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + moneyWonForRound);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + (moneyWonForLives * tank->getState().getLives()));
				tank->getScore().setWins(
					tank->getScore().getWins() + 1);
			}
		}
	}
	else
	{
		// Check which player has won and give points accordingly
		bool tankWon = false;
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = playingTank.begin();
			itor != playingTank.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getSpectator()) continue;

			if (tank->getScore().getWonGame())
			{
				StatsLogger::instance()->tankWon(tank);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + moneyWonForRound);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + (moneyWonForLives * tank->getState().getLives()));
				tank->getScore().setWins(
					tank->getScore().getWins() + 1);
				tank->getScore().setScore(
					tank->getScore().getScore() + scoreWonForRound);

				tankWon = true;
			}
		}

		if (!tankWon)
		{
			for (itor = playingTank.begin();
				itor != playingTank.end();
				itor++)
			{
				Tank *tank = (*itor).second;
				if (tank->getState().getSpectator()) continue;

				if (tank->getAlive())
				{
					StatsLogger::instance()->tankWon(tank);
					tank->getScore().setMoney(
						tank->getScore().getMoney() + moneyWonForRound);
					tank->getScore().setMoney(
						tank->getScore().getMoney() + (moneyWonForLives * tank->getState().getLives()));
					tank->getScore().setWins(
						tank->getScore().getWins() + 1);	
					tank->getScore().setScore(
						tank->getScore().getScore() + scoreWonForRound);
				}
			}
		}
	}

	// Give interest and round played money to all tanks
	{
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = playingTank.begin();
			itor != playingTank.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			if (tank->getState().getSpectator()) continue;

			int addMoney = int(float(tank->getScore().getMoney()) * interest) +
				ScorchedServer::instance()->getOptionsGame().getMoneyPerRound();
			tank->getScore().setMoney(tank->getScore().getMoney() + addMoney);

			// Make sure this tank is dead
			if (tank->getState().getState() == TankState::sNormal)
			{
				tank->getState().setState(TankState::sDead);
			}
		}
	}

	// Update the stats for the players before sending out the
	// stats message
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		// Money earned
		int newMoney = tank->getScore().getTotalMoneyEarnedStat();
		int scoreAdded = (newMoney * 
			ScorchedServer::instance()->getOptionsGame().getScorePerMoneyEntry())
			/ 1000;
		tank->getScore().setScore(tank->getScore().getScore() + scoreAdded);

		// Ensure stats are uptodate
		StatsLogger::instance()->updateStats(tank);

		// Reset the totaled stats
		tank->getScore().resetTotalEarnedStats();

		// Get the new tanks rank
		char *rank = StatsLogger::instance()->tankRank(tank);
		tank->getScore().setStatsRank(rank);
	}

	// Check if this is the last round that will be played
	bool overAllWinner = false;
	if (ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo() >=
		ScorchedServer::instance()->getOptionsGame().getNoRounds())
	{
		overAllWinner = true;
	}
	else
	{
		if (ServerTooFewPlayersStimulus::instance()->acceptStateChange(0, 
			ServerState::ServerStateTooFewPlayers, 0.0f))
		{
			if (ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo() >
				ScorchedServer::instance()->getOptionsGame().getNoRounds() / 2 && 
				ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo() > 2)
			{
				overAllWinner = true;
			}
		}
	}

	// Its the very last round, score the overall winner
	if (overAllWinner)
	{
		scoreOverallWinner();
	}
	return overAllWinner;
}

void ServerShotFinishedState::scoreOverallWinner()
{
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
			Tank *current = (*itor).second;
			if (!current->getState().getSpectator() &&
				current->getState().getState() != TankState::sPending &&
				current->getState().getState() != TankState::sInitializing &&
				current->getState().getState() != TankState::sLoading)
			{
				sortedTanks.push_back(current);
			}
		}
		
		TankSort::getSortedTanks(sortedTanks, ScorchedServer::instance()->getContext());
		if (!sortedTanks.empty())
		{
			Tank *topScore = *(sortedTanks.begin());

			std::string names;
			std::list<Tank *> winners;
            std::list<Tank *>::iterator scoreitor;
			for (scoreitor = sortedTanks.begin();
				scoreitor != sortedTanks.end();
				scoreitor++)
			{
				Tank *current = *scoreitor;
				if (topScore->getScore().getScore() == 
					current->getScore().getScore())
				{
					winners.push_back(current);
					if (!names.empty()) names.append(",");
					names.append(current->getName());

					// Score the winning tank as the overall winner
					StatsLogger::instance()->tankOverallWinner(current);
				}
			}

			if (winners.size() == 1)
			{
				ServerCommon::sendStringMessage(0, 
					formatString("%s is the overall winner!", 
					names.c_str()));
			}
			else
			{
				ServerCommon::sendStringMessage(0, 
					formatString("%s are the overall winners!", 
					names.c_str()));
			}
		}
	}
	else
	{
		int winningTeam = TankSort::getWinningTeam(
			ScorchedServer::instance()->getContext());
		if (winningTeam == 0)
		{
			ServerCommon::sendStringMessage(0, "The game is a draw!");
		}
		else
		{
			ServerCommon::sendStringMessage(0, 
				formatString("%s team is the overall winner!",
				TankColorGenerator::getTeamName(winningTeam)));
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
					if (!tank->getState().getSpectator() &&
						tank->getState().getState() != TankState::sPending &&
						tank->getState().getState() != TankState::sInitializing &&
						tank->getState().getState() != TankState::sLoading)
					{
						StatsLogger::instance()->tankOverallWinner(tank);
					}
				}
			}
		}
	}
}
