#ifndef _GAMEINFOMESSAGE_H_
#define _GAMEINFOMESSAGE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>
#include "irc/GameInfoEntry.h"
#include <string>

#define PROTOCOL		"Scorched3D"
#define GAME_CLIENT_BOT 	"Scorched3D Client Bot"
#define GAME_SERVER_BOT 	"Scorched3D Server Bot"
#define GAME_CLIENT_BOT_USER	"sc3dclnt"
#define GAME_SERVER_BOT_USER	"sc3dsrvr"
#define GAME_SERVER_PORT	 26555
#define GAME_CLIENT_INFO_PORT    26554
#define GAME_SERVER_INFO_PORT    26553

class GameInfoMessage {

      private:
	std::string getField(char *data,char *field);

      public:
	 GameInfoMessage();
	 ~GameInfoMessage();	
	int setQuery(char *query);
	int setQueryReply(char *queryreply,char *query,char *host,char *port);
	int getQueryReplyInfo(GameInfoEntry &info,char *data,char *host,char *port);
	
};

#endif				/* _GAMEINFOMESSAGE_H_ */
