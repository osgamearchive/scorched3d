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

#include <coms/ComsMessageHandler.h>
#include <common/Defines.h>
#include <common/Logger.h>

ComsMessageConnectionHandlerI::~ComsMessageConnectionHandlerI()
{
}

ComsMessageHandlerI::~ComsMessageHandlerI()
{
}

ComsMessageHandler::ComsMessageHandler() : 
	connectionHandler_(0), comsMessageLogging_(false)
{
}

ComsMessageHandler::~ComsMessageHandler()
{
}

void ComsMessageHandler::setConnectionHandler(
		ComsMessageConnectionHandlerI *handler)
{
	connectionHandler_ = handler;
}

void ComsMessageHandler::addHandler(const char *messageType,
		ComsMessageHandlerI *handler)
{
	std::map<std::string, ComsMessageHandlerI *>::iterator itor =
		handlerMap_.find(messageType);
	DIALOG_ASSERT(itor == handlerMap_.end());

	handlerMap_[messageType] = handler;
}

void ComsMessageHandler::processMessage(NetMessage &message)
{
	switch(message.getMessageType())
	{
		case NetMessage::BufferMessage:
			processReceiveMessage(message);
			break;
		case NetMessage::DisconnectMessage:
			if (comsMessageLogging_)
			{
				Logger::log(0, "ComsMessage:Disconnected");
			}

			if (connectionHandler_) 
				connectionHandler_->clientDisconnected(message);
			break;
		case NetMessage::ConnectMessage:
			if (comsMessageLogging_)
			{
				Logger::log(0, "ComsMessage:Connected");
			}

			if (connectionHandler_)
				connectionHandler_->clientConnected(message);
			break;
		default:
			if (connectionHandler_)
				connectionHandler_->clientError(message,
					"Failed to recognise message type");
			break;
	}
}

void ComsMessageHandler::processReceiveMessage(NetMessage &message)
{
	NetBufferReader reader(message.getBuffer());

	std::string messageType;
	if (!reader.getFromBuffer(messageType))
	{
		if (connectionHandler_)
			connectionHandler_->clientError(message,
				"Failed to decode message type");
		return;
	}

	if (comsMessageLogging_)
	{
		Logger::log(0, "ComsMessageHandler::processReceiveMessage(%s)",
					messageType.c_str());
	}

	std::map<std::string, ComsMessageHandlerI *>::iterator itor =
		handlerMap_.find(messageType);
	if (itor == handlerMap_.end())
	{
		char buffer[1024];
		sprintf(buffer, "Failed to find message type handler \"%s\"",
			messageType.c_str());

		if (connectionHandler_)
			connectionHandler_->clientError(message,
				buffer);
		return;
	}

	ComsMessageHandlerI *handler = (*itor).second;
	if (!handler->processMessage(message.getDestinationId(), 
		messageType.c_str(), reader))
	{
		char buffer[1024];
		sprintf(buffer, "Failed to handle message type \"%s\"",
			messageType.c_str());

		if (connectionHandler_)
			connectionHandler_->clientError(message,
				buffer);
		return;
	}
}
