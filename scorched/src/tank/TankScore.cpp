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


// TankScore.cpp: implementation of the TankScore class.
//
//////////////////////////////////////////////////////////////////////

#include <common/OptionsGame.h>
#include <tank/TankScore.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The maximum amount of money allowed by anyone
// Range limited to 0 -> maxMoney
static const int maxMoney = 500000;

TankScore::TankScore() 
	
{
	startTime_ = time(0);
	reset();
}

TankScore::~TankScore()
{

}

void TankScore::reset()
{
	money_ = OptionsGame::instance()->getStartMoney();
	wins_ = 0;
	kills_ = 0;
	missedMoves_ = 0;
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
	div_t playedTime = div((int) seconds, 60);

	sprintf(timestr, "%i:%i secs",
		playedTime.quot,
		playedTime.rem);

	return timestr;
}

const char *TankScore::getScoreString()
{
	static char score[256];
	sprintf(score, "%i/%i/%i K/W/$", 
		getKills(),
		getWins(),
		getMoney());
	return score;
}

bool TankScore::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(kills_);
	buffer.addToBuffer(money_);
	buffer.addToBuffer(wins_);
	return true;
}

bool TankScore::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(kills_)) return false;
	if (!reader.getFromBuffer(money_)) return false;
	if (!reader.getFromBuffer(wins_)) return false;
	return true;
}
