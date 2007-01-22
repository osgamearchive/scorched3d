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
#include <coms/ComsTargetStateMessage.h>
#include <tank/TankTeamScore.h>
#include <tank/TankContainer.h>
#include <target/TargetState.h>
#include <tankai/TankAIAdder.h>
#include <common/Logger.h>
#include <movement/TargetMovement.h>
#include <set>

ComsTargetStateMessage::ComsTargetStateMessage(bool fullmessage) : 
	ComsMessage("ComsTargetStateMessage"),
	fullmessage_(fullmessage)
{

}

ComsTargetStateMessage::~ComsTargetStateMessage()
{

}

bool ComsTargetStateMessage::writeMessage(NetBuffer &buffer)
{
	std::map<unsigned int, Target *> targets;
	{
		std::map<unsigned int, Target *> &possibletargets =
			ScorchedServer::instance()->getTargetContainer().getTargets();
		std::map<unsigned int, Target *>::iterator itor;

		for (itor = possibletargets.begin();
			itor != possibletargets.end();
			itor++)
		{
			Target *target = (*itor).second;
			if (target->isTarget())
			{
				// If its not a full message only send the state of targets
				// than can move
				if (fullmessage_ || target->getTargetState().getMovement())
				{
					targets[(*itor).first] = (*itor).second;
				}
			}
		}
	}

	// Add full message ident
	buffer.addToBuffer(fullmessage_);

	// Add count
	buffer.addToBuffer((int) targets.size());

	// For each target
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		// Add each tank
		Target *target = (*itor).second;
		buffer.addToBuffer(target->getPlayerId());

		if (!target->writeMessage(buffer)) return false;
	}

	// Add all of the movement information
	if (!ScorchedServer::instance()->getTargetMovement().writeMessage(buffer)) return false;
	return true;
}

bool ComsTargetStateMessage::readMessage(NetBufferReader &reader)
{
#ifndef S3D_SERVER
	// Update all targets with the state from the targets on the
	// server
	std::set<unsigned int> updatedTargets;
	if (!reader.getFromBuffer(fullmessage_)) return false;
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
		else
		{
			if (playerId > TankAIAdder::MAX_TANK_ID)
			{
				target = new Target(
					playerId, "", 
					ScorchedClient::instance()->getContext());
				ScorchedClient::instance()->getTargetContainer().
					addTarget(target);

				if (!target->readMessage(reader)) return false;
				updatedTargets.insert(target->getPlayerId());
			}
			else
			{
				std::string name;
				reader.getFromBuffer(name);
				Logger::log(formatString("Error: Failed to find target %u\"%s\"",
					playerId, name.c_str()));
				return false;
			}
		}
	}

	if (fullmessage_)
	{
		// Remove any targets the client has but the server does not
		std::map<unsigned int, Target *> targets = // Note copy
			ScorchedClient::instance()->getTargetContainer().getTargets();
		std::map<unsigned int, Target *>::iterator itor;
		for (itor = targets.begin();
			itor != targets.end();
			itor++)
		{
			unsigned int playerId = (*itor).first;
			Target *target = (*itor).second;
			if (target->isTarget())
			{
				if (updatedTargets.find(playerId) == updatedTargets.end())
				{
					Target *removedTarget = 
						ScorchedClient::instance()->getTargetContainer().removeTarget(playerId);
					delete removedTarget;
				}
			}
		}
	}

	// Get all of the movement information
	if (!ScorchedClient::instance()->getTargetMovement().readMessage(reader)) return false;
#endif
	
	return true;
}
