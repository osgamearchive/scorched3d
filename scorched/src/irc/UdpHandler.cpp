#include "irc/UdpHandler.h"

UdpHandler::UdpHandler() 
{
	trace_enable = false;
	port = 0xffff;
	udpsock = NULL;
	udp_query_dispatcher_ = NULL;
}

UdpHandler::~UdpHandler()
{
	Stop();
	if (udpsock!=NULL)
		SDLNet_UDP_Close(udpsock);
}

int  UdpHandler::SetPort(Uint16 _port)
{
	if (port!=_port) {
		SDLNet_UDP_Close(udpsock);
		udpsock=NULL;
		port = _port;
	}

	if (udpsock==NULL)  {
		udpsock = SDLNet_UDP_Open(port); 
		if (udpsock == NULL) {
			fprintf(stderr, "failed: SDLNet_UDP_Open: %s\n\n",SDLNet_GetError());
			return 0;
		}
	}
	return 1;
}


UDPpacket *UdpHandler::Alloc()
{
	UDPpacket *packet;
	packet = SDLNet_AllocPacket(0xffff);
	if (packet == NULL) {
       	        fprintf(stderr, "failed SDLNet_AllocPacket: %s\n\n",SDLNet_GetError());
        }
	return packet;
}

void UdpHandler::Free(UDPpacket *packet)
{
	if (packet !=NULL)
		SDLNet_FreePacket(packet);

}

void UdpHandler::udp_trace(const char *fmt, ...)
{
        va_list ap;
        if (trace_enable) {
                va_start(ap, fmt);
                vfprintf(stderr, fmt, ap);
                va_end(ap);
        }
}


int  UdpHandler::Send(UDPpacket *packet)
{
	int bytes;
	if ((packet==NULL)||(udpsock==NULL))
		return -1;
	udp_trace("[UDP SEND] %s\n",packet->data);
	bytes=SDLNet_UDP_Send(udpsock,-1, packet);
	if (bytes <=0 ) {
       	        fprintf(stderr, "failed SDLNet_UDP_Send: %s\n\n",SDLNet_GetError());
        }
	return bytes;
}



int UdpHandler::Receive(UDPpacket *packet)
{
	if ((packet==NULL)||(udpsock==NULL))
                return -1;
	while (SDLNet_UDP_Recv(udpsock,packet) <= 0 ) {
		SDL_Delay(100);
        }
	udp_trace("[UDP RECV] %s\n",packet->data);
	return packet->len;
}

void UdpHandler::Stop()
{
	if (udp_query_dispatcher_ != NULL)
		SDL_KillThread(udp_query_dispatcher_);
	udp_query_dispatcher_ = NULL;
}

void UdpHandler::Start()
{
	if (udp_query_dispatcher_ != NULL)
		return;
	udp_query_dispatcher_ = SDL_CreateThread((int (*)(void *))
						 &udp_query_dispatcher,
						 (void *) this);
}

int UdpHandler::udp_query_dispatcher(void *_data)
{
	((UdpHandler *)_data)->udp_query_dispatch();
}

int UdpHandler::udp_query_dispatch()
{
	// dummy implementation 
	for (;;) {
		SDL_Delay(1000);
	}
	return 0;
}


void UdpHandler::SetDest(UDPpacket *packet,char *host,Uint16 _port)
{
	if (SDLNet_ResolveHost(&(packet->address),host,_port) == -1)  {
       	        fprintf(stderr, "failed SDLNet_ResolveHost: %s\n\n",SDLNet_GetError());
	}
}
