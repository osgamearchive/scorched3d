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

#ifdef HAVE_MYSQL

#if !defined(__INCLUDE_StatsLoggerMySQLh_INCLUDE__)
#define __INCLUDE_StatsLoggerMySQLh_INCLUDE__

#if defined(_WIN32)
#include <Winsock2.h>
#endif
#include <common/StatsLogger.h>
#include <mysql/mysql.h>
#include <string>
#include <map>

class Weapon;
class StatsLoggerMySQL : public StatsLogger
{
public:
	StatsLoggerMySQL();
	virtual ~StatsLoggerMySQL();

	virtual void gameStart(std::list<Tank *> &tanks);
	virtual void roundStart(std::list<Tank *> &tanks);

	virtual char *tankRank(Tank *tank);
	virtual void updateStats(Tank *tank);

	virtual void tankJoined(Tank *tank);
	virtual void tankLeft(Tank *tank);

	virtual void tankFired(Tank *firedTank, Weapon *weapon);
	virtual void tankResigned(Tank *resignedTank);

	virtual void tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon);
	virtual void tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon);
	virtual void tankSelfKilled(Tank *firedTank, Weapon *weapon);

	virtual void tankWon(Tank *tank);
	virtual void tankOverallWinner(Tank *tank);

	virtual void weaponFired(Weapon *weapon, bool deathAni);
	virtual void weaponKilled(Weapon *weapon, bool deathAni);

protected:
	MYSQL *mysql_;
	std::string prefix_;
	bool success_;
	std::map<std::string, int> playerId_;
	std::map<std::string, int> weaponId_;

	bool runQuery(const char *, ...);
	void createLogger();

};

#endif 

#endif // HAVE_MYSQL
