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

#include <common/StatsLoggerMySQL.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <server/ServerCommon.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>
#include <XML/XMLFile.h>
#include <stdlib.h>
#include <time.h>

enum EventType
{
	EventKill = 1,
	EventTeamKill = 2,
	EventSelfKill = 3,
	EventResigned = 4,
	EventWon = 5,
	EventOverallWinner = 6
};

StatsLoggerMySQL::StatsLoggerMySQL() : mysql_(0), success_(true)
{

}

StatsLoggerMySQL::~StatsLoggerMySQL()
{
}

bool StatsLoggerMySQL::runQuery(const char *fmt, ...)
{
	if (!success_) return false;

	static char text[2048];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	return (mysql_real_query(mysql_, text, strlen(text)) == 0);
}

void StatsLoggerMySQL::createLogger()
{
	if (!mysql_ && success_)
	{
		mysql_ = mysql_init(0);
		XMLFile file;
		if (file.readFile(getDataFile("data/mysql.xml")) &&
			file.getRootNode())
		{
			std::string host, user, passwd, db;
			if (file.getRootNode()->getNamedChild("host", host) &&
				file.getRootNode()->getNamedChild("user", user) &&
				file.getRootNode()->getNamedChild("passwd", passwd) &&
				file.getRootNode()->getNamedChild("db", db) &&
				file.getRootNode()->getNamedChild("prefix", prefix_)) 
			{
				if (mysql_real_connect(
					mysql_,
					host.c_str(),
					user.c_str(),
					passwd.c_str(),
					db.c_str(),
					0, "/tmp/mysql.sock", 0))
				{
					Logger::log(0, "mysql stats logger started");
				}
				else
				{
					success_ = false;
					Logger::log(0, "mysql stats logger failed to start. "
						"Error: %s",
						mysql_error(mysql_));
					Logger::log(0, "mysql params : host %s, user %s, passwd %s, db %s",
						host.c_str(), user.c_str(),
						passwd.c_str(), db.c_str());
				}
			}
			else
			{
				success_ = false;
				Logger::log(0, "Failed to parse mysql.xml settings file.");
			}
		}
		else
		{	
			success_ = false;
			Logger::log(0, "Failed to parse mysql.xml settings file. Error: %s", 
				file.getParserError());
		}

		if (success_)
		{
			// Add this server to the list of servers
			runQuery("INSERT INTO scorched3d_main "
				"(name, prefix) VALUES(\"%s\", \"%s\");",
				ScorchedServer::instance()->getOptionsGame().
				getServerName(),
				prefix_.c_str());

			// Add event types
			runQuery("INSERT INTO scorched3d%s_eventtypes "
				"(eventtype, name) VALUES "
				"(%i, \"KILL\"), "
				"(%i, \"TEAMKILL\"), "
				"(%i, \"SELFKILL\"), "
				"(%i, \"WON\"), "
				"(%i, \"OVERALLWINNER\"), "
				"(%i, \"RESIGNED\"); ",
				prefix_.c_str(),
				EventKill, EventTeamKill, EventSelfKill,
				EventWon, EventOverallWinner, EventResigned);

			// Add all the weapons
			std::list<Accessory *> weapons = 
				ScorchedServer::instance()->getAccessoryStore().getAllWeapons();
			std::list<Accessory *>::iterator itor;	
			for (itor = weapons.begin();
				itor != weapons.end();
				itor++)
			{
				Weapon *weapon = (Weapon *) *itor;

				// Try to determine this players sql playerid
				int weaponId = 0;
				if (runQuery("SELECT weaponid FROM scorched3d%s_weapons "
					"WHERE name = \"%s\";", prefix_.c_str(), weapon->getName()))
				{
					MYSQL_RES *result = mysql_store_result(mysql_);
					if (result)
					{
						int rows = (int) mysql_num_rows(result);
						for (int r=0; r<rows; r++)
						{
							MYSQL_ROW row = mysql_fetch_row(result);
							weaponId = atoi(row[0]);
						}
						mysql_free_result(result);
					}
				}

				if (weaponId == 0)
				{
					runQuery("INSERT INTO scorched3d%s_weapons (name, description, armslevel) "
						"VALUES(\"%s\", \"%s\", %i);", 
						prefix_.c_str(),
						weapon->getName(), 
						weapon->getDescription(),
						weapon->getArmsLevel());
					weaponId = (int) mysql_insert_id(mysql_);		
				}

				weaponId_[weapon->getName()] = weaponId;
			}
		}
	}

}

void StatsLoggerMySQL::gameStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_main SET games = games + 1 WHERE name = \"%s\";",
		ScorchedServer::instance()->getOptionsGame().
		getServerName());
}

void StatsLoggerMySQL::roundStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_main SET rounds = rounds + 1 WHERE name = \"%s\";",
		ScorchedServer::instance()->getOptionsGame().
		getServerName());

	std::list<Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = *itor;
		if (!tank->getState().getSpectator())
		{
			runQuery("UPDATE scorched3d%s_players SET roundsplayed=roundsplayed+1 "
				"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]);
		}
	}
}

void StatsLoggerMySQL::tankFired(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d%s_players SET shots=shots+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankResigned(Tank *tank)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventResigned,
                playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d%s_players SET resigns=resigns+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

void StatsLoggerMySQL::updateStats(Tank *tank)
{
	if (!tank->getState().getSpectator())
	{
		runQuery("UPDATE scorched3d%s_players SET timeplayed=timeplayed+%i, moneyearned=moneyearned+%i "
			"WHERE playerid = %i;", 
			prefix_.c_str(),
			tank->getScore().getTimePlayedStat(), 
			tank->getScore().getTotalMoneyEarnedStat(), 
			playerId_[tank->getUniqueId()]);
	}
}

char *StatsLoggerMySQL::tankRank(Tank *tank)
{
	char *retval = "-";
	createLogger();
	if (!success_) return retval;

	// Try to determine this players sql playerid
	int kills = 0;
	if (runQuery("SELECT kills FROM scorched3d%s_players "
			"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				kills = atoi(row[0]);
			}
			mysql_free_result(result);
		}
	}

	if (runQuery("SELECT count(*) FROM scorched3d%s_players "
		"WHERE kills > \"%i\";", prefix_.c_str(), kills))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				int rank = atoi(row[0]) + 1;

				static char rankStr[100];
				sprintf(rankStr, "%i", rank);
				retval = rankStr;
			}
			mysql_free_result(result);
		}
	}

	return retval;
}

void StatsLoggerMySQL::tankJoined(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Try to determine this players sql playerid
	int playerId = 0;
	if (runQuery("SELECT playerid FROM scorched3d%s_players "
			"WHERE uniqueid = \"%s\";", prefix_.c_str(), tank->getUniqueId()))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				playerId = atoi(row[0]);

				Logger::log(0, "Found stats user \"%i\"", playerId);
			}
			mysql_free_result(result);
		}
	}

	// We don't have a player id, create one
	if (playerId == 0)
	{
		runQuery("INSERT INTO scorched3d%s_players (uniqueid) "
			"VALUES(\"%s\");", prefix_.c_str(),
			tank->getUniqueId());
		playerId = (int) mysql_insert_id(mysql_);
		Logger::log(0, "Add new stats user \"%i\"", playerId);
	}

	// Store this new player id
	playerId_[tank->getUniqueId()] = playerId;

	// Add the players name (may fail if duplicates)
	runQuery("INSERT INTO scorched3d%s_names (playerid, name, count) VALUES "
		"(%i, \"%s\", 0);", prefix_.c_str(), playerId, tank->getName());
	runQuery("UPDATE scorched3d%s_names SET count=count+1 WHERE "
		"playerid=%i AND name=\"%s\";", prefix_.c_str(), playerId, tank->getName());
	
	// Joining stats
	runQuery("UPDATE scorched3d%s_players SET connects=connects+1, "
		"lastconnected=NOW(), name=\"%s\", osdesc=\"%s\" "
		"WHERE playerid = %i;", prefix_.c_str(), tank->getName(), tank->getHostDesc(), playerId);
}

void StatsLoggerMySQL::tankLeft(Tank *tank)
{
	createLogger();
	if (!success_) return;

	updateStats(tank);
}

void StatsLoggerMySQL::tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, NOW());",
		prefix_.c_str(),
		EventKill,
		playerId_[firedTank->getUniqueId()], 
		playerId_[deadTank->getUniqueId()], 
		weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d%s_weapons SET kills=kills+1 "
		"WHERE weaponid = \"%i\";", prefix_.c_str(), weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d%s_players SET kills=kills+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
		
	runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[deadTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, NOW());",
		prefix_.c_str(),
		EventTeamKill,
		playerId_[firedTank->getUniqueId()],
		playerId_[deadTank->getUniqueId()], 
		weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d%s_players SET teamkills=teamkills+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
		
	runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[deadTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankSelfKilled(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, %i, NOW());",
		prefix_.c_str(),
		EventSelfKill,
		playerId_[firedTank->getUniqueId()],
		weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d%s_players SET selfkills=selfkills+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
		
	runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankWon(Tank *tank)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventWon,
		playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d%s_players SET wins=wins+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

void StatsLoggerMySQL::tankOverallWinner(Tank *tank)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventOverallWinner,
		playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d%s_players SET overallwinner=overallwinner+1 "
		"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

#endif

