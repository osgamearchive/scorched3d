#pragma once

#include <coms/NetServer.h>
#include <client/ServerBrowserServerList.h>

class ServerBrowserCollect : public NetMessageHandlerI
{
public:
	ServerBrowserCollect(ServerBrowserServerList &list);
	virtual ~ServerBrowserCollect();

	// Fetch the new list from the server
	bool fetchServerList();
	bool fetchLANList();

	// Inherited from NetMessageHandlerI
	virtual void processMessage(NetMessage &message);

protected:
	ServerBrowserServerList &list_;
	bool complete_;
	UDPpacket *sendPacket_;
	UDPpacket *recvPacket_;
	NetServer netServer_;
	NetBuffer sendNetBuffer_;
};
