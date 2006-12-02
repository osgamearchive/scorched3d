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

#include <set>
#include <tankai/TankAIAdder.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIStrings.h>
#include <tank/TankColorGenerator.h>
#include <tank/TankContainer.h>
#include <tank/TankModelStore.h>
#include <tank/TankAvatar.h>
#include <tank/TankModelContainer.h>
#include <server/ServerState.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsPlayerStateMessage.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <common/Defines.h>

static unsigned int tankId_ = TankAIAdder::MIN_TANK_ID;

unsigned int TankAIAdder::getNextTankId(ScorchedContext &context)
{
	if (++tankId_ >= MAX_TANK_ID) tankId_ = MIN_TANK_ID;

	while (context.targetContainer->getTargetById(tankId_))
	{
		++tankId_;
	}

	return tankId_;
}

static unsigned int targetId_ = TankAIAdder::MIN_TARGET_ID;

unsigned int TankAIAdder::getNextTargetId(ScorchedContext &context)
{
	if (++targetId_ >= MAX_TARGET_ID) targetId_ = MIN_TARGET_ID;

	while (context.targetContainer->getTargetById(targetId_))
	{
		++targetId_;
	}

	return targetId_;
}

void TankAIAdder::addTankAIs(ScorchedServer &context)
{
	// On the server
	// Ensure that we cannot add more ais than the server is setup for
	int maxComputerAIs = context.getOptionsGame().getNoMaxPlayers();
	for (int i=0; i<maxComputerAIs; i++)
	{
		const char *playerType = 
			context.getOptionsGame().getPlayerType(i);
		if (0 != stricmp(playerType, "Human"))
		{
			addTankAI(context, playerType);
		}
	}
}

void TankAIAdder::addTankAI(ScorchedServer &context, const char *aiName)
{
	TankAI *ai = context.getTankAIs().getAIByName(aiName);
	if (ai)
	{
		// Create our uniqueid
		char uniqueId[256];
		{
			std::set<int> usedIds;
			snprintf(uniqueId, 256, "%s - computer - %%i", aiName);
			std::map<unsigned int, Tank *> &playingTanks = 
				context.getTankContainer().getPlayingTanks();
			std::map<unsigned int, Tank *>::iterator playingItor;
			for (playingItor = playingTanks.begin();
				playingItor != playingTanks.end();
				playingItor++)
			{
				Tank *current = (*playingItor).second;
				if (current->getDestinationId() == 0)
				{
					int id = 1;
					if (sscanf(current->getUniqueId(), uniqueId, &id) == 1)
					{
						usedIds.insert(id);
					}
				}
			}

			int uniqueIdCount = 1;
			while (usedIds.find(uniqueIdCount) != usedIds.end()) uniqueIdCount++;

			snprintf(uniqueId, 256, "%s - computer - %i", aiName, uniqueIdCount);
		}

		// Chose this tanks team
		int team = 0;
		if (context.getOptionsGame().getTeams() > 1)
		{
			team = context.getOptionsTransient().getLeastUsedTeam(
				context.getTankContainer());
		}

		// For the tank ai's name
		std::string newname = 
			context.getOptionsGame().getBotNamePrefix();
		newname += TankAIStrings::instance()->getAIPlayerName();

		// Form the tank ai model
		Vector color = TankColorGenerator::instance()->getNextColor(
			context.getTankContainer().getPlayingTanks());
		TankModel *tankModel = 
			context.getTankModels().getRandomModel(team, false);

		// Create the new tank
		Tank *tank = new Tank(
			context.getContext(),
			getNextTankId(context.getContext()),
			0,
			newname.c_str(),
			color,
			tankModel->getName(),
			tankModel->getTypeName());

		tank->getAvatar().loadFromFile("computer.png");
		tank->setUniqueId(uniqueId);
		tank->setTankAI(ai->getCopy(tank));
		context.getTankContainer().addTank(tank);

		if (context.getOptionsGame().getTeams() > 1)
		{
			tank->setTeam(team);
		}

		Logger::log(
			formatString("Player connected dest=\"%i\" id=\"%i\" name=\"%s\" unique=[%s]",
			tank->getDestinationId(),
			tank->getPlayerId(),
			tank->getName(),
			tank->getUniqueId()));

		StatsLogger::instance()->tankConnected(tank);
		StatsLogger::instance()->tankJoined(tank);

		if (true) // Raise an event
		{
			// Tell the clients to create this tank
			ComsAddPlayerMessage addPlayerMessage(
				tank->getPlayerId(),
				tank->getName(),
				tank->getColor(),
				tank->getModelContainer().getTankModelName(),
				tank->getModelContainer().getTankTypeName(),
				tank->getDestinationId(),
				tank->getTeam(),
				""); 
			addPlayerMessage.setPlayerIconName(tank->getAvatar().getName());
			addPlayerMessage.getPlayerIcon().addDataToBuffer(
				tank->getAvatar().getFile().getBuffer(),
				tank->getAvatar().getFile().getBufferUsed());
			ComsMessageSender::sendToAllConnectedClients(addPlayerMessage);
		}

		if (context.getGameState().getState() == ServerState::ServerStateTooFewPlayers ||
			context.getGameState().getState() == ServerState::ServerStateStarting)
		{
			ComsPlayerStateMessage message(true);
			ComsMessageSender::sendToAllConnectedClients(message);
		}
	}
}
