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

NetServer *NetServer::instance_ = 0;

NetServer *NetServer::instance()
{
	if (!instance_)
	{
	  instance_ = new NetServer();
	}

	return instance_;
}

NetServer::NetServer() : sockSet_(0), maxClients_(-1), 
	server_(0), outgoingMessagesMutex_(0)
{
	outgoingMessagesMutex_ = SDL_CreateMutex();
	setMutex_ = SDL_CreateMutex();
}

NetServer::~NetServer()
{
	SDL_DestroyMutex(outgoingMessagesMutex_);
	SDL_DestroyMutex(setMutex_);
}

bool NetServer::start(int port, int maxClients)
{
	maxClients_ = maxClients;
	if(SDLNet_Init()==-1)
	{
		return false;
	}

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
	{
		return false;
	}

	// TODO we seem to be able to open the same port
	// multiple times!!!
	server_=SDLNet_TCP_Open(&ip);
	if (!server_)
	{
		return false;
	}
	NetBufferUtil::setBlockingIO(server_);

	addClient(server_);

	SDL_CreateThread(NetServer::threadFunc, (void *) 0);
	return true;
}

bool NetServer::started()
{
	return (server_ != 0);
}

void NetServer::stop()
{
	SDL_LockMutex(setMutex_);
	std::set<TCPsocket>::iterator itor;
	for (itor = connections_.begin();
		 itor != connections_.end();
		 itor++)
	{
		destroyClient(*itor);
	}

	server_ = 0;
	connections_.clear();
	if (sockSet_) SDLNet_FreeSocketSet(sockSet_);
	sockSet_ = 0;
	SDLNet_Quit();
	SDL_UnlockMutex(setMutex_);
}

int NetServer::threadFunc(void *)
{
	bool quit = false;
	while (!quit)
	{
		SDL_LockMutex(instance_->setMutex_);
		if (!instance_->pollIncoming()) quit = true;
		instance_->pollOutgoing();
		SDL_UnlockMutex(instance_->setMutex_);
		SDL_Delay(10);
	}

	instance_->stop();
	return 0;
}

bool NetServer::pollIncoming()
{
	if (!sockSet_) return false;

	int numready = SDLNet_CheckSockets(sockSet_, 10);
	if (numready == -1) return false;
	if (numready == 0) return true;

	std::set<TCPsocket>::iterator itor;
	for (itor = connections_.begin();
		 itor != connections_.end() && numready;
		 itor++)
	{
		TCPsocket currentSock = *itor;
		if(SDLNet_SocketReady(*itor))
		{
			numready--;
			if (currentSock == server_)
			{
				TCPsocket sock = SDLNet_TCP_Accept(server_);
				if (sock)
				{
					if ((maxClients_ == -1) ||
						((int) connections_.size() != maxClients_ + 1))
					{
						addClient(sock);

						NetMessage *message = NetMessagePool::instance()->
							getFromPool(NetMessage::ConnectMessage, sock);
						NetMessageHandler::instance()->addMessage(message);
						return true;
					}
					else
					{
						// Dont accept at present
						SDLNet_TCP_Close(sock);
					}
				}
			}
			else
			{
				NetMessage *message = NetBufferUtil::readBuffer(currentSock);
				if (!message)
				{
					Logger::log(0, "Read failed from client socket, has been closed?");

					// Socket has been closed
					destroyClient(currentSock);
					return true;
				}
				else
				{
					// We have a buffer containing the message
					NetMessageHandler::instance()->addMessage(message);
				}
			}
		}
	}

	return true;
}

void NetServer::pollOutgoing()
{
	std::list<NetMessage *> newMessages;
	SDL_LockMutex(outgoingMessagesMutex_);
	outgoing_.swap(newMessages);
	SDL_UnlockMutex(outgoingMessagesMutex_);

	while (!newMessages.empty())
	{
		NetMessage *message = newMessages.front();
		newMessages.pop_front();

		std::set<TCPsocket>::iterator itor = 
			connections_.find(message->getPlayerId());
		if (itor != connections_.end())
		{		
			if (!NetBufferUtil::sendBuffer(
				message->getBuffer(), message->getPlayerId()))
			{
				dialogMessage("NetBuffer", "ERORR: Failed to send message");
			}
		}

		NetMessagePool::instance()->addToPool(message);
	}
}

int NetServer::getNoClients()
{ 
	SDL_LockMutex(setMutex_);
	int result = (int) connections_.size() - 1; 
	SDL_UnlockMutex(setMutex_);
	return result;
}

void NetServer::addClient(TCPsocket &client)
{
	connections_.insert(client);
	updateSockSet();
}

void NetServer::rmClient(TCPsocket &client)
{
	std::set<TCPsocket>::iterator itor = 
		connections_.find(client);
	if (itor != connections_.end())
	{
		connections_.erase(itor);
	}
	updateSockSet();
}

void NetServer::updateSockSet()
{
	if (sockSet_) SDLNet_FreeSocketSet(sockSet_);
	sockSet_ = SDLNet_AllocSocketSet((int) connections_.size());

	std::set<TCPsocket>::iterator itor;
	for (itor = connections_.begin();
		 itor != connections_.end();
		 itor++)
	{
		TCPsocket currentSock = *itor;
		SDLNet_TCP_AddSocket(sockSet_, currentSock);
	}
}

void NetServer::destroyClient(TCPsocket client)
{
	if (client == server_) return;

	SDL_LockMutex(setMutex_);
	std::set<TCPsocket>::iterator itor =
		connections_.find(client);
	if (itor != connections_.end())
	{
		NetMessage *message = NetMessagePool::instance()->
			getFromPool(NetMessage::DisconnectMessage, client);
		NetMessageHandler::instance()->addMessage(message);

		SDLNet_TCP_Close(client);
		rmClient(client);
	}
	SDL_UnlockMutex(setMutex_);
}

void NetServer::sendMessage(NetBuffer &buffer,
							TCPsocket destination)
{
	DIALOG_ASSERT(destination);

	// Get a new buffer from the pool
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::NoMessage, destination);

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

