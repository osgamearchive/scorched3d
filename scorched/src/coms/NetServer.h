////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////


#ifndef __INCLUDE_netServer_h_INCLUDE__
#define __INCLUDE_netServer_h_INCLUDE__

#include <coms/NetMessageHandler.h>
#include <coms/NetServerProtocol.h>
#include <list>
#include <set>

class NetServer
{
public:
	NetServer(NetServerProtocol *protocol);
	virtual ~NetServer();

	bool connect(const char *hostName, int portNo);
	bool start(int portNo, int maxClients = -1);
	bool started();

	int getMaxClients() { return maxClients_; }
	int getNoClients();

	int processMessages() { return messageHandler_.processMessages(); }
	void setMessageHandler(NetMessageHandlerI *handler) 
		{ messageHandler_.setMessageHandler(handler); }

	void disconnectClient(TCPsocket &client);
	void sendMessage(NetBuffer &buffer);
	void sendMessage(NetBuffer &buffer,
					 TCPsocket destination);

protected:
	int maxClients_;
	NetServerProtocol *protocol_;
	TCPsocket server_;
	SDLNet_SocketSet sockSet_;
	std::set<TCPsocket> connections_;
	std::list<NetMessage *> outgoing_;
	SDL_mutex *outgoingMessagesMutex_;
	SDL_mutex *setMutex_;
	NetMessageHandler messageHandler_;

	static int threadFunc(void *);

	bool pollIncoming();
	void pollOutgoing();
	void addClient(TCPsocket &client);
	void rmClient(TCPsocket &client);
	void destroyClient(TCPsocket client);
	void updateSockSet();

private:

	NetServer(const NetServer &);
	const NetServer & operator=(const NetServer &);

};

#endif // __INCLUDE_netServer_h_INCLUDE__

