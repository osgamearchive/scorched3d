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

#include <coms/ComsMessageSender.h>
#include <coms/ComsMessageHandler.h>
#include <coms/NetBuffer.h>
#include <coms/NetServer.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>

bool ComsMessageSender::sendToServer(ComsMessage &message)
{
	if (!ScorchedClient::instance()->getNetInterface().started()) return false;

	if (ScorchedClient::instance()->getComsMessageHandler().getMessageLogging())
	{
		Logger::log(0, "sendToServer(%s)", message.getMessageType());
	}

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log(0, "ERROR: ComsMessageSender::sendToServer - Failed to write message type");
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log(0, "ERROR: ComsMessageSender::sendToServer - Failed to write message");
		return false;
	}
	ScorchedClient::instance()->getNetInterface().sendMessage(NetBufferDefault::defaultBuffer);
	return true;
}

bool ComsMessageSender::sendToSingleClient(ComsMessage &message,
						unsigned int destination)
{
	if (!ScorchedServer::instance()->getNetInterface().started()) return false;
	if (destination == 0) return true;

	if (ScorchedServer::instance()->getComsMessageHandler().getMessageLogging())
	{
		Logger::log(0, "sendToSingleClient(%s, %i)", 
					message.getMessageType(),
					(int) destination);
	}

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log(0, "ERROR: ComsMessageSender::sendToSingleClient - Failed to write message");
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log(0, "ERROR: ComsMessageSender::sendToSingleClient - Failed to write message");
		return false;
	}
	ScorchedServer::instance()->getNetInterface().sendMessage(NetBufferDefault::defaultBuffer,
		destination);
	return true;
}

bool ComsMessageSender::sendToAllConnectedClients(ComsMessage &message)
{
	bool result = true;

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	std::set<unsigned int>::iterator findItor;

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		unsigned int destination = tank->getDestinationId();
		findItor = destinations.find(destination);
		if (findItor == destinations.end())
		{
			destinations.insert(destination);
			if (!sendToSingleClient(message, destination))
			{
				result = false;
			}
		}
	}

	return result;
}

bool ComsMessageSender::sendToAllPlayingClients(ComsMessage &message)
{
	bool result = true;

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	std::set<unsigned int>::iterator findItor;

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() != TankState::sPending)
		{
			unsigned int destination = tank->getDestinationId();
			findItor = destinations.find(destination);
			if (findItor == destinations.end())
			{
				destinations.insert(destination);
				if (!sendToSingleClient(message, destination))
				{
					result = false;
				}
			}
		}
	}

	return result;
}
