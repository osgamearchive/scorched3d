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

#include <tankai/TankAIAdder.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIStrings.h>
#include <tank/TankColorGenerator.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <common/StatsLogger.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsMessageSender.h>

static unsigned int tankId_ = 2;

unsigned int TankAIAdder::getNextTankId()
{
	return ++tankId_;
}

void TankAIAdder::addTankAIs(ScorchedContext &context)
{
	// On the server
	// Ensure that we cannot add more ais than the server is setup for
	int maxComputerAIs = context.optionsGame->getNoMaxPlayers();
	for (int i=0; i<maxComputerAIs; i++)
	{
		const char *playerType = 
			context.optionsGame->getPlayerType(i);
		if (0 != stricmp(playerType, "Human"))
		{
			std::string botName = 
				context.optionsGame->getBotNamePrefix();
			botName += TankAIStrings::instance()->getAIPlayerName();

			addTankAI(context, playerType, "Random", botName.c_str());
		}
	}
}

void TankAIAdder::addTankAI(ScorchedContext &context,
							const char *aiName,
							const char *modelName,
							const char *name,
							bool raiseEvent)
{
	TankAI *ai = 
		TankAIStore::instance()->getAIByName(aiName);
	if (ai)
	{
		std::string newname = name;
		if (newname.size() == 0)
		{
			newname = 
				context.optionsGame->getBotNamePrefix();
			newname += TankAIStrings::instance()->getAIPlayerName();
		}

		Vector color = TankColorGenerator::instance()->getNextColor();
		TankModelId modelId(modelName);	
		Tank *tank = new Tank(
			context,
			getNextTankId(),
			0,
			newname.c_str(),
			color,
			modelId);
		char uniqueId[256];
		sprintf(uniqueId, "%s - computer", aiName);
		tank->setUnqiueId(uniqueId);
		tank->setTankAI(ai->getCopy(tank, &context));
		context.tankContainer->addTank(tank);

		if (context.optionsGame->getTeams() > 1)
		{
			tank->setTeam(context.optionsTransient->getLeastUsedTeam(
				*context.tankContainer));
		}

		Logger::log(0, 
			"Player connected \"%i\" \"%s\"",
			tank->getPlayerId(),
			tank->getName());

		StatsLogger::instance()->tankJoined(tank);

		if (raiseEvent)
		{
			// Tell the clients to create this tank
			ComsAddPlayerMessage addPlayerMessage(
				tank->getPlayerId(),
				tank->getName(),
				tank->getColor(),
				tank->getModel().getModelName(),
				tank->getDestinationId(),
				tank->getTeam()); 
			ComsMessageSender::sendToAllConnectedClients(addPlayerMessage);
		}
	}
}
