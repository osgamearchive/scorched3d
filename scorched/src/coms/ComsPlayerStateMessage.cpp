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

	std::map<unsigned int, Target *> &targets = 
		ScorchedServer::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;

	// Add count
	buffer.addToBuffer((int) targets.size());

	// For each tank
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		// Add each tank
		Target *target = (*itor).second;
		buffer.addToBuffer(target->getPlayerId());

		if (!target->isTarget())
		{
			Tank *tank = (Tank *) target;
			if (!tank->writeMessage(buffer, (destinationId == tank->getDestinationId()))) return false;
		}
		else
		{
			if (!target->writeMessage(buffer, true)) return false;
		}
	}
	return true;
}

bool ComsPlayerStateMessage::readMessage(NetBufferReader &reader)
{
	if (!ScorchedClient::instance()->getContext().tankTeamScore->
		readMessage(reader)) return false;

	// Update all targets with the state from the targets on the
	// server
	std::set<unsigned int> updatedTargets;
	int count = 0;
	if (!reader.getFromBuffer(count)) return false;
	for (int i=0; i<count; i++)
	{
		unsigned int playerId;
		if (!reader.getFromBuffer(playerId)) return false;
		Target *target = ScorchedClient::instance()->getTargetContainer().
			getTargetById(playerId);
		if (target)
		{
			if (!target->readMessage(reader)) return false;
			updatedTargets.insert(target->getPlayerId());
		}
		//else return false; // TODO FIXME
	}

	// Remove any targets the client has but the server does not
	std::map<unsigned int, Target *> targets = // Note copy
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		if (updatedTargets.find(playerId) == updatedTargets.end())
		{
			Target *removedTarget = 
				ScorchedClient::instance()->getTargetContainer().removeTarget(playerId);
			delete removedTarget;
		}
	}
	
	return true;
}
