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
#include <server/ServerState.h>
#include <server/ServerTooFewPlayersStimulus.h>

ServerBrowserInfo *ServerBrowserInfo::instance_ = 0;

ServerBrowserInfo *ServerBrowserInfo::instance()
{
	if (!instance_)
	{
		instance_ = new ServerBrowserInfo();
	}
	return instance_;
}

ServerBrowserInfo::ServerBrowserInfo() : udpsock_(0)
{
	packetV_ = SDLNet_AllocPacketV(20, 10000);
}

ServerBrowserInfo::~ServerBrowserInfo()
{
}

void ServerBrowserInfo::start()
{
	udpsock_ = SDLNet_UDP_Open(ScorchedServer::instance()->getOptionsGame().getPortNo() + 1);
	if(!udpsock_)
	{
		dialogMessage("ServerBrowserInfo",
			"Failed to open port \"%i\"",
			ScorchedServer::instance()->getOptionsGame().getPortNo() + 1);
		return;
	}
}

void ServerBrowserInfo::processMessages()
{ 
	if (!udpsock_) return;

	int numrecv = SDLNet_UDP_RecvV(udpsock_, packetV_);
	if(numrecv <=0) return;
	
	static char buffer[10000];
	for(int i=0; i<numrecv; i++) 
	{
		if (packetV_[i]->len == 0) packetV_[i]->data[0] = '\0';

		//printf("Packet = %s\n", (char *) packetV_[i]->data);

		if (0 == strcmp((char *) packetV_[i]->data, "status")) processStatusMessage(buffer);
		else if (0 == strcmp((char *) packetV_[i]->data, "info")) processInfoMessage(buffer);
		else if (0 == strcmp((char *) packetV_[i]->data, "players")) processPlayerMessage(buffer);
		else processPingMessage(buffer);
		
		int len = (int) strlen(buffer)+1;
		memcpy(packetV_[i]->data, buffer, len);
		packetV_[i]->len = len;

		//printf("Packet len = %i\n", packetV_[i]->len);
		//printf("Packet = %s\n", packetV_[i]->data);
	}
	
	SDLNet_UDP_SendV(udpsock_, packetV_, numrecv);
}

void ServerBrowserInfo::processPingMessage(char *buffer)
{
	strcpy(buffer, "<pong/>");
}

void ServerBrowserInfo::processStatusMessage(char *buffer)
{
	char *serverName = (char *) ScorchedServer::instance()->getOptionsGame().getServerName();
	char version[256];
	sprintf(version, "%s (%s)", ScorchedVersion, ScorchedProtocolVersion);
	unsigned currentState = ScorchedServer::instance()->getGameState().getState();
	bool started = !ServerTooFewPlayersStimulus::instance()->acceptStateChange(0, 0, 0.0f);
	char players[25];
	sprintf(players, "%i", ScorchedServer::instance()->getTankContainer().getNoOfTanks());
	char maxplayers[25];
	sprintf(maxplayers, "%i", ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers());

	strcpy(buffer, "<status ");
	addTag(buffer, "state", (started?"Started":"Waiting"));
	addTag(buffer, "servername", serverName);
	addTag(buffer, "fullversion", version);
	addTag(buffer, "version", ScorchedVersion);
	addTag(buffer, "protocolversion", ScorchedProtocolVersion);
	addTag(buffer, "password", ScorchedServer::instance()->getOptionsGame().getServerPassword()[0]?"On":"Off");
	addTag(buffer, "noplayers", players);
	addTag(buffer, "maxplayers", maxplayers);
	strcat(buffer, "/>");
}


void ServerBrowserInfo::processInfoMessage(char *buffer)
{
	// Add all of the other tank options
	// Currently nothing on the client uses this info
	strcpy(buffer, "<info ");
	std::list<OptionEntry *> &options = ScorchedServer::instance()->getOptionsGame().getOptions();
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
	strcat(buffer, "/>");
}

void ServerBrowserInfo::processPlayerMessage(char *buffer)
{
	// Add all of the player information
	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator tankItor;
	int i=0;

	strcpy(buffer, "<players ");
	for (tankItor =  tanks.begin();
		tankItor != tanks.end();
		tankItor++, i++)
	{
		Tank *tank = (*tankItor).second;
		static char tmp[128];

		sprintf(tmp, "pn%i", i);
		addTag(buffer, tmp, tank->getName());

		sprintf(tmp, "ps%i", i);
		addTag(buffer, tmp, tank->getScore().getScoreString());

		sprintf(tmp, "pt%i", i);
		addTag(buffer, tmp, tank->getScore().getTimePlayedString());
	}
	strcat(buffer, "/>");
}

void ServerBrowserInfo::addTag(char *buffer, const char *name, const char *value)
{
	static char newvalue[256];
	strcpy(newvalue, value);
	for (char *a=newvalue; *a; a++) if (*a == '\'') *a='\"';

	strcat(buffer, name); strcat(buffer, "='");
	strcat(buffer, newvalue); strcat(buffer, "' ");
}
