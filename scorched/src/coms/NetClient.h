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


#ifndef __INCLUDE_netClient_h_INCLUDE__
#define __INCLUDE_netClient_h_INCLUDE__

#include <coms/NetMessage.h>
#include <list>

class NetClient
{
public:
	static NetClient* instance();

	bool connect(const char *hostName, int portNo);
	bool started();

	void sendMessage(NetBuffer &buffer);

protected:
	static NetClient* instance_;
	TCPsocket client_;
	SDLNet_SocketSet sockSet_;
	std::list<NetMessage *> outgoing_;
	SDL_mutex *outgoingMessagesMutex_;

	static int threadFunc(void *);
	void stop();
	bool pollIncoming();
	void pollOutgoing();

private:
	NetClient();
	virtual ~NetClient();

	NetClient(const NetClient &);
	const NetClient & operator=(const NetClient &);

};

#endif // __INCLUDE_netClient_h_INCLUDE__

