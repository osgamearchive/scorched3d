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

#if !defined(__INCLUDE_NetInterfaceh_INCLUDE__)
#define __INCLUDE_NetInterfaceh_INCLUDE__

#include <coms/NetMessageHandler.h>

class NetInterface
{
public:
	NetInterface();
	virtual ~NetInterface();

	virtual bool started() = 0;

	virtual int processMessages() = 0;
	virtual void setMessageHandler(NetMessageHandlerI *handler) = 0;

	virtual void disconnectAllClients() = 0;
	virtual void disconnectClient(unsigned int client) = 0;
	virtual void sendMessage(NetBuffer &buffer) = 0;
	virtual void sendMessage(NetBuffer &buffer, unsigned int destination) = 0;

	virtual unsigned int getIpAddress(unsigned int destination) = 0;

	static unsigned int &getBytesIn() { return bytesIn_; }
	static unsigned int &getBytesOut() { return bytesOut_; }
	static unsigned int &getPings() { return pings_; }
	static unsigned int &getConnects() { return connects_; }

protected:
	static unsigned int bytesIn_;
	static unsigned int bytesOut_;
	static unsigned int pings_;
	static unsigned int connects_;
};

#endif
