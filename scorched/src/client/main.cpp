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


#include <client/LauncherDialog.h>
#include <client/ScorchedDialog.h>
#include <client/MainCamera.h>
#include <client/resource.h>
#include <client/OptionsClient.h>
#include <common/ARGParser.h>
#include <common/Keyboard.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/Mouse.h>
#include <common/Display.h>
#include <common/Gamma.h>
#include <common/Sound.h>
#include <common/Registry.h>
#include <coms/NetMessageHandler.h>
#include <coms/NetClient.h>
#include <engine/MainLoop.h>
#include <engine/GameState.h>
#include <math.h>
#include <float.h>
#include <GLEXT/GLStateExtension.h>
#include <SDL/SDL.h>
#ifdef _WIN32
#include <commctrl.h>
#endif

static char *optionsFile = PKGDIR "data/options.xml";

char *scorchedOptionsFile = PKGDIR "data/scorchedoptions.xml";

bool parseCommandLine(int argc, char *argv[],
					  bool &skipLauncher)
{
	OptionsGame::instance()->setShotTime(0);

	ARGParser aParser;
	OptionsClient::instance()->addToArgParser(aParser);
	aParser.addEntry("-nooptions", &skipLauncher, 
		"Don't display options screen");
	if (!aParser.parse(argc, argv)) return FALSE;

	return TRUE;
}

// Compilers from Borland report floating-point exceptions in a manner 
// that is incompatible with Microsoft Direct3D.
int _matherr(struct _exception  *e)
{
    e;               // Dummy reference to catch the warning.
    return 1;        // Error has been handled.
}

void cleanup()
{
	Gamma::instance()->reset();
	Sound::instance()->destroy();

	if (!OptionsTransient::instance()->getServer())
	{
		// save options to a file
		OptionsGame::instance()->writeOptionsToFile(optionsFile);
	}

	SDL_Quit();
}

int main(int argc, char *argv[])
{
	// Check we are in the correct directory
	FILE *checkfile = fopen(PKGDIR "data/autoexec.xml", "r");
	if (!checkfile)
	{
		MessageBox(NULL, 
			"Warning: This game requires the Scorched3D data directory to run.\n"
			"Your machine does not appear to have the Scorched3D data directory in\n"
			"the same directory as the scorched.exe file.\n\n"
			"If Scorched3D does not run please re-install Scorched3D.",
			"Scorched 3D",
			MB_OK | MB_ICONWARNING);
		exit(1);
	}
	fclose(checkfile);

	// Read options from a file
	if (!OptionsGame::instance()->readOptionsFromFile(optionsFile) ||
		!OptionsClient::instance()->readOptionsFromFile(scorchedOptionsFile))
	{
		return 0;
	}

#ifdef _WIN32
	// Do a check for directX 8.0
	HINSTANCE hD3D8DLL = LoadLibrary( "D3D8.DLL" );
	if(hD3D8DLL == NULL)
	{
		MessageBox(NULL, 
			"Warning: This game requires DirectX 8.0 or later to run.\n"
			"Your machine does not appear to have DirectX 8.0 installed.\n\n"
			"The latest version of DirectX can be obtained freely from Microsoft at:\n"
			"http://www.microsoft.com/windows/directx/default.asp",
			"DirectX Version 8.0",
			MB_OK | MB_ICONWARNING);
	}
	FreeLibrary(hD3D8DLL);

	// Disable floating point exceptions. 
	_control87(MCW_EM,MCW_EM);

	// Load accelerators
	HACCEL hAccelTable = LoadAccelerators((HINSTANCE) GetModuleHandle(NULL), 
		(LPCTSTR)IDC_LAUNCHER);
	InitCommonControls();

	// Set directory location for ASE
	{
		Registry baseRegistry("Software\\Scorched");
		CHAR directoryName[1024];
		GetCurrentDirectory(1024, directoryName);
		baseRegistry.setString("", "Location", directoryName);
	}

#endif

	// Parse command line
	bool skipLauncher = false;
	if (!parseCommandLine(argc, argv, skipLauncher))
	{
		return false;
	}

#ifdef _WIN32
	if (!skipLauncher)
	{
		// Get the settings from the launcher dialog
		if (!createLauncherDialog()) return FALSE;
	}
#endif

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		MessageBox(NULL, 
			"Warning: This game uses the SDL library.\n"
			"This library failed to initialize.\n",
			"Scorched 3D",
			MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	// Create an exit handler to free all used resources
	atexit(cleanup);

	// Try to create the main scorched3d game window
	if (!createScorchedWindow()) return false;
	OptionsClient::instance()->addToConsole();

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
				Display::instance()->changeSettings(
					event.resize.w,event.resize.h, 
					OptionsClient::instance()->getFullScreen());
				MainCamera::instance()->getCamera().setWindowSize(
					event.resize.w, event.resize.h);
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

		if (OptionsTransient::instance()->getServer())
		{
			NetMessageHandler::instance()->processMessages();
		}
	}

	return TRUE;
}

