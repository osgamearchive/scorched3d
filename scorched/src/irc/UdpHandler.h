#ifndef _UDPHANDLER_H_
#define _UDPHANDLER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>
#include "irc/GameInfoEntry.h"


class UdpHandler {

      private:
      	UDPsocket 	udpsock;
	SDL_Thread * 	udp_query_dispatcher_;
	static int 	udp_query_dispatcher(void *data);
      	Uint16 		port; 
	bool		trace_enable;      	                 
	char *		getField(void *data,char *field);
	void 		udp_trace(const char *fmt, ...);
	
       protected:

        virtual int udp_query_dispatch();

       public:
	 UdpHandler();
	 ~UdpHandler();

	UDPpacket *	Alloc();
	void		Free(UDPpacket *);
	int 		SetPort(Uint16 _port);
	int 		Receive(UDPpacket *);
	int 		Send(UDPpacket *);
	int		Reply(UDPpacket *);
	void 		SetDest(UDPpacket *packet,char *host,Uint16 port);
	void Stop();
	void Start();


};

#endif				/* _UDPHANDLER_H_ */
