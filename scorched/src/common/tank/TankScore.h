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

#if !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)
#define AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_

#include <set>
#include <string>
#include <time.h>
#include <net/NetBuffer.h>

class Tank;
class ScorchedContext;
class TankScore  
{
public:
	TankScore(ScorchedContext &context);
	virtual ~TankScore();

	void setTank(Tank *tank) { tank_ = tank; }

	void newMatch();
	void newGame();
	void clientNewGame();

	int getKills() { return kills_; }
	void setKills(int kills) { kills_ = kills; }

	int getAssists() { return assists_; }
	void setAssists(int assists) { assists_ = assists; }

	int getMoney() { return money_; }
	void setMoney(int money);

	int getWins() { return wins_; }
	void setWins(int wins) { wins_ = wins; }

	int getScore() { return score_; }
	void setScore(int score);

	int getTurnKills() { return turnKills_; }
	void setTurnKills(int kills) { turnKills_ = kills; }

	int getMissedMoves() { return missedMoves_; }
	void setMissedMoves(int miss) { missedMoves_ = miss; }

	bool getWonGame() { return wonGame_; }
	void setWonGame() { wonGame_ = true; }

	int getRank() { return rank_; }
	void setRank(int rank) { rank_ = rank; }

	int getSkill() { return skill_; }
	void setSkill(int skill);

	int getStartSkill() { return startSkill_; }

	std::set<unsigned int> &getHurtBy() { return hurtBy_; }

	time_t getTimePlayed() { return time(0) - startTime_; }
	time_t getStartTime() { return startTime_; }

	// Stuff for stats
	time_t getTimePlayedStat();
	int getTotalMoneyEarnedStat() { return totalMoneyEarned_; }
	int getTotalScoreEarnedStat() { return totalScoreEarned_; }
	void resetTotalEarnedStats();

	// Get info as text
	const char *getTimePlayedString();
	const char *getScoreString();

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	Tank *tank_;
	std::set<unsigned int> hurtBy_;
	ScorchedContext &context_;
	int rank_, skill_, startSkill_;
	int kills_, turnKills_;
	int assists_;
	int money_;
	int totalMoneyEarned_, totalScoreEarned_;
	int wins_;
	int score_;
	int missedMoves_;
	bool wonGame_;
	time_t startTime_;
	time_t lastStatTime_;

};

#endif // !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)