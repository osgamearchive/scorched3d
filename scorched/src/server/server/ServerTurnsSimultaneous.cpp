////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#include <server/ServerTurnsSimultaneous.h>
#include <server/ScorchedServer.h>
#include <tank/TankContainer.h>
#include <set>
#include <list>

ServerTurnsSimultaneous::ServerTurnsSimultaneous() :
	user_(0)
{
}

ServerTurnsSimultaneous::~ServerTurnsSimultaneous()
{
}

void ServerTurnsSimultaneous::setUser(ServerTurnsSimultaneousI *user)
{
	user_ = user;
}

void ServerTurnsSimultaneous::clear()
{
	waitingPlayers_.clear();
}

void ServerTurnsSimultaneous::addPlayer(unsigned int playerId)
{
	static unsigned int moveId = 0;
	waitingPlayers_[playerId] = ++moveId;
}

void ServerTurnsSimultaneous::playerFinished(unsigned int playerId, unsigned int moveId)
{
	std::map<unsigned int, unsigned int>::iterator itor =
		playingPlayers_.find(playerId);
	if (itor == playingPlayers_.end()) return;
	if (itor->second == moveId)
	{
		playingPlayers_.erase(itor);
	}
}

void ServerTurnsSimultaneous::simulate(float frameTime)
{
	// Check if all the tanks have made their moves
	if (waitingPlayers_.empty() &&
		playingPlayers_.empty()) {
		user_->allPlayersFinished();
	}

	// Check if any of the playing tanks have timed out
	// or if they have left the game
	std::set<unsigned int> playingDestinations;
	std::list<unsigned int> removePlaying;
	std::map<unsigned int, unsigned int>::iterator playItor;
	for (playItor = playingPlayers_.begin();
		playItor != playingPlayers_.end();
		playItor++)
	{
		unsigned int playerId = playItor->first;
		Tank *tank =
			ScorchedServer::instance()->getTankContainer().getTankById(playerId);
		if (!tank)
		{
			removePlaying.push_back(playerId);
		}
		else if (tank->getDestinationId() != 0)
		{
			playingDestinations.insert(tank->getDestinationId());
		}
	}

	// Remove any that are not playing
	std::list<unsigned int>::iterator removeItor;
	for (removeItor = removePlaying.begin();
		removeItor != removePlaying.end();
		removeItor++)
	{
		unsigned int playerId = *removeItor;
		playingPlayers_.erase(playerId);
	}

	// Add any waiting tanks to the game (if possible)
	std::map<unsigned int, unsigned int>::iterator waitingItor;
	std::list<unsigned int> removeWaiting;
	for (waitingItor = waitingPlayers_.begin();
		waitingItor != waitingPlayers_.end();
		waitingItor++)
	{
		unsigned int playerId = waitingItor->first;
		Tank *tank =
			ScorchedServer::instance()->getTankContainer().getTankById(playerId);
		if (!tank)
		{
			removeWaiting.push_back(playerId);
		}
		else if (playingDestinations.find(tank->getDestinationId()) == playingDestinations.end())
		{
			playingDestinations.insert(tank->getDestinationId());
			playingPlayers_[playerId] = waitingItor->second;
			user_->playerPlaying(playerId, waitingItor->second);
			removeWaiting.push_back(playerId);
		}
	}

	// Remove any that are not waiting
	for (removeItor = removeWaiting.begin();
		removeItor != removeWaiting.end();
		removeItor++)
	{
		unsigned int playerId = *removeItor;
		waitingPlayers_.erase(playerId);
	}
}
