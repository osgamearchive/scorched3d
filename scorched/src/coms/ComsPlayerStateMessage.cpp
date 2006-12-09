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

#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <coms/ComsPlayerStateMessage.h>
#include <tank/TankTeamScore.h>
#include <tank/TankContainer.h>
#include <tankai/TankAIAdder.h>
#include <common/Logger.h>
#include <set>

ComsPlayerStateMessage::ComsPlayerStateMessage() : 
	ComsMessage("ComsPlayerStateMessage")
{

}

ComsPlayerStateMessage::~ComsPlayerStateMessage()
{

}

bool ComsPlayerStateMessage::writeMessage(NetBuffer &buffer, unsigned int destinationId)
{
	if (!ScorchedServer::instance()->getContext().tankTeamScore->
		writeMessage(buffer)) return false;

	std::map<unsigned int, Tank *> &tanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();

	// Add count
	buffer.addToBuffer((int) tanks.size());

	// For each tank
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		// Add each tank
		Tank *tank = (*itor).second;
		buffer.addToBuffer(tank->getPlayerId());
		if (!tank->writeMessage(buffer, (destinationId == tank->getDestinationId()))) return false;
	}
	return true;
}

bool ComsPlayerStateMessage::readMessage(NetBufferReader &reader)
{
#ifndef S3D_SERVER
	if (!ScorchedClient::instance()->getContext().tankTeamScore->
		readMessage(reader)) return false;

	// Update all targets with the state from the targets on the
	// server
	int count = 0;
	if (!reader.getFromBuffer(count)) return false;
	for (int i=0; i<count; i++)
	{
		unsigned int playerId;
		if (!reader.getFromBuffer(playerId)) return false;
		Tank *tank = ScorchedClient::instance()->getTankContainer().
			getTankById(playerId);
		if (tank)
		{
			if (!tank->readMessage(reader)) return false;
		}
		else
		{
			std::string name;
			reader.getFromBuffer(name);
			Logger::log(formatString("Error: Failed to find tank %u\"%s\"",
				playerId, name.c_str()));
			return false;
		}
	}
#endif
	
	return true;
}
