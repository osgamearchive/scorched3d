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

#include <coms/NetServerRead.h>
#include <coms/NetMessagePool.h>
#include <common/Clock.h>
#include <common/Logger.h>

NetServerRead::NetServerRead(TCPsocket socket,
							 NetServerProtocol *protocol,
							 NetMessageHandler *messageHandler,
							 bool *checkDeleted) : 
	socket_(socket), sockSet_(0), protocol_(protocol), 
	outgoingMessagesMutex_(0), checkDeleted_(checkDeleted),
	disconnect_(false), messageHandler_(messageHandler),
	thread_(0)
{
	sockSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(sockSet_, socket);
	outgoingMessagesMutex_ = SDL_CreateMutex();

	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::ConnectMessage, (unsigned int) socket);
	messageHandler_->addMessage(message);

	thread_ = SDL_CreateThread(NetServerRead::threadFunc, (void *) this);
	if (!thread_) disconnect_ = true;
}

NetServerRead::~NetServerRead()
{
	SDL_LockMutex(outgoingMessagesMutex_);
	while (!newMessages_.empty())
	{
		NetMessage *message = newMessages_.front();
		newMessages_.pop_front();
		NetMessagePool::instance()->addToPool(message);
	}
	SDL_UnlockMutex(outgoingMessagesMutex_);

	SDL_DestroyMutex(outgoingMessagesMutex_);
	outgoingMessagesMutex_ = 0;
	SDLNet_FreeSocketSet(sockSet_);
	sockSet_ = 0;
}

void NetServerRead::addMessage(NetMessage *message)
{
	SDL_LockMutex(outgoingMessagesMutex_);
	newMessages_.push_back(message);
	SDL_UnlockMutex(outgoingMessagesMutex_);
}

bool NetServerRead::getDisconnect()
{ 
	SDL_LockMutex(outgoingMessagesMutex_);
	bool result = disconnect_;	
	SDL_UnlockMutex(outgoingMessagesMutex_);

	if (result && thread_)
	{
		int status = 0;
		SDL_WaitThread(thread_, &status);
	}
	return result; 
}

int NetServerRead::threadFunc(void *netServerRead)
{
	NetServerRead *ns = (NetServerRead *) netServerRead;
	ns->actualThreadFunc();

	SDL_LockMutex(ns->outgoingMessagesMutex_);
	ns->disconnect_ = true;
	(*ns->checkDeleted_) = true;
	SDL_UnlockMutex(ns->outgoingMessagesMutex_);
	return 0;
}

void NetServerRead::actualThreadFunc()
{
	Clock netClock;
	for (;;)
	{
		netClock.getTimeDifference();

		if (!pollIncoming()) break;
		if (!pollOutgoing()) break;

		float timeDiff = netClock.getTimeDifference();
		if (timeDiff > 15.0f)
		{
			Logger::log(0, "Warning: Net loop took %.2f seconds, client %i", 
				timeDiff, (unsigned int) socket_);
		}

		SDL_Delay(10);
	}

	{
		NetMessage *message = NetMessagePool::instance()->
			getFromPool(NetMessage::DisconnectMessage, (unsigned int) socket_);
		messageHandler_->addMessage(message);
	}
	SDLNet_TCP_Close(socket_);
}

bool NetServerRead::pollIncoming()
{
	int numready = SDLNet_CheckSockets(sockSet_, 10);
	if (numready == -1) return false;
	if (numready == 0) return true;

	if(SDLNet_SocketReady(socket_))
	{
		NetMessage *message = protocol_->readBuffer(socket_);
		if (!message)
		{
			Logger::log(0, "Client socket has been closed.");
			return false;
		}
		else
		{
			// We have a buffer containing the message
			messageHandler_->addMessage(message);
		}
	}

	return true;
}

bool NetServerRead::pollOutgoing()
{
	std::list<NetMessage *> newMessages;
	SDL_LockMutex(outgoingMessagesMutex_);
	newMessages_.swap(newMessages);
	SDL_UnlockMutex(outgoingMessagesMutex_);

	bool result = true;
	while (!newMessages.empty())
	{
		NetMessage *message = newMessages.front();
		newMessages.pop_front();

		if (result)
		{
			if (message->getMessageType() == NetMessage::DisconnectMessage)
			{
				result = false;
			}
			else
			{
				if (!protocol_->sendBuffer(message->getBuffer(), socket_))
				{
					Logger::log(0, "Failed to send message to client");
					result = false;
				}
			}
		}

		NetMessagePool::instance()->addToPool(message);
	}

	return result;
}
