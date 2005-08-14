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
#include <server/ServerLog.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <server/ServerTextHandler.h>
#include <engine/ModFiles.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <coms/NetInterface.h>
#include <tank/TankContainer.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIAdder.h>

static const char *getField(std::map<std::string, std::string> &fields, const char *field)
{
	std::map<std::string, std::string>::iterator itor = 
		fields.find(field);
	if (itor != fields.end())
	{
		return (*itor).second.c_str();
	}
	return 0;
}

bool ServerWebHandler::PlayerHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;

	// Check for any action
	const char *action = getField(fields, "action");
	if (action)
	{
		for (itor = tanks.begin(); itor != tanks.end(); itor++)
		{
			// Is this tank selected
			Tank *tank = (*itor).second;
			if (getField(fields, formatString("player-%u", tank->getPlayerId())))
			{
				if (0 == strcmp(action, "Kick"))
				{
					ServerCommon::kickPlayer(tank->getPlayerId(), true);
					break;
				}
				else if (0 == strcmp(action, "Mute"))
				{
					tank->getState().setMuted(true);
				}
				else if (0 == strcmp(action, "UnMute"))
				{
					tank->getState().setMuted(false);
				}
				else if (0 == strcmp(action, "Poor"))
				{
					ServerCommon::poorPlayer(tank->getPlayerId());
				}
				else if (0 == strcmp(action, "PermMute"))
				{
					ServerCommon::banPlayer(tank->getPlayerId(), ServerBanned::Muted);
				}
				else if (0 == strcmp(action, "UnPermMute"))
				{
					ServerCommon::banPlayer(tank->getPlayerId(), ServerBanned::NotBanned);
				}
				else if (0 == strcmp(action, "Banned"))
				{
					ServerCommon::banPlayer(tank->getPlayerId(), ServerBanned::Banned);
				}
				else if (0 == strcmp(action, "Slap"))
				{
					ServerCommon::slapPlayer(tank->getPlayerId(), 25.0f);
				}
				else
				{
					Logger::log("Unknown web handler player action \"%s\"", action);
				}
			}
		}
	}

	// Check for an add
	const char *addType = getField(fields, "add");
	if (addType)
	{
		TankAIAdder::addTankAI(*ScorchedServer::instance(),
			addType, "Random", "", true);
	}

	// Player Entries
	std::string players;
	for (itor = tanks.begin(); itor != tanks.end(); itor++)
	{
		Tank *tank = (*itor).second;
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
			tank->getName(),
			tank->getTankAI()?tank->getTankAI()->getName():"Human",
			tank->getScore().getTimePlayedString(),
			tank->getScore().getScoreString(),
			tank->getState().getStateString(),
			(tank->getTeam() == 0?"None":(tank->getTeam() == 1?"Red":(tank->getTeam() == 2?"Blue":(tank->getTeam() == 2?"Green":"Yellow")))),
			tank->getPlayerId()
		);
	}
	fields["PLAYERS"] = players;

	// Add entries
	std::string add;
	std::list<TankAI *> &ais = ScorchedServer::instance()->getTankAIs().getAis();
	std::list<TankAI *>::iterator aiitor;
	for (aiitor = ais.begin();
		aiitor != ais.end();
		aiitor++)
	{
		TankAI *ai = (*aiitor);
		
		add += formatString(
			"<input type=\"submit\" name=\"add\" value=\"%s\"\n>",
			ai->getName());
	}
	fields["ADD"] = add;

	return ServerWebServer::getTemplate("player.html", fields, text);
}

bool ServerWebHandler::LogHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	std::deque<ServerLog::ServerLogEntry> &entries = 
		ServerLog::instance()->getEntries();

	const int pagesize = 20;
	int start = (int(entries.size()) / pagesize) * pagesize;
	const char *page = getField(fields, "page");
	if (page) start = atoi(page);

	// Log entries
	std::string log;

	int min = MAX(start, 0);
	int max = MIN((int) entries.size(), start + pagesize);
	for (int i=min; i<max; i++)
	{
		log += formatString(
			"<tr>"
			"<td><font size=-2>%u</font></td>"
			"<td><font size=-2>%s</font></td>"
			"</tr>\n",
			i,
			entries[i].text.c_str());
	}
	fields["LOG"] = log;

	// Pages
	std::string pages;
	for (int i =0; i<=int(entries.size())/pagesize; i++)
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

	return ServerWebServer::getTemplate("log.html", fields, text);
}

bool ServerWebHandler::GameHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	// Check for any action
	const char *action = getField(fields, "action");
	if (action)
	{
		if (0 == strcmp(action, "NewGame"))
		{
			ServerCommon::startNewGame();
		}
		else if (0 == strcmp(action, "KillAll"))
		{
			ServerCommon::killAll();
		}
	}

	return ServerWebServer::getTemplate("game.html", fields, text);
}

bool ServerWebHandler::SettingsHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	std::list<OptionEntry *>::iterator itor;
	std::list<OptionEntry *> &options = 
		ScorchedServer::instance()->getOptionsGame().
			getChangedOptions().getOptions();

	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			readOptionsFromFile((char *) OptionsParam::instance()->getServerFile());
	}
	else
	{
		// Check if any changes have been made
		for (itor = options.begin();
			itor != options.end();
			itor++)
		{
			OptionEntry *entry = (*itor);
			std::map<std::string, std::string>::iterator findItor =
				fields.find(entry->getName());
			if (findItor != fields.end())
			{
				const char *value = (*findItor).second.c_str();
				entry->setValueFromString(value);
			}
		}
	}

	// Show the current settings
	std::string settings;
	for (itor = options.begin();
		itor != options.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		if (!(entry->getData() & OptionEntry::DataDepricated))
		{
			std::string value;
			if (entry->getEntryType() == OptionEntry::OptionEntryTextType)
			{
				value = formatString("<textarea name='%s' cols=20 rows=5>%s</textarea>",
					entry->getName(),
					entry->getValueAsString());
			}
			else
			{
				value = formatString("<input type='text' name='%s' value='%s'>",
					entry->getName(),
					entry->getValueAsString());
			}

			settings += formatString("<tr><td>%s</td>"
				"<td><font size=-1>%s</font></td>"
				"<td>%s</td></tr>\n",
				entry->getName(),
				entry->getDescription(),
				value.c_str());
		}
	}

	fields["SETTINGS"] = settings;

	if (action && 0 == strcmp(action, "Save"))
	{
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
			writeOptionsToFile((char *) OptionsParam::instance()->getServerFile());
	}

	return ServerWebServer::getTemplate("settings.html", fields, text);
}

bool ServerWebHandler::TalkHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	const char *say = getField(fields, "text");
	if (say)
	{
		const char *type = getField(fields, "type");
		if (!type || 0 == strcmp(type, "all"))
		{
			ServerCommon::sendString(0, say);
			ServerCommon::serverLog(0, "Says : %s", say);
		}
		else if (0 == strcmp(type, "message"))
		{
			ServerCommon::sendStringMessage(0, say);
			ServerCommon::serverLog(0, "Messages : %s", say);
		}
		else if (0 == strcmp(type, "admin"))
		{
			ServerCommon::sendStringAdmin(say);
			ServerCommon::serverLog(0, "Admins Says : %s", say);
		}
	}

	std::string texts;
	std::list<std::string> &textsList = 
		ServerTextHandler::instance()->getLastMessages();
	std::list<std::string>::iterator textsListItor;
	for (textsListItor = textsList.begin();
		textsListItor != textsList.end();
		textsListItor++)
	{
		texts += (*textsListItor);
		texts += "<br>\n";
	}
	fields["TEXTS"] = texts;

	return ServerWebServer::getTemplate("talk.html", fields, text);
}

bool ServerWebHandler::BannedHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
	std::string &text)
{
	const char *action = getField(fields, "action");
	if (action && 0 == strcmp(action, "Load")) 
		ScorchedServerUtil::instance()->bannedPlayers.load(true);

	const char *selected = getField(fields, "selected");
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
				entry.type = ServerBanned::NotBanned;

			banned += formatString("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>"
				"<td><input type=\"checkbox\" name=\"selected\" value=\"%s\"></td>" // Select
				"</tr>",
				(entry.bantime?ctime(&entry.bantime):""),
				entry.name.c_str(),
				entry.uniqueid.c_str(),
				ServerBanned::getBannedTypeStr(entry.type),
				ipName.c_str(), mask.c_str(),
				ipName.c_str());
		}
	}
	fields["BANNED"] = banned;

	if (action && 0 == strcmp(action, "Save")) 
		ScorchedServerUtil::instance()->bannedPlayers.save();

	return ServerWebServer::getTemplate("banned.html", fields, text);
}

bool ServerWebHandler::ModsHandler::processRequest(const char *url,
	std::map<std::string, std::string> &fields,
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
		modfiles += formatString("<tr><td>%s</td><td>%u</td><td>%u</td><td>%u</td></tr>",
			entry->getFileName(), 
			entry->getCompressedSize(),
			entry->getCompressedCrc(),
			entry->getUncompressedSize());
	}
	fields["MODFILES"] = modfiles;

	return ServerWebServer::getTemplate("mods.html", fields, text);
}

