////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <server/ServerWebHandler.h>
#include <server/ServerAdminHandler.h>
#include <server/ServerLog.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerWebServerUtil.h>
#include <server/ServerCommon.h>
#include <server/ServerAdminCommon.h>
#include <server/ServerState.h>
#include <server/ServerParams.h>
#include <server/ServerChannelManager.h>
#include <landscapedef/LandscapeDefinitionsBase.h>
#include <engine/ModFiles.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <common/FileList.h>
#include <net/NetInterface.h>
#include <tank/TankContainer.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankState.h>
#include <tank/TankScore.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIAdder.h>
#include <XML/XMLParser.h>
#include <vector>
#include <algorithm>

struct LogFile
{
	std::string fileName;
	time_t fileTime;
};

static inline bool lt_logfile(const LogFile &o1, const LogFile &o2) 
{ 
	return o1.fileTime < o2.fileTime;
}

static const char *getAdminUserName(std::map<std::string, std::string> &fields)
{
	std::map<unsigned int, ServerWebServer::SessionParams>::iterator findItor = 
		ServerWebServer::instance()->getSessions().find(
		atoi(ServerWebServerUtil::getField(fields, "sid")));
	if (findItor != ServerWebServer::instance()->getSessions().end())
	{
		return findItor->second.userName.c_str();
	}
	return "Unknown";
}

bool ServerWebHandler::PlayerHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	// Check for an add
	const char *addType = ServerWebServerUtil::getField(fields, "add");
	if (addType)
	{
		TankAIAdder::addTankAI(*ScorchedServer::instance(), addType);
	}

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getAllTanks();
	std::map<unsigned int, Tank *>::iterator itor;

	const char *adminName = getAdminUserName(fields);

	// Check for any action
	const char *action = ServerWebServerUtil::getField(fields, "action");
	if (action)
	{
		for (itor = tanks.begin(); itor != tanks.end(); itor++)
		{
			// Is this tank selected
			Tank *tank = (*itor).second;
			if (ServerWebServerUtil::getField(fields, formatString("player-%u", tank->getPlayerId())))
			{
				if (0 == strcmp(action, "Kick"))
				{
					ServerAdminCommon::kickPlayer(adminName, tank->getPlayerId());
					break;
				}
				else if (0 == strcmp(action, "Mute"))
				{
					ServerAdminCommon::mutePlayer(adminName, tank->getPlayerId(), true);
				}
				else if (0 == strcmp(action, "UnMute"))
				{
					ServerAdminCommon::mutePlayer(adminName, tank->getPlayerId(), false);
				}
				else if (0 == strcmp(action, "Flag"))
				{
					ServerAdminCommon::flagPlayer(adminName, tank->getPlayerId(),
						ServerWebServerUtil::getField(fields, "reason"));
				}
				else if (0 == strcmp(action, "Poor"))
				{
					ServerAdminCommon::poorPlayer(adminName, tank->getPlayerId());
				}
				else if (0 == strcmp(action, "PermMute"))
				{
					ServerAdminCommon::permMutePlayer(adminName, tank->getPlayerId(),
						ServerWebServerUtil::getField(fields, "reason"));
				}
				else if (0 == strcmp(action, "UnPermMute"))
				{
					ServerAdminCommon::unpermMutePlayer(adminName, tank->getPlayerId());
				}
				else if (0 == strcmp(action, "Banned"))
				{
					ServerAdminCommon::banPlayer(adminName, tank->getPlayerId(),
						ServerWebServerUtil::getField(fields, "reason"));
				}
				else if (0 == strcmp(action, "Slap"))
				{
					ServerAdminCommon::slapPlayer(adminName, tank->getPlayerId(), 25.0f);
				}
				else if (0 == strcmp(action, "ShowAliases"))
				{
					ServerWebServerUtil::getHtmlRedirect(
						formatString("/playersthreaded?sid=%s&action=%s&uniqueid=%s",
							ServerWebServerUtil::getField(fields, "sid"),
							ServerWebServerUtil::getField(fields, "action"),
							tank->getUniqueId()), text);
					return true;
				}
				else if (0 == strcmp(action, "ShowIPAliases"))
				{
					ServerWebServerUtil::getHtmlRedirect(
						formatString("/playersthreaded?sid=%s&action=%s&uniqueid=%s",
							ServerWebServerUtil::getField(fields, "sid"),
							ServerWebServerUtil::getField(fields, "action"),
							tank->getUniqueId()), text);
					return true;
				}
				else
				{
					Logger::log(formatString("Unknown web handler player action \"%s\"", action));
				}
			}
		}
	}

	// Player Entries
	std::string players;
	for (itor = tanks.begin(); itor != tanks.end(); itor++)
	{
		Tank *tank = (*itor).second;
		std::string cleanName;
		std::string dirtyName(tank->getName());
		XMLNode::removeSpecialChars(dirtyName, cleanName);
		players += formatString(
			"<tr>"
			"<td>dest=%i ip=%s id=%i</td>" // Id
			"<td>%s</td>" // Name
			"<td>%s</td>" // Type
			"<td>%s</td>" // Time
			"<td>%s</td>" // Score
			"<td>%s</td>" // State
			"<td>%s</td>" // Team
			"<td><input type=\"checkbox\" name=\"player-%u\"></td>" // Select
			"</tr>\n",
			tank->getDestinationId(), NetInterface::getIpName(tank->getIpAddress()), tank->getPlayerId(),
			cleanName.c_str(),
			tank->getTankAI()?tank->getTankAI()->getName():"Human",
			tank->getScore().getTimePlayedString(),
			tank->getScore().getScoreString(),
			tank->getState().getStateString(),
			TankColorGenerator::getTeamName(tank->getTeam()),
			tank->getPlayerId()
		);
	}
	fields["PLAYERS"] = players;
	fields["NOPLAYERS"] = formatString("%i/%i", tanks.size(), 
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers());

	// Add entries
	std::string add;
	std::list<TankAI *> &ais = ScorchedServer::instance()->getTankAIs().getAis();
	std::list<TankAI *>::iterator aiitor;
	for (aiitor = ais.begin();
		aiitor != ais.end();
		aiitor++)
	{
		TankAI *ai = (*aiitor);
		if (ai->availableForPlayers())
		{
			add += formatString(
				"<input type=\"submit\" name=\"add\" value=\"%s\"\n>",
				ai->getName());
		}
	}
	fields["ADD"] = add;

	return ServerWebServerUtil::getHtmlTemplate("player.html", fields, text);
}

bool ServerWebHandler::PlayerHandlerThreaded::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	// Check for any action
	const char *action = ServerWebServerUtil::getField(fields, "action");
	const char *uniqueid = ServerWebServerUtil::getField(fields, "uniqueid");
	if (action && uniqueid)
	{
		if (0 == strcmp(action, "ShowAliases"))
		{
			std::list<std::string> aliases =
				StatsLogger::instance()->getAliases(uniqueid);
			std::string lines = ServerWebServerUtil::concatLines(aliases);
			return ServerWebServerUtil::getHtmlMessage(
				"ShowAliases", lines.c_str(), fields, text);
		}
		else if (0 == strcmp(action, "ShowIPAliases"))
		{
			std::list<std::string> aliases =
				StatsLogger::instance()->getIpAliases(uniqueid);
			std::string lines = ServerWebServerUtil::concatLines(aliases);
			return ServerWebServerUtil::getHtmlMessage(
				"ShowIPAliases", lines.c_str(), fields, text);
		}
	}

	return ServerWebServerUtil::getHtmlTemplate("player.html", fields, text);
}

bool ServerWebHandler::LogHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::deque<ServerLog::ServerLogEntry> &entries = 
		ServerLog::instance()->getEntries();

	const int pagesize = 20;
	int start = (int(entries.size()) / pagesize) * pagesize;
	const char *page = ServerWebServerUtil::getField(fields, "page");
	if (page) start = atoi(page);
	else start = entries.size() - pagesize;

	// Log entries
	std::string log;

	int min = MAX(start, 0);
	int max = MIN((int) entries.size(), start + pagesize);
	for (int i=min; i<max; i++)
	{
		std::string cleanText;
		XMLNode::removeSpecialChars(entries[i].text, cleanText);
		log += formatString(
			"<tr>"
			"<td><font size=-1>%u</font></td>"
			"<td><font size=-1>%s</font></td>"
			"</tr>\n",
			i,
			cleanText.c_str());
	}
	fields["LOG"] = log;

	// Pages
	std::string pages;
	for (int i =0; i<=int(entries.size())/pagesize-1; i++)	
	{
		pages +=
			formatString("<a href='?sid=%s&page=%i'>%i - %i</a>&nbsp;",
				fields["sid"].c_str(), 
				i * pagesize, i * pagesize, i * pagesize + pagesize - 1);
	}
	pages +=
		formatString("<a href='?sid=%s'>Last</a>",
			fields["sid"].c_str());
	fields["PAGES"] = pages;

        const char *refreshRate = ServerWebServerUtil::getField(fields, "RefreshRate");
        int refreshSeconds = 0;
        if (refreshRate) refreshSeconds = atoi(refreshRate);
        else fields["RefreshRate"]="0";

	if (refreshSeconds > 0)
        fields["Meta"] = formatString("<meta  HTTP-EQUIV=\"Refresh\" CONTENT=\"%d;URL=%s?sid=%s&RefreshRate=%s\">", refreshSeconds, url,fields["sid"].c_str(),refreshRate);

	return ServerWebServerUtil::getHtmlTemplate("log.html", fields, text);
}

bool ServerWebHandler::LogFileHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::deque<ServerLog::ServerLogEntry> &entries = 
		ServerLog::instance()->getEntries();

	const char *logFilename = ServerWebServerUtil::getField(fields, "filename");
	if (logFilename &&
		// Disallow directory backtracking
		!strstr(logFilename, "..") &&
		!strstr(logFilename, ":") &&
		!strstr(logFilename, "/") &&
		!strstr(logFilename, "\\"))
	{
		// We've requested to view a log file
		// So load the file and display it
		text.append(formatString(
			"HTTP/1.1 200 OK\r\n"
			"Server: Scorched3D\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Disposition: filename=%s.txt\r\n"
			"Connection: Close\r\n"
			"\r\n", logFilename));

		const char *filename = getLogFile(logFilename);
		std::string file = ServerWebServerUtil::getFile(filename);
		fields["FILE"] = file;

		return ServerWebServerUtil::getTemplate("logfile.html", fields, text);
	}
	else
	{
		// We've requested to see the list of current log files

		// Check to see if we want to search these files
		const char *search = 0;
		const char *action = ServerWebServerUtil::getField(fields, "action");
		if (action && 0 == strcmp(action, "Search"))
		{
			search = ServerWebServerUtil::getField(fields, "search");
		}

		// Iterator through all of the log files in the logs directory
		std::vector<LogFile> logFiles;
		std::string portNumber = formatString("%i",
			ScorchedServer::instance()->getOptionsGame().getPortNo());
		const char *dirName = getLogFile("");
		FileList dir(dirName, "*.log", false);
		if (dir.getStatus())
		{
			std::list<std::string> &files = dir.getFiles();
			std::list<std::string>::iterator itor;
			for (itor = files.begin();
				itor!= files.end();
				itor++)
			{
				const char *fileName = (*itor).c_str();
				const char *fullFilename = getLogFile(fileName);

				// Only show files from this server (this port)
				if (0 == strstr(fileName, portNumber.c_str())) continue;

				// If searching is enabled check to see if this file contains 
				// the specified string
				if (search)
				{
					std::string file = ServerWebServerUtil::getFile(fullFilename);
					if (0 == strstr(file.c_str(), search)) continue;
				}

				// Add this file to the list of files to view
				LogFile logFile;
				logFile.fileName = fileName;
				logFile.fileTime = s3d_fileModTime(fullFilename);
				logFiles.push_back(logFile);
			}
		}

		// Sort the list of files to view
		std::sort(logFiles.begin(), logFiles.end(), lt_logfile);

		// Create the table that will show the list of log files
		std::string log;
		{
			std::vector<LogFile>::iterator itor;
			for (itor = logFiles.begin();
				itor != logFiles.end();
				itor++)
			{
				LogFile &logFile = *itor;
				const char *fileName = logFile.fileName.c_str();
				log += formatString(
					"<tr>"
					"<td><font size=-1><a href=?filename=%s&sid=%s>%s</a></font></td>"
					"<td><font size=-1>%s</font></td>"
					"</tr>\n",
					fileName,
					fields["sid"].c_str(),
					fileName,
					ctime(&logFile.fileTime));
			}
		}
		fields["LOG"] = log;

		return ServerWebServerUtil::getHtmlTemplate("logfiles.html", fields, text);
	}
	return false;
}

bool ServerWebHandler::GameHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	const char *adminName = getAdminUserName(fields);

	// Check for any action
	const char *action = ServerWebServerUtil::getField(fields, "action");
	if (action)
	{
		if (0 == strcmp(action, "NewGame"))
		{
			ServerAdminCommon::newGame(adminName);
		}
		else if (0 == strcmp(action, "KillAll"))
		{
			ServerAdminCommon::killAll(adminName);
		}
	}

	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	fields["PLAYERS"] = formatString("%i/%i", tanks.size(), 
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers());

	unsigned int state = ScorchedServer::instance()->getGameState().getState();
	fields["STATE"] = ((state == ServerState::ServerStateTooFewPlayers)?"Not Playing":"Playing");

	fields["ROUND"] = formatString("%i/%i",
		ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo(),
		ScorchedServer::instance()->getOptionsGame().getNoRounds());
	fields["MOVE"] = formatString("%i/%i",
		ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo(),
		ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns());
	
	fields["BI"] = formatString("%uK", NetInterface::getBytesIn() / 1000);
	fields["BO"] = formatString("%uK", NetInterface::getBytesOut() / 1000);
	fields["P"] = formatString("%u", NetInterface::getPings());
	fields["C"] = formatString("%u", NetInterface::getConnects());

	return ServerWebServerUtil::getHtmlTemplate("game.html", fields, text);
}

bool ServerWebHandler::ServerHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	bool &messageLogging = ScorchedServer::instance()->
		getComsMessageHandler().getMessageLogging();
	bool &stateLogging = ScorchedServer::instance()->
		getGameState().getStateLogging();

	// Check for any action
	const char *action = ServerWebServerUtil::getField(fields, "action");
	if (action)
	{
		if (0 == strcmp(action, "Stop Server"))
		{
			exit(0);
		}
		else if (0 == strcmp(action, "Stop Server When Empty"))
		{
			ServerCommon::getExitEmpty() = true;
		}
		else if (0 == strcmp(action, "Set Logging"))
		{
			messageLogging = (0 == strcmp(fields["MessageLogging"].c_str(), "on"));
			stateLogging = (0 == strcmp(fields["StateLogging"].c_str(), "on"));
		}
	}

	{
		fields["MESSAGELOGGING"] = formatString(
			"<input type='radio' name='MessageLogging' %s value='on'>On</input>"
			"<input type='radio' name='MessageLogging' %s value='off'>Off</input>",
			(messageLogging?"checked":""),
			(!messageLogging?"checked":""));
		fields["STATELOGGING"] =  formatString(
			"<input type='radio' name='StateLogging' %s value='on'>On</input>"
			"<input type='radio' name='StateLogging' %s value='off'>Off</input>",
			(stateLogging?"checked":""),
			(!stateLogging?"checked":""));
	}

	unsigned int state = ScorchedServer::instance()->getGameState().getState();
	fields["STATE"] = ((state == ServerState::ServerStateTooFewPlayers)?"Not Playing":"Playing");
	fields["VERSION"] = formatString("%s (%s) - Built %s", 
		ScorchedVersion, ScorchedProtocolVersion, ScorchedBuildTime);
	fields["STARTTIME"] = getStartTime();
	fields["EXITEMPTY"] = (ServerCommon::getExitEmpty()?"True":"False");

	return ServerWebServerUtil::getHtmlTemplate("server.html", fields, text);
}

bool ServerWebHandler::BannedHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	const char *action = ServerWebServerUtil::getField(fields, "action");
	if (action && 0 == strcmp(action, "Load")) 
		ScorchedServerUtil::instance()->bannedPlayers.load(true);

	const char *selected = ServerWebServerUtil::getField(fields, "selected");
	std::string banned;
	std::list<ServerBanned::BannedRange> &bannedIps = 
		ScorchedServerUtil::instance()->bannedPlayers.getBannedIps();
	std::list<ServerBanned::BannedRange>::iterator itor;
	for (itor = bannedIps.begin();
		itor != bannedIps.end();
		itor++)
	{
		ServerBanned::BannedRange &range = (*itor);
		std::string mask = NetInterface::getIpName(range.mask);

		std::map<unsigned int, ServerBanned::BannedEntry>::iterator ipitor;
		for (ipitor = range.ips.begin();
			ipitor != range.ips.end();
			ipitor++)
		{
			unsigned int ip = (*ipitor).first;
			ServerBanned::BannedEntry &entry = (*ipitor).second;
			std::string ipName = NetInterface::getIpName(ip);

			if (selected && 0 == strcmp(selected, ipName.c_str()))
			{
				entry.type = ServerBanned::NotBanned;
			}

			std::string cleanName;
			XMLNode::removeSpecialChars(entry.name, cleanName);
			banned += formatString("<tr><td>%s</td><td>%s</td><td>%s</td>"
				"<td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>"
				"<td><input type=\"checkbox\" name=\"selected\" value=\"%s\"></td>" // Select
				"</tr>",
				(entry.bantime?ctime(&entry.bantime):""),
				cleanName.c_str(),
				entry.uniqueid.c_str(),
				entry.SUI.c_str(),
				ServerBanned::getBannedTypeStr(entry.type),
				ipName.c_str(), mask.c_str(),
				entry.adminname.c_str(), entry.reason.c_str(),
				ipName.c_str());
		}
	}
	fields["BANNED"] = banned;

	if (action && 0 == strcmp(action, "Save")) 
		ScorchedServerUtil::instance()->bannedPlayers.save();

	return ServerWebServerUtil::getHtmlTemplate("banned.html", fields, text);
}

bool ServerWebHandler::ModsHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::string modfiles;
	std::map<std::string, ModFileEntry *> &modFiles = 
		ScorchedServer::instance()->getModFiles().getFiles();
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = modFiles.begin();
		itor != modFiles.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;
		std::string cleanFileName;
		std::string dirtyFileName(entry->getFileName());
		XMLNode::removeSpecialChars(dirtyFileName, cleanFileName);
		modfiles += formatString("<tr><td>%s</td><td>%u</td><td>%u</td><td>%u</td></tr>",
			cleanFileName.c_str(),
			entry->getCompressedSize(),
			entry->getCompressedCrc(),
			entry->getUncompressedSize());
	}
	fields["MODFILES"] = modfiles;

	return ServerWebServerUtil::getHtmlTemplate("mods.html", fields, text);
}

bool ServerWebHandler::SessionsHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	// Sessions Entries
	{
		std::string sessions;
		std::map<unsigned int, ServerWebServer::SessionParams> &sessionparams = 
			ServerWebServer::instance()->getSessions();
		std::map<unsigned int, ServerWebServer::SessionParams>::iterator itor;
		for (itor = sessionparams.begin(); itor != sessionparams.end(); itor++)
		{
			ServerWebServer::SessionParams &params = (*itor).second;

			time_t theTime = (time_t) params.sessionTime;
			const char *timeStr = ctime(&theTime);
			sessions += formatString(
				"<tr>"
				"<td>%s</td>" // Name
				"<td>%s</td>" // Time
				"<td>%s</td>" // Ip
				"</tr>\n",
				params.userName.c_str(),
				timeStr,
				params.ipAddress.c_str()
			);
		}
		fields["SESSIONS"] = sessions;
	}

	// List of admins
	{
		std::string admins;
		std::list<ServerAdminHandler::Credential> creds;
		std::list<ServerAdminHandler::Credential>::iterator itor;
		ServerAdminHandler::instance()->getAllCredentials(creds);
		for (itor = creds.begin();
			itor != creds.end();
			itor++)
		{
			ServerAdminHandler::Credential &crendential = (*itor);
			admins += formatString(
				"<tr>"
				"<td>%s</td>" // Name
				"</tr>\n",
				crendential.username.c_str()
			);
		}
		fields["ADMINS"] = admins;
	}

	return ServerWebServerUtil::getHtmlTemplate("sessions.html", fields, text);
}

bool ServerWebHandler::StatsHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::map<std::string, NetMessage *> &parts,
	std::string &text)
{
	std::string message;

	const char *find;
	const char *action = ServerWebServerUtil::getField(fields, "action");
	if (action && 
		(0 == strcmp(action, "Find")) &&
		(find = ServerWebServerUtil::getField(fields, "find")))
	{
		message.append("<b>Players</b>\n");
		message.append(StatsLogger::instance()->getPlayerInfo(find));
	}
	else if (action && (0 == strcmp(action, "Combine")))
	{
		const char *player1 = ServerWebServerUtil::getField(fields, "player1");
		const char *player2 = ServerWebServerUtil::getField(fields, "player2");
		if (player1 && player2)
		{
			int p1 = atoi(player1);
			int p2 = atoi(player2);
			if (p1 && p2)
			{
				StatsLogger::instance()->combinePlayers(p1, p2);
				message.append("<b>Combined</b>\n");
			}
		}
	}
	else
	{
		message.append("<b>Ranks</b>\n");
		message.append(StatsLogger::instance()->getTopRanks());
	}

	int pos;
	while ((pos = message.find("\n")) > 0)
	{
		message.replace(pos, 1, "<br>");
	}

	fields["RANKS"] = message;

	return ServerWebServerUtil::getHtmlTemplate("stats.html", fields, text);
}
