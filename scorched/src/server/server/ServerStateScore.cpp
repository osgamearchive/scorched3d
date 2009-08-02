////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#include <server/ServerStateScore.h>
#include <server/ScorchedServer.h>
#include <server/ServerSimulator.h>
#include <server/ServerChannelManager.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <simactions/ShowScoreSimAction.h>
#include <tank/TankContainer.h>
#include <tank/TankScore.h>
#include <tank/TankSort.h>
#include <tank/TankTeamScore.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankState.h>
#include <LUA/LUAScriptHook.h>

ServerStateScore::ServerStateScore()
{
}

ServerStateScore::~ServerStateScore()
{
}

void ServerStateScore::enterState()
{
	finished_ = false;

	bool finalScore = scoreWinners();

	fixed scoreTime = 0;
	if (finalScore)
	{
		scoreTime = fixed(ScorchedServer::instance()->getOptionsGame().getScoreTime());
	}
	else
	{
		scoreTime = fixed(ScorchedServer::instance()->getOptionsGame().getRoundScoreTime());
	}

	fixed(ScorchedServer::instance()->getOptionsGame().getScoreTime());
	ShowScoreSimAction *showScoreAction = new ShowScoreSimAction(scoreTime, finalScore);
	ScorchedServer::instance()->getServerSimulator().addSimulatorAction(showScoreAction);
}

void ServerStateScore::scoreFinished()
{
	finished_ = true;
}

bool ServerStateScore::simulate()
{
	if (finished_)
	{
		bool actions = ScorchedServer::instance()->getServerSimulator().getSendActionsEmpty();
		return actions;
	}
	return false;
}

bool ServerStateScore::scoreWinners()
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
	int skillWonForRound = 
		ScorchedServer::instance()->getOptionsGame().getSkillForRound();

	// Add score for each life left
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = playingTank.begin();
	        itor != playingTank.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (!tank->getState().getTankPlaying()) continue;
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
			if (!tank->getState().getTankPlaying()) continue;

			if (tank->getAlive())
			{
				ScorchedServer::instance()->getContext().getTankTeamScore().addScore(
				        scoreWonForLives * tank->getState().getLives(), tank->getTeam());
			}
		}

		std::set<int> winningTeams;
		int winningTeam = 
			ScorchedServer::instance()->getContext().getTankTeamScore().getWonGame();
		if (winningTeam != 0)
		{
			winningTeams.insert(winningTeam);
			ScorchedServer::instance()->getContext().getTankTeamScore().addScore(
				scoreWonForRound, winningTeam);
		}
		else
		{
			for (itor = playingTank.begin();
				itor != playingTank.end();
				itor++)
			{
				Tank *tank = (*itor).second;
				if (!tank->getState().getTankPlaying()) continue;

				if (tank->getAlive())
				{
					if (winningTeams.find(tank->getTeam()) == winningTeams.end())
					{
						ScorchedServer::instance()->getContext().getTankTeamScore().addScore(
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
			if (!tank->getState().getTankPlaying()) continue;

			if (winningTeams.find(tank->getTeam()) != winningTeams.end())
			{
				StatsLogger::instance()->tankWon(tank);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + moneyWonForRound);
				tank->getScore().setMoney(
					tank->getScore().getMoney() + (moneyWonForLives * tank->getState().getLives()));
				tank->getScore().setWins(
					tank->getScore().getWins() + 1);
				tank->getScore().setSkill(
					tank->getScore().getSkill() + skillWonForRound);
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
			if (!tank->getState().getTankPlaying()) continue;

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
				tank->getScore().setSkill(
					tank->getScore().getSkill() + skillWonForRound);

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
				if (!tank->getState().getTankPlaying()) continue;

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
					tank->getScore().setSkill(
						tank->getScore().getSkill() + skillWonForRound);
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
			if (!tank->getState().getTankPlaying()) continue;

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

	// Check if this is the last round that will be played
	bool overAllWinner = false;
	if (ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo() >=
		ScorchedServer::instance()->getOptionsGame().getNoRounds())
	{
		overAllWinner = true;
	}
	else
	{
		//if (ServerTooFewPlayersStimulus::instance()->acceptStateChange(0, 
		//	ServerState::ServerStateTooFewPlayers, 0.0f))
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

	// Tell scripts to score 
	ScorchedServer::instance()->getLUAScriptHook().callHook("server_score", 
		overAllWinner);

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
			ScorchedServer::instance()->getOptionsGame().getScorePerMoney())
			/ 1000;
		tank->getScore().setScore(tank->getScore().getScore() + scoreAdded);

		// Ensure stats are uptodate
		StatsLogger::instance()->updateStats(tank);

		// Reset the totaled stats
		tank->getScore().resetTotalEarnedStats();

		// Get the new tanks rank
		StatsLogger::TankRank rank = StatsLogger::instance()->tankRank(tank);
		tank->getScore().setRank(rank.rank);
	}

	StatsLogger::instance()->periodicUpdate();
	return overAllWinner;
}

void ServerStateScore::scoreOverallWinner()
{
	int skillWonForMatch = 
		ScorchedServer::instance()->getOptionsGame().getSkillForMatch();

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
			if (current->getState().getTankPlaying())
			{
				sortedTanks.push_back(current);
			}
		}
		
		TankSort::getSortedTanks(sortedTanks, ScorchedServer::instance()->getContext());
		if (!sortedTanks.empty())
		{
			Tank *topScore = *(sortedTanks.begin());

			LangString names;
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
					if (!names.empty()) names.append(LANG_STRING(","));
					names.append(current->getTargetName());

					// Score the winning tank as the overall winner
					StatsLogger::instance()->tankOverallWinner(current);

					current->getScore().setSkill(
						current->getScore().getSkill() + skillWonForMatch);
				}
			}

			if (winners.size() == 1)
			{
				ServerChannelManager::instance()->sendText(
					ChannelText("banner",
						"PLAYER_OVERALL_WINNER",
						"{0} is the overall winner!", 
						names),
					true);
			}
			else
			{
				ServerChannelManager::instance()->sendText(
					ChannelText("banner",
						"PLAYERS_OVERALL_WINNERS",
						"{0} are the overall winners!", 
						names),
					true);
			}
		}
	}
	else
	{
		int winningTeam = TankSort::getWinningTeam(
			ScorchedServer::instance()->getContext());
		if (winningTeam == 0)
		{
			ServerChannelManager::instance()->sendText(
				ChannelText("banner", 
					"GAME_DRAWN",
					"The game is a draw!"),
				true);
		}
		else
		{
			ServerChannelManager::instance()->sendText(
				ChannelText("banner",
					"TEAM_OVERALL_WINNER",
					"{0} team is the overall winner!",
					TankColorGenerator::getTeamName(winningTeam)),
				true);
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
					if (tank->getState().getTankPlaying())
					{
						StatsLogger::instance()->tankOverallWinner(tank);
						tank->getScore().setSkill(
							tank->getScore().getSkill() + skillWonForMatch);
					}
				}
			}
		}
	}
}
