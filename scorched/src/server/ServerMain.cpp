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

#include <windows.h>
#include <stdlib.h>
#include <common/Defines.h>
#include <common/Timer.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <coms/ComsGateway.h>
#include <engine/GameState.h>
#include <engine/ActionController.h>
#include <engine/ScorchedCollisionHandler.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/GlobalHMap.h>
#include <tankai/TankAIAdder.h>
#include <tankai/TankAIStore.h>
#include <tank/TankContainer.h>
#include <scorched/ServerDialog.h>
#include <server/ServerMessageHandler.h>
#include <server/ServerPlayerReadyHandler.h>
#include <server/ServerTextHandler.h>
#include <server/ServerDefenseHandler.h>
#include <server/ServerPlayedMoveHandler.h>
#include <server/ServerBuyAccessoryHandler.h>
#include <server/ServerConnectHandler.h>
#include <server/ServerRegistration.h>
#include <server/ServerBrowserInfo.h>
#include <server/ServerState.h>
#include <SDL/SDL.h>

Timer serverTimer;

bool serverMain()
{
	// Set the options so we are a server
	OptionsParam::instance()->setServerFile("Hmm");

	// Setup the message handling classes
	ComsGateway::instance()->setMessageHandler(
		ComsMessageHandler::instance());
	ComsMessageHandler::instance()->setConnectionHandler(
		ServerMessageHandler::instance());
	ServerConnectHandler::instance();
	ServerTextHandler::instance();
	ServerPlayerReadyHandler::instance();
	ServerPlayedMoveHandler::instance();
	ServerBuyAccessoryHandler::instance();
	ServerDefenseHandler::instance();

	// Init physics
	ActionController::instance();
	HeightMapCollision::instance()->create(
		GlobalHMap::instance()->getHMap());
	ScorchedCollisionHandler::instance();
	OptionsTransient::instance()->reset();

	ServerState::setupStates();

	// Add the server side bots
	TankAIStore::instance();
	TankAIAdder::addTankAIs();

	// Try to start the server
	if (!ComsGateway::instance()->start(
		OptionsGame::instance()->getPortNo(),
		OptionsGame::instance()->getNoMaxPlayers() - 
		TankContainer::instance()->getNoOfTanks()))
	{
		dialogMessage("Scorched3D Server", 
			"Failed to start the server.\n\n"
			"Ensure the specified port does not conflict with any other program.");
		exit(1);
	}

	ServerBrowserInfo::instance()->start();
 	if (OptionsGame::instance()->getPublishServer()) 
	{
		ServerRegistration::instance()->start();
	}

	Logger::log(0, "Server started");
	return true;
}

void serverLoop()
{
	// Main server loop:
	if (ComsGateway::instance()->started())
	{
		Logger::processLogEntries();
		ComsGateway::instance()->processMessages();
		ServerBrowserInfo::instance()->processMessages();
		GameState::instance()->simulate(serverTimer.getTimeDifference());
	}
}
