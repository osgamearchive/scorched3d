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

#include <server/ServerAuthHandlerMinKills.h>
#include <server/ScorchedServer.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>

ServerAuthHandlerMinKills::ServerAuthHandlerMinKills() : 
	minKills_(-1), maxKills_(-1)
{
}

ServerAuthHandlerMinKills::~ServerAuthHandlerMinKills()
{
}

bool ServerAuthHandlerMinKills::authenticateUser(std::string &uniqueId, 
	const char *username, const char *password, std::string &message)
{
	setup();

	int killCount = StatsLogger::instance()->getKillCount(uniqueId.c_str());
	if (minKills_ > 0 && killCount < minKills_)
	{
		message = formatString(
			"You need to have at least %i kills to play on this server.\n"
			"You only have %i kills.\n",
			minKills_, killCount);
		return false;
	}
	if (maxKills_ > 0 && killCount > maxKills_)
	{
		message = formatString(
			"You have too many kills to play on this server.\n"
			"You have %i kills and the server allows %i kills.\n",
			killCount, maxKills_);
		return false;
	}	
	
	return true;
}

bool ServerAuthHandlerMinKills::authenticateUserName(const char *uniqueId, 
	const char *playername)
{
	return true;
}

void ServerAuthHandlerMinKills::banUser(const char *uniqueId)
{
}

void ServerAuthHandlerMinKills::setup()
{
	if (minKills_ != -1) return;

	XMLFile file;
	const char *fileName = getSettingsFile(formatString("minkills-%i.xml",
		ScorchedServer::instance()->getOptionsGame().getPortNo()));

	if (!file.readFile(fileName) ||
		!file.getRootNode())
	{
		Logger::log(formatString("Failed to parse %s settings file. Error: %s", 
			fileName,
			file.getParserError()));
		return;
	}

	if (!file.getRootNode()->getNamedChild("minkills", minKills_) ||
		!file.getRootNode()->getNamedChild("maxkills", maxKills_)) 
	{
		Logger::log(formatString("Failed to parse %s settings file.", fileName));
		return;
	}
}
