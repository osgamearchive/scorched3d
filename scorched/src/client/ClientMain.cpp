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

#include <server/ServerMain.h>
#include <client/ClientDialog.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <client/ClientGameStoppedHandler.h>
#include <client/ClientMessageHandler.h>
#include <client/ClientTextHandler.h>
#include <client/ClientStartGameHandler.h>
#include <client/ClientScoreHandler.h>
#include <client/ClientAddPlayerHandler.h>
#include <client/ClientNewGameHandler.h>
#include <client/ClientBuyAccessoryHandler.h>
#include <client/ClientConnectionAcceptHandler.h>
#include <client/ClientRmPlayerHandler.h>
#include <client/ClientGameStateHandler.h>
#include <client/ClientStartTimerHandler.h>
#include <client/ClientActionsHandler.h>
#include <client/ClientNextRoundHandler.h>
#include <client/ClientDefenseHandler.h>
#include <client/ClientState.h>
#include <client/WindowSetup.h>
#include <server/ScorchedServer.h>
#include <GLEXT/GLConsoleFileReader.h>
#include <tankai/TankAIStore.h>
#include <landscape/HeightMapCollision.h>
#include <tankgraph/TankModelStore.h>
#include <engine/ScorchedCollisionHandler.h>
#include <weapons/AccessoryStore.h>
#include <coms/NetServer.h>
#include <coms/NetLoopBack.h>
#include <common/OptionsDisplay.h>
#include <common/ARGParser.h>
#include <common/Keyboard.h>
#include <common/Logger.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Mouse.h>
#include <common/Display.h>
#include <common/Gamma.h>
#include <common/Sound.h>
#include <SDL/SDL.h>

void startClient()
{
	ScorchedClient::instance();

	bool useServer = (OptionsParam::instance()->getConnectedToServer());
	ClientState::setupGameState(useServer);
	if (useServer)
	{
		ScorchedClient::instance()->getContext().netInterface = 
			//new NetServer(new NetServerScorchedProtocol());
			new NetServer(new NetServerCompressedProtocol());
	}
	else
	{
		NetLoopBack *serverLoopBack = new NetLoopBack(NetLoopBack::ServerLoopBackID);
		ScorchedServer::instance()->getContext().netInterface = serverLoopBack;
		NetLoopBack *clientLoopBack = new NetLoopBack(NetLoopBack::ClientLoopBackID);
		ScorchedClient::instance()->getContext().netInterface = clientLoopBack;
		serverLoopBack->setLoopBack(clientLoopBack);
		clientLoopBack->setLoopBack(serverLoopBack);
	}

	// Setup the coms handlers
	ScorchedClient::instance()->getNetInterface().setMessageHandler(
		&ScorchedClient::instance()->getComsMessageHandler());
	ScorchedClient::instance()->getComsMessageHandler().setConnectionHandler(
		ClientMessageHandler::instance());
	ClientTextHandler::instance();
	ClientConnectionAcceptHandler::instance();
	ClientAddPlayerHandler::instance();
	ClientNewGameHandler::instance();
	ClientRmPlayerHandler::instance();
	ClientStartTimerHandler::instance();
	ClientGameStoppedHandler::instance();
	ClientStartGameHandler::instance();
	ClientNextRoundHandler::instance();
	ClientGameStateHandler::instance();
	ClientDefenseHandler::instance();
	ClientActionsHandler::instance();
	ClientBuyAccessoryHandler::instance();
	ClientScoreHandler::instance();

	HeightMapCollision *hmcol = 
		new HeightMapCollision(&ScorchedClient::instance()->getContext());
	ScorchedClient::instance()->getActionController().getPhysics().setCollisionHandler(
		new ScorchedCollisionHandler(&ScorchedClient::instance()->getContext()));

	if (!TankModelStore::instance()->loadTankMeshes())
	{
		dialogMessage("Scorched 3D", "Failed to load all tank models");		
		exit(1);
	}

	TankAIStore::instance();

	WindowSetup::setup();

	std::string errorString;
	if (!GLConsoleFileReader::loadFileIntoConsole(PKGDIR "data/autoexec.xml", errorString))
	{
		dialogMessage("Failed to parse data/autoexec.xml", errorString.c_str());
		exit(1);
	}
}

void clientMain()
{
	if (!createScorchedWindow()) return;

	startClient();
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		startServer(true);
	}

	// Try to create the main scorched3d game window
	OptionsDisplay::instance()->addToConsole();

	// Enter the SDL main loop to process SDL events
	bool paused = false;
	SDL_Event event;
	for (;;)
	{
		bool idle = false;
		if (SDL_PollEvent(&event))
		{
			idle = false;
			switch (event.type)
			{
			/* keyboard events */
			case SDL_KEYUP:
				break;
			case SDL_KEYDOWN:
				/* keyevents are handled in mainloop */
				Keyboard::instance()->processKeyboardEvent(event);
				break;

				/* mouse events */
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				Mouse::instance()->processMouseEvent(event);
				break;
			case SDL_ACTIVEEVENT:
				paused = (event.active.gain == 0);
				break;
			case SDL_VIDEORESIZE:
				/*Display::instance()->changeSettings(
					event.resize.w,event.resize.h, 
					OptionsDisplay::instance()->getFullScreen());
				MainCamera::instance()->getCamera().setWindowSize(
					event.resize.w, event.resize.h);*/
				break;
			case SDL_QUIT:
				ScorchedClient::instance()->getMainLoop().exitLoop();
				break;
			}
		} else {
			idle = true;
		}

		if (!ScorchedClient::instance()->getMainLoop().mainLoop()) break;
		if ((!paused) && (idle) ) ScorchedClient::instance()->getMainLoop().draw();
		if (paused) SDL_Delay(100);  // Otherwise when not drawing graphics its an infinite loop

		if (!OptionsParam::instance()->getConnectedToServer())
		{
			serverLoop();
		}

		Logger::processLogEntries();
		if (ScorchedClient::instance()->getContext().netInterface)
		{
			ScorchedClient::instance()->getNetInterface().processMessages();
		}
	}

	Gamma::instance()->reset();
	Sound::instance()->destroy();
	SDL_Quit();
}

