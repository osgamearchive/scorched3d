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

#include <coms/NetServerProtocol.h>
#include <coms/NetMessagePool.h>
#include <coms/NetInterface.h>
#include <common/Logger.h>

NetServerProtocol::NetServerProtocol()
{
}

NetServerProtocol::~NetServerProtocol()
{
}

NetServerScorchedProtocol::NetServerScorchedProtocol()
{
}

NetServerScorchedProtocol::~NetServerScorchedProtocol()
{
}

bool NetServerScorchedProtocol::sendBuffer(NetBuffer &buffer, TCPsocket socket)
{
	Uint32 len = buffer.getBufferUsed();
	Uint32 netlen=0;
	SDLNet_Write32(len, &netlen);

	// send the length of the string
	int result = SDLNet_TCP_Send(socket, &netlen, sizeof(netlen));
	if(result<(int) sizeof(netlen))
	{
		Logger::log(0, "Failed to send buffer length. Sent %i of %i.",
			result, sizeof(netlen));
		return false;
	}
	
	// send the buffer
	result = SDLNet_TCP_Send(socket,buffer.getBuffer(),len);
	if(result<int(len))
	{
		Logger::log(0, "Failed to send buffer. Sent %i of %i.",
			result, int(len));
		return false;
	}
	NetInterface::getBytesOut() += len;
	
	// return the length sent
	return true;
}

NetMessage *NetServerScorchedProtocol::readBuffer(TCPsocket socket)
{
	// receive the length of the string message
	Uint32 netlen;
	int result=SDLNet_TCP_Recv(socket,&netlen,sizeof(netlen));
	if (result <= 0) return 0;

	if(result<(int) sizeof(netlen))
	{
		Logger::log(0, "Failed to read buffer length. Read %i of %i.",
			result, sizeof(netlen));
		return 0;
	}
	
	// swap byte order to our local order
	Uint32 len=SDLNet_Read32(&netlen);
	
	// check if anything is strange, like a zero length buffer
	if(!len)
	{
		Logger::log(0, "Zero length buffer recieved.");
		return 0;
	}

	// Cannot recieve a message large than .5 MB
	if (len > 512000)
	{
		Logger::log(0, "Buffer was too large to recieve.  Size %i.",
			len);
		return 0;
	}

	// allocate the buffer memory
	NetMessage *buffer = NetMessagePool::instance()->
		getFromPool(NetMessage::BufferMessage, (unsigned int) socket);
	buffer->getBuffer().allocate(len);
	buffer->getBuffer().setBufferUsed(len);

	// get the string buffer over the socket
	result = 0;
	while (len>0)
	{
		int recv = SDLNet_TCP_Recv(socket,
			&buffer->getBuffer().getBuffer()[result], len);
		if (recv <= 0) 
		{
			Logger::log(0, "Read failed for buffer chunk");
			NetMessagePool::instance()->addToPool(buffer);
			return 0;
		}

		result += recv;
		len -= recv;
	}
	NetInterface::getBytesIn() += result;

	// return the new buffer
	return buffer;
}

NetServerHTTPProtocol::NetServerHTTPProtocol()
{
}

NetServerHTTPProtocol::~NetServerHTTPProtocol()
{
}

bool NetServerHTTPProtocol::sendBuffer(NetBuffer &buffer, TCPsocket socket)
{
	Uint32 len = buffer.getBufferUsed();
	
	// send the buffer
	int result = SDLNet_TCP_Send(socket,buffer.getBuffer(),len);
	if(result<int(len))
	{
		Logger::log(0, "Failed to send HTTP buffer. Sent %i of %i.",
			result, int(len));
		return false;
	}
	NetInterface::getBytesOut() += len;
	
	// return the length sent
	return true;
}

NetMessage *NetServerHTTPProtocol::readBuffer(TCPsocket socket)
{
	// allocate the buffer memory
	NetMessage *netBuffer = NetMessagePool::instance()->
		getFromPool(NetMessage::BufferMessage, (unsigned int) socket);
	netBuffer->getBuffer().reset();

	// get the string buffer over the socket
	Uint32 len = 0;
	char buffer[1];
	for (;;)
	{
		int recv = SDLNet_TCP_Recv(socket, buffer, 1);
		if (recv <= 0) 
		{
			// For HTTP the socket being closed signifies the end
			// of the transmission and is probably not an error!
			if (len == 0)
			{
				// If the len is zero then we have been disconnected
				NetMessagePool::instance()->addToPool(netBuffer);
				return 0;
			}
			else break;
		}

		netBuffer->getBuffer().addDataToBuffer(buffer, 1);
		len += 1;
	}
	NetInterface::getBytesIn() += len;

	// return the new buffer
	return netBuffer;
}
