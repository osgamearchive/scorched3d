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


#if !defined(__INCLUDE_ComsMessageHandlerh_INCLUDE__)
#define __INCLUDE_ComsMessageHandlerh_INCLUDE__

#include <coms/NetMessageHandler.h>
#include <map>

class ComsMessageConnectionHandlerI
{
public:
	virtual ~ComsMessageConnectionHandlerI();

	virtual void clientConnected(NetMessage &message) = 0;
	virtual void clientDisconnected(NetMessage &message) = 0;
	virtual void clientError(NetMessage &message,
		const char *errorString) = 0;
};

class ComsMessageHandlerI
{
public:
	virtual ~ComsMessageHandlerI();

	virtual bool processMessage(NetPlayerID &id,
		const char *message,
		NetBufferReader &reader) = 0;
};

class ComsMessageHandler : public NetMessageHandlerI
{
public:
	static ComsMessageHandler *instance();

	// Used to add a handler for a specific message type
	void setConnectionHandler(
		ComsMessageConnectionHandlerI *handler);
	void addHandler(const char *messageType,
		ComsMessageHandlerI *handler);

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

	bool &getMessageLogging() { return comsMessageLogging_; }

protected:
	static ComsMessageHandler *instance_;
	std::map<std::string, ComsMessageHandlerI *> handlerMap_;
	ComsMessageConnectionHandlerI *connectionHandler_;

	bool comsMessageLogging_;
	void processReceiveMessage(NetMessage &message);

private:
	ComsMessageHandler();
	virtual ~ComsMessageHandler();
};


#endif
