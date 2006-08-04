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

#include <coms/NetServer.h>
#include <coms/NetBufferUtil.h>
#include <coms/NetMessageHandler.h>
#include <coms/NetMessagePool.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/Clock.h>

NetServer::NetServer(NetServerProtocol *protocol) : 
	sockSet_(0), firstDestination_(0),
	server_(0), protocol_(protocol), checkDeleted_(false),
	lastId_(0)
{
	sockSet_ = SDLNet_AllocSocketSet(1);
	setMutex_ = SDL_CreateMutex();
	SDL_CreateThread(NetServer::threadFunc, (void *) this);
}

NetServer::~NetServer()
{
	SDL_DestroyMutex(setMutex_); 
	setMutex_ = 0;
	SDLNet_FreeSocketSet(sockSet_);
	sockSet_ = 0;
}

void NetServer::setMessageHandler(NetMessageHandlerI *handler) 
{ 
	messageHandler_.setMessageHandler(handler); 
}

int NetServer::processMessages()
{ 
	float timeDifference = delayedClock_.getTimeDifference();
	if (!delayedMessages_.empty())
	{
		std::list<std::pair<float, NetMessage *> >::iterator itor;
		for (itor = delayedMessages_.begin();
				itor != delayedMessages_.end();
				itor++)
		{
			(*itor).first -= timeDifference;
		}
		while (!delayedMessages_.empty() &&
			delayedMessages_.front().first <= 0.0f)
		{
			NetMessage *message = (delayedMessages_.front()).second;
			delayedMessages_.pop_front();
			sendMessage(message->getDestinationId(), message);
		}
	}

	return messageHandler_.processMessages(); 
}

bool NetServer::started()
{
	return (server_ != 0 || firstDestination_ != 0);
}

bool NetServer::start(int port)
{
	if(SDLNet_Init()==-1)
	{
		return false;
	}

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
	{
		return false;
	}

	// we seem to be able to open the same port
	// multiple times!!!
	// This is fixed as the server info port catches it
	server_=SDLNet_TCP_Open(&ip);
	if (!server_)
	{
		return false;
	}
	NetBufferUtil::setBlockingIO(server_);
	SDLNet_TCP_AddSocket(sockSet_, server_);

	return true;
}

bool NetServer::connect(const char *hostName, int portNo)
{
	if(SDLNet_Init()==-1)
	{
		return false;
	}

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip,(char *) hostName,portNo)==-1)
	{
		return false;
	}

	TCPsocket client=SDLNet_TCP_Open(&ip);
	if (!client)
	{
		return false;
	}
	NetBufferUtil::setBlockingIO(client);

	addClient(client);
	return true;
}

int NetServer::threadFunc(void *param)
{
	NetServer *netServer = (NetServer *) param;

	Clock netClock;
	for (;;)
	{
		netClock.getTimeDifference();

		if (netServer->server_) 
		{
			netServer->pollIncoming();
		}
		if (netServer->checkDeleted_)
		{
			netServer->checkDeleted_ = false;
			netServer->pollDeleted();
		}

		float timeDiff = netClock.getTimeDifference();
		if (timeDiff > 1.0f)
		{
			Logger::log(formatString("Warning: Net loop took %.2f seconds, server", 
				timeDiff));
		}

		SDL_Delay(100);
	}

	return 0;
}

bool NetServer::pollIncoming()
{
	DIALOG_ASSERT(sockSet_ && server_);
	int numready = SDLNet_CheckSockets(sockSet_, 10);
	if (numready == -1) return false;
	if (numready == 0) return true;

	if(SDLNet_SocketReady(server_))
	{
		TCPsocket sock = SDLNet_TCP_Accept(server_);
		if (sock)
		{
			NetBufferUtil::setBlockingIO(sock);
			addClient(sock);
			getConnects()++;
		}
	}

	return true;
}

bool NetServer::pollDeleted()
{
	SDL_LockMutex(setMutex_);
	std::list<unsigned int> remove;
	std::map<unsigned int, NetServerRead *>::iterator itor;
	for (itor = connections_.begin();
		itor != connections_.end();
		itor++)
	{
		NetServerRead *serverRead = (*itor).second;
		unsigned int id = (*itor).first;
		if (serverRead->getDisconnect())
		{
			delete serverRead;
			remove.push_back(id);
		}
	}
	std::list<unsigned int>::iterator itor2;
	for (itor2 = remove.begin();
		itor2 != remove.end();
		itor2++)
	{
		unsigned int id = (*itor2);
		connections_.erase(id);
	}
	SDL_UnlockMutex(setMutex_);
	return true;
}

void NetServer::addClient(TCPsocket client)
{
	// Calculate the current client id
	++lastId_;
	if (lastId_ == 0) ++lastId_;

	// Create the thread to read this socket
	NetServerRead *serverRead = new NetServerRead(
		lastId_, client, protocol_, &messageHandler_, &checkDeleted_);

	// Add this to the collection of sockets (connections)
	SDL_LockMutex(setMutex_);
	connections_[lastId_] = serverRead;
	firstDestination_ = (*connections_.begin()).first;
	SDL_UnlockMutex(setMutex_);

	// Start the sockets
	serverRead->start();
}

void NetServer::disconnectAllClients()
{
	SDL_LockMutex(setMutex_);
	std::map<unsigned int, NetServerRead *>::iterator itor;
	for (itor = connections_.begin();
		itor != connections_.end();
		itor++)
	{
		unsigned int id = (*itor).first;
		disconnectClient(id);
	}
	SDL_UnlockMutex(setMutex_);
}

void NetServer::disconnectClient(unsigned int dest, bool delayed)
{
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::DisconnectMessage, 
				dest, getIpAddress(dest));
	if (delayed)
	{
		std::pair<float, NetMessage *> item(1.0f, message);
		delayedMessages_.push_back(item);
	}
	else
	{
		// Add the message to the list of out going
		sendMessage(dest, message);
	}
}

void NetServer::sendMessage(NetBuffer &buffer)
{
	sendMessage(buffer, firstDestination_);
}

void NetServer::sendMessage(NetBuffer &buffer, unsigned int dest)
							
{
	// Get a new buffer from the pool
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::NoMessage, 
				dest, getIpAddress(dest));

	// Add message to new buffer
	message->getBuffer().allocate(buffer.getBufferUsed());
	memcpy(message->getBuffer().getBuffer(), 
		buffer.getBuffer(), buffer.getBufferUsed());
	message->getBuffer().setBufferUsed(buffer.getBufferUsed());

	// Send Mesage
	sendMessage(dest, message);
}

void NetServer::sendMessage(unsigned int client, NetMessage *message)
{
	// Find the client
	SDL_LockMutex(setMutex_);
	std::map<unsigned int, NetServerRead *>::iterator itor = 
		connections_.find(client);
	if (itor != connections_.end()) 
	{
		// Add the message to the list of out going
		NetServerRead *serverRead = (*itor).second;
		serverRead->addMessage(message);
	}
	else
	{
		NetMessagePool::instance()->addToPool(message);
		Logger::log(formatString("Unknown sendMessage destination %u",
			client));
	}
	SDL_UnlockMutex(setMutex_);
}

unsigned int NetServer::getIpAddress(unsigned int destination)
{
	unsigned int addr = 0;
	SDL_LockMutex(setMutex_);
	std::map<unsigned int, NetServerRead *>::iterator itor = 
		connections_.find(destination);
	if (itor != connections_.end()) 
	{
		NetServerRead *read = (*itor).second;
		addr = read->getIpAddress();
	}
	SDL_UnlockMutex(setMutex_);

	return addr;
}

