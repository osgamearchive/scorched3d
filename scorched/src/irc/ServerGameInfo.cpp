#include <irc/ServerGameInfo.h>
#include <common/Logger.h>

ServerGameInfo *ServerGameInfo::instance_ = 0;
                                                                                                                                  
ServerGameInfo *ServerGameInfo::instance()
{
        if (!instance_)
        {
                instance_ = new ServerGameInfo;
        }
                                                                                                                                  
        return instance_;
}

ServerGameInfo::ServerGameInfo() : 
	IrcBot(GAME_SERVER_BOT_USER,GAME_SERVER_BOT), server_host_("localhost"), 
	server_port_(),net_published(0),lan_published(0)
{
}

ServerGameInfo::~ServerGameInfo()
{
	Stop();
}

void ServerGameInfo::Stop()
{
	net_published = lan_published = 0;
	IrcBot::Stop();
	UdpHandler::Stop();
}

void ServerGameInfo::Start(Uint16 _port)
{
	char itoa[20];
	sprintf(&itoa[0],"%d",int(_port));
	server_port_ = string(&itoa[0]);

	IrcBot::Start();
	UdpHandler::Start();
}

int ServerGameInfo::udp_query_dispatch()
{
	UDPpacket *rx,*tx;

	if (!SetPort(GAME_SERVER_INFO_PORT))
		return 0;

	rx = Alloc();
	tx = Alloc();

	if (!lan_published) {
		lan_published = 1;
		Logger::log(0,"Publishing server on LAN.");
	}

	for (;;) {
		Receive(rx);
		tx->address = rx->address;
		tx->len = setQueryReply((char *)(tx->data),(char *)(rx->data),
				"autodetect",(char *)server_port_.c_str());
		if (tx->len)
			Send(tx);
	}
	Free(tx);
	Free(rx);
	return 0;
}


#define CHK_FIRST_TOKEN(a,b) if (b==NULL) return; if ((a=strtok(b," :!@\t\n\r\b"))==NULL) return;
#define CHK_NEXT_TOKEN(a)    if ((a=strtok(NULL," !@\t\n\r\b"))==NULL) return;
#define CHK_LAST_TOKEN(a)    if ((a=strtok(NULL,"\n\r"))==NULL) return;

void ServerGameInfo::irc_reply()
{
	if (!isConnected())
		net_published = 0;		

	if (!net_published) {
		net_published = 1;
		Logger::log(0,"Published server on IRC.");
	}

	if ((cmd == NULL) || (prefix == NULL))
		return;

	if (strcmp(cmd, C_RPL_ENDOFNAMES) == 0) {
		// query myself
		irc_cmd("WHOIS %s\n\r",nick);
		return;
	}

	if (strcmp(cmd, C_RPL_WHOISUSER) == 0) {
		char *_nick,*_user,*_ip,*_dummy,*_name;
		IPaddress addr;	
		unsigned int ipaddr;
		char ipstr[32];

		// extract info and do sanity checks
		CHK_FIRST_TOKEN(_dummy,params);
		CHK_NEXT_TOKEN(_nick);
                CHK_NEXT_TOKEN(_user);
                CHK_NEXT_TOKEN(_ip);
                CHK_NEXT_TOKEN(_dummy);
		CHK_LAST_TOKEN(_name);

		// validate
		if (strcmp(_name,":" GAME_SERVER_BOT)) return;
		if (strcmp(_nick,nick)) return;
		
		// resolve ip as viewed by the outside world
		if (SDLNet_ResolveHost(&addr,_ip,0)==-1) {
			server_host_=string(_ip);
			return;
		}		
		ipaddr=SDL_SwapBE32(addr.host);
		sprintf(&ipstr[0],"%d.%d.%d.%d",ipaddr>>24,(ipaddr>>16)&0xff,
				(ipaddr>>8)&0xff,ipaddr&0xff);
		server_host_=string(ipstr);
		return;
	}

	if (strcmp(cmd, "PRIVMSG") == 0) {
		char *_nick,*_user,*_ip;
		char reply[MAX_IRC_CMD];

		// extract info and do sanity checks
		CHK_FIRST_TOKEN(_nick,prefix);
                CHK_NEXT_TOKEN(_user);
                CHK_LAST_TOKEN(_ip);

		// validate query
		if (*_nick != '\\' ) return;
		if (strcmp(_user,"~" GAME_CLIENT_BOT_USER)) return;

		if ( setQueryReply( reply, params, 
				(char *)server_host_.c_str(), 
				(char *)server_port_.c_str()) )
			irc_cmd("PRIVMSG %s :%s\n\r", _nick, reply);
		return;
	}

	// must be at end 
	IrcBot::irc_reply();

}

