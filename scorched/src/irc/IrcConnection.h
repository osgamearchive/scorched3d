#ifndef _IRC_CLIENT_H_
#define _IRC_CLIENT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>

#include "irc/IrcDefines.h"

#define TSK_DISCONNECTED -1
#define TSK_SND_NICK      0
#define TSK_SND_USER      1
#define TSK_EXPECT_PING   2
#define TSK_EXPECT_001    3
#define TSK_EXPECT_002    4
#define TSK_EXPECT_003    5
#define TSK_EXPECT_004    6
#define TSK_EXPECT_JOIN   7
#define TSK_CONNECTED     8


class IrcConnection {

      private:

	int task;
	bool trace_enable;
	TCPsocket sock;
	IPaddress hostip;
	SDL_Thread *connection_dispatch_;
	SDLNet_SocketSet set;
	
	static int connection_dispatcher(void *unused);
	char *irc_receive();
	int irc_send(char *_buf);
	int irc_msg(char *msg);
	int irc_split(char *msg);
	int irc_action();
	void irc_parse();
	void irc_done();
	void irc_dispatch();
	char *hostname;

      protected:
	char *nick;
	char *user;
	char *fullname;
	char *channel;
	char *server;
	char *port;
	char *prefix;
	char *cmd;
	char *params;

	void irc_trace(const char *fmt, ...);
	void irc_name(const char *_nick, const char *_user,
		      const char *_fullname);
	int IrcConnect(const char *_server, const char *_port,
		       const char *_channel);
	void IrcDisconnect();	
	int irc_state();
	int irc_cmd(const char *fmt, ...);

	virtual void irc_reply();

      public:
	 IrcConnection();
	~IrcConnection();

};

#endif				/* _IRC_CLIENT_H_ */
