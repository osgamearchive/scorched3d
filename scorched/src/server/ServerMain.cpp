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
#include <common/Defines.h>
#include <common/Timer.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <coms/NetServer.h>
#include <coms/NetMessageHandler.h>
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
#include <server/ServerState.h>
#include <SDL/SDL.h>

Timer serverTimer;

#ifdef _NO_SERVER_ASE_
#include <irc/ServerGameInfo.h>
#else
extern "C" {
#include <ASE/ASEQuerySDK.h>
}
#endif

void serverCleanup()
{
	if (NetServer::instance()->started())
	{
#ifdef _NO_SERVER_ASE_
 		ServerGameInfo::instance()->Stop();
#else
		ASEQuery_shutdown();
		WSACleanup();
#endif
	}
}

bool serverMain()
{
	// Create an exit handler to free all used resources
	atexit(serverCleanup);

	// Set the options so we are a server
	OptionsParam::instance()->setServerFile("Hmm");

#ifndef _NO_SERVER_ASE_
	WORD WSAVerReq = (MAKEWORD((1),(1)));
	WSADATA WSAData;
	if (WSAStartup(WSAVerReq, &WSAData) != 0)
	{
		dialogMessage("Scorched3D Server", 
			"Failed to start windows sockets.");
		return false;
	}
#endif

	// Setup the message handling classes
	NetMessageHandler::instance()->setMessageHandler(
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
	if (!NetServer::instance()->start(
		OptionsGame::instance()->getPortNo(),
		OptionsGame::instance()->getNoMaxPlayers() - 
		TankContainer::instance()->getNoOfTanks()))
	{
		dialogMessage("Scorched3D Server", 
			"Failed to start the server.\n\n"
			"Ensure the specified port does not conflict with any other program.");
		return false;
	}

#ifdef _NO_SERVER_ASE_
 	if (OptionsGame::instance()->getPublishServer()) 
	{
 		// launch game info servers
 		ServerGameInfo::instance()->Start(
			OptionsGame::instance()->getPortNo());
	}
#else
	char *publishAddress = 0;
	if ((0 != strcmp("AutoDetect", OptionsGame::instance()->getPublishAddress())) &&
		OptionsGame::instance()->getPublishAddress()[0])
	{
		publishAddress = (char *) OptionsGame::instance()->getPublishAddress();
	}

	if (!ASEQuery_initialize(OptionsGame::instance()->getPortNo(), 
		(OptionsGame::instance()->getPublishServer()?1:0),
		publishAddress))
	{
		dialogMessage("Scorched3D Server", 
			"Failed to initialize the ASE server reporting.");
		return false;
	}
#endif

	Logger::log(0, "Server started");

	return true;
}

void serverLoop()
{
	// Main server loop:
	if (NetServer::instance()->started())
	{
		Logger::processLogEntries();
		NetMessageHandler::instance()->processMessages();
		GameState::instance()->simulate(serverTimer.getTimeDifference());
#ifndef _NO_SERVER_ASE_
		ASEQuery_check();
#endif
	}
}
