#ifndef _CLIENTGAMEINFO_H_
#define _CLIENTGAMEINFO_H_

#include "irc/GameInfoList.h"
#include "irc/GameInfoMessage.h"
#include "irc/UdpHandler.h"
#include "irc/IrcBot.h"

class ClientGameInfo: 
	public IrcBot , 
	public UdpHandler, 
	public GameInfoMessage,
	public GameInfoList {

      protected:
	static ClientGameInfo *instance_;

	virtual void irc_reply();
	virtual int udp_query_dispatch();
	void Start();
	void Stop();
       
      public:
	static ClientGameInfo *instance();

	 ClientGameInfo();
	~ClientGameInfo();
	void IrcQuery();
	void LanQuery();

	int Servers() { return Entries(); };
};

#endif				/* _CLIENTGAMEINFO_H_ */
