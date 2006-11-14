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

#include <coms/NetServerUDP.h>
#include <coms/NetMessagePool.h>
#include <common/Logger.h>

NetServerUDPDestination::NetServerUDPDestination(NetServerUDP *server, IPaddress &address) :
	server_(server),
	sendSeq_(1), recvSeq_(1), recvMessage_(0),
	packetLogging_(false)
{
	memcpy(&address_, &address, sizeof(address));
}

NetServerUDPDestination::~NetServerUDPDestination()
{
	// Tidy the currently recieving message
	if (recvMessage_) NetMessagePool::instance()->addToPool(recvMessage_);
	recvMessage_ = 0;

	// Tidy the current list of outgoing messages
	while (!outgoingMessages_.empty())
	{
		OutgoingMessage *message = outgoingMessages_.front();
		outgoingMessages_.pop_front();
		NetMessagePool::instance()->addToPool(message->message_);
		delete message;
	}

	// Tidy the current list of incoming messages
	std::map<unsigned int, NetMessage *>::iterator itor;
	for (itor = incomingMessages_.begin();
		itor != incomingMessages_.end();
		itor++)
	{
		NetMessagePool::instance()->addToPool((*itor).second);
	}
}

void NetServerUDPDestination::processData(unsigned int destinationId, int len, unsigned char *data, bool fin)
{
	if (len<6)
	{
		Logger::log(formatString("NetServerUDP: Small packet encountered, size %i", len));
		return;
	}

	data++; len--;// Remove type
	Uint32 seqValue = 0;
	memcpy(&seqValue, data, sizeof(seqValue));
	unsigned int seq = SDLNet_Read32(&seqValue);
    data+=4; len-=4;// Remove seq

	if (packetLogging_)
	{
		Logger::log(formatString("Recieving part %u%s - %i bytes", seq, (fin?"*":" "), len));
	}

	if (seq == recvSeq_)
	{
		// We've recieved the next message in the sequence
		// add it to the back of the list
		addData(destinationId, len, data, fin);
		recvSeq_++;

		// Check if we already have the next in the sequence
		while (incomingMessages_.find(recvSeq_) != incomingMessages_.end())
		{
			NetMessage *message = incomingMessages_[recvSeq_];
			incomingMessages_.erase(recvSeq_);

			addData(destinationId, 
				(int) message->getBuffer().getBufferUsed(), 
				(unsigned char *) message->getBuffer().getBuffer(), 
				(message->getMessageType() == NetMessage::DisconnectMessage));

			NetMessagePool::instance()->addToPool(message);

			recvSeq_++;
		}
	}
	else if (seq > recvSeq_) 
	{
		// We've recieved a message later on in the sequence
		// Check we have not already got this message
		if (incomingMessages_.find(seq) == incomingMessages_.end())
		{
			// Add the data
			NetMessage *message = NetMessagePool::instance()->
				getFromPool((fin?NetMessage::DisconnectMessage:NetMessage::BufferMessage),
					0, 0);
			recvMessage_->getBuffer().addDataToBuffer(data, len);
			incomingMessages_[seq] = message;
		}
	}

	// Send an ack
	if (packetLogging_)
	{
		Logger::log(formatString("Sending ack %u", seq));
	}
	server_->packetVOut_[0]->len = 5;
	server_->packetVOut_[0]->address.host = address_.host;
	server_->packetVOut_[0]->address.port = address_.port;
	server_->packetVOut_[0]->data[0] = server_->eDataAck;
	server_->packetVOut_[0]->channel = -1;
	SDLNet_Write32(seq, &server_->packetVOut_[0]->data[1]);
	if (SDLNet_UDP_SendV(server_->udpsock_, server_->packetVOut_, 1) == 0)
	{
		Logger::log(formatString("NetServerUDP: Failed to send ack packet"));
	}
}

void NetServerUDPDestination::addData(unsigned int destinationId, int len, unsigned char *data, bool fin)
{
	// If this is a new message, create it
	if (!recvMessage_)
	{
		recvMessage_ = NetMessagePool::instance()->
			getFromPool(NetMessage::BufferMessage,
				destinationId, server_->getIpAddress(destinationId));
	}

	// Add the data
	recvMessage_->getBuffer().addDataToBuffer(data, len);

	// If this is a finished message, complete it
	if (fin)
	{
		server_->incomingMessageHandler_.addMessage(recvMessage_);
		recvMessage_ = 0;
	}
}

void NetServerUDPDestination::processDataAck(unsigned int destinationId, int len, unsigned char *data)
{
	if (outgoingMessages_.empty()) return;

	// Get the sequence number
	data++; len--;// Remove type
	Uint32 seqValue = 0;
	memcpy(&seqValue, data, sizeof(seqValue));
	unsigned int seq = SDLNet_Read32(&seqValue);

	if (packetLogging_)
	{
		Logger::log(formatString("Recieving ack %u", seq));
	}

	// Remove this packet from the list of sent packets
	OutgoingMessage *message = outgoingMessages_.front();
	message->sentParts_.erase(seq);

	// Check if we have finished sending this message
	if (message->pendingParts_.empty() &&
		message->sentParts_.empty())
	{
		// Inform the client that this message has been fully sent
		server_->incomingMessageHandler_.addMessage(message->message_);
		delete message;
		outgoingMessages_.pop_front();
	}
}

bool NetServerUDPDestination::checkOutgoing()
{
	// Check we have stuff to send
	if (outgoingMessages_.empty()) return false;

	// Send more parts (if any)
	int sentParts = 0;
	OutgoingMessage *message = outgoingMessages_.front();
	while (!message->pendingParts_.empty() &&
		message->sentParts_.size() < 10)
	{
		sentParts ++;

		// Get the next part to send
		MessagePart part = message->pendingParts_.front();

		// Send this part
		if (sendPart(part, *message->message_))
		{
			message->pendingParts_.pop_front();
			message->sentParts_[part.seq] = part;
		}
	}

	// Check for any out of date parts
	if (!message->sentParts_.empty())
	{
		time_t theTime = time(0);
		std::map<unsigned int, MessagePart>::iterator itor;
		for (itor = message->sentParts_.begin();
			itor != message->sentParts_.end();
			itor++)
		{
			MessagePart &part = (*itor).second;
			const int PartTimeout = 2;
			if (theTime - part.sendtime > PartTimeout)
			{
				sendPart(part, *message->message_);
			}
			else break;
		}
	}

	return (sentParts > 0);
}

void NetServerUDPDestination::addMessage(NetMessage &oldmessage)
{
	// Get a new buffer from the pool
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::SentMessage, 
			oldmessage.getDestinationId(), 0);

	// Add message to new buffer
	NetBuffer &buffer = oldmessage.getBuffer();
	message->getBuffer().allocate(buffer.getBufferUsed());
	memcpy(message->getBuffer().getBuffer(), 
		buffer.getBuffer(), buffer.getBufferUsed());
	message->getBuffer().setBufferUsed(buffer.getBufferUsed());

	// Split this message into parts that will be sent
	const int SendLength = 800;
	OutgoingMessage *outgoingMessage = new OutgoingMessage();
	outgoingMessage->message_ = message;

	// Check how many parts
	// Remainder is the amount to send in last part
	int parts = message->getBuffer().getBufferUsed() / SendLength;
	int remainder = message->getBuffer().getBufferUsed() % SendLength;
	if (remainder > 0) parts ++;
	else remainder = SendLength; 

	// Record each part
	for (int p=0; p<parts; p++)
	{
		MessagePart part;
		part.end = (p == parts - 1);
		part.length = ((p == parts - 1)?remainder:SendLength);
		part.offset = p * SendLength;
		part.seq = sendSeq_++;
		outgoingMessage->pendingParts_.push_back(part);
	}
	outgoingMessages_.push_back(outgoingMessage);
}

bool NetServerUDPDestination::sendPart(MessagePart &part, NetMessage &message)
{
	// Update the last time this part was sent
	part.sendtime = time(0);

	// Send this part
	server_->packetVOut_[0]->len = 5 + part.length;
	server_->packetVOut_[0]->address.host = address_.host;
	server_->packetVOut_[0]->address.port = address_.port;
	server_->packetVOut_[0]->channel = -1;
	server_->packetVOut_[0]->data[0] = (part.end?server_->eDataFin:server_->eData);
	SDLNet_Write32(part.seq, &server_->packetVOut_[0]->data[1]);
	memcpy(&server_->packetVOut_[0]->data[5], &message.getBuffer().getBuffer()[part.offset], part.length);
	if (SDLNet_UDP_SendV(server_->udpsock_, server_->packetVOut_, 1) == 0)
	{
		Logger::log(formatString("NetServerUDP: Failed to send part packet"));
		return false;
	}

	if (packetLogging_)
	{
		Logger::log(formatString("Sending part %u%s - %i bytes, %i offset", 
			part.seq, (part.end?"*":" "), part.length, part.offset));
	}
	
	return true;
}
