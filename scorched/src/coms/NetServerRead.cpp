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
#include <coms/NetServer.h>
#include <coms/NetMessagePool.h>
#include <common/Clock.h>
#include <common/Logger.h>
#include <common/Defines.h>

NetServerRead::NetServerRead(TCPsocket socket,
							 NetServerProtocol *protocol,
							 NetMessageHandler *messageHandler,
							 bool *checkDeleted) : 
	socket_(socket), sockSet_(0), protocol_(protocol), 
	outgoingMessagesMutex_(0), checkDeleted_(checkDeleted),
	disconnect_(false), messageHandler_(messageHandler),
	sentDisconnect_(false), startCount_(0),
	ctrlThread_(0), recvThread_(0), sendThread_(0)
{
	sockSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(sockSet_, socket);
	outgoingMessagesMutex_ = SDL_CreateMutex();
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

void NetServerRead::start()
{
	// Send the player connected notification
	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::ConnectMessage, 
		(unsigned int) socket_,
		NetServer::getIpAddress(socket_));
	messageHandler_->addMessage(message);

	recvThread_ = SDL_CreateThread(
		NetServerRead::recvThreadFunc, (void *) this);
	sendThread_ = SDL_CreateThread(
		NetServerRead::sendThreadFunc, (void *) this);
	ctrlThread_ = SDL_CreateThread(
		NetServerRead::ctrlThreadFunc, (void *) this);
	if (!ctrlThread_ || !recvThread_ || !sendThread_)
	{
		Logger::log(0, "ERROR: Run out of threads");
	}
}

void NetServerRead::addMessage(NetMessage *message)
{
	if (message->getMessageType() != NetMessage::DisconnectMessage && 
		message->getBuffer().getBuffer() == 0) 
	{ 
		DIALOG_ASSERT(0); 
	}

	SDL_LockMutex(outgoingMessagesMutex_);
	newMessages_.push_back(message);
	if (message->getMessageType() == NetMessage::DisconnectMessage &&
		!sentDisconnect_)
	{
		sentDisconnect_ = true;
		NetMessage *message = NetMessagePool::instance()->
			getFromPool(NetMessage::DisconnectMessage, 
				(unsigned int) socket_,
				NetServer::getIpAddress(socket_));
		messageHandler_->addMessage(message);
	}
	SDL_UnlockMutex(outgoingMessagesMutex_);
}

bool NetServerRead::getDisconnect()
{ 
	SDL_LockMutex(outgoingMessagesMutex_);
	bool result = disconnect_;	
	SDL_UnlockMutex(outgoingMessagesMutex_);

	if (result)
	{
		int status = 0;
		SDL_WaitThread(ctrlThread_, &status);
	}
	return result; 
}

int NetServerRead::ctrlThreadFunc(void *netServerRead)
{
	NetServerRead *ns = (NetServerRead *) netServerRead;
	ns->actualCtrlThreadFunc();
	return 0;
}

int NetServerRead::sendThreadFunc(void *netServerRead)
{
	NetServerRead *ns = (NetServerRead *) netServerRead;
	ns->actualSendRecvThreadFunc(true);
	return 0;
}

int NetServerRead::recvThreadFunc(void *netServerRead)
{
	NetServerRead *ns = (NetServerRead *) netServerRead;
	ns->actualSendRecvThreadFunc(false);
	return 0;
}

void NetServerRead::actualCtrlThreadFunc()
{
	// Ensure the other threads have started
	bool done = false;
	while (!done)
	{
		SDL_LockMutex(outgoingMessagesMutex_);
		if (startCount_ == 2) done = true;
		SDL_UnlockMutex(outgoingMessagesMutex_);
		SDL_Delay(100);
	}

	// Wait for the other threads to end
	int status;
	SDL_WaitThread(sendThread_, &status);
	SDL_WaitThread(recvThread_, &status);

	// Tidy socket
	SDLNet_TCP_Close(socket_);

	// Delete self
	SDL_LockMutex(outgoingMessagesMutex_);
	disconnect_ = true;
	*checkDeleted_ = true;
	SDL_UnlockMutex(outgoingMessagesMutex_);
}

void NetServerRead::actualSendRecvThreadFunc(bool send)
{
	SDL_LockMutex(outgoingMessagesMutex_);
	startCount_++;
	SDL_UnlockMutex(outgoingMessagesMutex_);

	Clock netClock;
	while (!sentDisconnect_)
	{
		netClock.getTimeDifference();

		if (send)
		{	
			if (!pollOutgoing()) break;
		}
		else
		{
			if (!pollIncoming()) break;
		}

		float timeDiff = netClock.getTimeDifference();
		if (timeDiff > 15.0f)
		{
			Logger::log(0, 
				"Warning: %s net loop took %.2f seconds, client %i", 
				(send?"Send":"Recv"),
				timeDiff, (unsigned int) socket_);
		}
	}

	SDL_LockMutex(outgoingMessagesMutex_);
	if (!sentDisconnect_)
	{
		sentDisconnect_ = true;
		NetMessage *message = NetMessagePool::instance()->
			getFromPool(NetMessage::DisconnectMessage, 
				(unsigned int) socket_,
				NetServer::getIpAddress(socket_));
		messageHandler_->addMessage(message);
	}
	SDL_UnlockMutex(outgoingMessagesMutex_);
}

bool NetServerRead::pollIncoming()
{
	int numready = SDLNet_CheckSockets(sockSet_, 100);
	if (numready == -1) return false;
	if (numready == 0)
	{
		SDL_Delay(10);
		return true;
	}

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
			if (!sentDisconnect_)
			{
				// We have a buffer containing the message
				messageHandler_->addMessage(message);
			}
			else
			{
				NetMessagePool::instance()->addToPool(message);
			}
		}
	}

	return true;
}

bool NetServerRead::pollOutgoing()
{
	NetMessage *message = 0;
	SDL_LockMutex(outgoingMessagesMutex_);
	if (!newMessages_.empty())
	{
		message = newMessages_.front();
		newMessages_.pop_front();
	}
	SDL_UnlockMutex(outgoingMessagesMutex_);

	bool result = true;
	if (message)
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

		NetMessagePool::instance()->addToPool(message);
	}
	else SDL_Delay(100);

	return result;
}
