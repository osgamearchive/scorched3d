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


#include <client/ClientDialog.h>
#include <client/MainCamera.h>
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
#include <coms/NetMessageHandler.h>
#include <coms/NetClient.h>
#include <engine/MainLoop.h>
#include <engine/GameState.h>
#include <SDL/SDL.h>

void clientMain()
{
	// Try to create the main scorched3d game window
	if (!createScorchedWindow()) return;
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
				MainLoop::instance()->exitLoop();
				break;
			}
		} else {
			idle = true;
		}

		if (!MainLoop::instance()->mainLoop()) break;
		if ((!paused) && (idle) ) MainLoop::instance()->draw();
		if (paused) SDL_Delay(100);  // Otherwise when not drawing graphics its an infinite loop

		Logger::processLogEntries();
		if ((OptionsParam::instance()->getConnectedToServer()))
		{
			NetMessageHandler::instance()->processMessages();
		}
	}

	Gamma::instance()->reset();
	Sound::instance()->destroy();
	SDL_Quit();
}

