#ifndef _GAMEINFOENTRY_H_
#define _GAMEINFOENTRY_H_

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <string>

class GameInfoEntry {

      private:
	std::string	name_;
	std::string 	address_host_;
	std::string	address_port_;
	std::string     no_clients_;
	std::string     max_clients_;
	std::string     prot_version_;
	std::string 	ping_;

      public:
      	 GameInfoEntry();
	 GameInfoEntry(
	 	std::string _name,
	 	std::string _address,std::string _port,
	 	std::string _proto,
	 	std::string _noClients, std::string _maxClients);
	 ~GameInfoEntry();

	const char *getDescription() { return name_.c_str(); };
	const char *getHost()        { return address_host_.c_str(); };
	const char *getProtocol()    { return prot_version_.c_str(); };
	const char *getPort()        { return address_port_.c_str(); };
	const char *getNoClients()   { return no_clients_.c_str(); };
	const char *getMaxClients()  { return max_clients_.c_str(); };	
	const char *getAddress()     { return string(address_host_+":"+address_port_).c_str(); };
};

#endif				/* _GAMEINFOENTRY_H_ */
