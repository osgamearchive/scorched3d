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
#include <common/Logger.h>
#include <weapons/AccessoryStore.h>
#include <server/ScorchedServer.h>
#include <XML/XMLFile.h>
#include <stdlib.h>

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

void StatsLoggerMySQL::runQuery(const char *fmt, ...)
{
	if (!success_) return;

	static char text[2048];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	mysql_real_query(mysql_, text, strlen(text));
}

void StatsLoggerMySQL::createLogger()
{
	if (!mysql_ && success_)
	{
		mysql_ = mysql_init(0);
		XMLFile file;
		if (file.readFile(PKGDIR "data/mysql.xml") &&
			file.getRootNode())
		{
			XMLNode *hostNode = 
				file.getRootNode()->getNamedChild("host");
			XMLNode *userNode =
				file.getRootNode()->getNamedChild("user");
			XMLNode *passwdNode =
				file.getRootNode()->getNamedChild("passwd");
			XMLNode *dbNode =
				file.getRootNode()->getNamedChild("db");
			if (hostNode && userNode && passwdNode && dbNode)
			{
				if (mysql_real_connect(
					mysql_,
					hostNode->getContent(),
					userNode->getContent(),
					passwdNode->getContent(),
					dbNode->getContent(),
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
						hostNode->getContent(), userNode->getContent(),
						passwdNode->getContent(), dbNode->getContent());
				}
			}
			else
			{
				success_ = false;
				Logger::log(0, "Failed to parse mysql.xml settings file." 
					"Failed to find required nodes:"
					"host, user, passwd, db");
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
				"(name) VALUES(\"%s\");",
				ScorchedServer::instance()->getOptionsGame().
				getServerName());

			// Add event types
			runQuery("INSERT INTO scorched3d_eventtypes "
				"(eventtype, name) VALUES "
				"(%i, \"KILL\"), "
				"(%i, \"TEAMKILL\"), "
				"(%i, \"SELFKILL\"), "
				"(%i, \"WON\"), "
				"(%i, \"OVERALLWINNER\"), "
				"(%i, \"RESIGNED\"); ",
				EventKill, EventTeamKill, EventSelfKill,
				EventWon, EventOverallWinner, EventResigned);

			// Add all the weapons
			std::list<Accessory *> weapons = 
				AccessoryStore::instance()->getAllWeapons();
			std::list<Accessory *>::iterator itor;	
			for (itor = weapons.begin();
				itor != weapons.end();
				itor++)
			{
				Weapon *weapon = (Weapon *) *itor;

				// Try to determine this players sql playerid
				int weaponId = 0;
				char buffer[1024];
				sprintf(buffer, "SELECT weaponid FROM scorched3d_weapons "
					"WHERE name = \"%s\";", weapon->getName());
				if (mysql_real_query(mysql_, buffer, strlen(buffer)) == 0)
				{
					MYSQL_RES *result = mysql_store_result(mysql_);
					if (result)
					{
						int rows = mysql_num_rows(result);
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
					runQuery("INSERT INTO scorched3d_weapons (name, description) "
						"VALUES(\"%s\", \"%s\");", 
						weapon->getName(), weapon->getDescription());
					weaponId = mysql_insert_id(mysql_);		
				}

				weaponId_[weapon->getName()] = weaponId;
			}
		}
	}

}

void StatsLoggerMySQL::gameStart()
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_main SET games = games + 1 WHERE name = \"%s\";",
		ScorchedServer::instance()->getOptionsGame().
		getServerName());
}

void StatsLoggerMySQL::roundStart()
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_main SET rounds = rounds + 1 WHERE name = \"%s\";",
		ScorchedServer::instance()->getOptionsGame().
		getServerName());	
}

void StatsLoggerMySQL::tankFired(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("UPDATE scorched3d_players SET shots=shots+1 "
		"WHERE playerid = %i;", playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankResigned(Tank *tank)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, 0, 0, NOW());",
		EventResigned,
                playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_players SET resigns=resigns+1 "
		"WHERE playerid = %i;", playerId_[tank->getUniqueId()]);
}

void StatsLoggerMySQL::tankJoined(Tank *tank)
{
	createLogger();
	if (!success_) return;

	char buffer[1024];
	
	// Try to determine this players sql playerid
	int playerId = 0;
	sprintf(buffer, "SELECT playerid FROM scorched3d_players "
			"WHERE uniqueid = \"%s\";",
		tank->getUniqueId());
	if (mysql_real_query(mysql_, buffer, strlen(buffer)) == 0)
	{
		MYSQL_RES *result = mysql_store_result(mysql_);
		if (result)
		{
			int rows = mysql_num_rows(result);
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
		runQuery("INSERT INTO scorched3d_players (uniqueid) "
			"VALUES(\"%s\");",
			tank->getUniqueId());
		playerId = mysql_insert_id(mysql_);
		Logger::log(0, "Add new stats user \"%i\"", playerId);
	}

	// Store this new player id
	playerId_[tank->getUniqueId()] = playerId;

	// Add the players name (may fail if duplicates)
	runQuery("INSERT INTO scorched3d_names (playerid, name, count) VALUES "
		"(%i, \"%s\", 0);", playerId, tank->getName());
	runQuery("UPDATE scorched3d_names SET count=count+1 WHERE "
		"playerid=%i AND name=\"%s\";", playerId, tank->getName());
	
	// Joining stats
	runQuery("UPDATE scorched3d_players SET connects=connects+1, "
		"lastconnected=NOW(), name=\"%s\" "
		"WHERE playerid = %i;", tank->getName(), playerId);
}

void StatsLoggerMySQL::tankLeft(Tank *tank)
{
	createLogger();
	if (!success_) return;

	// Remove this player id
	playerId_.erase(tank->getUniqueId());
}

void StatsLoggerMySQL::tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

	runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
		"VALUES(%i, %i, %i, %i, NOW());",
		EventKill,
		playerId_[firedTank->getUniqueId()], 
		playerId_[deadTank->getUniqueId()], 
		weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d_weapons SET kills=kills+1 "
		"WHERE weaponid = \"%i\";", weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d_players SET kills=kills+1 "
		"WHERE playerid = %i;", playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, %i, %i, NOW());",
		EventTeamKill,
                playerId_[firedTank->getUniqueId()],
                playerId_[deadTank->getUniqueId()], 
                weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d_players SET teamkills=teamkills+1 "
		"WHERE playerid = %i;", playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankSelfKilled(Tank *firedTank, Weapon *weapon)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, 0, %i, NOW());",
		EventSelfKill,
                playerId_[firedTank->getUniqueId()],
                weaponId_[weapon->getName()]);

	runQuery("UPDATE scorched3d_players SET selfkills=selfkills+1 "
		"WHERE playerid = %i;", playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerMySQL::tankWon(Tank *tank)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, 0, 0, NOW());",
		EventWon,
                playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_players SET wins=wins+1 "
		"WHERE playerid = %i;", playerId_[tank->getUniqueId()]);
}

void StatsLoggerMySQL::tankOverallWinner(Tank *tank)
{
	createLogger();
	if (!success_) return;

        runQuery("INSERT INTO scorched3d_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
                "VALUES(%i, %i, 0, 0, NOW());",
		EventOverallWinner,
                playerId_[tank->getUniqueId()]);

	runQuery("UPDATE scorched3d_players SET overallwinner=overallwinner+1 "
		"WHERE playerid = %i;", playerId_[tank->getUniqueId()]);
}

#endif

