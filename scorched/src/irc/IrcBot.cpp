#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <list>
#include <algorithm>
#include "irc/IrcBot.h"

IrcBot::IrcBot(const char *_user,const char *_name) 
{
	set_string(user_,(char *)_user);
	set_string(name_,(char *)_name);
	set_string(nick_,NULL);
	set_string(server_,"irc.quakenet.org");
	set_string(port_,"6667");
	set_string(channel_,"#Scorched");
	bot_dispatcher_ = NULL;
}

IrcBot::~IrcBot()
{
	Stop();
}

void IrcBot::Start()
{
	if (bot_dispatcher_ != NULL)
		return;
	bot_dispatcher_ = SDL_CreateThread((int (*)(void *))
					   &bot_dispatcher, (void *) this);
}

void IrcBot::Stop()
{
	IrcDisconnect();
	if (bot_dispatcher_ != NULL)
		SDL_KillThread(bot_dispatcher_);
	bot_dispatcher_ = NULL;
}

int IrcBot::Connect()
{
	irc_name( nick_.c_str(), user_.c_str(), name_.c_str() );
	names_.clear();
	return IrcConnect( server_.c_str(), port_.c_str(), channel_.c_str() );
}

int IrcBot::bot_dispatch()
{
	if (!Connect()) {
		irc_trace("[IRC BOT] connect failed.\n");
		Stop();
	}

	for (;;) {
		int state_curr;
		int state_prev;
		state_curr = irc_state();
		switch (state_curr) {
		case TSK_DISCONNECTED :		
			irc_trace("[IRC BOT] disconnected.\n");
			set_string(nick_,NULL);
			if (!Connect()) 
				Stop();	
			break;
		case TSK_CONNECTED:
			if (state_prev != state_curr)
				irc_trace("[IRC BOT] %s ready.\n", nick);
			irc_check();
			break;
		default :
			if (state_prev != state_curr)
				irc_trace("[IRC BOT] %s awaiting connection\n",nick);
		}
		SDL_Delay(100);
		state_prev = state_curr;
	}
	return 0;
}

int IrcBot::isConnected()
{
	return (irc_state()==TSK_CONNECTED);
}

int IrcBot::bot_dispatcher(void *data)
{
	return ((IrcBot *) data)->bot_dispatch();
}

void IrcBot::set_string(std::string &dst, char *src)
{
	char buf[16];
	if (src == NULL) {
		unsigned int r = clock(); /* random */
		sprintf(buf, "\\%8.8X", r);
		buf[sizeof(buf) - 1] = 0;
		src = buf;
	}
	dst = std::string(src);
}


void IrcBot::update_names(char *n, bool add)
{
	std::list < std::string >::iterator itor =
	    std::find(names_.begin(), names_.end(), std::string(n));
	if (*n == 0)
		return;
	if (itor == names_.end()) {
		if (add)
			names_.push_back(std::string(n));
	} else {
		if (!add)
			names_.erase(itor);
	}
}

void IrcBot::irc_check()
{

}


void IrcBot::irc_reply()
{
	char *n, *w;

	if ((cmd == NULL) || (prefix == NULL))
		return;

	if (strcmp(cmd, "JOIN") == 0) {
		for (n = prefix; (*n != 0) && (*n != ' ') && (*n != '!');
		     n++);
		*n = 0;
		update_names(prefix, true);
		return;
	}
	if ( (strcmp(cmd, "QUIT") == 0) || (strcmp(cmd, "PART") == 0)) {
		for (n = prefix; (*n != 0) && (*n != ' ') && (*n != '!');
		     n++);
		*n = 0;
		update_names(prefix, false);
		return;
	}
	if (strcmp(cmd, C_RPL_NAMREPLY) == 0) {
		if (params != NULL) {
			if ((n=strchr(params,':'))==NULL)
				return;
			for (w = n+1; (n = strtok(w, " \t\n\r\b")) != NULL; w = NULL) {
				if ((*n == '@') || (*n == '+') || 
				    (*n =='*') || (*n == '=') )
					n++;
				update_names(n, true);
			}
		}
		return;
	}
}
