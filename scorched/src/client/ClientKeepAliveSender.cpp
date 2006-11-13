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

#include <client/ClientKeepAliveSender.h>
#include <client/ScorchedClient.h>
#include <coms/ComsKeepAliveMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetInterface.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <time.h>

ClientKeepAliveSender *ClientKeepAliveSender::instance_ = 0;

ClientKeepAliveSender *ClientKeepAliveSender::instance()
{
	if (!instance_)
	{
		instance_ = new ClientKeepAliveSender;
	}
	return instance_;
}

ClientKeepAliveSender::ClientKeepAliveSender() : 
	lastSendTime_(0), recvMessage_(true), sendThread_(0)
{
	ComsKeepAliveMessage message;
	message.writeTypeMessage(buffer_);
	message.writeMessage(buffer_, 0);
}

ClientKeepAliveSender::~ClientKeepAliveSender()
{
}

void ClientKeepAliveSender::sendKeepAlive()
{
	if (ScorchedClient::instance()->getOptionsGame().getKeepAliveTimeoutTime() == 0)
	{
		return;
	}

	if (sendThread_)
	{
		return;
	}

	// Create the processing thread
	sendThread_ = SDL_CreateThread(ClientKeepAliveSender::sendThreadFunc, 0);
	if (sendThread_ == 0)
	{
		Logger::log(formatString("ClientKeepAliveSender: Failed to create send thread"));
		return;
	}	
}

int ClientKeepAliveSender::sendThreadFunc(void *)
{
	while (true)
	{
		SDL_Delay(500);
		instance_->send();
	}
	return 1;
}

void ClientKeepAliveSender::send()
{
	unsigned int sendTime = (unsigned int)
		ScorchedClient::instance()->getOptionsGame().getKeepAliveTime();
	unsigned int theTime = (unsigned int) time(0);

	if (theTime - lastSendTime_ >= sendTime)
	{
		// Use this directly as it is thread safe
		ScorchedClient::instance()->getNetInterface().sendMessage(buffer_);

		lastSendTime_ = theTime;
		lastWarnTime_ = theTime;
	}
}

