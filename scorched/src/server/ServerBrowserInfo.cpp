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

#include <server/ServerBrowserInfo.h>
#include <server/ScorchedServer.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <tank/TankContainer.h>
#include <server/ServerState.h>

ServerBrowserInfo *ServerBrowserInfo::instance_ = 0;

ServerBrowserInfo *ServerBrowserInfo::instance()
{
	if (!instance_)
	{
		instance_ = new ServerBrowserInfo();
	}
	return instance_;
}

ServerBrowserInfo::ServerBrowserInfo()
{
	packetV_ = SDLNet_AllocPacketV(20, 10000);
}

ServerBrowserInfo::~ServerBrowserInfo()
{
}

void ServerBrowserInfo::start()
{
	udpsock_ = SDLNet_UDP_Open(OptionsGame::instance()->getPortNo() + 1);
	if(!udpsock_)
	{
		dialogMessage("ServerBrowserInfo",
			"Failed to open port \"%i\"",
			OptionsGame::instance()->getPortNo() + 1);
		return;
	}
}

void ServerBrowserInfo::processMessages()
{ 
	int numrecv = SDLNet_UDP_RecvV(udpsock_, packetV_);
	if(numrecv <=0) return;
	
	char *serverName = (char *) OptionsGame::instance()->getServerName();
	char version[256];
	sprintf(version, "%s (%s)", ScorchedVersion, ScorchedProtocolVersion);
	unsigned currentState = ScorchedServer::instance()->getGameState().getState();
	bool started = (currentState!=ServerState::ServerStateWaitingForPlayers);

	// Create the server info buffer that will be sent back to the 
	// client
	static char buffer[10000];
	sprintf(buffer,
		"<serverinfo>\n"
		"\t<state>%s</state>\n"
		"\t<servername>%s</servername>\n"
		"\t<fullversion>%s</fullversion>\n"
		"\t<version>%s</version>\n"
		"\t<protocolversion>%s</protocolversion>\n"
		"\t<password>%s</password>\n"
		"\t<noplayers>%i</noplayers>\n"
		"\t<maxplayers>%i</maxplayers>\n",
			(started?"Started":"Waiting"),
			serverName,
			version,
			ScorchedVersion, 
			ScorchedProtocolVersion,
			OptionsGame::instance()->getServerPassword()[0]?"On":"Off",
			TankContainer::instance()->getNoOfTanks(),
			OptionsGame::instance()->getNoMaxPlayers());

	// Add all of the other tank options
	// Currently nothing on the client uses this info
	std::list<OptionEntry *> &options = OptionsGame::instance()->getOptions();
	std::list<OptionEntry *>::iterator optionItor;
	for (optionItor = options.begin();
		optionItor != options.end();
		optionItor ++)
	{
		OptionEntry *entry = (*optionItor);
		if (strcmp(entry->getName(), "ServerPassword"))
		{
			addTag(buffer, entry->getName(), entry->getValueAsString());
		}
	}

	// Add all of the player information
	std::map<unsigned int, Tank *> &tanks =
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator tankItor;
	int i=0;
	for (tankItor =  tanks.begin();
		tankItor != tanks.end();
		tankItor++, i++)
	{
		Tank *tank = (*tankItor).second;
		static char tmp[128];

		sprintf(tmp, "playername_%i", i);
		addTag(buffer, tmp, tank->getName());

		sprintf(tmp, "playerscore_%i", i);
		addTag(buffer, tmp, tank->getScore().getScoreString());

		sprintf(tmp, "playertime_%i", i);
		addTag(buffer, tmp, tank->getScore().getTimePlayedString());
	}
	strcat(buffer, "</serverinfo>");

	int len = (int) strlen(buffer)+1;
	for(int i=0; i<numrecv; i++) 
	{
		memcpy(packetV_[i]->data, buffer, len);
		packetV_[i]->len = len;
	}
	
	SDLNet_UDP_SendV(udpsock_, packetV_, numrecv);
}

void ServerBrowserInfo::addTag(char *buffer, const char *name, const char *value)
{
	strcat(buffer, "\t<"); strcat(buffer, name); strcat(buffer, ">");
	strcat(buffer, value);
	strcat(buffer, "</"); strcat(buffer, name); strcat(buffer, ">\n");
}

