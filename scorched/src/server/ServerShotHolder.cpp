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


#include <server/ServerShotHolder.h>
#include <tank/TankContainer.h>
#include <tank/TankAILogic.h>

ServerShotHolder *ServerShotHolder::instance_ = 0;

ServerShotHolder *ServerShotHolder::instance()
{
	if (!instance_)
	{
		instance_ = new ServerShotHolder;
	}
	return instance_;
}

ServerShotHolder::ServerShotHolder()
{
}

ServerShotHolder::~ServerShotHolder()
{
}

void ServerShotHolder::clearShots()
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor;
	for (itor = messages_.begin();
		itor != messages_.end();
		itor++)
	{
		delete (*itor).second;
	}
	messages_.clear();
}

void ServerShotHolder::addShot(unsigned int playerId,
							   ComsPlayedMoveMessage *message)
{
	// Ensure each player can only add one message
	if (!haveShot(playerId))
	{
		messages_[playerId] = message;
	}
}

bool ServerShotHolder::haveShot(unsigned int playerId)
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor =
		messages_.find(playerId);
	return (itor != messages_.end());
}

bool ServerShotHolder::haveAllShots()
{
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal)
		{
			if (!haveShot(tank->getPlayerId())) return false;
		}
	}
	return true;
}

void ServerShotHolder::playShots()
{
	std::map<unsigned int, ComsPlayedMoveMessage *>::iterator itor;
	for (itor = messages_.begin();
		itor != messages_.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		ComsPlayedMoveMessage *message = (*itor).second;

		// Check the tank exists for this player
		// (may not if the player has left the game after firing)
		Tank *tank = TankContainer::instance()->getTankById(playerId);
		if (tank)
		{
			TankAILogic::processPlayedMoveMessage(*message, tank);
		}
	}
	clearShots();
}
