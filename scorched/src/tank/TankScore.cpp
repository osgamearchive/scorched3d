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

#include <tank/TankScore.h>
#include <engine/ScorchedContext.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>

// The maximum amount of money allowed by anyone
// Range limited to 0 -> maxMoney
static const int maxMoney = 999999;

TankScore::TankScore(ScorchedContext &context) : 
	context_(context), totalMoneyEarned_(0),
	statsRank_("-")
{
	startTime_ = lastStatTime_ = time(0);
	newMatch();
}

TankScore::~TankScore()
{

}

void TankScore::newMatch()
{
	money_ = 0;
	setMoney(context_.optionsGame->getStartMoney());
	wins_ = 0;
	kills_ = 0;
	assists_ = 0;
	score_ = 0;
	missedMoves_ = 0;

	newGame();
}

void TankScore::newGame()
{
	wonGame_ = false;
	hurtBy_.clear();
}

void TankScore::clientNewGame()
{
	newGame();
}

void TankScore::setMoney(int money)
{
	money_ = money;
	if (money_ > maxMoney) money_ = maxMoney;
	if (money_ < 0) money_ = 0;
}

const char *TankScore::getTimePlayedString()
{
	static char timestr[256];
	time_t seconds = time(0) - startTime_;
	div_t playedTimeHr = div((int) seconds, 3600);
	div_t playedTime = div(playedTimeHr.rem, 60);

	snprintf(timestr, 256, "%i:%i:%i secs",
		playedTimeHr.quot,
		playedTime.quot,
		playedTime.rem);

	return timestr;
}

const char *TankScore::getScoreString()
{
	static char score[256];
	snprintf(score, 256, "%i", getScore());
	return score;
}

bool TankScore::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(kills_);
	buffer.addToBuffer(assists_);
	buffer.addToBuffer(money_);
	buffer.addToBuffer(wins_);
	buffer.addToBuffer(score_);
	buffer.addToBuffer(statsRank_);
	return true;
}

bool TankScore::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(kills_)) return false;
	if (!reader.getFromBuffer(assists_)) return false;
	if (!reader.getFromBuffer(money_)) return false;
	if (!reader.getFromBuffer(wins_)) return false;
	if (!reader.getFromBuffer(score_)) return false;
	if (!reader.getFromBuffer(statsRank_)) return false;
	return true;
}

time_t TankScore::getTimePlayedStat()
{
	time_t val = lastStatTime_;
	lastStatTime_ = time(0);
	time_t res = lastStatTime_ - val;
	return res;
}

int TankScore::getTotalMoneyEarnedStat()
{ 
	int moneyEarned = totalMoneyEarned_;
	totalMoneyEarned_ = 0;
	return moneyEarned; 
}
