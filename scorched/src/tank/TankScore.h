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


// TankScore.h: interface for the TankScore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)
#define AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_

#include <time.h>

class TankScore  
{
public:
	TankScore();
	virtual ~TankScore();

	void reset();

	int getKills() { return kills_; }
	void setKills(int kills) { kills_ = kills; }

	int getMoney() { return money_; }
	void setMoney(int money);

	int getWins() { return wins_; }
	void setWins(int wins) { wins_ = wins; }

	// Get info as text
	const char *getTimePlayedString();
	const char *getScoreString();

	// Serialize the tank
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	int kills_;
	int money_;
	int wins_;
	time_t startTime_;

};

#endif // !defined(AFX_TANKSCORE_H__ED963414_2B77_4027_B35A_D69960165470__INCLUDED_)
