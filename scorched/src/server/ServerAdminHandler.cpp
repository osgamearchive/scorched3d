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

#include <server/ServerAdminHandler.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <common/Defines.h>
#include <coms/ComsAdminMessage.h>
#include <coms/NetInterface.h>
#include <tank/TankContainer.h>
#include <tank/TankAdmin.h>
#include <XML/XMLFile.h>
#include <stdlib.h>

ServerAdminHandler *ServerAdminHandler::instance()
{
	static ServerAdminHandler *instance = 
		new ServerAdminHandler;
	return instance;
}

ServerAdminHandler::ServerAdminHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsAdminMessage",
		this);
}

ServerAdminHandler::~ServerAdminHandler()
{
}

bool ServerAdminHandler::processMessage(unsigned int destinationId,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsAdminMessage message;
	if (!message.readMessage(reader)) return false;

	// Find the tank for this destination
	Tank *adminTank = 0;
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->
			getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getDestinationId() == destinationId)
			adminTank = tank;
	}
	if (!adminTank) return true;

	// Login if that is what is happening
	if (message.getType() == ComsAdminMessage::AdminLogin)
	{
		if (login(message.getParam1(), message.getParam2()))
		{
			ServerCommon::sendString(0,
				formatString("server admin \"%s\" logged in",
				message.getParam1()));
			ServerCommon::serverLog(
				formatString("\"%s\" logged in as server admin \"%s\"",
				adminTank->getName(),
				message.getParam1()));
			adminTank->getState().setAdmin(
				new TankAdmin(message.getParam1()));
		}
		else
		{
			adminTank->getState().setAdminTries(
				adminTank->getState().getAdminTries() + 1);
			
			ServerCommon::sendString(destinationId,
				formatString("Incorrect admin password (try %i/3)", 
				adminTank->getState().getAdminTries()));
			ServerCommon::serverLog(
				formatString("Incorrect admin password (try %i/3)", 
				adminTank->getState().getAdminTries()));
			if (adminTank->getState().getAdminTries() > 3)
			{
				ServerCommon::kickPlayer(adminTank->getPlayerId());
			}
		}
		return true;
	}
	// Else only allow logged in tanks
	else if (!adminTank->getState().getAdmin())
	{
		ServerCommon::sendString(destinationId,
			"You are not logged in as admin");
		return true;
	}

	// Do admin fn (we are logged in at this point)
	switch (message.getType())
	{
	case ComsAdminMessage::AdminShow:
		{
			std::map<unsigned int, Tank *> &tanks = 
				ScorchedServer::instance()->getTankContainer().getPlayingTanks();
			std::string result;
			result += 
				"--Admin Show-----------------------------------------\n";
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *tank = (*itor).second;

				result += 
					formatString("%i \"%s\" \"%s\" \"%s\" %s \n",
						tank->getPlayerId(), 
						tank->getName(),
						NetInterface::getIpName(tank->getIpAddress()),
						StatsLogger::instance()->getStatsId(tank),
						(tank->getState().getMuted()?"Muted":"Not Muted"));
			}
			result +=
				"-----------------------------------------------------\n";

			ServerCommon::sendString(destinationId, result.c_str());
		}
		break;
	case ComsAdminMessage::AdminLogout:
		{
			ServerCommon::sendString(0,
				formatString("server admin \"%s\" logged out",
				adminTank->getState().getAdmin()->getName()));
			ServerCommon::serverLog(
				formatString("\"%s\" logged out as server admin \"%s\"",
				adminTank->getName(),
				adminTank->getState().getAdmin()->getName()));
			adminTank->getState().setAdmin(0);
		}
		break;
	case ComsAdminMessage::AdminShowBanned:
		{
			std::string result;
			result += 
				"--Admin Show Banned----------------------------------\n";

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

					result += formatString("\"%s:%s\" %s %s (%s) - %s",
						entry.name.c_str(),
						entry.uniqueid.c_str(),
						ServerBanned::getBannedTypeStr(entry.type),
						ipName.c_str(), mask.c_str(),
						(entry.bantime?ctime(&entry.bantime):"\n"));
				}
			}
			result +=
				"-----------------------------------------------------\n";

			ServerCommon::sendString(destinationId, result.c_str());
		}
		break;
	case ComsAdminMessage::AdminShowAliases:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				std::string result;
				result += 
					"--Admin Show Aliases---------------------------------\n";

				std::list<std::string> aliases =
					StatsLogger::instance()->getAliases(targetTank);
				std::list<std::string>::iterator itor;
				for (itor = aliases.begin();
					itor != aliases.end();
					itor++)
				{
					std::string alias = (*itor);

					result += formatString("\"%s\"\n",
						alias.c_str());
				}
				result +=
					"-----------------------------------------------------\n";

				ServerCommon::sendString(destinationId, result.c_str());
			}
			else ServerCommon::sendString(destinationId, "Unknown player for showaliases");
		}
		break;
	case ComsAdminMessage::AdminShowIpAliases:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				std::string result;
				result += 
					"--Admin Show Ip Aliases------------------------------\n";

				std::list<std::string> aliases =
					StatsLogger::instance()->getIpAliases(targetTank);
				std::list<std::string>::iterator itor;
				for (itor = aliases.begin();
					itor != aliases.end();
					itor++)
				{
					std::string alias = (*itor);

					result += formatString("\"%s\"\n",
						alias.c_str());
				}
				result +=
					"-----------------------------------------------------\n";

				ServerCommon::sendString(destinationId, result.c_str());
			}
			else ServerCommon::sendString(destinationId, "Unknown player for showipaliases");
		}
		break;
	case ComsAdminMessage::AdminBan:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin ban \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::banPlayer(
					targetTank->getPlayerId());
			}
			else ServerCommon::sendString(destinationId, "Unknown player for ban");
		}
		break;
	case ComsAdminMessage::AdminFlag:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin flag \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::banPlayer(
					targetTank->getPlayerId(), ServerBanned::Flagged);
			}
			else ServerCommon::sendString(destinationId, "Unknown player for flag");
		}
		break;
	case ComsAdminMessage::AdminPoor:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin poor \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::poorPlayer(
					targetTank->getPlayerId());
			}
			else ServerCommon::sendString(destinationId, "Unknown player for poor");
		}
		break;	
	case ComsAdminMessage::AdminKick:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin kick \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::kickPlayer(
					targetTank->getPlayerId());
			}
			else ServerCommon::sendString(destinationId, "Unknown player for kick");
		}
		break;
	case ComsAdminMessage::AdminMute:
	case ComsAdminMessage::AdminUnMute:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				bool mute = (message.getType() == ComsAdminMessage::AdminMute);
				ServerCommon::serverLog(
					formatString("\"%s\" admin %s \"%s\"",
					adminTank->getName(),
					(mute?"mute":"unmute"),
					targetTank->getName()));
				ServerCommon::sendString(0,
					formatString("admin %s \"%s\"",
					(mute?"mute":"unmute"),
					targetTank->getName()));
				targetTank->getState().setMuted(mute); 
			}
			else ServerCommon::sendString(destinationId, "Unknown player for mute");
		}
		break;
	case ComsAdminMessage::AdminPermMute:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin permmute \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::sendString(0,
					formatString("admin permmute \"%s\"",
					targetTank->getName()));
				ServerCommon::banPlayer(
					targetTank->getPlayerId(),
					ServerBanned::Muted);
				targetTank->getState().setMuted(true);
			}
			else ServerCommon::sendString(destinationId, "Unknown player for permmute");
		}
		break;
	case ComsAdminMessage::AdminUnPermMute:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{
				ServerCommon::serverLog(
					formatString("\"%s\" admin unpermmute \"%s\"",
					adminTank->getName(),
					targetTank->getName()));
				ServerCommon::sendString(0,
					formatString("admin unpermmute \"%s\"",
					targetTank->getName()));
				ServerCommon::banPlayer(
					targetTank->getPlayerId(),
					ServerBanned::NotBanned);
				targetTank->getState().setMuted(false);
			}
			else ServerCommon::sendString(destinationId, "Unknown player for unpermmute");
		}
		break;
	case ComsAdminMessage::AdminTalk:
		ServerCommon::serverLog(
			formatString("\"%s\" admin talk \"%s\"",
			adminTank->getName(),
			message.getParam1()));
		ServerCommon::sendString(0, message.getParam1());
		break;
	case ComsAdminMessage::AdminAdminTalk:
		ServerCommon::serverLog(
			formatString("\"%s\" admin admintalk \"%s\"",
			adminTank->getName(),
			message.getParam1()));
		ServerCommon::sendStringAdmin(message.getParam1());
		break;
	case ComsAdminMessage::AdminMessage:
		ServerCommon::serverLog(
			formatString("\"%s\" admin message \"%s\"",
			adminTank->getName(),
			message.getParam1()));
		ServerCommon::sendStringMessage(0, message.getParam1());
		break;
	case ComsAdminMessage::AdminKillAll:
		ServerCommon::serverLog(
			formatString("\"%s\" admin killall",
			adminTank->getName()));
		ServerCommon::sendString(0,
			"admin killall");
		ServerCommon::killAll();
		break;
	case ComsAdminMessage::AdminNewGame:
		ServerCommon::serverLog(
			formatString("\"%s\" admin new game",
			adminTank->getName()));
		ServerCommon::sendString(0,
			"admin new game");
		ServerCommon::killAll();
		ScorchedServer::instance()->getOptionsTransient().startNewGame();	
		break;	
	case ComsAdminMessage::AdminSlap:
		{
			Tank *targetTank = ScorchedServer::instance()->
				getTankContainer().getTankById(atoi(message.getParam1()));
			if (targetTank)
			{	
				ServerCommon::serverLog(
					formatString("\"%s\" admin slap \"%s\" %.0f",
					adminTank->getName(),
					targetTank->getName(),
					(float) atof(message.getParam2())));
				ServerCommon::sendString(0,
					formatString("admin slap \"%s\" %.0f",
					targetTank->getName(),
					(float) atof(message.getParam2())));
				ServerCommon::slapPlayer(
					targetTank->getPlayerId(),
					(float) atof(message.getParam2()));
			}
			else ServerCommon::sendString(destinationId, "Unknown player for slap");
		}
		break;
	}

	return true;
}

bool ServerAdminHandler::login(const char *name, const char *password)
{
	const char *fileName = 
		getSettingsFile(formatString("adminpassword-%i.xml",
			ScorchedServer::instance()->getOptionsGame().getPortNo()));

	XMLFile file;
	if (!file.readFile(fileName))
	{
		ServerCommon::serverLog( 
			formatString("Failed to parse \"%s\"\n%s", 
			fileName,
			file.getParserError()));
		return false;
	}
	if (!file.getRootNode())
	{
		ServerCommon::serverLog( 
			formatString("Please create file %s to have admin users", 
			fileName));
		return false;
	}

	// Itterate all of the users in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
        XMLNode *currentNode = (*childrenItor);
		if (strcmp(currentNode->getName(), "user")) return false;

		std::string currentName, currentPassword;
		if (!currentNode->getNamedChild("name", currentName)) return false;
		if (!currentNode->getNamedChild("password", currentPassword)) return false;
		if (!currentNode->failChildren()) return false;

		if (0 == strcmp(name, currentName.c_str()) &&
			0 == strcmp(password, currentPassword.c_str()))
		{
			return true;
		}
	}

	return false;	
}
