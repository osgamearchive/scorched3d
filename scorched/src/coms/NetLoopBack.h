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

#if !defined(__INCLUDE_NetLoopBackh_INCLUDE__)
#define __INCLUDE_NetLoopBackh_INCLUDE__

#include <coms/NetInterface.h>
#include <coms/NetMessageHandler.h>

class NetLoopBack : public NetInterface
{
public:
	enum LoopBackIDS
	{
		ClientLoopBackID = 100001,
		ServerLoopBackID = 200002
	};

	NetLoopBack(unsigned int currentId);
	virtual ~NetLoopBack();

	void setLoopBack(NetLoopBack *loopback);

	virtual bool started();

	virtual int processMessages();
	virtual void setMessageHandler(NetMessageHandlerI *handler);

	virtual void disconnectAllClients();
	virtual void disconnectClient(unsigned int client, bool delay = false);
	virtual void sendMessage(NetBuffer &buffer);
	virtual void sendMessage(NetBuffer &buffer, unsigned int destination);

protected:
	unsigned int currentId_;
	NetMessageHandler messageHandler_;
	NetLoopBack *loopback_;
};

#endif
