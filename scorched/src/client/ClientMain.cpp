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
#include <client/Main2DCamera.h>
#include <client/ScorchedClient.h>
#include <client/ClientAdmin.h>
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
#include <client/ClientFileHandler.h>
#include <client/ClientPlayerAimHandler.h>
#include <client/ClientActionsHandler.h>
#include <client/ClientNextRoundHandler.h>
#include <client/ClientLastChanceHandler.h>
#include <client/ClientDefenseHandler.h>
#include <client/ClientPlayerStatusHandler.h>
#include <client/ClientKeepAliveSender.h>
#include <client/ClientState.h>
#include <client/WindowSetup.h>
#include <server/ScorchedServer.h>
#include <GLEXT/GLConsoleFileReader.h>
#include <GLEXT/GLConsole.h>
#include <GLW/GLWWindowSkinManager.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/SkyRoofCollision.h>
#include <engine/MainLoop.h>
#include <engine/ScorchedCollisionHandler.h>
#include <engine/ActionController.h>
#include <dialogs/ProgressDialog.h>
#include <coms/NetServer.h>
#include <coms/NetLoopBack.h>
#include <common/OptionsDisplay.h>
#include <common/ARGParser.h>
#include <common/Keyboard.h>
#include <common/Logger.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/Keyboard.h>
#include <common/ProgressCounter.h>
#include <common/Mouse.h>
#include <common/Display.h>
#include <common/Gamma.h>
#include <common/Clock.h>
#include <sound/Sound.h>
#include <SDL/SDL.h>

static int mouseEventCount = 0;
static bool paused = false;

bool initHardware(ProgressCounter *progressCounter)
{
	progressCounter->setNewPercentage(0.0f);
	progressCounter->setNewOp("Initializing Keyboard");
	if (!Keyboard::instance()->init())
	{
		dialogMessage("Scorched3D Keyboard", 
			"SDL failed to aquire keyboard.\n"
#ifdef WIN32
			"Is DirectX 5.0 installed?"
#endif
		);
		return false;
	}
	progressCounter->setNewOp("Loading Keyboard Bindings");
	if (!Keyboard::instance()->loadKeyFile())
	{
		dialogMessage("Scorched3D Keyboard", 
			"Failed to process keyboard file keys.xml");
		return false;
	}

	if (!OptionsDisplay::instance()->getNoSound())
	{
		progressCounter->setNewOp("Initializing Sound SW");
		if (!Sound::instance()->init(
			OptionsDisplay::instance()->getSoundChannels()))
		{
			dialogMessage("Scorched3D Sound", 
				"Failed to aquire sound.\n"
#ifdef WIN32
				"Is DirectX 5.0 installed?\n"
#endif
				"Is anything else currently using the sound card?");
		}
		Sound::instance()->getDefaultListener()->setGain(
			float(OptionsDisplay::instance()->getSoundVolume()) / 128.0f);
	}
	return true;
}

bool startClient(ProgressCounter *progressCounter)
{
	progressCounter->setNewPercentage(0.0f);
	progressCounter->setNewOp("Initializing Client");
	ScorchedClient::instance();
	if (OptionsParam::instance()->getConnectedToServer())
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
	ClientAdmin::instance();
	ScorchedClient::instance()->getNetInterface().setMessageHandler(
		&ScorchedClient::instance()->getComsMessageHandler());
	ScorchedClient::instance()->getComsMessageHandler().setConnectionHandler(
		ClientMessageHandler::instance());
	ClientTextHandler::instance();
	ClientConnectionAcceptHandler::instance();
	ClientAddPlayerHandler::instance();
	ClientNewGameHandler::instance();
	ClientRmPlayerHandler::instance();
	ClientPlayerAimHandler::instance();
	ClientStartTimerHandler::instance();
	ClientFileHandler::instance();
	ClientGameStoppedHandler::instance();
	ClientStartGameHandler::instance();
	ClientNextRoundHandler::instance();
	ClientLastChanceHandler::instance();
	ClientGameStateHandler::instance();
	ClientDefenseHandler::instance();
	ClientActionsHandler::instance();
	ClientBuyAccessoryHandler::instance();
	ClientPlayerStatusHandler::instance();
	ClientScoreHandler::instance();

	HeightMapCollision *hmcol = 
		new HeightMapCollision(&ScorchedClient::instance()->getContext());
	SkyRoofCollision *srcol = 
		new SkyRoofCollision(&ScorchedClient::instance()->getContext());
	ScorchedClient::instance()->getActionController().getPhysics().setCollisionHandler(
		new ScorchedCollisionHandler(&ScorchedClient::instance()->getContext()));

	progressCounter->setNewPercentage(0.0f);
	progressCounter->setNewOp("Initializing Windows");
	if (!GLWWindowSkinManager::instance()->loadWindows()) return false;
	WindowSetup::setup();
	std::string errorString;
	if (!GLConsoleFileReader::loadFileIntoConsole(getDataFile("data/autoexec.xml"), errorString))
	{
		dialogMessage("Failed to parse data/autoexec.xml", errorString.c_str());
		return false;
	}
	return true;
}

bool clientEventLoop()
{
	static SDL_Event event;
	bool idle = true;
	if (SDL_PollEvent(&event))
	{
		idle = false;
		switch (event.type)
		{
		/* keyboard events */
		case SDL_KEYUP:
			break;
		case SDL_KEYDOWN:
			if (OptionsParam::instance()->getScreenSaverMode()) 
			{
				if (!(SDL_GetModState() & KMOD_LSHIFT))
				{
					ScorchedClient::instance()->getMainLoop().exitLoop();
				}
			}

			/* keyevents are handled in mainloop */
			Keyboard::instance()->processKeyboardEvent(event);
			break;

			/* mouse events */
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
			if (OptionsParam::instance()->getScreenSaverMode()) 
			{
				if (!(SDL_GetModState() & KMOD_LSHIFT))
				{
					if (++ mouseEventCount > 5)
					{
						ScorchedClient::instance()->getMainLoop().exitLoop();
					}
				}
			}

			Mouse::instance()->processMouseEvent(event);
			break;
		case SDL_ACTIVEEVENT:
			paused = (event.active.gain == 0);
			break;
		case SDL_VIDEORESIZE:
			/*Display::instance()->changeSettings(
				event.resize.w,event.resize.h, 
				OptionsDisplay::instance()->getFullScreen());*/
			MainCamera::instance()->getCamera().setWindowSize(
				event.resize.w, event.resize.h);
			Main2DCamera::instance()->getViewPort().setWindowSize(
				event.resize.w, event.resize.h);
				
			break;
		case SDL_QUIT:
			ScorchedClient::instance()->getMainLoop().exitLoop();
			break;
		}
	}

	ClientKeepAliveSender::instance()->sendKeepAlive();

	return idle;
}

bool clientMain()
{
	if (!createScorchedWindow()) return false;

	ClientState::setupGameState(
		OptionsParam::instance()->getConnectedToServer());

	ProgressCounter progressCounter;
	ProgressDialog::instance()->changeTip();
	progressCounter.setUser(ProgressDialog::instance());
	progressCounter.setNewPercentage(0.0f);
	if (!initHardware(&progressCounter)) return false;
	if (!startClient(&progressCounter)) return false;
	
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		if (!startServer(true)) return false;
	}

	// Try to create the main scorched3d game window
	OptionsDisplay::instance()->addToConsole();

	// Enter the SDL main loop to process SDL events
	Clock loopClock;
	bool timeLoop = false;
	for (;;)
	{
		bool idle = clientEventLoop();

		if(timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 1");

		if (!ScorchedClient::instance()->getMainLoop().mainLoop()) break;
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 2");

		if ((!paused) && (idle) ) ScorchedClient::instance()->getMainLoop().draw();
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 3");

		if (paused) SDL_Delay(100);  // Otherwise when not drawing graphics its an infinite loop
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 4");

		if (!OptionsParam::instance()->getConnectedToServer())
		{
			serverLoop();
		}
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 5");

		Logger::processLogEntries();
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 6");

		if (ScorchedClient::instance()->getContext().netInterface)
		{
			ScorchedClient::instance()->getNetInterface().processMessages();
		}
		if (timeLoop) if (loopClock.getTimeDifference() > 0.1f) 
			GLConsole::instance()->addLine(false, "Clock clientMain 7");
	}

	Gamma::instance()->reset();
	Sound::instance()->destroy();
	SDL_Quit();

	return true;
}

