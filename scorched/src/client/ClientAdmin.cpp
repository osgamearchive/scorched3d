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

#include <client/ClientAdmin.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <GLEXT/GLConsole.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsAdminMessage.h>

ClientAdmin *ClientAdmin::instance_ = 0;

ClientAdmin *ClientAdmin::instance()
{
	if (!instance_)
	{
		instance_ = new ClientAdmin();
	}
	return instance_;
}

ClientAdmin::ClientAdmin() 
{
	new GLConsoleRuleMethodIAdapterEx2<ClientAdmin>(
		this, &ClientAdmin::admin, "admin");
}

ClientAdmin::~ClientAdmin()
{

}

void ClientAdmin::admin(std::list<GLConsoleRuleSplit> split, 
	std::list<std::string> &result)
{
	split.pop_front();
	if (split.empty()) adminHelp(result);
	else
	{
		GLConsoleRuleSplit firstsplit = split.front();
		split.pop_front();

		bool failed = false;
		if (0 == stricmp(firstsplit.rule.c_str(), "help")) adminHelp(result);
		else if (0 == stricmp(firstsplit.rule.c_str(), "show")) adminShow(result);
		else if (0 == stricmp(firstsplit.rule.c_str(), "killall"))
		{
			result.push_back("  sending killall...");
			ComsAdminMessage message(ComsAdminMessage::AdminKillAll);
			ComsMessageSender::sendToServer(message);
		}
		else if (0 == stricmp(firstsplit.rule.c_str(), "login") ||
			0 == stricmp(firstsplit.rule.c_str(), "kick") ||
			0 == stricmp(firstsplit.rule.c_str(), "ban") ||
			0 == stricmp(firstsplit.rule.c_str(), "mute") ||
			0 == stricmp(firstsplit.rule.c_str(), "unmute") ||
			0 == stricmp(firstsplit.rule.c_str(), "talk") ||
			0 == stricmp(firstsplit.rule.c_str(), "message"))
		{
			if (split.empty()) failed = true;
			else
			{
				GLConsoleRuleSplit secondsplit = split.front();
				result.push_back(formatString("  sending %s %s...", 
					firstsplit.rule.c_str(),
					secondsplit.rule.c_str()));
					
				ComsAdminMessage::ComsAdminMessageType type = 
					ComsAdminMessage::AdminLogin;
				if (0 == stricmp(firstsplit.rule.c_str(), "kick"))
					type = ComsAdminMessage::AdminKick;
				else if (0 == stricmp(firstsplit.rule.c_str(), "ban"))
					type = ComsAdminMessage::AdminBan;
				else if (0 == stricmp(firstsplit.rule.c_str(), "mute"))
					type = ComsAdminMessage::AdminMute;
				else if (0 == stricmp(firstsplit.rule.c_str(), "unmute"))
					type = ComsAdminMessage::AdminUnMute;	
				else if (0 == stricmp(firstsplit.rule.c_str(), "talk"))
					type = ComsAdminMessage::AdminTalk;	
				else if (0 == stricmp(firstsplit.rule.c_str(), "message"))
					type = ComsAdminMessage::AdminMessage;	
				ComsAdminMessage message(type, secondsplit.rule.c_str());
				ComsMessageSender::sendToServer(message);
			}
		}
		else if (0 == stricmp(firstsplit.rule.c_str(), "slap"))
		{
			if (split.empty()) failed = true;
			else
			{
				GLConsoleRuleSplit secondsplit = split.front();
				split.pop_front();
				if (split.empty()) failed = true;
				else
				{
					GLConsoleRuleSplit thirdsplit = split.front();
					result.push_back(formatString("  sending %s %s %s...", 
						firstsplit.rule.c_str(),
						secondsplit.rule.c_str(),
						thirdsplit.rule.c_str()));
					
					ComsAdminMessage::ComsAdminMessageType type = 
						ComsAdminMessage::AdminSlap;
					ComsAdminMessage message(type, 
						secondsplit.rule.c_str(), 
						thirdsplit.rule.c_str());
					ComsMessageSender::sendToServer(message);
				}
			}
		}
		else failed = true;

		if (failed)
		{
			std::string failed;
			GLConsoleRule::addRuleFail(failed, firstsplit.position, 
					(int) firstsplit.rule.length());
			result.push_back(failed);
			result.push_back(std::string("Unrecognised admin function ") + 
				"\"" + firstsplit.rule + "\"");
			adminHelp(result);
		}
	}
}

void ClientAdmin::adminHelp(std::list<std::string> &result)
{
	result.push_back("  help - This help");
	result.push_back("  show - Show ids for all current players");
	result.push_back("  login <password> - Login as admin");
	result.push_back("  killall - Kills all current players and starts next round");
	result.push_back("  kick <player id> - Kicks specified player");
	result.push_back("  ban <player id> - Bans and kicks specified player");
	result.push_back("  mute <player id> - Mutes specified player for everyone");
	result.push_back("  unmute <player id> - Un-mutes specified player for everyone");
	result.push_back("  slap <player id> <health> - Removes health from specified player");
	result.push_back("  talk <text> - Admin talk to all players (white with no name)");
	result.push_back("  message <text> - Message to all players (yellow in center of screen)");
}

void ClientAdmin::adminShow(std::list<std::string> &result)
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	Tank *currentTank = 
		ScorchedClient::instance()->getTankContainer().getCurrentTank();

	result.push_back(
		"--Admin Show-----------------------------------------");
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		result.push_back(
			formatString("%i \"%s\"",                
				tank->getPlayerId(), tank->getName()));
	}
	result.push_back(
		"----------------------------------------------------");
}

