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
#include <coms/NetServer.h>
#include <common/Logger.h>
#include <zlib/zlib.h>

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

static bool realSDLNet_TCP_Recv(TCPsocket socket, char *dest, int len)
{
	int result = 0;
	while (len > 0)
	{
		int recv = SDLNet_TCP_Recv(socket, &dest[result], len);
		if (recv <= 0) 
		{
			Logger::log(0, "Read failed for buffer chunk");
			return false;
		}

		result += recv;
		len -= recv;

		if (len > 0)
		{
			//Logger::log(0, "Partial read, %i/%i %s", recv, len + recv, 
				//SDLNet_GetError());
			SDL_Delay(10);
		}
	}	

	return true;
}

NetMessage *NetServerScorchedProtocol::readBuffer(TCPsocket socket)
{
	// receive the length of the string message
	char lenbuf[4];
	if (!realSDLNet_TCP_Recv(socket, lenbuf, 4))
	{
		Logger::log(0, "Failed to read buffer length.");
		return 0;
	}

	// swap byte order to our local order
	Uint32 len = SDLNet_Read32(lenbuf);
	
	// check if anything is strange, like a zero length buffer
	if(len == 0)
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
		getFromPool(NetMessage::BufferMessage, 
				(unsigned int) socket,
				NetServer::getIpAddress(socket));
	buffer->getBuffer().allocate(len);
	buffer->getBuffer().setBufferUsed(len);

	// get the string buffer over the socket
	if (!realSDLNet_TCP_Recv(socket, 
		buffer->getBuffer().getBuffer(),
		len))
	{
		Logger::log(0, "Read failed for buffer");
		NetMessagePool::instance()->addToPool(buffer);
		return 0;
	}
	NetInterface::getBytesIn() += len;

	// return the new buffer
	return buffer;
}

NetServerCompressedProtocol::NetServerCompressedProtocol()
{
}

NetServerCompressedProtocol::~NetServerCompressedProtocol()
{
}

bool NetServerCompressedProtocol::sendBuffer(NetBuffer &buffer, TCPsocket socket)
{
	unsigned long destLen = buffer.getBufferUsed() * 2;
	unsigned long srcLen = buffer.getBufferUsed();

	// Allocate a new buffer
	NetMessage *newMessage = NetMessagePool::instance()->
		getFromPool(NetMessage::BufferMessage, 
				(unsigned int) socket,
				NetServer::getIpAddress(socket));
	NetBuffer &newBuffer = newMessage->getBuffer();
	newBuffer.allocate(destLen);

	// Compress the message into the new buffer
	newBuffer.reset();
	newBuffer.addToBuffer((unsigned int) srcLen); // First 4 bytes are for uncompressed size
	bool result = 
		(compress2((unsigned char *) (newBuffer.getBuffer() + 4), &destLen, 
				   (unsigned char *) buffer.getBuffer(), srcLen, 
				   6) == Z_OK);
	
	// Send the new buffer instead
	bool retVal = false;
	if (result)
	{
		// Set the correct lengths
		newBuffer.setBufferUsed(destLen + 4);

		// Actualy send the new message
		//Logger::log(0, "Compressed %i->%i",
		//			buffer.getBufferUsed(), newBuffer.getBufferUsed());

		retVal = NetServerScorchedProtocol::sendBuffer(newBuffer, socket);
	}
	else
	{
		Logger::log(0, "Failed to compress coms buffer");
	}

	// Re-cycle the new message buffer
	NetMessagePool::instance()->addToPool(newMessage);
	return retVal;
}

NetMessage *NetServerCompressedProtocol::readBuffer(TCPsocket socket)
{
	// Read the message from the socket
	NetMessage *message = NetServerScorchedProtocol::readBuffer(socket);
	if (message)
	{
		// Get the uncompressed size from the buffer
		NetBufferReader reader(message->getBuffer());
		unsigned int dLen = 0;
		if (!reader.getFromBuffer(dLen))
		{
			// Return the error to the caller
			Logger::log(0, "Failed to get compressed coms buffer size");
			NetMessagePool::instance()->addToPool(message);	
			return 0;
		}

		// Check for silly sized buffer
		if (dLen > 512000)
		{
			Logger::log(0, "Compressed coms buffer was too large to recieve. Size = %i.",
				dLen);
			NetMessagePool::instance()->addToPool(message);	
			return 0;
		}

		// Create a new buffer for the uncompressed data
		unsigned long srcLen = message->getBuffer().getBufferUsed() - 4;
		unsigned long destLen = dLen;
		NetMessage *newMessage =
			NetMessagePool::instance()->getFromPool(message->getMessageType(),
													message->getDestinationId(),
													NetServer::getIpAddress(socket));
		NetBuffer &newBuffer = newMessage->getBuffer();
		newBuffer.allocate(destLen);
		newBuffer.setBufferUsed(destLen);

		// Uncompress the data
		unsigned uncompressResult = 
			uncompress((unsigned char *) newBuffer.getBuffer(), &destLen, 
					   (unsigned char *) (message->getBuffer().getBuffer() + 4), srcLen);
		if (Z_OK == uncompressResult)
		{
			// Add the new data back into the original message
			message->getBuffer().reset();
			message->getBuffer().addDataToBuffer(newBuffer.getBuffer(), destLen);
		}
		else
		{
			Logger::log(0, "Failed to uncompress coms buffer");

			// Return the error to the caller
			NetMessagePool::instance()->addToPool(message);	
			NetMessagePool::instance()->addToPool(newMessage);		
			return 0;
		}
		
		// Re-cycle the new message buffer
		NetMessagePool::instance()->addToPool(newMessage);		
	}
	return message;
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
		getFromPool(NetMessage::BufferMessage,
				(unsigned int) socket,
				NetServer::getIpAddress(socket));
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
