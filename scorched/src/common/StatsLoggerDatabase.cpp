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

#include <common/StatsLoggerDatabase.h>
#include <common/OptionsScorched.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <net/NetInterface.h>
#include <server/ServerCommon.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <tank/TankAvatar.h>
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

StatsLoggerDatabase::StatsLoggerDatabase() : 
	success_(false),
	serverid_(0), seriesid_(0), prefixid_(0)
{
}

StatsLoggerDatabase::~StatsLoggerDatabase()
{
}

void StatsLoggerDatabase::createLogger()
{
	if (success_) return;

	XMLFile file;
	const char *fileName = getSettingsFile(formatString("mysql-%i.xml",
		ScorchedServer::instance()->getOptionsGame().getPortNo()));

	std::string host, port, user, passwd, db, prefix;
	if (!file.readFile(fileName) ||
		!file.getRootNode())
	{
		Logger::log(formatString("Failed to parse %s settings file. Error: %s", 
			fileName,
			file.getParserError()));
		return;
	}

	if (!file.getRootNode()->getNamedChild("host", host) ||
		!file.getRootNode()->getNamedChild("port", port) ||
		!file.getRootNode()->getNamedChild("user", user) ||
		!file.getRootNode()->getNamedChild("passwd", passwd) ||
		!file.getRootNode()->getNamedChild("db", db) ||
		!file.getRootNode()->getNamedChild("prefix", prefix)) 
	{
		Logger::log(formatString("Failed to parse %s settings file.", fileName));
		return;
	}

	// Create the database connection
	success_ = false;
	if (!connectDatabase(host.c_str(), port.c_str(), 
		user.c_str(), passwd.c_str(), 
		db.c_str())) return;
	success_ = true;

	// Add event types
	runQuery("INSERT INTO scorched3d_eventtypes "
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
		EventKill, EventTeamKill, EventSelfKill,
		EventWon, EventOverallWinner, 
		EventConnected, EventDisconnected, 
		EventJoined, EventResigned);

	// Get/allocate the prefixid
	std::list<StatsLoggerDatabase::RowResult> prefixRows =
		runSelectQuery("SELECT prefixid FROM scorched3d_prefixs "
		"WHERE prefix = \"%s\";",
		prefix.c_str());
	if (!prefixRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = prefixRows.begin();
			itor != prefixRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			prefixid_ = atoi(rowResult.columns[0].c_str());
		}
	}
	if (prefixid_ == 0)
	{
		if (runQuery("INSERT INTO scorched3d_prefixs "
			"(prefix) VALUES(\"%s\");",
			prefix.c_str()))
		{
			prefixid_ = getLastInsertId();
		}
	}

	// Get/allocate the server id
	std::list<StatsLoggerDatabase::RowResult> serverIdRows =
		runSelectQuery("SELECT serverid, displaystats FROM scorched3d_servers "
		"WHERE name = \"%s\";",
		ScorchedServer::instance()->getOptionsGame().getServerName());
	if (!serverIdRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = prefixRows.begin();
			itor != prefixRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			serverid_ = atoi(rowResult.columns[0].c_str());
			displayStats_ = (atoi(rowResult.columns[1].c_str()) != 0);
		}
	}
	if (serverid_ == 0)
	{
		if (runQuery("INSERT INTO scorched3d_servers "
			"(name, published) VALUES(\"%s\", \"%s\");",
			ScorchedServer::instance()->getOptionsGame().getServerName(),
			ScorchedServer::instance()->getOptionsGame().getPublishAddress()))
		{
			serverid_ = getLastInsertId();
			displayStats_ = true;
		}
	}

	// Get/allocate the series id
	std::list<StatsLoggerDatabase::RowResult> seriesIdRows =
		runSelectQuery("SELECT seriesid FROM scorched3d_series "
		"WHERE type = 0;");
	if (!seriesIdRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = prefixRows.begin();
			itor != prefixRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			seriesid_ = atoi(rowResult.columns[0].c_str());
		}
	}
	if (seriesid_ == 0)
	{
		if (runQuery("INSERT INTO scorched3d_series "
			"(started, ended) VALUES(NOW(), NOW());"))
		{
			seriesid_ = getLastInsertId();
		}
	}

	// Add this server in this series
	runQuery("INSERT INTO scorched3d_statssource "
		"(serverid, prefixid, seriesid) VALUES "
		"(%i, %i, %i);", 
		serverid_, prefixid_, seriesid_);


	// Add all the weapons
	std::list<Accessory *> weapons = 
		ScorchedServer::instance()->getAccessoryStore().getAllAccessories();
	std::list<Accessory *>::iterator itor;	
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{
		Accessory *accessory = *itor;

		int weaponId = 0;
		std::list<StatsLoggerDatabase::RowResult> weaponIdRows =
			runSelectQuery("SELECT weaponid FROM scorched3d_weapons "
				"WHERE name = \"%s\" AND seriesid = %i AND prefixid = %i;", 
				accessory->getName(),
				seriesid_,
				prefixid_);
		if (!weaponIdRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = prefixRows.begin();
				itor != prefixRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				weaponId = atoi(rowResult.columns[0].c_str());
			}
		}

		if (weaponId == 0)
		{
			if (runQuery("INSERT INTO scorched3d_weapons "
				"(seriesid, prefixid, name, description, armslevel, cost, bundlesize, icon) "
				"VALUES(%i, %i, \"%s\", \"%s\", %i, %i, %i, \"%s\");", 
				seriesid_,
				prefixid_,
				accessory->getName(), 
				accessory->getDescription(),
				accessory->getArmsLevel(),
				accessory->getOriginalPrice(),
				accessory->getBundle(),
				accessory->getIconName()))
			{
				weaponId = getLastInsertId();
			}
		}
		else
		{
			runQuery("UPDATE scorched3d_weapons SET "
				"description = \"%s\", "
				"armslevel = %i, "
				"cost = %i, "
				"bundlesize = %i, "
				"icon = \"%s\" "
				"WHERE name = \"%s\" AND seriesid = %i AND prefixid = %i;", 
				accessory->getDescription(),
				accessory->getArmsLevel(),
				accessory->getOriginalPrice(),
				accessory->getBundle(),
				accessory->getIconName(),
				accessory->getName(),
				seriesid_,
				prefixid_);
		}

		weaponId_[accessory->getName()] = weaponId;
	}

	Logger::log(formatString("database stats logger started, prefix=%i, server=%i, series=%i",
		prefixid_, serverid_, seriesid_));
}

void StatsLoggerDatabase::addIpAliases(int playerId, 
	std::set<int> &currentPlayers, std::list<std::string> &results)
{
	currentPlayers.insert(playerId);
	addAliases(playerId, results);

	std::list<std::string> ipaddresses;
	std::list<StatsLoggerDatabase::RowResult> ipaddressesRows =
		runSelectQuery("SELECT ipaddress FROM scorched3d_ipaddress "
			"WHERE playerid = %i;", playerId);
	if (!ipaddresses.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = ipaddressesRows.begin();
			itor != ipaddressesRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			ipaddresses.push_back(rowResult.columns[0]);
		}
	}

	std::list<std::string>::iterator itor;
	for (itor = ipaddresses.begin();
		itor != ipaddresses.end();
		itor++)
	{
		const char *ipaddress = (*itor).c_str();
		std::list<int> newplayers;

		std::list<StatsLoggerDatabase::RowResult> ipaddressRows =
			runSelectQuery("SELECT playerid FROM scorched3d_ipaddress "
				"WHERE ipaddress = \"%s\";", ipaddress);
		if (!ipaddressRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = ipaddressRows.begin();
				itor != ipaddressRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				int newplayerid = atoi(rowResult.columns[0].c_str());
				if (currentPlayers.find(newplayerid) == currentPlayers.end())
				{
					newplayers.push_back(newplayerid);
				}
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

char *StatsLoggerDatabase::getTopRanks()
{
	const char *columns = 
		"kills, deaths, selfkills, teamkills, shots, wins, "
		"overallwinner, resigns, gamesplayed, timeplayed, roundsplayed, "
		"moneyearned, skill, name";

	std::string stringResult;
	std::list<StatsLoggerDatabase::RowResult> rankRows =
		runSelectQuery(
			"select %s from scorched3d_stats "
			"left join scorched3d_players on scorched3d_stats.playerid = "
			"scorched3d_players.playerid where seriesid=%i order by kills desc",
			columns, seriesid_);
	stringResult.append(columns).append("\n");
	if (!rankRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = rankRows.begin();
			itor != rankRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &result = (*itor);
			for (unsigned int i=0; i<result.columns.size(); i++)
			{
				stringResult.append(result.columns[i]);
				if (i < result.columns.size() - 1) stringResult.append(",");
				else stringResult.append("\n");
			}
		}
	}
	return (char *) formatString("%s", stringResult.c_str());
}

std::list<std::string> StatsLoggerDatabase::getIpAliases(Tank *tank)
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

void StatsLoggerDatabase::addAliases(int playerId, 
	std::list<std::string> &results)
{
	// Make sure the last used name is in the list and
	// it is always first
	{
		std::list<StatsLoggerDatabase::RowResult> nameRows =
			runSelectQuery("SELECT name FROM scorched3d_players "
				"WHERE playerid = %i;", playerId);
		if (!nameRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = nameRows.begin();
				itor != nameRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				if (!findInList(results, rowResult.columns[0].c_str()))
				{
					results.push_back(rowResult.columns[0].c_str());
				}
			}
		}
	}

	// Add all other UNIQUE aliases
	{
		std::list<StatsLoggerDatabase::RowResult> nameRows =
			runSelectQuery("SELECT name FROM scorched3d_names "
				"WHERE playerid = %i;", playerId);
		if (!nameRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = nameRows.begin();
				itor != nameRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				if (!findInList(results, rowResult.columns[0].c_str()))
				{
					results.push_back(rowResult.columns[0].c_str());
				}
			}
		}
	}
}

std::list<std::string> StatsLoggerDatabase::getAliases(Tank *tank)
{
	std::list<std::string> results;
	createLogger();
	if (!success_) return results;
	
	int playerId = getPlayerId(tank->getUniqueId());
	if (playerId == 0) return results;
	addAliases(playerId, results);

	return results;
}

void StatsLoggerDatabase::gameStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_series SET games = games + 1, "
		"ended = NOW() WHERE seriesid = %i;",
		seriesid_);

	std::list<Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = *itor;
		if (!tank->getState().getSpectator())
		{
			runQuery("UPDATE scorched3d_stats SET gamesplayed=gamesplayed+1 "
				"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
				playerId_[tank->getUniqueId()],
				prefixid_,
				seriesid_);
		}
	}
}

void StatsLoggerDatabase::roundStart(std::list<Tank *> &tanks)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_series SET rounds = rounds + 1, "
		"ended = NOW() WHERE seriesid = %i;",
		seriesid_);

	std::list<Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = *itor;
		if (!tank->getState().getSpectator())
		{
			runQuery("UPDATE scorched3d_stats SET roundsplayed=roundsplayed+1 "
				"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
				playerId_[tank->getUniqueId()],
				prefixid_,
				seriesid_);
		}
	}
}

void StatsLoggerDatabase::tankFired(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_stats SET shots=shots+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[firedTank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankResigned(Tank *tank)
{
	createLogger();
	if (!success_) return;

    runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventResigned,
		playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_stats SET resigns=resigns+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[tank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::updateStats(Tank *tank)
{
	if (!tank->getState().getSpectator())
	{
		runQuery("UPDATE scorched3d_stats SET "
			"timeplayed=timeplayed+%i, moneyearned=moneyearned+%i, scoreearned=scoreearned+%i "
			"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
			tank->getScore().getTimePlayedStat(), 
			tank->getScore().getTotalMoneyEarnedStat(), 
			tank->getScore().getTotalScoreEarnedStat(),
			playerId_[tank->getUniqueId()],
			prefixid_,
			seriesid_);
	}
}

char *StatsLoggerDatabase::tankRank(Tank *tank)
{
	char *retval = "-";
	createLogger();
	if (!success_ || !displayStats_) return retval;

	// Try to determine this players sql playerid
	int kills = 0;
	std::list<StatsLoggerDatabase::RowResult> killsRows =
		runSelectQuery("SELECT kills FROM scorched3d_stats "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[tank->getUniqueId()],
		prefixid_,
		seriesid_);
	if (!killsRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = killsRows.begin();
			itor != killsRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			kills = atoi(rowResult.columns[0].c_str());
		}
	}

	std::list<StatsLoggerDatabase::RowResult> countRows =
		runSelectQuery("SELECT count(*) FROM scorched3d_stats "
		"WHERE kills > \"%i\" AND prefixid = %i AND seriesid = %i;", 
		kills,
		prefixid_,
		seriesid_);
	if (!countRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = countRows.begin();
			itor != countRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);

			int rank = atoi(rowResult.columns[0].c_str()) + 1;

			static char rankStr[100];
			snprintf(rankStr, 100, "%i", rank);
			retval = rankStr;
		}
	}

	return retval;
}

int StatsLoggerDatabase::getPlayerId(const char *uniqueId)
{
	createLogger();
	if (!success_) return 0;

	// Try to determine this players sql playerid
	int playerId = 0;
	std::list<StatsLoggerDatabase::RowResult> playerIdRows =
		runSelectQuery("SELECT playerid FROM scorched3d_players "
			"WHERE uniqueid = \"%s\";", uniqueId);
	if (!playerIdRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = playerIdRows.begin();
			itor != playerIdRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);
			playerId = atoi(rowResult.columns[0].c_str());
		}
	}

	return playerId;
}

char *StatsLoggerDatabase::getStatsId(Tank *tank)
{
	createLogger();
	if (!success_) return "";
	int id = getPlayerId(tank->getUniqueId());
	static char buffer[25];
	snprintf(buffer, 25, "%i", id);
	return buffer;
}

char *StatsLoggerDatabase::allocateId() 
{ 
	const char possibleChars [] = {
		'1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S',
		'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	static char buffer[128];

	do
	{
		int pos = 0;
		for (int j=0; j<3; j++)
		{
			for (int i=0; i<8; i++)
			{
				buffer[pos++] = possibleChars[rand() % 33];
			}
			buffer[pos++] = '-';
		}
		buffer[pos - 1] = '\0';

	} while (getPlayerId(buffer) != 0);
	return buffer;
}

void StatsLoggerDatabase::addInfo(Tank *tank)
{
	// Add the players name (may fail if duplicates)
	runQuery("INSERT INTO scorched3d_names (playerid, name, count) VALUES "
		"(%i, \"%s\", 0);", 
		playerId_[tank->getUniqueId()], 
		tank->getName());
	runQuery("UPDATE scorched3d_names SET count=count+1 WHERE "
		"playerid=%i AND name=\"%s\";", 
		playerId_[tank->getUniqueId()], 
		tank->getName());

	// Add the ipaddress (may fail if duplicates)
	runQuery("INSERT INTO scorched3d_ipaddress (playerid, ipaddress, count) VALUES "
		"(%i, \"%s\", 0);",
		playerId_[tank->getUniqueId()], 
		NetInterface::getIpName(tank->getIpAddress()));
	runQuery("UPDATE scorched3d_ipaddress SET count=count+1 WHERE "
		"playerid=%i AND ipaddress=\"%s\";",
		playerId_[tank->getUniqueId()], 
		NetInterface::getIpName(tank->getIpAddress()));

	// Update last username etc
	runQuery("UPDATE scorched3d_players SET "
		"name=\"%s\", ipaddress=\"%s\" "
		"WHERE playerid = %i;",
		tank->getName(), 
		NetInterface::getIpName(tank->getIpAddress()),
		playerId_[tank->getUniqueId()]);
}

void StatsLoggerDatabase::tankConnected(Tank *tank)
{
	// We don't have a player id, create one
	int playerId = getPlayerId(tank->getUniqueId());
	if (playerId == 0)
	{
		if (runQuery("INSERT INTO scorched3d_players (uniqueid) "
			"VALUES(\"%s\");",
			tank->getUniqueId()))
		{
			playerId = getLastInsertId();
			Logger::log(formatString("Add new stats user \"%i\"", playerId));
		}
	}
	else
	{
		Logger::log(formatString("Found stats user \"%i\"", playerId));
	}

	// Create the players stats entry if it does not exist
	std::list<StatsLoggerDatabase::RowResult> playerIdRows =
		runSelectQuery("SELECT playerid FROM scorched3d_stats "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId,
		prefixid_,
		seriesid_);
	if (playerIdRows.empty())
	{
		runQuery("INSERT INTO scorched3d_stats (playerid, prefixid, seriesid) "
			"VALUES(%i, %i, %i);",
			playerId,
			prefixid_,
			seriesid_);
	}

	// Store this new player id
	playerId_[tank->getUniqueId()] = playerId;

	// Add name and ip address
	addInfo(tank);

	// Connecting events
	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventConnected,
		playerId_[tank->getUniqueId()]);

	// Connecting stats
	runQuery("UPDATE scorched3d_players SET osdesc=\"%s\" "
		"WHERE playerid = %i;", 
		tank->getHostDesc(),
		playerId);
	runQuery("UPDATE scorched3d_stats SET connects=connects+1, "
		"lastconnected=NOW() "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId,
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankJoined(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Joined events
	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventJoined,
		playerId_[tank->getUniqueId()]);

	// add new info
	addInfo(tank);

	// Add the avatar
	if (tank->getAvatar().getName()[0])
	{
		char buffer[32];
		for (int i=0; i<30; i++)
		{
			buffer[i] = tank->getAvatar().getName()[i];
			if (!buffer[i]) break;
		}
		buffer[30] = '\0';

		int binaryid = 0;
		unsigned int crc = tank->getAvatar().getCrc();
		std::list<StatsLoggerDatabase::RowResult> binaryIdRows =
			runSelectQuery("SELECT binaryid FROM scorched3d_binary "
			"WHERE name = \"%s\" AND crc = %u;", 
			buffer,
			crc);
		if (!binaryIdRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = binaryIdRows.begin();
				itor != binaryIdRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				binaryid = atoi(rowResult.columns[0].c_str());
			}
		}

		if (binaryid == 0)
		{
			char *to = new char[tank->getAvatar().getFile().getBufferUsed() * 2];
            escapeString(to, 
				tank->getAvatar().getFile().getBuffer(),
				tank->getAvatar().getFile().getBufferUsed());
			if (runQuery("INSERT INTO scorched3d_binary "
				"(name, crc, length, data) "
				"VALUES(\"%s\", %u, %u, \"%s\");", 
				buffer,
				crc,
				tank->getAvatar().getFile().getBufferUsed(),
				to))
			{
				binaryid = getLastInsertId();
			}

			delete [] to;
		}

		// Set the avatar id
		runQuery("UPDATE scorched3d_players SET avatarid = %i "
			"WHERE playerid = %i;", 
			binaryid,
			playerId_[tank->getUniqueId()]);
	}
}

int StatsLoggerDatabase::getKillCount(const char *uniqueId)
{
	int kills = 0;
	int playerId = getPlayerId(uniqueId);
	if (playerId != 0) 
	{
		std::list<StatsLoggerDatabase::RowResult> killsRows =
			runSelectQuery("SELECT kills FROM scorched3d_stats "
			"WHERE playerid = %i;",
			playerId);
		if (!killsRows.empty())
		{
			std::list<StatsLoggerDatabase::RowResult>::iterator itor;
			for (itor = killsRows.begin();
				itor != killsRows.end();
				itor++)
			{
				StatsLoggerDatabase::RowResult &rowResult = (*itor);
				kills += atoi(rowResult.columns[0].c_str());
			}
		}
	}

	return kills;
}

void StatsLoggerDatabase::tankDisconnected(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Leaving events
	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventDisconnected,
		playerId_[tank->getUniqueId()]);

	updateStats(tank);
}

void StatsLoggerDatabase::tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, %i, %i, NOW());",
		prefixid_, seriesid_,
		EventKill,
		playerId_[firedTank->getUniqueId()], 
		playerId_[deadTank->getUniqueId()], 
		weaponId_[weapon->getParent()->getName()]);

	// Update both players skill points
	std::list<StatsLoggerDatabase::RowResult> skillRows =
		runSelectQuery("SELECT a.skill, b.skill FROM "
		"scorched3d_stats as a, scorched3d_stats as b "
		"WHERE a.playerid = %i AND b.playerid = %i AND a.prefixid = %i "
		"AND a.seriesid = %i AND b.prefixid = %i AND b.seriesid = %i;",
		playerId_[firedTank->getUniqueId()],
		playerId_[deadTank->getUniqueId()],
		prefixid_,
		seriesid_,
		prefixid_,
		seriesid_);
	if (!skillRows.empty())
	{
		std::list<StatsLoggerDatabase::RowResult>::iterator itor;
		for (itor = skillRows.begin();
			itor != skillRows.end();
			itor++)
		{
			StatsLoggerDatabase::RowResult &rowResult = (*itor);

			int firedSkill = atoi(rowResult.columns[0].c_str());
			int deadSkill = atoi(rowResult.columns[1].c_str());

			float weaponMult = (float(weapon->getArmsLevel()) / 10.0f) + 1.0f;

			int skillDiff = 
				int((20.0f * weaponMult) / (1.0f + powf(10.0f, (float(firedSkill - deadSkill) / 1000.0f))));

			runQuery("UPDATE scorched3d_stats SET skill=skill+%i "
				"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
				skillDiff,
				playerId_[firedTank->getUniqueId()],
				prefixid_,
				seriesid_);

			runQuery("UPDATE scorched3d_stats SET skill=skill-%i "
				"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
				skillDiff,
				playerId_[deadTank->getUniqueId()],
				prefixid_,
				seriesid_);
		}
	}

	runQuery("UPDATE scorched3d_stats SET kills=kills+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[firedTank->getUniqueId()],
		prefixid_,
		seriesid_);
		
	runQuery("UPDATE scorched3d_stats SET deaths=deaths+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[deadTank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, %i, %i, NOW());",
		prefixid_, seriesid_,
		EventTeamKill,
		playerId_[firedTank->getUniqueId()],
		playerId_[deadTank->getUniqueId()], 
		weaponId_[weapon->getParent()->getName()]);

	runQuery("UPDATE scorched3d_stats SET teamkills=teamkills+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[firedTank->getUniqueId()],
		prefixid_,
		seriesid_);
		
	runQuery("UPDATE scorched3d_stats SET deaths=deaths+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[deadTank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankSelfKilled(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, %i, NOW());",
		prefixid_, seriesid_, 
		EventSelfKill,
		playerId_[firedTank->getUniqueId()],
		weaponId_[weapon->getParent()->getName()]);

	runQuery("UPDATE scorched3d_stats SET selfkills=selfkills+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[firedTank->getUniqueId()],
		prefixid_,
		seriesid_);
		
	runQuery("UPDATE scorched3d_stats SET deaths=deaths+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[firedTank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankWon(Tank *tank)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventWon,
		playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_stats SET wins=wins+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;", 
		playerId_[tank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::tankOverallWinner(Tank *tank)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events "
		"(prefixid, seriesid, eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, 0, 0, NOW());",
		prefixid_, seriesid_,
		EventOverallWinner,
		playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_stats SET overallwinner=overallwinner+1 "
		"WHERE playerid = %i AND prefixid = %i AND seriesid = %i;",
		playerId_[tank->getUniqueId()],
		prefixid_,
		seriesid_);
}

void StatsLoggerDatabase::weaponFired(Weapon *weapon, bool deathAni)
{
	if (deathAni)
	{
		runQuery("UPDATE scorched3d_weapons SET deathshots=deathshots+1 "
			"WHERE weaponid = \"%i\" AND prefixid = %i AND seriesid = %i;",
			weaponId_[weapon->getParent()->getName()],
			prefixid_,
			seriesid_);
	}
	else
	{
		runQuery("UPDATE scorched3d_weapons SET shots=shots+1 "
			"WHERE weaponid = \"%i\" AND prefixid = %i AND seriesid = %i;", 
			weaponId_[weapon->getParent()->getName()],
			prefixid_,
			seriesid_);
	}
}

void StatsLoggerDatabase::weaponKilled(Weapon *weapon, bool deathAni)
{
	if (deathAni)
	{
		runQuery("UPDATE scorched3d_weapons SET deathkills=deathkills+1 "
			"WHERE weaponid = \"%i\" AND prefixid = %i AND seriesid = %i;", 
			weaponId_[weapon->getParent()->getName()],
			prefixid_,
			seriesid_);
	}
	else
	{
		runQuery("UPDATE scorched3d_weapons SET kills=kills+1 "
			"WHERE weaponid = \"%i\" AND prefixid = %i AND seriesid = %i;", 
			weaponId_[weapon->getParent()->getName()],
			prefixid_,
			seriesid_);
	}
}

