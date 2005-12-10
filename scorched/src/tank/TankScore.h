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

#if !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)
#define AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_

#include <string>
#include <time.h>
#include <coms/NetBuffer.h>

class ScorchedContext;
class TankScore  
{
public:
	TankScore(ScorchedContext &context);
	virtual ~TankScore();

	void newMatch();

	int getKills() { return kills_; }
	void setKills(int kills) { kills_ = kills; }

	int getMoney() { return money_; }
	void setMoney(int money);

	int getWins() { return wins_; }
	void setWins(int wins) { wins_ = wins; }

	int getMissedMoves() { return missedMoves_; }
	void setMissedMoves(int miss) { missedMoves_ = miss; }

	time_t getTimePlayed() { return time(0) - startTime_; }
	time_t getStartTime() { return startTime_; }

	// Stuff for stats
	time_t getTimePlayedStat();
	void addTotalMoneyEarnedStat(int tm) { totalMoneyEarned_ += tm; }
	int getTotalMoneyEarnedStat();
	void setStatsRank(const char *rank) { statsRank_ = rank; }
	const char *getStatsRank() { return statsRank_.c_str(); }

	// Get info as text
	const char *getTimePlayedString();
	const char *getScoreString();

	// Serialize the tank
	bool writeMessage(NetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	std::string statsRank_;
	ScorchedContext &context_;
	int kills_;
	int money_;
	int totalMoneyEarned_;
	int wins_;
	int missedMoves_;
	time_t startTime_;
	time_t lastStatTime_;

};

#endif // !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)
