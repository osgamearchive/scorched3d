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
#include <net/NetBuffer.h>
#include <net/NetInterface.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <set>
#include <zlib.h>

bool ComsMessageSender::formMessage(ComsMessage &message)
{
	// Write the message and its type to the buffer
	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log( "ERROR: ComsMessageSender - Failed to write message type");
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer))
	{
		Logger::log( "ERROR: ComsMessageSender - Failed to write message");
		return false;
	}

	// Compress the message
	NetBufferDefault::defaultBuffer.compressBuffer();

	return true;
}

#ifndef S3D_SERVER
bool ComsMessageSender::sendToServer(
	ComsMessage &message, unsigned int flags)
{
	if (!ScorchedClient::instance()->getNetInterface().started()) return false;
	if (!formMessage(message)) return false;

	if (ScorchedClient::instance()->getComsMessageHandler().getMessageLogging())
	{
		Logger::log(formatString("Client::send(%s, %u)", 
			message.getMessageType(),
			NetBufferDefault::defaultBuffer.getBufferUsed()));
	}	
	ScorchedClient::instance()->getNetInterface().sendMessageServer(
		NetBufferDefault::defaultBuffer, flags);
	return true;
}
#endif

bool ComsMessageSender::sendToSingleClient(ComsMessage &message,
	unsigned int destination, unsigned int flags)
{
	if (destination == 0) return true;
	if (!ScorchedServer::instance()->getNetInterface().started())
	{
		Logger::log( "ERROR: ComsMessageSender::sendToSingleClient - Server not started");
		return false;
	}

	if (!formMessage(message)) return false;

	if (ScorchedServer::instance()->getComsMessageHandler().getMessageLogging())
	{
		Logger::log(formatString("Server::send(%s, %u, %u)", 
			message.getMessageType(),
			destination,
			NetBufferDefault::defaultBuffer.getBufferUsed()));
	}	
	ScorchedServer::instance()->getNetInterface().sendMessageDest(
		NetBufferDefault::defaultBuffer, destination, flags);
	return true;
}

bool ComsMessageSender::sendToAllConnectedClients(
	ComsMessage &message, unsigned int flags)
{
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	if (tanks.empty()) return true;

	if (!ScorchedServer::instance()->getNetInterface().started())
	{
		Logger::log( "ERROR: ComsMessageSender::sendToAllConnectedClients - Server not started");
		return false;
	}

	// Serialize the same message once for all client
	if (!formMessage(message)) return false;

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	destinations.insert(0); // Make sure we don't send to dest 0
	std::set<unsigned int>::iterator findItor;
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

			if (ScorchedServer::instance()->getComsMessageHandler().getMessageLogging())
			{
				Logger::log(formatString("Server::send(%s, %u, %u)", 
					message.getMessageType(),
					destination,
					NetBufferDefault::defaultBuffer.getBufferUsed()));
			}	
			ScorchedServer::instance()->getNetInterface().sendMessageDest(
				NetBufferDefault::defaultBuffer, destination, flags);
		}
	}

	return true;
}

bool ComsMessageSender::sendToAllPlayingClients(
	ComsMessage &message, unsigned int flags)
{
	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	if (tanks.empty()) return true;

	if (!ScorchedServer::instance()->getNetInterface().started())
	{
		Logger::log( "ERROR: ComsMessageSender::sendToAllPlayingClients - Server not started");
		return false;
	}

	// Serialize the same message once for all client
	if (!formMessage(message)) return false;

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	destinations.insert(0); // Make sure we don't send to dest 0
	std::set<unsigned int>::iterator findItor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() != TankState::sPending &&
			tank->getState().getState() != TankState::sLoading &&
			tank->getState().getState() != TankState::sInitializing)
		{
			unsigned int destination = tank->getDestinationId();
			findItor = destinations.find(destination);
			if (findItor == destinations.end())
			{
				destinations.insert(destination);

				if (ScorchedServer::instance()->getComsMessageHandler().getMessageLogging())
				{
					Logger::log(formatString("Server::send(%s, %u, %u)", 
						message.getMessageType(),
						destination,
						NetBufferDefault::defaultBuffer.getBufferUsed()));
				}	
				ScorchedServer::instance()->getNetInterface().sendMessageDest(
					NetBufferDefault::defaultBuffer, destination, flags);
			}
		}
	}

	return true;
}
