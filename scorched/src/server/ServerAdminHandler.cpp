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
#include <server/ServerChannelManager.h>
#include <server/ServerCommon.h>
#include <server/ServerAdminCommon.h>
#include <common/OptionsScorched.h>
#include <common/OptionsTransient.h>
#include <common/StatsLogger.h>
#include <common/Defines.h>
#include <coms/ComsAdminMessage.h>
#include <coms/ComsSyncCheckMessage.h>
#include <coms/ComsMessageSender.h>
#include <net/NetInterface.h>
#include <tank/TankContainer.h>
#include <tank/TankAdmin.h>
#include <tank/TankState.h>
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

bool ServerAdminHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsAdminMessage message;
	if (!message.readMessage(reader)) return false;

	unsigned int destinationId = netMessage.getDestinationId();

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
			ServerChannelManager::instance()->refreshDestination(
				netMessage.getDestinationId());
		}
		else
		{
			adminTank->getState().setAdminTries(
				adminTank->getState().getAdminTries() + 1);
			
			ServerCommon::sendString(destinationId,
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
				ScorchedServer::instance()->getTankContainer().getAllTanks();
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
			ServerChannelManager::instance()->refreshDestination(
				netMessage.getDestinationId());
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

					result += formatString("\"%s:%s:%s\" %s %s (%s) - %s",
						entry.name.c_str(),
						entry.uniqueid.c_str(),
						entry.SUI.c_str(),
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
	case ComsAdminMessage::AdminBan:
		{
			if (!ServerAdminCommon::banPlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for ban");
			}
		}
		break;
	case ComsAdminMessage::AdminFlag:
		{
			if (!ServerAdminCommon::flagPlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for flag");
			}
		}
		break;
	case ComsAdminMessage::AdminPoor:
		{
			if (!ServerAdminCommon::poorPlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for poor");
			}
		}
		break;	
	case ComsAdminMessage::AdminKick:
		{
			if (!ServerAdminCommon::kickPlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for kick");
			}
		}
		break;
	case ComsAdminMessage::AdminMute:
	case ComsAdminMessage::AdminUnMute:
		{
			bool mute = (message.getType() == ComsAdminMessage::AdminMute);
			if (!ServerAdminCommon::mutePlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1()), mute))
			{
				ServerCommon::sendString(destinationId, "Unknown player for mute");
			}	
		}
		break;
	case ComsAdminMessage::AdminPermMute:
		{
			if (!ServerAdminCommon::permMutePlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for permmute");
			}	
		}
		break;
	case ComsAdminMessage::AdminUnPermMute:
		{
			if (!ServerAdminCommon::unpermMutePlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for inpermmute");
			}
		}
		break;
	case ComsAdminMessage::AdminTalk:
		ServerCommon::sendString(0, message.getParam1());
		break;
	case ComsAdminMessage::AdminAdminTalk:
		ServerCommon::sendStringAdmin(message.getParam1());
		break;
	case ComsAdminMessage::AdminMessage:
		ServerCommon::sendStringMessage(0, message.getParam1());
		break;
	case ComsAdminMessage::AdminSyncCheck:
		{
			ServerCommon::sendString(destinationId, "sending sync...");
			ComsSyncCheckMessage syncCheck;
			ComsMessageSender::sendToSingleClient(syncCheck, destinationId);
		}
		break;
	case ComsAdminMessage::AdminKillAll:
		ServerAdminCommon::killAll(adminTank->getState().getAdmin()->getName());
		break;
	case ComsAdminMessage::AdminNewGame:
		ServerAdminCommon::newGame(adminTank->getState().getAdmin()->getName());
		break;	
	case ComsAdminMessage::AdminSlap:
		{
			if (!ServerAdminCommon::slapPlayer(
				adminTank->getState().getAdmin()->getName(),
				atoi(message.getParam1()), (float) atof(message.getParam2())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for slap");
			}
		}
		break;
	}

	return true;
}

bool ServerAdminHandler::login(const char *name, const char *password)
{
#ifndef S3D_SERVER
	{
		return true;
	}
#endif

	std::list<Credential> creds;
	std::list<Credential>::iterator itor;
	if (!getAllCredentials(creds)) return false;

	for (itor = creds.begin();
		itor != creds.end();
		itor++)
	{
		Credential &credential = (*itor);
		if (0 == strcmp(name, credential.username.c_str()) &&
			0 == strcmp(password, credential.password.c_str()))
		{
			return true;
		}
	}
	return false;
}

bool ServerAdminHandler::getAllCredentials(std::list<Credential> &creds)
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

		Credential credential;
		if (!currentNode->getNamedChild("name", credential.username)) return false;
		if (!currentNode->getNamedChild("password", credential.password)) return false;
		if (!currentNode->failChildren()) return false;
		creds.push_back(credential);
	}

	return true;	
}
