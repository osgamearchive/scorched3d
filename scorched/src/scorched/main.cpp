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


#include <wx/wx.h>
#include <SDL/SDL.h>
#include <client/ClientMain.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/ARGParser.h>
#include <common/Registry.h>
#include <common/Defines.h>
#include <common/Resources.h>
#include <common/OptionsTransient.h>
#include <locale.h>
#include <math.h>
#include <float.h>

extern bool wxWindowInit;
extern bool wxWindowExit;
char *displayOptions = PKGDIR "data/display.xml";
char *resourceFile = PKGDIR "data/resource.xml";
char scorched3dAppName[128];
static bool allowExceptions = false;

bool parseCommandLine(int argc, char *argv[])
{
	if (!Resources::instance()->main.initFromFile(resourceFile)) return false;

	// Read display options from a file
	if (!OptionsDisplay::instance()->readOptionsFromFile(displayOptions))
	{
		return false;
	}
	if (!OptionsDisplay::instance()->getUniqueUserId()[0])
	{
		char buffer[128];
		sprintf(buffer, "%i-%i-%i", rand(), rand(), rand());
		OptionsDisplay::instance()->setUniqueUserId(buffer);
	}
	// Read options from command line
	ARGParser aParser;
	if (!OptionEntryHelper::addToArgParser(
		OptionsParam::instance()->getOptions(), aParser)) return false;
	aParser.addEntry("-allowexceptions", &allowExceptions, 
					 "Allows any program exceptions to be thrown (core dumps)");
	if (!aParser.parse(argc, argv)) return FALSE;

	// Check options
	if (OptionsParam::instance()->getConnectedToServer() &&
		OptionsParam::instance()->getDedicatedServer())
	{
		dialogMessage(
			scorched3dAppName,
			"ERROR: You cannot specify parameters to both"
			" join a server and start a server.");
		return false;
	}

	return true;
}

// Compilers from Borland report floating-point exceptions in a manner 
// that is incompatible with Microsoft Direct3D.
int _matherr(struct _exception  *e)
{
    e;               // Dummy reference to catch the warning.
    return 1;        // Error has been handled.
}

int main(int argc, char *argv[])
{
	// Generate the version
	sprintf(scorched3dAppName, "Scorched3D - Version %s (%s)", ScorchedVersion, ScorchedProtocolVersion);

	// Check we are in the correct directory
	FILE *checkfile = fopen(PKGDIR "data/autoexec.xml", "r");
	if (!checkfile)
	{
		dialogMessage(
			scorched3dAppName,
			"Warning: This game requires the Scorched3D data directory to run.\n"
			"Your machine does not appear to have the Scorched3D data directory in\n"
			"the same directory as the scorched.exe file.\n\n"
			"If Scorched3D does not run please re-install Scorched3D.");
		exit(1);
	}
	fclose(checkfile);

#ifdef _WIN32
	// Do a check for directX 8.0
	HINSTANCE hD3D8DLL = LoadLibrary( "D3D8.DLL" );
	if(hD3D8DLL == NULL)
	{
		dialogMessage(
			scorched3dAppName,
			"Warning: This game requires DirectX 8.0 or later to run.\n"
			"Your machine does not appear to have DirectX 8.0 installed.\n\n"
			"The latest version of DirectX can be obtained freely from Microsoft at:\n"
			"http://www.microsoft.com/windows/directx/default.asp");
	}
	FreeLibrary(hD3D8DLL);

	// Disable floating point exceptions. 
	_control87(MCW_EM,MCW_EM);

	// Set directory location for ASE
	{
		Registry baseRegistry("Software\\Scorched");
		CHAR directoryName[1024];
		GetCurrentDirectory(1024, directoryName);
		baseRegistry.setString("", "Location", directoryName);
	}

#endif

	srand((unsigned)time(0));
	// Parse command line
	if (!parseCommandLine(argc, argv))
	{
		return false;
	}

	//for (;;)
	{
		// Init SDL
		unsigned int initFlags = SDL_INIT_VIDEO;
		if (allowExceptions) initFlags |= SDL_INIT_NOPARACHUTE;
		if (SDL_Init(initFlags) < 0)
		{
			dialogMessage(
				scorched3dAppName,
				"Warning: This game uses the SDL library to provide graphics.\n"
				"The graphics section of this library failed to initialize.\n"
				"You will only be able to run a server for this game.");
		}
		else
		{
			OptionsParam::instance()->getSDLInitVideo() = true;
		}

		if (OptionsParam::instance()->getAction() == OptionsParam::NoAction ||
			OptionsParam::instance()->getAction() == OptionsParam::RunServer)
		{
		// Run the wxWindows main loop
		// Dialogs are created int CreateDialogs.cpp
#ifdef _WIN32
		wxEntry((WXHINSTANCE) (HINSTANCE) GetModuleHandle(NULL),
			(WXHINSTANCE) NULL, "", SW_SHOWNORMAL);
#else
		wxEntry(argc, argv);
#endif
		}
		wxWindowInit = false;

		// Actually start the client game if that is desired
		if (OptionsParam::instance()->getSDLInitVideo() &&
			OptionsParam::instance()->getAction() == OptionsParam::RunClient)
		{
			if (setlocale(LC_ALL, "C") == 0)
			{
				dialogMessage(
					scorched3dAppName,
					"Warning: Failed to set client locale");
			}

			clientMain();
		}

		OptionsParam::instance()->clearAction();

		//if (wxWindowExit) break;
	}

	// Write display options back to the file
	OptionsDisplay::instance()->writeOptionsToFile(displayOptions);

	return TRUE;
}
