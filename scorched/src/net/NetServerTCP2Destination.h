////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_NetServerTCP2Destinationh_INCLUDE__)
#define __INCLUDE_NetServerTCP2Destinationh_INCLUDE__

#include <net/NetInterface.h>
#include <list>
#include <map>

class NetServerTCP2;
class NetServerTCP2Destination
{
public:
	NetServerTCP2Destination(NetServerTCP2 *server, TCPsocket socket, unsigned int destinationId);
	virtual ~NetServerTCP2Destination();

	void addMessage(NetMessage &message);
	void printStats(unsigned int destinationId);

	enum SocketResult
	{
		SocketActivity,
		SocketEmpty,
		SocketClosed,
		SocketTimeout
	};
	SocketResult checkSocket();

	unsigned int getIpAddress();
	static unsigned int getIpAddressFromSocket(TCPsocket socket);

protected:
	enum HeaderType
	{
		TypeNone = 0,
		TypeMessage = 1,
		TypeAck = 2,
		TypeLast = 4
	};

	struct MessagePart
	{
		int offset;
		int length;
		NetMessage *message;
	};
	struct MessageParts
	{
		bool last;
		NetMessage *message;
		std::list<MessagePart> parts;
	};

	bool packetLogging_;
	unsigned int destinationId_;
	NetServerTCP2 *server_;
	TCPsocket socket_;
	SDLNet_SocketSet socketSet_;
	NetMessage *currentMessage_, *currentMessagePart_;
	int currentMessageLen_;
	char currentMessageType_;
	std::map<NetMessage *, int> messageRefs_;
	std::list<MessageParts> waitingParts_;
	std::list<MessageParts> sendParts_;
	std::list<MessageParts> sendingParts_;
	unsigned int messagesSent_, messagesRecieved_;
	unsigned int partialSends_;
	unsigned int bytesIn_, bytesOut_;

	SocketResult checkIncoming();
	SocketResult checkOutgoing();

};

#endif // __INCLUDE_NetServerTCP2Destinationh_INCLUDE__
