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


#include <coms/NetServer.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <engine/GameState.h>
#include <tank/TankContainer.h>
#include <server/ServerState.h>
#include <wx/utils.h>

#ifndef _NO_SERVER_ASE_
extern "C" {
#include <ASE/ASEQuerySDK.h>
}

void ASEQuery_wantstatus(void)
{
	char *serverName = (char *) OptionsGame::instance()->getServerName();
	char version[256];
	sprintf(version, "%s (%s)", ScorchedVersion, ScorchedProtocolVersion);
	
	unsigned currentState = GameState::instance()->getState();
	bool started = (currentState!=ServerState::ServerStateWaitingForPlayers);
	ASEQuery_status(
		serverName, 
		"Scorched3D", (started?"Random":"NotStarted"), 
		version,
		OptionsGame::instance()->getServerPassword()[0],
		TankContainer::instance()->getNoOfTanks(),
		OptionsGame::instance()->getNoMaxPlayers());
}

void ASEQuery_wantplayers(void)
{
	char color[64];
	std::map<unsigned int, Tank *> &tanks =
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor =  tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *tank = (*itor).second;
		sprintf(color, "%i %i %i", 
				(int) tank->getColor()[0] * 255,
				(int) tank->getColor()[1] * 255,
				(int) tank->getColor()[2] * 255);
		
		ASEQuery_addplayer(tank->getName(), 
						   color,
						   tank->getModel().getModelName(),
						   tank->getScore().getScoreString(),
						   "", //Ping
						   tank->getScore().getTimePlayedString());
	}
}

void ASEQuery_wantrules(void)
{
	static char buffer[256];
	sprintf(buffer, "%i", OptionsTransient::instance()->getNoRoundsLeft());
	ASEQuery_addrule("RoundsLeft", buffer);
	ASEQuery_addrule("ProtocolVersion", ScorchedProtocolVersion);
	ASEQuery_addrule("Version", ScorchedVersion);
	static wxString osbuffer;
	osbuffer = ::wxGetOSDescription();
	ASEQuery_addrule("OS", osbuffer.data());
	
	unsigned currentState = GameState::instance()->getState();
	bool started = (currentState!=ServerState::ServerStateWaitingForPlayers);
	ASEQuery_addrule("GameStarted", (started?"Yes":"No"));
	std::list<OptionEntry *> &options = OptionsGame::instance()->getOptions();
	std::list<OptionEntry *>::iterator itor;
	for (itor = options.begin();
		itor != options.end();
		itor ++)
	{
		OptionEntry *entry = (*itor);
		if (strcmp(entry->getName(), "ServerPassword")!=0)
		{
			ASEQuery_addrule(entry->getName(), entry->getValueAsString());
		}
	}
}
#endif
