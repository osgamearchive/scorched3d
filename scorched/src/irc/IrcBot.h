#ifndef _IRC_BOT_H_
#define _IRC_BOT_H_

#include "irc/IrcConnection.h"
#include <string>
#include <list>

class IrcBot: public IrcConnection {

      public:
	IrcBot(const char *_user,const char *_name);
	~IrcBot();
	void Start();
	void Stop();
	int Connect();
	int isConnected();
	
      protected: 
	std::string nick_;
	std::string user_;
	std::string name_;
	std::string server_;
	std::string port_;
	std::string channel_;	
	std::list <std::string> names_;

	virtual void irc_reply();
	virtual void irc_check();
	
      private:
	SDL_Thread * 	bot_dispatcher_;
	static int 	bot_dispatcher(void *unused);
	int 		bot_dispatch();

	void 	update_names(char *_name, bool _add);
	void 	set_string(std::string &dst, char *src);
	
};

#endif	/* _IRC_BOT_H_ */
