#include <common/Logger.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#else
#include <windows.h>
#include <time.h>
#endif

#include "irc/IrcConnection.h"

IrcConnection::IrcConnection()
{
	irc_name("noname", "noname", "noname");
	hostname = server = "localhost";
	port = "0";

	task = TSK_DISCONNECTED;
	trace_enable = false;
	prefix = cmd = params = NULL;
	sock = NULL;
	set = NULL;
	hostip.host = 0;
	hostip.port = 0;

	connection_dispatch_=NULL;
}

void IrcConnection::irc_name(const char *_nick, const char *_user,
			     const char *_fullname)
{
	nick = (char *) _nick;
	user = (char *) _user;
	fullname = (char *) _fullname;
}


IrcConnection::~IrcConnection()
{
	IrcDisconnect();
}



int IrcConnection::irc_send(char *buf)
{
	Uint32 len, result;

	if (!buf || !(len = strlen(buf)))
		return (-1);

	irc_trace("[IRC SEND] %s", buf);
	result = SDLNet_TCP_Send(sock, buf, len);
	if (result < 0)
		fprintf(stderr, "failed SDLNet_TCP_Send: %s\n",
			SDLNet_GetError());

	return (result);
}

char *IrcConnection::irc_receive()
{
	char *msg;
	Uint32 result, len;
	char buf[MAX_IRC_CMD + 1];

	// verify if anything ready
	if (SDLNet_CheckSockets(set, 0) == -1) {
		fprintf(stderr, "failed SDLNet_CheckSockets: %s\n",
			SDLNet_GetError());
		return 0;
	}
	// verify if anything ready
	if (SDLNet_SocketReady(sock) == 0) {
		return (NULL);
	}
	// get the string buffer over the socket
	for (msg = buf, len = 0;; msg++) {
		result = SDLNet_TCP_Recv(sock, msg, 1);
		if (result < 0) {
			fprintf(stderr, "failed SDLNet_TCP_Recv: %s\n",
				SDLNet_GetError());
			return NULL;
		}
		len++;
		if (*msg != '\n')
			continue;

		msg = (char *) malloc(len + 1);
		if (msg == NULL)
			return NULL;
		*(msg + len) = 0;
		memcpy(msg, buf, len);
		irc_trace("[IRC RECV] %s", msg);
		prefix = cmd = params = NULL;
		return msg;
	}

}

int IrcConnection::irc_cmd(const char *fmt, ...)
{
	char buf[MAX_IRC_CMD + 1];
	buf[sizeof(buf) - 1] = 0;
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);
	return irc_send(buf);
}

int IrcConnection::irc_split(char *msg)
{
	if (msg == NULL)
		return 0;

	// optional prefix 
	if (*msg != ':') {
		prefix = NULL;
	} else {
		msg++;
		prefix = msg;
		msg = strchr(msg, ' ');
		if (msg == NULL)
			return 0;
	}

	// drop trailing spaces
	while (*msg == ' ')
		*msg++ = 0;

	// requiered command
	cmd = msg;
	msg = strchr(msg, ' ');
	if (msg == NULL) {
		params = NULL;
		return 1;
	}
	// drop trailing spaces
	while (*msg == ' ')
		*msg++ = 0;

	// optional parameters
	params = msg;
	return 1;
}

void IrcConnection::irc_done()
{
	prefix = params = cmd = NULL;
}

int IrcConnection::irc_state()
{
	return task;
}

void IrcConnection::irc_parse()
{
	// handle PING
	if (strcmp(cmd, "PING") == 0) {
		irc_cmd("PONG %s", params);
		return;
	}

	// handle being kicked
	if (strcmp(cmd, "KICK") == 0) {
		Logger::log(0,"IRC kicked.");
		task = TSK_DISCONNECTED;
		return;
	}

	// handle all ERR_ replies
	if (atoi(cmd) > 400) {
		task = TSK_DISCONNECTED;
		Logger::log(0,params);
		SDL_Delay(5000);
		return;
	}
}


int IrcConnection::irc_msg(char *msg)
{
	return irc_cmd("PRIVMSG %s :%s\n\r", channel, msg);
}


int IrcConnection::irc_action()
{
	switch (task) {
	case TSK_SND_NICK:
		irc_trace("[IRC LOGIN]\n");
		Logger::log(0,"IRC awaiting login on %s ...",channel);	
		irc_cmd("NICK %s\n\r", nick);
		break;
	case TSK_SND_USER:
		irc_cmd("USER %s %s %s :%s\n\r", user, hostname, server,
			fullname);
		break;
	case TSK_EXPECT_PING:
		if ((cmd == NULL) || (strcmp(cmd, "PING") != 0))
			return 0;
		break;
	case TSK_EXPECT_001:
		if ((cmd == NULL) || (strcmp(cmd, "001") != 0))
			return 0;
		break;
	case TSK_EXPECT_002:
		if ((cmd == NULL) || (strcmp(cmd, "002") != 0))
			return 0;
		break;
	case TSK_EXPECT_003:
		if ((cmd == NULL) || (strcmp(cmd, "003") != 0))
			return 0;
		break;
	case TSK_EXPECT_004:
		if ((cmd == NULL) || (strcmp(cmd, "004") != 0))
			return 0;
		irc_cmd("JOIN %s\n\r", channel);
		break;
	case TSK_EXPECT_JOIN:
		if ((cmd == NULL) || (strcmp(cmd, "JOIN") != 0))
			return 0;
		irc_trace("[IRC READY]\n");
		Logger::log(0,"IRC ready.");
		break;
	default:
		// connected 
		irc_reply();
		return 0;
	}
	task++;
	return 1;
}

void IrcConnection::irc_reply()
{
	if ((cmd == NULL) || (prefix == NULL))
		return;

	irc_trace("[IRC REPLY] %s %s %s",
		  (prefix == NULL ? "" : prefix),
		  (cmd == NULL ? "" : cmd), (params == NULL ? "" : params)
	    );
}


void IrcConnection::irc_trace(const char *fmt, ...)
{
	va_list ap;
	if (trace_enable) {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
	}
}

void IrcConnection::irc_dispatch()
{
	char *msg;
	for (;;) {
		msg = irc_receive();
		if (msg == NULL) {
			// idle for 100 ms,if nothing to do
			if (!irc_action())
				SDL_Delay(100);
		} else {
			// all the rest
			irc_split(msg);
			irc_parse();
			irc_action();
			free(msg);
		}
		irc_done();
	}
}

int IrcConnection::connection_dispatcher(void *data)
{
	((IrcConnection *) data)->irc_dispatch();
	return (0);
}

void IrcConnection::IrcDisconnect()
{
	if (connection_dispatch_ != NULL) {
		Logger::log(0,"IRC disconnected.");
		SDL_KillThread(connection_dispatch_);
		connection_dispatch_ = NULL;
	}

	if (sock != NULL) {
		SDLNet_TCP_Close(sock);
		sock = NULL;
	}
	if (set != NULL) {
		SDLNet_FreeSocketSet(set);
		set = NULL;
	}

	task = TSK_DISCONNECTED;
	prefix = cmd = params = NULL;
}

int IrcConnection::IrcConnect(const char *_server, const char *_port,
			      const char *_channel)
{
	IPaddress ip;
	Uint16 prt;

	// make sure all resources are cleaned up first
	IrcDisconnect();

	irc_trace("[IRC CONNECT] server %s, port %s\n", _server, _port);
	Logger::log(0,"Connecting to IRC (server: %s, port: %s)...", 
			_server,_port);
	prt = atoi(_port);

	// Resolve own host into an IPaddress type
	if (SDLNet_ResolveHost(&hostip, NULL, prt) != -1) {
		char *n;

		// resolve the hostname for the IPaddress
		n = (char *)SDLNet_ResolveIP(&hostip);
		if (n != NULL)
			hostname = n;
	}

	// Reserve socket set
	set = SDLNet_AllocSocketSet(1);
	if (!set) {
		fprintf(stderr, "failed SDLNet_AllocSocketSet: %s\n",
			SDLNet_GetError());
		return 0;
	}

	// Resolve servername into an IPaddress type
	if (SDLNet_ResolveHost(&ip, (char *) _server, prt) == -1) {
		fprintf(stderr, "failed SDLNet_ResolveHost: %s\n",
			SDLNet_GetError());
		return 0;
	}
	// open the connection socket
	sock = SDLNet_TCP_Open(&ip);
	if (!sock) {
		fprintf(stderr, "failed SDLNet_TCP_Open: %s\n",
			SDLNet_GetError());
		return 0;
	}

	if (SDLNet_TCP_AddSocket(set, sock) == -1) {
		fprintf(stderr, "failed SDLNet_TCP_AddSocket: %s\n",
			SDLNet_GetError());
		return 0;
	}

	server = (char *) _server;
	channel = (char *) _channel;
	port = (char *) _port;
	task = TSK_SND_NICK;

	irc_trace("[IRC CONNECTED]\n");
	Logger::log(0,"IRC connected.");	

	connection_dispatch_ = SDL_CreateThread(
			(int (*)(void *)) &connection_dispatcher,
			(void *) this);
	return 1;
}
