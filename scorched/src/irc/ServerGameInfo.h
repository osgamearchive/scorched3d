#ifndef _SERVERGAMEINFO_H_
#define _SERVERGAMEINFO_H_

#include "irc/GameInfoEntry.h"
#include "irc/GameInfoMessage.h"
#include "irc/UdpHandler.h"
#include "irc/IrcBot.h"
#include <string>

class ServerGameInfo: public IrcBot , public UdpHandler, public GameInfoMessage {

      private:
        std::string 	server_host_;
	std::string 	server_port_;
	int 		net_published;
	int 		lan_published;
	
      protected:
	static ServerGameInfo *instance_;
        virtual int udp_query_dispatch();
	virtual void irc_reply();
      
      public:
	static ServerGameInfo *instance();

	 ServerGameInfo();
	~ServerGameInfo();

	void Start(Uint16 _port);
	void Stop();
	
};

#endif				/* _SERVERGAMEINFO_H_ */
