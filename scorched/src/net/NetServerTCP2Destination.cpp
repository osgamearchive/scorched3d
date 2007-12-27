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

#include <net/NetServerTCP2.h>
#include <net/NetMessagePool.h>
#include <net/NetOptions.h>
#include <common/Logger.h>
#include <set>

NetServerTCP2Destination::NetServerTCP2Destination(NetServerTCP2 *server, 
	TCPsocket socket,
	unsigned int destinationId) :
	server_(server), socket_(socket), socketSet_(0), 
	currentMessage_(0), currentMessagePart_(0), currentMessageLen_(0),
	destinationId_(destinationId), 
	packetLogging_(false),
	messagesSent_(0), messagesRecieved_(0), 
	partialSends_(0), bytesIn_(0), bytesOut_(0)
{
	socketSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet_, socket_);

	packetLogging_ = NetOptions::instance()->getPacketLogging();
	currentMessagePart_ = NetMessagePool::instance()->
		getFromPool(NetMessage::BufferMessage, 0, 0);
}

NetServerTCP2Destination::~NetServerTCP2Destination()
{
	SDLNet_FreeSocketSet(socketSet_);
	socketSet_ = 0;

	if (socket_) SDLNet_TCP_Close(socket_);
	socket_ = 0;

	if (currentMessage_) NetMessagePool::instance()->addToPool(currentMessage_);
	currentMessage_ = 0;
	NetMessagePool::instance()->addToPool(currentMessagePart_);
	currentMessagePart_ = 0;

	// Return any NetMessages to the pool
	std::set<NetMessage *>::iterator netItor;
	std::set<NetMessage *> netMessages;
	std::list<MessageParts>::iterator partItor;
	for (partItor = waitingParts_.begin();
		partItor != waitingParts_.end();
		partItor++)
	{
		netMessages.insert(partItor->message);
	}
	for (partItor = sendParts_.begin();
		partItor != sendParts_.end();
		partItor++)
	{	
		netMessages.insert(partItor->message);
	}
	for (netItor = netMessages.begin();
		netItor != netMessages.end();
		netItor++)
	{
		NetMessagePool::instance()->addToPool(*netItor);
	}
}

unsigned int NetServerTCP2Destination::getIpAddress()
{
	return getIpAddressFromSocket(socket_);
}

unsigned int NetServerTCP2Destination::getIpAddressFromSocket(TCPsocket socket)
{
	unsigned int addr = 0;
	IPaddress *address = SDLNet_TCP_GetPeerAddress(socket);
	if (address)
	{
		addr = SDLNet_Read32(&address->host);
	}
	return addr;
}

void NetServerTCP2Destination::addMessage(NetMessage &oldmessage)
{
	// Add a whole new buffer to send to this destination

	// Get a new buffer from the pool
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::SentMessage, 
			oldmessage.getDestinationId(), getIpAddress());

	// Add message to new buffer
	NetBuffer &buffer = oldmessage.getBuffer();
	message->getBuffer().allocate(buffer.getBufferUsed());
	memcpy(message->getBuffer().getBuffer(), 
		buffer.getBuffer(), buffer.getBufferUsed());
	message->getBuffer().setBufferUsed(buffer.getBufferUsed());

	// Split this message into parts that will be sent
	const int SendLength = NetOptions::instance()->getPacketSize();

	// Check how many parts
	// Remainder is the amount to send in last part
	int parts = message->getBuffer().getBufferUsed() / SendLength;
	int remainder = message->getBuffer().getBufferUsed() % SendLength;
	if (remainder > 0) parts ++;
	else remainder = SendLength; 

	// Add each part into the list of parts pending to send to this client
	for (int p=0; p<parts; p++)
	{
		bool lastPart = (p == parts - 1);

		MessageParts messageParts;
		messageParts.last = lastPart;
		messageParts.message = message;

		int partLength = (lastPart?remainder:SendLength);
		{
			MessagePart headerPart;
			headerPart.length = 5;
			headerPart.offset = message->getBuffer().getBufferUsed();
			headerPart.message = message;

			char headerType = (lastPart?TypeLast:TypeNone) | TypeMessage;
			message->getBuffer().addToBuffer(headerType);
			message->getBuffer().addToBuffer(int(partLength + 5));

			messageParts.parts.push_back(headerPart);
		}
		{
			MessagePart part;
			part.length = partLength;
			part.offset = p * SendLength;
			part.message = message;
			
			messageParts.parts.push_back(part);
		}
		sendParts_.push_back(messageParts);
	}

	if (packetLogging_)
	{
		Logger::log(formatStringBuffer(
			"NetServerTCP2Destination: Adding a new message, resulting in %i parts", parts));
	}
}

NetServerTCP2Destination::SocketResult NetServerTCP2Destination::checkSocket()
{
	SocketResult incomingResult = checkIncoming();
	if (incomingResult == SocketClosed || incomingResult == SocketTimeout) return incomingResult;
	SocketResult outgoingResult = checkOutgoing();
	if (outgoingResult == SocketClosed || incomingResult == SocketTimeout) return outgoingResult;
	
	return ((incomingResult == SocketEmpty && 
		outgoingResult == SocketEmpty)?SocketEmpty:SocketActivity);
}

NetServerTCP2Destination::SocketResult NetServerTCP2Destination::checkIncoming()
{
	int i;
	for (i=0; i<2000; i++)
	{
		int numready = SDLNet_CheckSockets(socketSet_, 0);
		if (numready == -1) return SocketClosed;
		if (numready == 0) return (i==0?SocketEmpty:SocketActivity);

		char buffer[1];
		int recv = SDLNet_TCP_Recv(socket_, &buffer, 1);
		if (recv <= 0) return SocketClosed;

		NetInterface::getBytesIn()++;
		bytesIn_++;

		currentMessagePart_->getBuffer().addDataToBuffer(buffer, 1);
		if (currentMessagePart_->getBuffer().getBufferUsed() >= 5)
		{
			if (currentMessageLen_ == 0)
			{
				NetBufferReader reader(currentMessagePart_->getBuffer());
				reader.getFromBuffer(currentMessageType_);
				reader.getFromBuffer(currentMessageLen_);
			}

			// A full part has been recieved
			if (currentMessagePart_->getBuffer().getBufferUsed() == currentMessageLen_)
			{
				if (currentMessageType_ & TypeAck)
				{
					if (waitingParts_.empty()) 
					{
						Logger::log(formatStringBuffer("NetServerTCP2Destination: No waiting parts for Ack"));
						return SocketClosed; // Should Never Happen
					}

					// Remove one of the messages we are waiting on
					MessageParts waitingPart = waitingParts_.front();
					waitingParts_.pop_front();

					// If this is the last part in a message, notify that the message has been sent
					if (waitingPart.last) server_->incomingMessageHandler_.addMessage(waitingPart.message);

					if (packetLogging_)
					{
						Logger::log(formatStringBuffer(
							"NetServerTCP2Destination: Recieved a message part ack, %u parts waiting, %s", 
							waitingParts_.size(), (waitingPart.last?"last":"not last")));
					}
				}
				else if (currentMessageType_ & TypeMessage)
				{
					if (!currentMessage_)
					{
						currentMessage_ = NetMessagePool::instance()->
							getFromPool(NetMessage::BufferMessage, 
								destinationId_, getIpAddress());
					}

					// A full message part has been received
					currentMessage_->getBuffer().addDataToBuffer(
						currentMessagePart_->getBuffer().getBuffer() + 5,
						currentMessagePart_->getBuffer().getBufferUsed() - 5);
					if (currentMessageType_ & TypeLast)
					{
						// A finished message has been recieved
						messagesRecieved_++;
						server_->incomingMessageHandler_.addMessage(currentMessage_);
						currentMessage_ = 0;
					}

					// Acknowledge
					char ackType = TypeAck;
					int ackLen = 5;
					MessageParts messageParts;
					MessagePart messagePart;
					messagePart.length = 5;
					messagePart.offset = 0;
					messagePart.message = NetMessagePool::instance()->
						getFromPool(NetMessage::BufferMessage, 0, 0);
					messagePart.message->getBuffer().addToBuffer(ackType);
					messagePart.message->getBuffer().addToBuffer(ackLen);
					messageParts.parts.push_back(messagePart);
					messageParts.last = true;
					messageParts.message = messagePart.message;
					sendingParts_.push_back(messageParts);

					// Log
					if (packetLogging_)
					{
						Logger::log(formatStringBuffer(
							"NetServerTCP2Destination: Recieved a message part, %s", 
							(currentMessageType_ & TypeLast?"last":"not last")));
					}
				}
				else 
				{
					Logger::log(formatStringBuffer(
						"NetServerTCP2Destination: Unknown message type %i received", currentMessageType_));
					return SocketClosed;
				}

				currentMessageLen_ = 0;
				currentMessagePart_->getBuffer().reset();
			}
		}
	}	

	return (i==0?SocketEmpty:SocketActivity);
}

NetServerTCP2Destination::SocketResult NetServerTCP2Destination::checkOutgoing()
{
	// Figure out how many packets we can send
	const int SendParts = NetOptions::instance()->getOutstandingPackets();
	while (int(waitingParts_.size()) < SendParts && !sendParts_.empty())
	{
		MessageParts messageParts = sendParts_.front();
		sendParts_.pop_front();

		sendingParts_.push_back(messageParts);
		waitingParts_.push_back(messageParts);
	}

	// See if we have any packets to send
	bool sent = false;
	while (!sendingParts_.empty())
	{
		// Send the packets in each part
		MessageParts messageParts = sendingParts_.front();
		while (!messageParts.parts.empty())
		{
			MessagePart messagePart = messageParts.parts.front();
			messageParts.parts.pop_front();

			const char *buffer = messagePart.message->getBuffer().getBuffer();

			// Send buffer
			int result = SDLNet_TCP_Send(socket_, &buffer[messagePart.offset], messagePart.length);

			if(result == -1) // Socket Closed
			{
				Logger::log(formatStringBuffer("NetServerTCP2Destination: Failed to send buffer length. Socket closed"));
				return SocketClosed;
			}

			if (result == 0) return SocketEmpty; // Nothing sent
			NetInterface::getBytesOut() += result;
			bytesOut_ += result;
			
			sent = true;
			if (result < messagePart.length) // Partial send
			{
				partialSends_++;

				MessagePart newPart;
				newPart.length = messagePart.length - result;
				newPart.offset = messagePart.offset + result;
				newPart.message = messagePart.message;
				messageParts.parts.push_front(newPart);
			}

			if (packetLogging_)
			{
				Logger::log(formatStringBuffer(
					"NetServerTCP2Destination: Sent a message part, %s", 
					(messageParts.parts.empty()?"last":"not last")));
			}
		}

		// Finished send an entire part
		messagesSent_++;
		sendingParts_.pop_front();
	}	

	return (sent?SocketActivity:SocketEmpty);
}

void NetServerTCP2Destination::printStats(unsigned int destination)
{
	Logger::log(formatStringBuffer("TCP2 Destination %u", destination));
	Logger::log(formatStringBuffer("  %u messages sent, %u messages recieved",
		messagesSent_, messagesRecieved_));
	Logger::log(formatStringBuffer("  %u still sending, %u waiting for ack, %u pending send",
		sendingParts_.size(), waitingParts_.size(), sendParts_.size()));
	Logger::log(formatStringBuffer("  %u partial sends, %u bytes in, %u bytes out",
		partialSends_, bytesIn_, bytesOut_));
}
