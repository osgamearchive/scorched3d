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
#include <client/ClientNewGameHandler.h>
#include <client/ClientState.h>
#include <client/ClientWaitState.h>
#include <graph/SpeedChange.h>
#include <graph/MainCamera.h>
#include <tankgraph/RenderTracer.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <engine/MainLoop.h>
#include <GLW/GLWWindowManager.h>
#include <client/ClientParams.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <coms/ComsNewGameMessage.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/ProgressDialog.h>
#include <dialogs/RulesDialog.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapemap/HeightMapSender.h>
#include <landscapedef/LandscapeDefinitions.h>
#include <landscape/Landscape.h>
#include <tank/TankContainer.h>
#include <tank/TankCamera.h>

ClientNewGameHandler *ClientNewGameHandler::instance_ = 0;

ClientNewGameHandler *ClientNewGameHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientNewGameHandler();
	}

	return instance_;
}

ClientNewGameHandler::ClientNewGameHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsNewGameMessage",
		this);
}

ClientNewGameHandler::~ClientNewGameHandler()
{

}

bool ClientNewGameHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsNewGameMessage message;
	if (!message.readMessage(reader)) return false;

	// Set the progress dialog nicities
	ProgressDialog::instance()->changeTip();
	LandscapeDefinitionsEntry *landscapeDefinition =
		ScorchedClient::instance()->getLandscapes().getLandscapeByName(
			message.getLevelMessage().getGroundMapsDefn().getName());
	if (landscapeDefinition)
	{
		const char *fileName = getDataFile(
			formatString("data/landscapes/%s", 
			landscapeDefinition->picture.c_str()));
		ProgressDialog::instance()->setIcon(fileName);
	}

	// Remove any old targets
	removeTargets();

	// Generate new landscape
	ScorchedClient::instance()->getLandscapeMaps().generateMaps(
		ScorchedClient::instance()->getContext(),
		message.getLevelMessage().getGroundMapsDefn(),
		ProgressDialogSync::instance());

	if (!HeightMapSender::generateHMapFromDiff(
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(),
		message.getLevelMessage().getHeightMap()))
	{
		dialogExit("Scorched3D", "Failed to generate heightmap diff");
	}

	// Calculate all the new landscape settings (graphics)
	Landscape::instance()->generate(ProgressDialogSync::instance());

	// Make sure the landscape has been optimized
	Landscape::instance()->reset(ProgressDialogSync::instance());

	RenderTracer::instance()->newGame();
	SpeedChange::instance()->resetSpeed();

	// Remove all actions (graphical objects) from the last round
	ScorchedClient::instance()->getActionController().clear();

	// Tell all tanks to update transient settings
	ScorchedClient::instance()->getTankContainer().clientNewGame();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	ScorchedClient::instance()->getMainLoop().getTimer().getTimeDifference();

	// Reset camera positions for each tank
	std::map<unsigned int, Tank *>::iterator tankItor;
	for (tankItor = ScorchedClient::instance()->getTankContainer().getAllTanks().begin();
		tankItor != ScorchedClient::instance()->getTankContainer().getAllTanks().end();
		tankItor++)
	{
		Tank *current = (*tankItor).second;
		current->getCamera().setCameraType(1);
	}

	// Tell the server we have finished processing the landscape
	ClientWaitState::instance()->sendClientReady();

	// Move into the wait state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();

	return true;
}

void ClientNewGameHandler::removeTargets()
{
	std::map<unsigned int, Target *> targets = // Note copy
		ScorchedClient::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		Target *target = (*itor).second;
		if (target->isTemp())
		{
			if (target->isTarget())
			{
				Target *removedTarget = 
					ScorchedClient::instance()->getTargetContainer().removeTarget(playerId);
				delete removedTarget;
			}
			else
			{
				Tank *removedTank = 
					ScorchedClient::instance()->getTankContainer().removeTank(playerId);
				delete removedTank;
			}
		}
	}
}
