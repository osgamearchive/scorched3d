#include "irc/GameInfoEntry.h"
#include "common/OptionsGame.h"
#include "server/ServerState.h"

GameInfoEntry::GameInfoEntry()
{
}

GameInfoEntry::GameInfoEntry(std::string _name,
		std::string _host,std::string _port,std::string _proto, 
		std::string _clients,std::string _maxclients)
{
	name_	      = _name;
	address_host_ = _host;
	address_port_ = _port;
	no_clients_   = _clients; 
	max_clients_  = _maxclients ;
	prot_version_ = _proto;	
}


GameInfoEntry::~GameInfoEntry()
{

}


