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


#include <coms/NetClient.h>
#include <coms/NetBufferUtil.h>
#include <coms/NetMessageHandler.h>
#include <coms/NetMessagePool.h>
#include <common/Logger.h>
#include <SDL/SDL_thread.h>

NetClient *NetClient::instance_ = 0;

NetClient *NetClient::instance()
{
	if (!instance_)
	{
	  instance_ = new NetClient;
	}

	return instance_;
}

NetClient::NetClient() : sockSet_(0), client_(0), outgoingMessagesMutex_(0)
{
	outgoingMessagesMutex_ = SDL_CreateMutex();
}

NetClient::~NetClient()
{
	SDL_DestroyMutex(outgoingMessagesMutex_);
}

bool NetClient::connect(const char *hostName, int port)
{
	if(SDLNet_Init()==-1)
	{
		return false;
	}

	if (sockSet_) SDLNet_FreeSocketSet(sockSet_);
	sockSet_ = SDLNet_AllocSocketSet(1);
	if (!sockSet_)
	{
		return false;
	}

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip,(char *) hostName,port)==-1)
	{
		return false;
	}

	client_=SDLNet_TCP_Open(&ip);
	if (!client_)
	{
		return false;
	}
	NetBufferUtil::setBlockingIO(client_);

	if (SDLNet_TCP_AddSocket(sockSet_,client_) == -1)
	{
		return false;
	}

	SDL_CreateThread(NetClient::threadFunc, (void *) 0);
	return true;
}

bool NetClient::started()
{
	return (client_ != 0);
}

void NetClient::stop()
{
	if (client_) SDLNet_TCP_Close(client_);
	client_ = 0;
	if (sockSet_) SDLNet_FreeSocketSet(sockSet_);
	SDLNet_Quit();
}

int NetClient::threadFunc(void *)
{
	while (true)
	{
		if (!instance_->pollIncoming()) break;
		instance_->pollOutgoing();
	}

	instance_->stop();
	return 0;
}

bool NetClient::pollIncoming()
{
	int numready = SDLNet_CheckSockets(sockSet_, 100);
	if (numready == -1) 
	{
		Logger::log(0, "Server socket has been closed");

		// Socket has been closed
		NetMessage *message = NetMessagePool::instance()->
			getFromPool(NetMessage::DisconnectMessage,
						   client_);
		NetMessageHandler::instance()->addMessage(message);	
		return false;
	}
	if (numready == 0) return true;

	if(SDLNet_SocketReady(client_))
	{
		NetMessage *message = NetBufferUtil::readBuffer(client_);
		if (!message)
		{
			Logger::log(0, "Read failed from server socket, has been closed?");

			// Socket has been closed
			NetMessage *message = NetMessagePool::instance()->
				getFromPool(NetMessage::DisconnectMessage,
							client_);
			NetMessageHandler::instance()->addMessage(message);
			return false;
		}
		else
		{
			// We have a buffer containing the message
			NetMessageHandler::instance()->addMessage(message);
		}
	}
	return true;
}

void NetClient::pollOutgoing()
{
	std::list<NetMessage *> newMessages;
	SDL_LockMutex(outgoingMessagesMutex_);
	outgoing_.swap(newMessages);
	SDL_UnlockMutex(outgoingMessagesMutex_);

	while (!newMessages.empty())
	{
		NetMessage *message = newMessages.front();
		newMessages.pop_front();

		NetBufferUtil::sendBuffer(
			message->getBuffer(), message->getPlayerId());

		NetMessagePool::instance()->addToPool(message);
	}
}

void NetClient::sendMessage(NetBuffer &buffer)
{
	if (!client_) return;

	// Get a new buffer from the pool
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::NoMessage, client_);

	// Add message to new buffer
	message->getBuffer().allocate(buffer.getBufferUsed());
	memcpy(message->getBuffer().getBuffer(), 
		buffer.getBuffer(), buffer.getBufferUsed());
	message->getBuffer().setBufferUsed(buffer.getBufferUsed());

	// Add the message to the list of out going
	SDL_LockMutex(outgoingMessagesMutex_);
	outgoing_.push_back(message);
	SDL_UnlockMutex(outgoingMessagesMutex_);
}
