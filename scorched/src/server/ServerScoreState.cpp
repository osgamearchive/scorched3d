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


#include <server/ServerScoreState.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <tank/TankContainer.h>
#include <tank/TankSort.h>
#include <coms/ComsScoreMessage.h>
#include <coms/ComsMessageSender.h>

ServerScoreState::ServerScoreState()
{
}

ServerScoreState::~ServerScoreState()
{
}

void ServerScoreState::enterState(const unsigned state)
{
	// Score order
	std::list<Tank *> sortedTanks;
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		sortedTanks.push_back((*itor).second);
	}

	TankSort::getSortedTanks(sortedTanks);

	std::list<Tank *>::iterator scoreitor = sortedTanks.begin();

	Logger::log(0, "Showing Score");
	Logger::log((*scoreitor)->getPlayerId(), "\"%s\" is the overall winner!", (*scoreitor)->getName());
	sendString(0, "\"%s\" is the overall winner!", (*scoreitor)->getName());
	sendString(0, "This game has finished.");
	sendString(0, "The next game will start soon...");

	ComsScoreMessage scoreMessage;
	ComsMessageSender::sendToAllPlayingClients(scoreMessage);

	// Reset the amount of time taken
	totalTime_ = 0.0f;
}

bool ServerScoreState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	totalTime_ += frameTime;
	return (totalTime_ > OptionsGame::instance()->getScoreTime());
}
