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
#include <tank/TankContainer.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>

void TankAIAdder::addTankAIs()
{
	// On the server
	// Ensure that we cannot add more ais than the server is setup for
	int maxComputerAIs = OptionsGame::instance()->getNoMaxPlayers();

	// On a client however add all ais that are in the options file
	// On the client the OptionsGame::instance()->getNoMaxPlayers()
	// Specified the max number of clients that the user will be asked
	// to define in the PlayersDialog
	if (!OptionsParam::instance()->getOnServer())
	{
		maxComputerAIs = 24;
	}

	for (int i=0; i<maxComputerAIs; i++)
	{
		const char *playerType = 
			OptionsGame::instance()->getPlayerType(i);
		if (0 != stricmp(playerType, "Human"))
		{
			TankAIComputer *ai = 
				TankAIStore::instance()->getAIByName(playerType);
			if (ai)
			{
				Vector color = TankColorGenerator::instance()->getNextColor();
				// Computer AIs start at 1000
				static unsigned int tankId = 2;
				// Do not give a model, this will actualy get a random
				// tank model assigned to the tank
				TankModelId modelId("Random");
				++tankId;

				std::string botName = 
					OptionsGame::instance()->getBotNamePrefix();
				botName += TankAIStrings::instance()->getPlayerName();
				Tank *tank = new Tank(
					tankId,
					botName.c_str(),
					color,
					modelId);
				tank->setTankAI(ai->getCopy(tank));
				TankContainer::instance()->addTank(tank);
			}
		}
	}
}
