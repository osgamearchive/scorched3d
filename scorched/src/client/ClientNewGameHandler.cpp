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


#include <client/ClientNewGameHandler.h>
#include <client/ClientState.h>
#include <client/SpeedChange.h>
#include <client/MainBanner.h>
#include <client/ScorchedClient.h>
#include <dialogs/ConnectDialog.h>
#include <dialogs/ProgressDialog.h>
#include <landscape/Landscape.h>
#include <landscape/GlobalHMap.h>
#include <tank/TankContainer.h>
#include <tank/TankStart.h>
#include <tankgraph/TankRenderer.h>
#include <coms/ComsNewGameMessage.h>
#include <GLEXT/GLLenseFlare.h>
#include <sprites/ExplosionTextures.h>

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
	ComsMessageHandler::instance()->addHandler(
		"ComsNewGameMessage",
		this);
}

ClientNewGameHandler::~ClientNewGameHandler()
{

}

bool ClientNewGameHandler::processMessage(NetPlayerID &id,
		const char *messageType,
		NetBufferReader &reader)
{
	ComsNewGameMessage message;
	if (!message.readMessage(reader)) return false;

	// Generate new landscape
	if (!GlobalHMap::instance()->generateHMapFromDiff(
		message.getLevelMessage(),
		ProgressDialog::instance()))
	{
		dialogMessage("Scorched3D", "Failed to generate heightmap diff");
		return false;
	}

	// Generate all the graphics
	newGame();

	// Make sure the landscape has been optimized
	Landscape::instance()->recalculate(0, 0, 10000);
	return true;
}

void ClientNewGameHandler::clientNewGame()
{
	// Reset all tanks and generate landscape
	TankStart::newGame();

	// Generate all the graphics
	newGame();

	// Move all dead tanks into the game
	// (the server does through the tank sync messages)
	std::map<unsigned, Tank *> &tankList = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned, Tank *>::iterator itor;
	for (itor = tankList.begin();
		itor != tankList.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		tank->getState().setState(TankState::sNormal);
	}

	// Reset the current player id
	TankContainer::instance()->setCurrentPlayerId(0);

	// Make sure the landscape has been optimized
	Landscape::instance()->recalculate(0, 0, 10000);
}

void ClientNewGameHandler::newGame()
{
	// Calculate all the new landscape settings (graphics)
	Landscape::instance()->generate(ProgressDialog::instance());

	// Calculate other graphics that need to be built, only once
	static bool initOnce = false;
	if (!initOnce)
	{
		ExplosionTextures::instance()->createTextures(
			ProgressDialog::instance());
		GLLenseFlare::instance()->init(
			ProgressDialog::instance());
		initOnce = true;
	}
	TankRenderer::instance()->newGame();
	SpeedChange::instance()->resetSpeed();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	ScorchedClient::instance()->getMainLoop().getTimer().getTimeDifference();

	// Stimulate into the next round state
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimNextRound);
}
