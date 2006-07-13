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
#include <client/SpeedChange.h>
#include <client/ClientWaitState.h>
#include <tankgraph/RenderTracer.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <engine/MainLoop.h>
#include <GLW/GLWWindowManager.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <coms/ComsNewGameMessage.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/ProgressDialog.h>
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeDefinitions.h>
#include <landscape/Landscape.h>
#include <landscape/HeightMapSender.h>
#include <tank/TankContainer.h>

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

bool ClientNewGameHandler::processMessage(unsigned int id,
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

	// Read the accessory prices
	if (!ScorchedClient::instance()->getAccessoryStore().
		readEconomyFromBuffer(reader)) return false;

	// Remove any old targets
	removeTargets();

	// Generate new landscape
	ScorchedClient::instance()->getLandscapeMaps().generateMaps(
		ScorchedClient::instance()->getContext(),
		message.getLevelMessage().getGroundMapsDefn(),
		ProgressDialog::instance());

	if (!HeightMapSender::generateHMapFromDiff(
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(),
		message.getLevelMessage().getHeightMap()))
	{
		dialogExit("Scorched3D", "Failed to generate heightmap diff");
	}

	// Calculate all the new landscape settings (graphics)
	Landscape::instance()->generate(ProgressDialog::instance());

	// Make sure that objects have been removed from the landscape
	removeObjects();

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

	// Make sure the landscape has been optimized
	Landscape::instance()->reset(ProgressDialog::instance());

	// Tell the server we have finished processing the landscape
	ClientWaitState::instance()->sendClientReady();

	// Move into the wait state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();
	return true;
}

void ClientNewGameHandler::removeObjects()
{
	HeightMap &hMap = 
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap();

	// Debugging code that draws the area that has changed
	/*GLBitmap &bmap = 
		Landscape::instance()->getMainMap();
	GLubyte *bits = bmap.getBits();
	for (int y=0; y<bmap.getHeight(); y++)
	{
		for (int x=0; x<bmap.getWidth(); x++)
		{
			float fi = float(x) / float(bmap.getWidth()) * float(hMap.getMapWidth());
			float fj = float(y) / float(bmap.getHeight()) * float(hMap.getMapHeight());

			int i = (int) fi;
			int j = (int) fj;

			if (hMap.getHeight(i, j) != hMap.getBackupHeight(i, j))
			{
				bits[0] = 255;
				bits[1] = 255;
				bits[2] = 255;
			}

			bits+=3;
		}
	}*/

	int count = 0;
	for (int j=0; j<(hMap.getMapHeight() + 1); j++)
	{
		for (int i=0; i<(hMap.getMapWidth() + 1); i++)
		{
			if (hMap.getHeight(i, j) != hMap.getBackupHeight(i, j))
			{
				count++;

				ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().
					getObjects().removeObjects(
					ScorchedClient::instance()->getContext(), 
					i, j, 0, 0);
			}
		}
	}
	//Logger::log(formatString("%i differing height entries", count));
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
			Target *removedTarget = 
				ScorchedClient::instance()->getTargetContainer().removeTarget(playerId);
			delete removedTarget;
		}
	}
}
