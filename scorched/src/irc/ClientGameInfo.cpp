#include <irc/ClientGameInfo.h>
#include <common/Logger.h>

ClientGameInfo *ClientGameInfo::instance_ = 0;
 
ClientGameInfo *ClientGameInfo::instance()
{
        if (!instance_)
        {
                instance_ = new ClientGameInfo;
        }
         return instance_;
}

ClientGameInfo::ClientGameInfo() : 
	IrcBot(GAME_CLIENT_BOT_USER,GAME_CLIENT_BOT) 		
{
}

ClientGameInfo::~ClientGameInfo()
{
	Stop();
	ClearEntries();
}

void ClientGameInfo::Start()
{
	IrcBot::Start();
	UdpHandler::Start();

}

void ClientGameInfo::Stop()
{
	IrcBot::Stop();
	UdpHandler::Stop();
}

#define CHK_FIRST_TOKEN(a,b) if (b==NULL) return; if ((a=strtok(b," !@\t\n\r\b"))==NULL) return;
#define CHK_NEXT_TOKEN(a)    if ((a=strtok(NULL," !@\t\n\r\b"))==NULL) return;
#define CHK_LAST_TOKEN(a)    if ((a=strtok(NULL,"\n\r"))==NULL) return;

void ClientGameInfo::irc_reply()
{
	std::list < std::string >::iterator itor;

	if ((cmd == NULL) || (prefix == NULL))
		return;

	if (strcmp(cmd, C_RPL_ENDOFNAMES) == 0) {
		const char *name;
		ClearEntries();
		for ( itor = names_.begin(); itor != names_.end(); itor ++)
		{
			// whois reserved names only
			name = itor->c_str();
			if (*name=='\\')
				irc_cmd("WHOIS %s\n\r",name);
		}
		return;
	}

	if (strcmp(cmd, C_RPL_WHOISUSER) == 0) {
		char *_nick,*_user,*_ip,*_dummy,*_name;
		char query[MAX_IRC_CMD];

		// extract info and do sanity checks
		CHK_FIRST_TOKEN(_dummy,params);
		CHK_NEXT_TOKEN(_nick);
                CHK_NEXT_TOKEN(_user);
                CHK_NEXT_TOKEN(_ip);
                CHK_NEXT_TOKEN(_dummy);
		CHK_LAST_TOKEN(_name);

		if (strcmp(_name,":" GAME_SERVER_BOT)) return;
		if (strcmp(_user,"~" GAME_SERVER_BOT_USER)) return;

		// send irc query
		setQuery(query);
		irc_cmd("PRIVMSG %s :%s\n\r", _nick, query);
		return;
	}

        if (strcmp(cmd, "PRIVMSG") == 0) {
		GameInfoEntry _info;
                char *_nick,*_user,*_ip;
                char reply[MAX_IRC_CMD];
 
                // extract info and do sanity checks
                CHK_FIRST_TOKEN(_nick,prefix);
                CHK_NEXT_TOKEN(_user);
                CHK_LAST_TOKEN(_ip);

		// accept reserved nicks only
		if (*_nick != '\\') return;
		if (strcmp(_user,"~" GAME_SERVER_BOT_USER)) return;
		
		if ( getQueryReplyInfo(_info,params,NULL,NULL) )
			AddEntry(_info);
                return;
        }


	// must be at end 
	IrcBot::irc_reply();

}

int ClientGameInfo::udp_query_dispatch()
{
	char _ip[32];
	unsigned int ipaddr;
	GameInfoEntry entry;
	UDPpacket *rx;

	rx = Alloc();
	for (;;) {
		SetPort(0);
		Receive(rx);

		// extract ip from sender iso message 
		ipaddr=SDL_SwapBE32(rx->address.host);
		sprintf(_ip,"%d.%d.%d.%d",ipaddr>>24,(ipaddr>>16)&0xff,
				(ipaddr>>8)&0xff,ipaddr&0xff);

		if (getQueryReplyInfo(entry,(char *)(rx->data),_ip,NULL))
			AddEntry(entry);
	}
	Free(rx);
	return 0;
}

void ClientGameInfo::IrcQuery()
{
	Logger::log(0,"IRC query launched.");
	IrcBot::Start(); // shouldn't do anything if already started

	if (isConnected()) {
		irc_cmd("NAMES\n\r");
	}
}


void ClientGameInfo::LanQuery()
{
	UDPpacket *tx;

	Logger::log(0,"LAN query launched.");
	UdpHandler::Start(); // shouldn't do anything if already started

	ClearEntries();

	tx = Alloc();
	tx->len = setQuery((char *)(tx->data));
	SetDest(tx,(char *)"255.255.255.255",Uint16(GAME_SERVER_INFO_PORT));
	Send(tx);
	Free(tx);

}


