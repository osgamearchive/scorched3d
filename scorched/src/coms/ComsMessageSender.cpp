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
#include <coms/NetInterface.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <zlib/zlib.h>
#include <set>

bool ComsMessageSender::sendToServer(ComsMessage &message)
{
	if (!ScorchedClient::instance()->getNetInterface().started()) return false;

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log( "ERROR: ComsMessageSender::sendToServer - Failed to write message type");
		return false;
	}
	
	if (ScorchedClient::instance()->getComsMessageHandler().getMessageLogging())
	{
		Logger::log(formatString("Client::send(%s, %u)", message.getMessageType(),
			NetBufferDefault::defaultBuffer.getBufferUsed()));
	}	

	if (!message.writeMessage(NetBufferDefault::defaultBuffer, 0))
	{
		Logger::log( "ERROR: ComsMessageSender::sendToServer - Failed to write message");
		return false;
	}
	ScorchedClient::instance()->getNetInterface().sendMessage(NetBufferDefault::defaultBuffer);
	return true;
}

bool ComsMessageSender::sendToSingleClient(ComsMessage &message,
						unsigned int destination)
{
	if (!ScorchedServer::instance()->getNetInterface().started())
	{
		Logger::log( "ERROR: ComsMessageSender::sendToSingleClient - Client not started");
		return false;
	}
	if (destination == 0) return true;

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log( "ERROR: ComsMessageSender::sendToSingleClient - Failed to write message type");
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer, destination))
	{
		Logger::log( "ERROR: ComsMessageSender::sendToSingleClient - Failed to write message");
		return false;
	}
	
	if (ScorchedServer::instance()->getComsMessageHandler().getMessageLogging())
	{
		NetBuffer tmpBuffer;
		unsigned long destLen = NetBufferDefault::defaultBuffer.getBufferUsed() * 2 + 12;
		tmpBuffer.allocate(destLen);
		compress2((unsigned char *) tmpBuffer.getBuffer(), 
			&destLen,
			(unsigned char *)NetBufferDefault::defaultBuffer.getBuffer(),
			NetBufferDefault::defaultBuffer.getBufferUsed(),
			6);
	
		Logger::log(formatString("Server::send(%s, %i, %u (%u))", 
					message.getMessageType(),
					(int) destination,
					NetBufferDefault::defaultBuffer.getBufferUsed(),
					destLen));
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
	std::map<unsigned int, Tank *> tanks = 
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
	std::map<unsigned int, Tank *> tanks = 
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
