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

#ifdef HAVE_PGSQL
/* Code contains several string format bugs, and at least one buffer overflow */

#include <common/StatsLoggerPGSQL.h>
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

StatsLoggerPGSQL::StatsLoggerPGSQL() : pgsql_(NULL), success_(true), lastresult_ (NULL)
{

}

StatsLoggerPGSQL::~StatsLoggerPGSQL()
{
}

#define SQL_BUFFER_SIZE 8192
bool StatsLoggerPGSQL::runQuery(const char *fmt, ...)
{
    if (!success_) return false;

    if(lastresult_) {
        PQclear(lastresult_);
        lastresult_ = NULL;
    }

    static char text[SQL_BUFFER_SIZE];
    va_list ap;
    va_start(ap, fmt);
    int sqlLen = vsnprintf(text, SQL_BUFFER_SIZE, fmt, ap);
    va_end(ap);

    if(sqlLen >= SQL_BUFFER_SIZE) {
        Logger::log(0, "pgsql: Query failed, too long.\n");
        return false;
    }

    lastresult_ = PQexec(pgsql_, text);
    
    return lastresult_ && (
            PQresultStatus(lastresult_) == PGRES_COMMAND_OK ||
            PQresultStatus(lastresult_) == PGRES_TUPLES_OK
            );
}

void StatsLoggerPGSQL::createLogger()
{
    if (!pgsql_ && success_)
    {
        XMLFile file;

        const char *fileName = getSettingsFile("pgsql-%i.xml",
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
                pgsql_ = PQsetdbLogin(
                        host.c_str(),
                        NULL,
                        NULL,
                        NULL,
                        db.c_str(),
                        user.c_str(),
                        passwd.c_str());
                if (pgsql_ && PQstatus(pgsql_) == CONNECTION_OK)
                {
                    Logger::log(0, "pgsql stats logger started");
                }
                else
                {
                    success_ = false;
                    Logger::log(0, "pgsql stats logger failed to start. "
                            "Error: %s",
                            PQerrorMessage(pgsql_));
                    Logger::log(0, "pgsql params : host %s, user %s, passwd %s, db %s",
                            host.c_str(), user.c_str(),
                            passwd.c_str(), db.c_str());
                    if(pgsql_) {
                        PQfinish(pgsql_);
                        pgsql_ = NULL;
                    }
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
            // XXX: Will fail after first insert
	    char server_name[64*2+1];
	    char published_address[64*2+1];
	    PQescapeString(server_name,
			    ScorchedServer::instance()->getOptionsGame().
			    getServerName(),64);
	    PQescapeString(published_address,
			    ScorchedServer::instance()->getOptionsGame().
			    getPublishAddress(),64);
            runQuery("INSERT INTO scorched3d_main "
                    "(name, published, prefix) VALUES('%s', '%s', '%s')",
		    server_name,
		    published_address,
                    prefix_.c_str());

            // Add event types
            // XXX: Will fail after first insert
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'KILL')",prefix_.c_str(),EventKill);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'TEAMKILL')",prefix_.c_str(), EventTeamKill);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'SELFKILL')",prefix_.c_str(), EventSelfKill);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'WON')",prefix_.c_str(), EventWon);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'OVERALLWINNER')",prefix_.c_str(), EventOverallWinner);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'CONNECTED')",prefix_.c_str(), EventConnected);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'DISCONNECTED')",prefix_.c_str(), EventDisconnected);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'JOINED')",prefix_.c_str(), EventJoined);
            runQuery("INSERT INTO scorched3d%s_eventtypes (eventtype, name) "
                    "VALUES (%i, 'RESIGNED')",prefix_.c_str(), EventResigned);

            // Add all the weapons
            std::list<Accessory *> weapons = 
                ScorchedServer::instance()->getAccessoryStore().getAllWeapons();
            std::list<Accessory *>::iterator itor;	
            for (itor = weapons.begin();
                    itor != weapons.end();
                    itor++)
            {
                Accessory *accessory = *itor;
                char name[64*2+1];
                char desc[255*2+1];
                char icon[64*2+1];
                PQescapeString(name, accessory->getName(), 64);
                PQescapeString(desc, accessory->getDescription(), 255);
                PQescapeString(icon, accessory->getIconName(), 64);

                // Try to determine this weapons sql id
                // XXX: Will fail if weapon name contains a single quote.
                int weaponId = 0;
                if (runQuery("SELECT weaponid FROM scorched3d%s_weapons "
                            "WHERE name = '%s'", prefix_.c_str(), name))
                {
                    if (lastresult_)
                    {
                        int rows = PQntuples(lastresult_);
                        for (int r=0; r<rows; r++)
                        {
                            weaponId = atoi(PQgetvalue(lastresult_, r, 0));
                        }
                    }
                }


                if (weaponId == 0)
                {

                    runQuery("INSERT INTO scorched3d%s_weapons "
                            "(name, description, armslevel, cost, bundlesize, icon) "
                            "VALUES('%s', '%s', %i, %i, %i, '%s')", 
                            prefix_.c_str(),
                            name, 
                            desc,
                            accessory->getArmsLevel(),
                            accessory->getOriginalPrice(),
                            accessory->getBundle(),
                            icon);
                    runQuery("SELECT currval('scorched3d_weapons_weaponid_seq')");
                    if(lastresult_ && PQntuples(lastresult_) > 0) {
                        weaponId = atoi(PQgetvalue(lastresult_,0,0));
                    }
                }
                else
                {
                    runQuery("UPDATE scorched3d%s_weapons SET "
                            "description = '%s', "
                            "armslevel = %i, "
                            "cost = %i, "
                            "bundlesize = %i, "
                            "icon = '%s' "
                            "WHERE name = '%s'", 
                            prefix_.c_str(),
                            desc,
                            accessory->getArmsLevel(),
                            accessory->getOriginalPrice(),
                            accessory->getBundle(),
                            icon,
                            name);
                }

                weaponId_[accessory->getName()] = weaponId;
            }
        }
    }

}

void StatsLoggerPGSQL::addIpAliases(int playerId, 
        std::set<int> &currentPlayers, std::list<std::string> &results)
{
    currentPlayers.insert(playerId);
    addAliases(playerId, results);

    std::list<std::string> ipaddresses;
    if (runQuery("SELECT ipaddress FROM scorched3d%s_ipaddress "
                "WHERE playerid = %i", prefix_.c_str(), playerId))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                ipaddresses.push_back(PQgetvalue(lastresult_, r, 0));
            }
        }
    }

    std::list<std::string>::iterator itor;
    for (itor = ipaddresses.begin();
            itor != ipaddresses.end();
            itor++)
    {
        const char *ipaddress = (*itor).c_str();
        std::list<int> newplayers;
	char addr[32*2+1];
	PQescapeString(addr,ipaddress,32);

        if (runQuery("SELECT playerid FROM scorched3d%s_ipaddress "
                    "WHERE ipaddress = '%s'", prefix_.c_str(), addr))
        {
            if (lastresult_)
            {
                int rows = PQntuples(lastresult_);
                for (int r=0; r<rows; r++)
                {
                    int newplayerid = atoi(PQgetvalue(lastresult_, r, 0));
                    if (currentPlayers.find(newplayerid) == currentPlayers.end())
                    {
                        newplayers.push_back(newplayerid);
                    }
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

std::list<std::string> StatsLoggerPGSQL::getIpAliases(Tank *tank)
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

void StatsLoggerPGSQL::addAliases(int playerId, 
        std::list<std::string> &results)
{
    if (runQuery("SELECT name FROM scorched3d%s_players "
                "WHERE playerid = %i", prefix_.c_str(), playerId))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                char *res = PQgetvalue(lastresult_, r, 0);
                if (!findInList(results, res))
                {
                    results.push_back(res);
                }
            }
        }
    }	

    if (runQuery("SELECT name FROM scorched3d%s_names "
                "WHERE playerid = %i", prefix_.c_str(), playerId))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                char *res = PQgetvalue(lastresult_, r, 0);
                if (!findInList(results, res))
                {
                    results.push_back(res);
                }
            }
        }
    }
}

std::list<std::string> StatsLoggerPGSQL::getAliases(Tank *tank)
{
    std::list<std::string> results;
    createLogger();
    if (!success_) return results;

    int playerId = getPlayerId(tank->getUniqueId());
    if (playerId == 0) return results;
    addAliases(playerId, results);

    return results;
}

void StatsLoggerPGSQL::gameStart(std::list<Tank *> &tanks)
{
    createLogger();
    if (!success_) return;

    char server_name[64*2+1];
    PQescapeString(server_name,ScorchedServer::instance()->getOptionsGame().
		    getServerName(),64);

    runQuery("UPDATE scorched3d_main SET games = games + 1 WHERE name = '%s'",
            server_name);

    std::list<Tank *>::iterator itor;
    for (itor = tanks.begin();
            itor != tanks.end();
            itor++)
    {
        Tank *tank = *itor;
        if (!tank->getState().getSpectator())
        {
            runQuery("UPDATE scorched3d%s_players SET gamesplayed=gamesplayed+1 "
                    "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]);
        }
    }
}

void StatsLoggerPGSQL::roundStart(std::list<Tank *> &tanks)
{
    createLogger();
    if (!success_) return;
    char server_name[64*2+1];
    PQescapeString(server_name,ScorchedServer::instance()->getOptionsGame().
		    getServerName(),64);

    runQuery("UPDATE scorched3d_main SET rounds = rounds + 1 WHERE name = '%s'",
		    server_name);

    std::list<Tank *>::iterator itor;
    for (itor = tanks.begin();
            itor != tanks.end();
            itor++)
    {
        Tank *tank = *itor;
        if (!tank->getState().getSpectator())
        {
            runQuery("UPDATE scorched3d%s_players SET roundsplayed=roundsplayed+1 "
                    "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]);
        }
    }
}

void StatsLoggerPGSQL::tankFired(Tank *firedTank, Weapon *weapon)
{
    createLogger();
    if (!success_) return;

    runQuery("UPDATE scorched3d%s_players SET shots=shots+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankResigned(Tank *tank)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, NULL, NOW());",
            prefix_.c_str(),
            EventResigned,
            playerId_[tank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET resigns=resigns+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

void StatsLoggerPGSQL::updateStats(Tank *tank)
{
    if (!tank->getState().getSpectator())
    {
        runQuery("UPDATE scorched3d%s_players SET timeplayed=timeplayed+%i, moneyearned=moneyearned+%i "
                "WHERE playerid = %i", 
                prefix_.c_str(),
                tank->getScore().getTimePlayedStat(), 
                tank->getScore().getTotalMoneyEarnedStat(), 
                playerId_[tank->getUniqueId()]);
    }
}

char *StatsLoggerPGSQL::tankRank(Tank *tank)
{
    char *retval = "-";
    createLogger();
    if (!success_) return retval;

    // Try to determine this players sql playerid
    int kills = 0;
    if (runQuery("SELECT kills FROM scorched3d%s_players "
                "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                char *res = PQgetvalue(lastresult_, r, 0);
                kills = atoi(res);
            }
        }
    }

    if (runQuery("SELECT count(*) FROM scorched3d%s_players "
                "WHERE kills > '%i'", prefix_.c_str(), kills))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                char *res = PQgetvalue(lastresult_, r, 0);
                int rank = atoi(res) + 1;

                static char rankStr[100];
                sprintf(rankStr, "%i", rank);
                retval = rankStr;
            }
        }
    }

    return retval;
}

int StatsLoggerPGSQL::getPlayerId(const char *uniqueId)
{
    createLogger();
    if (!success_) return 0;

    // Try to determine this players sql playerid
    int playerId = 0;

    char uniqueIdStr[64*2+1];
    PQescapeString(uniqueIdStr,uniqueId,64);
    if (runQuery("SELECT playerid FROM scorched3d%s_players "
                "WHERE uniqueid = '%s'", prefix_.c_str(), uniqueIdStr))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                char *res = PQgetvalue(lastresult_, r, 0);
                playerId = atoi(res);
            }
        }
    }

    return playerId;
}

char *StatsLoggerPGSQL::getStatsId(Tank *tank)
{
    createLogger();
    if (!success_) return "";
    int id = getPlayerId(tank->getUniqueId());
    static char buffer[10];
    sprintf(buffer, "%i", id);
    return buffer;
}

char *StatsLoggerPGSQL::allocateId() 
{ 
    static char buffer[128];
    do
    {
        sprintf(buffer, "%i-%i-%i", rand(), rand(), rand());
    } while (getPlayerId(buffer) != 0);
    return buffer;
}

void StatsLoggerPGSQL::addInfo(Tank *tank)
{
    // Add the players name (may fail if duplicates)
    char name[32 * 2 + 1];
    char ipaddr[32 * 2 + 1];
    PQescapeString(name, tank->getName(),32);
    PQescapeString(ipaddr, NetInterface::getIpName(tank->getIpAddress()),32);

    runQuery("INSERT INTO scorched3d%s_names (playerid, name, count) VALUES "
            "(%i, '%s', 0)", prefix_.c_str(), playerId_[tank->getUniqueId()], 
            name);
    runQuery("UPDATE scorched3d%s_names SET count=count+1 WHERE "
            "playerid=%i AND name='%s'", prefix_.c_str(), 
            playerId_[tank->getUniqueId()], name);

    // Add the ipaddress (may fail if duplicates)
    runQuery("INSERT INTO scorched3d%s_ipaddress (playerid, ipaddress, count) VALUES "
            "(%i, '%s', 0)", prefix_.c_str(), 
            playerId_[tank->getUniqueId()], ipaddr);
    runQuery("UPDATE scorched3d%s_ipaddress SET count=count+1 WHERE "
            "playerid=%i AND ipaddress='%s'", prefix_.c_str(), 
            playerId_[tank->getUniqueId()], ipaddr);

    // Update last username etc
    runQuery("UPDATE scorched3d%s_players SET "
            "name='%s', ipaddress='%s' "
            "WHERE playerid = %i",
            prefix_.c_str(), name, ipaddr, playerId_[tank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankConnected(Tank *tank)
{
    // We don't have a player id, create one
    int playerId = getPlayerId(tank->getUniqueId());
    if (playerId == 0)
    {
        char uniqueIdStr[64 * 2 + 1];
        PQescapeString(uniqueIdStr, tank->getUniqueId(), 64);
        runQuery("INSERT INTO scorched3d%s_players (uniqueid) "
                "VALUES('%s')", prefix_.c_str(),
                uniqueIdStr);
        runQuery("SELECT currval('scorched3d_players_playerid_seq')");
        if(lastresult_ && PQntuples(lastresult_) > 0) {
            playerId = atoi(PQgetvalue(lastresult_,0,0));
        }
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
            "VALUES(%i, %i, NULL, NULL, NOW())",
            prefix_.c_str(),
            EventConnected,
            playerId_[tank->getUniqueId()]);

    char osdesc[32 * 2 + 1];
    PQescapeString(osdesc, tank->getHostDesc(), 32);

    // Connecting stats
    runQuery("UPDATE scorched3d%s_players SET connects=connects+1, "
            "lastconnected=NOW(), osdesc='%s' "
            "WHERE playerid = %i", 
            prefix_.c_str(), osdesc, 
            playerId);
}

void StatsLoggerPGSQL::tankJoined(Tank *tank)
{
    createLogger();
    if (!success_) return;

    // Joined events
    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, NULL, NOW())",
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
        char avatarName[32*2+1];

        PQescapeString(avatarName, tank->getAvatar().getName(), 32);
        if (runQuery("SELECT binaryid FROM scorched3d_binary "
                    "WHERE name = '%s' AND crc = %u", 
                    avatarName,
                    crc))
        {
            if (lastresult_)
            {
                int rows = PQntuples(lastresult_);
                for (int r=0; r<rows; r++)
                {
                    char *res = PQgetvalue(lastresult_, r, 0);
                    binaryid = atoi(res);
                }
            }
        }

        if (binaryid == 0)
        {
            size_t len;
            unsigned char *to = PQescapeBytea(
                    (unsigned char *)tank->getAvatar().getFile().getBuffer(),
                    tank->getAvatar().getFile().getBufferUsed(),
                    &len);
            runQuery("INSERT INTO scorched3d_binary "
                    "(name, crc, length, data) "
                    "VALUES('%s', %u, %u, '%s')", 
                    avatarName,
                    crc,
                    tank->getAvatar().getFile().getBufferUsed(),
                    to);
            free(to);

            runQuery("SELECT currval('scorched3d_binary_binaryid_seq')");
            if(lastresult_ && PQntuples(lastresult_) > 0) {
                binaryid = atoi(PQgetvalue(lastresult_,0,0));
            }
        }

        if(binaryid != 0) {
        // Set the avatar id
        runQuery("UPDATE scorched3d%s_players SET avatarid = %i "
                "WHERE playerid = %i", 
                prefix_.c_str(), binaryid,
                playerId_[tank->getUniqueId()]);
        }
    }
}

void StatsLoggerPGSQL::tankDisconnected(Tank *tank)
{
    createLogger();
    if (!success_) return;

    // Leaving events
    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, NULL, NOW())",
            prefix_.c_str(),
            EventDisconnected,
            playerId_[tank->getUniqueId()]);

    updateStats(tank);
}

void StatsLoggerPGSQL::tankKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, %i, %i, NOW())",
            prefix_.c_str(),
            EventKill,
            playerId_[firedTank->getUniqueId()], 
            playerId_[deadTank->getUniqueId()], 
            weaponId_[weapon->getParent()->getName()]);

    // Update both players skill points
    if (runQuery("SELECT a.skill, b.skill FROM "
                "scorched3d%s_players as a, scorched3d%s_players as b "
                "WHERE a.playerid = %i AND b.playerid = %i",
                prefix_.c_str(), prefix_.c_str(), 
                playerId_[firedTank->getUniqueId()],
                playerId_[deadTank->getUniqueId()]))
    {
        if (lastresult_)
        {
            int rows = PQntuples(lastresult_);
            for (int r=0; r<rows; r++)
            {
                int firedSkill = atoi(PQgetvalue(lastresult_, r, 0));
                int deadSkill = atoi(PQgetvalue(lastresult_, r, 1));

                float weaponMult = (float(weapon->getArmsLevel()) / 10.0f) + 1.0f;

                int skillDiff = 
                    int((20.0f * weaponMult) / (1.0f + powf(10.0f, (float(firedSkill - deadSkill) / 1000.0f))));

                runQuery("UPDATE scorched3d%s_players SET skill=skill+%i "
                        "WHERE playerid = %i", 
                        prefix_.c_str(), 
                        skillDiff,
                        playerId_[firedTank->getUniqueId()]);

                runQuery("UPDATE scorched3d%s_players SET skill=skill-%i "
                        "WHERE playerid = %i", 
                        prefix_.c_str(), 
                        skillDiff,
                        playerId_[deadTank->getUniqueId()]);
            }
        }
    }

    runQuery("UPDATE scorched3d%s_players SET kills=kills+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[deadTank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankTeamKilled(Tank *firedTank, Tank *deadTank, Weapon *weapon)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, %i, %i, NOW())",
            prefix_.c_str(),
            EventTeamKill,
            playerId_[firedTank->getUniqueId()],
            playerId_[deadTank->getUniqueId()], 
            weaponId_[weapon->getParent()->getName()]);

    runQuery("UPDATE scorched3d%s_players SET teamkills=teamkills+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[deadTank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankSelfKilled(Tank *firedTank, Weapon *weapon)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, %i, NOW())",
            prefix_.c_str(),
            EventSelfKill,
            playerId_[firedTank->getUniqueId()],
            weaponId_[weapon->getParent()->getName()]);

    runQuery("UPDATE scorched3d%s_players SET selfkills=selfkills+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET deaths=deaths+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[firedTank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankWon(Tank *tank)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, NULL, NOW())",
            prefix_.c_str(),
            EventWon,
            playerId_[tank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET wins=wins+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

void StatsLoggerPGSQL::tankOverallWinner(Tank *tank)
{
    createLogger();
    if (!success_) return;

    runQuery("INSERT INTO scorched3d%s_events (eventtype, playerid, otherplayerid, weaponid, eventtime) "
            "VALUES(%i, %i, NULL, NULL, NOW())",
            prefix_.c_str(),
            EventOverallWinner,
            playerId_[tank->getUniqueId()]);

    runQuery("UPDATE scorched3d%s_players SET overallwinner=overallwinner+1 "
            "WHERE playerid = %i", prefix_.c_str(), playerId_[tank->getUniqueId()]);
}

void StatsLoggerPGSQL::weaponFired(Weapon *weapon, bool deathAni)
{
    if (deathAni)
    {
        runQuery("UPDATE scorched3d%s_weapons SET deathshots=deathshots+1 "
                "WHERE weaponid = %i", prefix_.c_str(), 
                weaponId_[weapon->getParent()->getName()]);
    }
    else
    {
        runQuery("UPDATE scorched3d%s_weapons SET shots=shots+1 "
                "WHERE weaponid = %i", prefix_.c_str(), 
                weaponId_[weapon->getParent()->getName()]);
    }
}

void StatsLoggerPGSQL::weaponKilled(Weapon *weapon, bool deathAni)
{
    if (deathAni)
    {
        runQuery("UPDATE scorched3d%s_weapons SET deathkills=deathkills+1 "
                "WHERE weaponid = %i", prefix_.c_str(), 
                weaponId_[weapon->getParent()->getName()]);
    }
    else
    {
        runQuery("UPDATE scorched3d%s_weapons SET kills=kills+1 "
                "WHERE weaponid = %i", prefix_.c_str(), 
                weaponId_[weapon->getParent()->getName()]);
    }
}

#endif // HAVE_PGSQL
