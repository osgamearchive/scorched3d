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


#if !defined(__INCLUDE_NetMessageh_INCLUDE__)
#define __INCLUDE_NetMessageh_INCLUDE__

#include <coms/NetBuffer.h>

class NetMessagePool;
class NetMessage
{
public:
	friend class NetMessagePool;

	enum MessageType
	{
		NoMessage,
		ConnectMessage,
		DisconnectMessage,
		BufferMessage,
		SentMessage
	};

	unsigned int &getDestinationId() { return destinationId_; }
	unsigned int &getIpAddress() { return ipAddress_; }
	MessageType getMessageType() { return type_; }
	NetBuffer &getBuffer() { return messageBuffer_; }
	void setType(MessageType type) { type_ = type; }

protected:
	NetBuffer messageBuffer_;
	MessageType type_;
	unsigned int destinationId_;
	unsigned int ipAddress_;

	void setDestinationId(unsigned int destinationId) { destinationId_ = destinationId; }
	void setIpAddress(unsigned int ipAddress) { ipAddress_ = ipAddress; }

	NetMessage(MessageType type = NetMessage::NoMessage,
			   unsigned int destinationId = 0,
			   unsigned int ipAddress = 0);
	virtual ~NetMessage();
};

#endif // __INCLUDE_NetMessageh_INCLUDE__
