#include "irc/IrcDefines.h"
#include "irc/GameInfoMessage.h"
#include "irc/ServerGameInfo.h"
#include "common/OptionsGame.h"
#include "coms/NetServer.h"
#include "server/ServerState.h"
#include <stdlib.h>
#include <string.h>

GameInfoMessage::GameInfoMessage() 
{
}

GameInfoMessage::~GameInfoMessage()
{
}


int  GameInfoMessage::setQuery(char *data)
{
	sprintf(data,PROTOCOL ": QUERY protocol=%s;",
		ScorchedProtocolVersion);
	return strlen(data);
}

int  GameInfoMessage::setQueryReply(char *reply,char *query,char *host,char *port)
{
	// veryfy query
	if (strstr(query,PROTOCOL ": QUERY protocol=") == NULL )
		return 0;

	// build reply
	sprintf(reply,"%s: QUERYREPLY protocol=%s; ip=%s; port=%d; clients=%d; maxclients=%d; description=%s;",
		PROTOCOL,
		ScorchedProtocolVersion,
		(host == NULL ? "unknown" : host  ),
		(port == NULL ? -1 : atoi(port) ),
		NetServer::instance()->getNoClients(),
		NetServer::instance()->getMaxClients(),
		OptionsGame::instance()->getServerName());

	return strlen(reply);
}

int GameInfoMessage::getQueryReplyInfo(GameInfoEntry &info, char *data,char *host,char *port)
{
	// verify queryreply
	if (strstr(data,PROTOCOL ": QUERYREPLY protocol=") == NULL )
		return 0;

	info = GameInfoEntry (
			getField(data,"description"),
			(host == NULL ? getField(data,"ip") : host ),
		        (port == NULL ? getField(data,"port") : port ),
			getField(data,"protocol"),
			getField(data,"clients"),
			getField(data,"maxclients"));
	return 1;
}

std::string GameInfoMessage::getField(char *data,char *field) 
{ 
	static char cmd[MAX_IRC_CMD+1];
	char *s,*d;
	if (data==NULL) 
		return "";
	strncpy(cmd,data,MAX_IRC_CMD);
	cmd[MAX_IRC_CMD-1]=0;
	s=strstr(cmd,field);
	if (s==NULL) 
		return "";
	s+=strlen(field);
	if (*s++ != '=')	
		return "";	
	d=strchr(s,';');
	if (d==NULL)
		return "";
	*d=0;
	return std::string(s);
}



