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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <weapons/AccessoryStore.h>
#include <coms/NetLoopBack.h>
#include <common/Defines.h>
#include <common/Clock.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <engine/ActionController.h>
#include <engine/ScorchedCollisionHandler.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/LandscapeDefinitions.h>
#include <tankai/TankAIAdder.h>
#include <tankai/TankAIStore.h>
#include <scorched/ServerDialog.h>
#include <server/ServerMessageHandler.h>
#include <server/ServerPlayerReadyHandler.h>
#include <server/ServerTextHandler.h>
#include <server/ServerDefenseHandler.h>
#include <server/ServerPlayedMoveHandler.h>
#include <server/ServerAddPlayerHandler.h>
#include <server/ServerPlayerAimHandler.h>
#include <server/ServerBuyAccessoryHandler.h>
#include <server/ServerConnectHandler.h>
#include <server/ServerRegistration.h>
#include <server/ServerBrowserInfo.h>
#include <server/ServerState.h>
#include <server/ServerCommon.h>
#include <server/ScorchedServer.h>
#include <SDL/SDL.h>

Clock serverTimer;

bool startServer(bool local)
{
	// Setup the message handling classes
	if (!local)
	{
		// Only create a net server for the actual multiplayer case
		// A loopback is created by the client for a single player game 
		ScorchedServer::instance()->getContext().netInterface = 
			//new NetServer(new NetServerScorchedProtocol());
			new NetServer(new NetServerCompressedProtocol());
	}

	ScorchedServer::instance()->getOptionsGame().updateChangeSet();
	ScorchedServer::instance()->getNetInterface().setMessageHandler(
		&ScorchedServer::instance()->getComsMessageHandler());
	ScorchedServer::instance()->getComsMessageHandler().setConnectionHandler(
		ServerMessageHandler::instance());
	ServerConnectHandler::instance();
	ServerTextHandler::instance();
	ServerPlayerReadyHandler::instance();
	ServerPlayerAimHandler::instance();
	ServerPlayedMoveHandler::instance();
	ServerBuyAccessoryHandler::instance();
	ServerAddPlayerHandler::instance();
	ServerDefenseHandler::instance();

	// Init physics
	HeightMapCollision *hmcol = 
		new HeightMapCollision(&ScorchedServer::instance()->getContext());
	ScorchedServer::instance()->getActionController().getPhysics().setCollisionHandler(
		new ScorchedCollisionHandler(&ScorchedServer::instance()->getContext()));

	ScorchedServer::instance()->getOptionsTransient().reset();
	LandscapeDefinitions::instance();

	ServerState::setupStates(ScorchedServer::instance()->getGameState());

	// Add the server side bots
	// Add any new AIs
	TankAIStore::instance();
	TankAIAdder::addTankAIs(ScorchedServer::instance()->getContext());

	return true;
}

void serverMain()
{
	// Create the server states
	if (!startServer(false))
	{
		exit(1);
	}

	// Try to start the server
	NetServer *netServer = (NetServer *) 
		ScorchedServer::instance()->getContext().netInterface;
	if (!netServer->start(
		ScorchedServer::instance()->getOptionsGame().getPortNo()))
	{
		dialogMessage("Scorched3D Server", 
			"Failed to start the server.\n\n"
			"Ensure the specified port does not conflict with any other program.");
		exit(1);
	}

	ServerBrowserInfo::instance()->start();
 	if (ScorchedServer::instance()->getOptionsGame().getPublishServer()) 
	{
		ServerRegistration::instance()->start();
	}

	Logger::log(0, "Server started");
}

void serverLoop()
{
	// Main server loop:
	if (ScorchedServer::instance()->getContext().netInterface)
	{
		Logger::processLogEntries();
		ScorchedServer::instance()->getNetInterface().processMessages();
		ServerBrowserInfo::instance()->processMessages();
		ScorchedServer::instance()->getGameState().simulate(serverTimer.getTimeDifference());
	}
}

void consoleServer()
{
	printf("Starting Server...\n");
	ServerCommon::startFileLogger();
	serverMain();
	printf("Server Started.\n");

	for (;;)
	{
		SDL_Delay(10);
		serverLoop();
	}
}

