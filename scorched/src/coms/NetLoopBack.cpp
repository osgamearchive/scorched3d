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

#include <coms/NetLoopBack.h>
#include <coms/NetMessagePool.h>
#include <common/Defines.h>
#include <common/Logger.h>

NetLoopBack::NetLoopBack(unsigned int currentId) 
	: loopback_(0), currentId_(currentId)
{
}

NetLoopBack::~NetLoopBack()
{
}

void NetLoopBack::setLoopBack(NetLoopBack *loopback)
{
	loopback_ = loopback;
}

bool NetLoopBack::started()
{
	return true;
}

void NetLoopBack::setMessageHandler(NetMessageHandlerI *handler) 
{ 
	messageHandler_.setMessageHandler(handler); 
}

int NetLoopBack::processMessages()
{ 
	return messageHandler_.processMessages(); 
}

void NetLoopBack::disconnectAllClients()
{
	Logger::log(0, "Cannot disconnect all clients, they are local!");
}

void NetLoopBack::disconnectClient(unsigned int client)
{
	Logger::log(0, "Cannot disconnect client %i, they are local!", client);
}

void NetLoopBack::sendMessage(NetBuffer &buffer)
{
	sendMessage(buffer, ServerLoopBackID);
}

void NetLoopBack::sendMessage(NetBuffer &buffer, unsigned int destination)
{
	DIALOG_ASSERT(loopback_);
	DIALOG_ASSERT(destination == ClientLoopBackID ||
		destination == ServerLoopBackID);

	NetMessage *message = NetMessagePool::instance()->
		getFromPool(NetMessage::BufferMessage, currentId_);
	message->getBuffer().reset();
	message->getBuffer().addDataToBuffer(buffer.getBuffer(), buffer.getBufferUsed());
	loopback_->messageHandler_.addMessage(message);
}
