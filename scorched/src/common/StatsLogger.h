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

#if !defined(__INCLUDE_StatsLoggerh_INCLUDE__)
#define __INCLUDE_StatsLoggerh_INCLUDE__

#include <tank/Tank.h>
#include <weapons/Weapon.h>
#include <list>

class StatsLogger
{
public:
	static StatsLogger *instance();

	StatsLogger();
	virtual ~StatsLogger();

	virtual void gameStart(std::list<Tank *> &tanks) = 0;
	virtual void roundStart(std::list<Tank *> &tanks) = 0;

	virtual void tankRank(Tank *tank) = 0;
	virtual void tankJoined(Tank *tank) = 0;
	virtual void tankLeft(Tank *tank) = 0;

	virtual void tankFired(Tank *firedTank, Weapon *weapon) = 0;
	virtual void tankResigned(Tank *resignedTank) = 0;

	virtual void tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon) = 0;
	virtual void tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon) = 0;
	virtual void tankSelfKilled(Tank *firedTank, Weapon *weapon) = 0;

	virtual void tankWon(Tank *tank) = 0;
	virtual void tankOverallWinner(Tank *tank) = 0;

protected:
	static StatsLogger *instance_;

};

class StatsLoggerNone : public StatsLogger
{
public:
        StatsLoggerNone() {}
        virtual ~StatsLoggerNone() {}

        virtual void gameStart(std::list<Tank *> &tanks) {}
        virtual void roundStart(std::list<Tank *> &tanks) {}

	virtual void tankRank(Tank *tank) {}
        virtual void tankJoined(Tank *tank) {}
        virtual void tankLeft(Tank *tank) {}

        virtual void tankFired(Tank *firedTank, Weapon *weapon) {}
        virtual void tankResigned(Tank *resignedTank) {}

        virtual void tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon) {}
        virtual void tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon) {}
        virtual void tankSelfKilled(Tank *firedTank, Weapon *weapon) {}

        virtual void tankWon(Tank *tank) {}
        virtual void tankOverallWinner(Tank *tank) {}
};

#endif

