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
#include <coms/NetBuffer.h>
#include <coms/NetClient.h>
#include <coms/NetServer.h>
#include <tank/TankContainer.h>

bool ComsMessageSender::sendToServer(ComsMessage &message)
{
	DIALOG_ASSERT(NetClient::instance()->started());

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer))
	{
		return false;
	}
	NetClient::instance()->sendMessage(NetBufferDefault::defaultBuffer);
	return true;
}

bool ComsMessageSender::sendToSingleClient(ComsMessage &message,
						TCPsocket destination)
{
	DIALOG_ASSERT(NetServer::instance()->started());

	unsigned int playerId = (unsigned int) destination;
	Tank *tank = TankContainer::instance()->getTankById(playerId);
	if (tank && tank->getTankAI()) return true;

	NetBufferDefault::defaultBuffer.reset();
	if (!message.writeTypeMessage(NetBufferDefault::defaultBuffer))
	{
		return false;
	}
	if (!message.writeMessage(NetBufferDefault::defaultBuffer))
	{
		return false;
	}
	NetServer::instance()->sendMessage(NetBufferDefault::defaultBuffer,
		destination);
	return true;
}

bool ComsMessageSender::sendToAllConnectedClients(ComsMessage &message)
{
	bool result = true;

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		TCPsocket socket = (TCPsocket) tank->getPlayerId();
		if (!sendToSingleClient(message, socket))
		{
			result = false;
		}
	}

	return result;
}

bool ComsMessageSender::sendToAllPlayingClients(ComsMessage &message)
{
	bool result = true;

	std::map<unsigned int, Tank *>::iterator itor;
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() != TankState::sPending)
		{
			TCPsocket socket = (TCPsocket) tank->getPlayerId();
			if (!sendToSingleClient(message, socket))
			{
				result = false;
			}
		}
	}

	return result;
}
