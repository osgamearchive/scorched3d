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
#include <common/Defines.h>
#include <common/Logger.h>
#include <coms/NetInterface.h>
#include <server/ServerCommon.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>
#include <XML/XMLFile.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

enum EventType
{
	EventKill = 1,
	EventTeamKill = 2,
	EventSelfKill = 3,
	EventResigned = 4,
	EventWon = 5,
	EventOverallWinner = 6,
	EventConnected = 7,
	EventDisconnected = 8,
	EventJoined = 9
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

		const char *fileName = getSettingsFile("mysql-%i.xml",
			ScorchedServer::instance()->getOptionsGame().getPortNo());
		if (file.readFile(fileName) &&
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
				Logger::log(0, "Failed to parse %s settings file.", fileName);
			}
		}
		else
		{	
			success_ = false;
			Logger::log(0, "Failed to parse %s settings file. Error: %s", 
				fileName,
				file.getParserError());
		}

		if (success_)
		{
			// Add this server to the list of servers
			runQuery("INSERT INTO scorched3d_main "
				"(name, published, prefix) VALUES(\"%s\", \"%s\", \"%s\");",
				ScorchedServer::instance()->getOptionsGame().
				getServerName(),
				ScorchedServer::instance()->getOptionsGame().
				getPublishAddress(),
				prefix_.c_str());

			// Add event types
			runQuery("INSERT INTO scorched3d%s_eventtypes "
				"(eventtype, name) VALUES "
				"(%i, \"KILL\"), "
				"(%i, \"TEAMKILL\"), "
				"(%i, \"SELFKILL\"), "
				"(%i, \"WON\"), "
				"(%i, \"OVERALLWINNER\"), "
				"(%i, \"CONNECTED\"), "
				"(%i, \"DISCONNECTED\"), "
				"(%i, \"JOINED\"), "
				"(%i, \"RESIGNED\"); ",
				prefix_.c_str(),
				EventKill, EventTeamKill, EventSelfKill,
				EventWon, EventOverallWinner, 
				EventConnected, EventDisconnected, 
				EventJoined, EventResigned);

			// Add all the weapons
			std::list<Accessory *> weapons = 
				ScorchedServer::instance()->getAccessoryStore().getAllWeapons();
			std::list<Accessory *>::iterator itor;	
			for (itor = weapons.begin();
				itor != weapons.end();
				itor++)
			{
				Accessory *accessory = *itor;

				// Try to determine this players sql playerid
				int weaponId = 0;
				if (runQuery("SELECT weaponid FROM scorched3d%s_weapons "
					"WHERE name = \"%s\";", prefix_.c_str(), accessory->getName()))
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
					runQuery("INSERT INTO scorched3d%s_weapons "
						"(name, description, armslevel, cost, bundlesize, icon) "
						"VALUES(\"%s\", \"%s\", %i, %i, %i, \"%s\");", 
						prefix_.c_str(),
						accessory->getName(), 
						accessory->getDescription(),
						accessory->getArmsLevel(),
						accessory->getOriginalPrice(),
						accessory->getBundle(),
						accessory->getIconName());
					weaponId = (int) mysql_insert_id(mysql_);		
				}
				else
				{
					runQuery("UPDATE scorched3d%s_weapons SET "
						"description = \"%s\", "
						"armslevel = %i, "
						"cost = %i, "
						"bundlesize = %i, "
						"icon = \"%s\" "
						"WHERE name = \"%s\"", 
						prefix_.c_str(),
						accessory->getDescription(),
						accessory->getArmsLevel(),
						accessory->getOriginalPrice(),
						accessory->getBundle(),
						accessory->getIconName(),
						accessory->getName());
				}

				weaponId_[accessory->getName()] = weaponId;
			}
		}
	}

}

void StatsLoggerMySQL::addIpAliases(int playerId, 
	std::set<int> &currentPlayers, std::list<std::string> &results)
{
	currentPlayers.insert(playerId);
	addAliases(playerId, results);

	std::list<std::string> ipaddresses;
	if (runQuery("SELECT ipaddress FROM scorched3d%s_ipaddress "
			"WHERE playerid = %i;", prefix_.c_str(), playerId))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				ipaddresses.push_back(row[0]);
			}
			mysql_free_result(result);
		}
	}

	std::list<std::string>::iterator itor;
	for (itor = ipaddresses.begin();
		itor != ipaddresses.end();
		itor++)
	{
		const char *ipaddress = (*itor).c_str();
		std::list<int> newplayers;

		if (runQuery("SELECT playerid FROM scorched3d%s_ipaddress "
				"WHERE ipaddress = \"%s\";", prefix_.c_str(), ipaddress))
		{
			MYSQL_RES *result = mysql_store_result(mysql_);
			if (result)
			{
				int rows = (int) mysql_num_rows(result);
				for (int r=0; r<rows; r++)
				{
					MYSQL_ROW row = mysql_fetch_row(result);
					int newplayerid = atoi(row[0]);
					if (currentPlayers.find(newplayerid) == currentPlayers.end())
					{
						newplayers.push_back(newplayerid);
					}
				}
				mysql_free_result(result);
			}
		}

		std::list<int>::iterator itor2;
		for (itor2 = newplayers.begin();
			itor2 != newplayers.end();
			itor2++)
		{
			addIpAliases((*itor2), currentPlayers, results);
		}
	}
}

std::list<std::string> StatsLoggerMySQL::getIpAliases(Tank *tank)
{
	std::list<std::string> results;
	createLogger();
	if (!success_) return results;
	
	int playerId = getPlayerId(tank->getUniqueId());
	if (playerId == 0) return results;

	std::set<int> currentPlayers;
	addIpAliases(playerId, currentPlayers, results);
	
	return results;
}

static bool findInList(std::list<std::string> &results, 
	const char *name)
{
	std::list<std::string>::iterator itor;
	for (itor = results.begin();
		itor != results.end();
		itor++)
	{
		if (0 == strcmp(name, (*itor).c_str())) return true;
	}
	return false;
}

void StatsLoggerMySQL::addAliases(int playerId, 
	std::list<std::string> &results)
{
	if (runQuery("SELECT name FROM scorched3d%s_players "
			"WHERE playerid = %i;", prefix_.c_str(), playerId))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				if (!findInList(results, row[0]))
				{
					results.push_back(row[0]);
				}
			}
			mysql_free_result(result);
		}
	}	
	
	if (runQuery("SELECT name FROM scorched3d%s_names "
			"WHERE playerid = %i;", prefix_.c_str(), playerId))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				if (!findInList(results, row[0]))
				{
					results.push_back(row[0]);
				}
			}
			mysql_free_result(result);
		}
	}
}

std::list<std::string> StatsLoggerMySQL::getAliases(Tank *tank)
{
	std::list<std::string> results;
	createLogger();
	if (!success_) return results;
	
	int playerId = getPlayerId(tank->getUniqueId());
	if (playerId == 0) return results;
	addAliases(playerId, results);

	return results;
}

void StatsLoggerMySQL::gameStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_main SET games = games + 1 WHERE name = \"%s\";",
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
			runQuery("UPDATE scorched3d%s_players SET gamesplayed=gamesplayed+1 "
				"WHERE playerid = %i;", prefix_.c_str(), playerId_[tank->getUniqueId()]);
		}
	}
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

int StatsLoggerMySQL::getPlayerId(const char *uniqueId)
{
	createLogger();
	if (!success_) return 0;

	// Try to determine this players sql playerid
	int playerId = 0;
	if (runQuery("SELECT playerid FROM scorched3d%s_players "
			"WHERE uniqueid = \"%s\";", prefix_.c_str(), uniqueId))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				playerId = atoi(row[0]);
			}
			mysql_free_result(result);
		}
	}

	return playerId;
}

char *StatsLoggerMySQL::getStatsId(Tank *tank)
{
	createLogger();
	if (!success_) return "";
	int id = getPlayerId(tank->getUniqueId());
	static char buffer[10];
	sprintf(buffer, "%i", id);
	return buffer;
}

char *StatsLoggerMySQL::allocateId() 
{ 
	static char buffer[128];
	do
	{
		sprintf(buffer, "%i-%i-%i", rand(), rand(), rand());
	} while (getPlayerId(buffer) != 0);
	return buffer;
}

void StatsLoggerMySQL::addInfo(Tank *tank)
{
	// Add the players name (may fail if duplicates)
	runQuery("INSERT INTO scorched3d%s_names (playerid, name, count) VALUES "
		"(%i, \"%s\", 0);", prefix_.c_str(), playerId_[tank->getUniqueId()], 
		tank->getName());
	runQuery("UPDATE scorched3d%s_names SET count=count+1 WHERE "
		"playerid=%i AND name=\"%s\";", prefix_.c_str(), 
		playerId_[tank->getUniqueId()], tank->getName());

	// Add the ipaddress (may fail if duplicates)
	runQuery("INSERT INTO scorched3d%s_ipaddress (playerid, ipaddress, count) VALUES "
		"(%i, \"%s\", 0);", prefix_.c_str(), 
		playerId_[tank->getUniqueId()], 
		NetInterface::getIpName(tank->getIpAddress()));
	runQuery("UPDATE scorched3d%s_ipaddress SET count=count+1 WHERE "
		"playerid=%i AND ipaddress=\"%s\";", prefix_.c_str(), 
		playerId_[tank->getUniqueId()], 
		NetInterface::getIpName(tank->getIpAddress()));

	// Update last username etc
        runQuery("UPDATE scorched3d%s_players SET "
                "name=\"%s\", ipaddress=\"%s\" "
                "WHERE playerid = %i;",
                prefix_.c_str(), tank->getName(), 
                NetInterface::getIpName(tank->getIpAddress()),
                playerId_[tank->getUniqueId()]);
}

void StatsLoggerMySQL::tankConnected(Tank *tank)
{
	// We don't have a player id, create one
	int playerId = getPlayerId(tank->getUniqueId());
	if (playerId == 0)
	{
		runQuery("INSERT INTO scorched3d%s_players (uniqueid) "
			"VALUES(\"%s\");", prefix_.c_str(),
			tank->getUniqueId());
		playerId = (int) mysql_insert_id(mysql_);
		Logger::log(0, "Add new stats user \"%i\"", playerId);
	}
	else
	{
		Logger::log(0, "Found stats user \"%i\"", playerId);
	}

	// Store this new player id
	playerId_[tank->getUniqueId()] = playerId;

	// Add name and ip address
	addInfo(tank);

	// Connecting events
	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventConnected,
		playerId_[tank->getUniqueId()]);

	// Connecting stats
	runQuery("UPDATE scorched3d%s_players SET connects=connects+1, "
		"lastconnected=NOW(), osdesc=\"%s\" "
		"WHERE playerid = %i;", 
		prefix_.c_str(), tank->getHostDesc(), 
		playerId);
}

void StatsLoggerMySQL::tankJoined(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Joined events
	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventJoined,
		playerId_[tank->getUniqueId()]);

	// add new info
	addInfo(tank);

	// Add the avatar
	if (tank->getAvatar().getName()[0])
	{
		int binaryid = 0;
		unsigned int crc = tank->getAvatar().getCrc();
		if (runQuery("SELECT binaryid FROM scorched3d_binary "
			"WHERE name = \"%s\" AND crc = %u;", 
			tank->getAvatar().getName(),
			crc))
		{
			MYSQL_RES *result = mysql_store_result(mysql_);
			if (result)
			{
				int rows = (int) mysql_num_rows(result);
				for (int r=0; r<rows; r++)
				{
					MYSQL_ROW row = mysql_fetch_row(result);
					binaryid = atoi(row[0]);
				}
				mysql_free_result(result);
			}
		}

		if (binaryid == 0)
		{
			char *to = new char[tank->getAvatar().getFile().getBufferUsed() * 2];
            mysql_real_escape_string(mysql_, to, 
				tank->getAvatar().getFile().getBuffer(),
				tank->getAvatar().getFile().getBufferUsed());
			runQuery("INSERT INTO scorched3d_binary "
				"(name, crc, length, data) "
				"VALUES(\"%s\", %u, %u, \"%s\");", 
				tank->getAvatar().getName(),
				crc,
				tank->getAvatar().getFile().getBufferUsed(),
				to);
			delete [] to;
			binaryid = (int) mysql_insert_id(mysql_);	
		}

		// Set the avatar id
		runQuery("UPDATE scorched3d%s_players SET avatarid = %i "
			"WHERE playerid = %i;", 
			prefix_.c_str(), binaryid,
			playerId_[tank->getUniqueId()]);
	}
}

void StatsLoggerMySQL::tankDisconnected(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Leaving events
	runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, 0, 0, NOW());",
		prefix_.c_str(),
		EventDisconnected,
		playerId_[tank->getUniqueId()]);

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
		weaponId_[weapon->getParent()->getName()]);

	// Update both players skill points
	if (runQuery("SELECT a.skill, b.skill FROM "
		"scorched3d%s_players as a, scorched3d%s_players as b "
		"WHERE a.playerid = %i AND b.playerid = %i;",
        prefix_.c_str(), prefix_.c_str(), 
		playerId_[firedTank->getUniqueId()],
		playerId_[deadTank->getUniqueId()]))
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = (int) mysql_num_rows(result);
			for (int r=0; r<rows; r++)
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				int firedSkill = atoi(row[0]);
				int deadSkill = atoi(row[1]);

				float weaponMult = (float(weapon->getArmsLevel()) / 10.0f) + 1.0f;

				int skillDiff = 
					int((20.0f * weaponMult) / (1.0f + powf(10.0f, (float(firedSkill - deadSkill) / 1000.0f))));

				runQuery("UPDATE scorched3d%s_players SET skill=skill+%i "
					"WHERE playerid = %i;", 
					prefix_.c_str(), 
					skillDiff,
					playerId_[firedTank->getUniqueId()]);

				runQuery("UPDATE scorched3d%s_players SET skill=skill-%i "
					"WHERE playerid = %i;", 
					prefix_.c_str(), 
					skillDiff,
					playerId_[deadTank->getUniqueId()]);
			}
			mysql_free_result(result);
		}
	}

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
		weaponId_[weapon->getParent()->getName()]);

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
		weaponId_[weapon->getParent()->getName()]);

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

void StatsLoggerMySQL::weaponFired(Weapon *weapon, bool deathAni)
{
	if (deathAni)
	{
		runQuery("UPDATE scorched3d%s_weapons SET deathshots=deathshots+1 "
			"WHERE weaponid = \"%i\";", prefix_.c_str(), 
			weaponId_[weapon->getParent()->getName()]);
	}
	else
	{
		runQuery("UPDATE scorched3d%s_weapons SET shots=shots+1 "
			"WHERE weaponid = \"%i\";", prefix_.c_str(), 
			weaponId_[weapon->getParent()->getName()]);
	}
}

void StatsLoggerMySQL::weaponKilled(Weapon *weapon, bool deathAni)
{
	if (deathAni)
	{
		runQuery("UPDATE scorched3d%s_weapons SET deathkills=deathkills+1 "
			"WHERE weaponid = \"%i\";", prefix_.c_str(), 
			weaponId_[weapon->getParent()->getName()]);
	}
	else
	{
		runQuery("UPDATE scorched3d%s_weapons SET kills=kills+1 "
			"WHERE weaponid = \"%i\";", prefix_.c_str(), 
			weaponId_[weapon->getParent()->getName()]);
	}
}

#endif
