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
#include <wx/utils.h>
#include <SDL/SDL.h>
#include <client/ClientMain.h>
#include <client/ClientSave.h>
#include <server/ScorchedServer.h>
#include <server/ServerMain.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/Resources.h>
#include <common/OptionsTransient.h>
#include <scorched/MainDialog.h>
#include <locale.h>
#include <math.h>
#include <signal.h>
#include <float.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern bool wxWindowInit;
extern bool wxWindowExit;
char scorched3dAppName[128];
static bool allowExceptions = false;

bool parseCommandLine(int argc, char *argv[])
{
	// Read the resources 
	if (!Resources::instance()->main.initFromFile(
		getDataFile("data/resource.xml"))) return false;

	// Read display options from a file
	if (!OptionsDisplay::instance()->readOptionsFromFile())
	{
		return false;
	}

	// Create a unique userid (if one has not already been created)
	if (!OptionsDisplay::instance()->getUniqueUserId()[0])
	{
		char buffer[128];
		sprintf(buffer, "%i-%i-%i", rand(), rand(), rand());
		OptionsDisplay::instance()->setUniqueUserId(buffer);
	}

	// Get this host's description and username
	if (!OptionsDisplay::instance()->getHostDescription()[0])
	{
		wxString osDesc = ::wxGetOsDescription();
		OptionsDisplay::instance()->setHostDescription(osDesc.c_str());

		wxString userName = ::wxGetUserName();
		if (userName.c_str()[0])
		{
			for (char *c=(char *) userName.c_str(); *c; c++)
			{
				if (*c < 32 || *c > 125) *c = ' ';
			}
			OptionsDisplay::instance()->setOnlineUserName(userName.c_str());
		}
	}

	// Write the new options back the the file
	OptionsDisplay::instance()->writeOptionsToFile();

	// Read options from command line
	ARGParser aParser;
	if (!OptionEntryHelper::addToArgParser(
		OptionsParam::instance()->getOptions(), aParser)) return false;
	aParser.addEntry("-allowexceptions", &allowExceptions, 
					 "Allows any program exceptions to be thrown (core dumps)");
	if (!aParser.parse(argc, argv)) return false;

	// Set the path the executable was run with
	setExeName((const char *) argv[0], allowExceptions);

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
	sprintf(scorched3dAppName, "Scorched3D - Version %s (%s)", 
		ScorchedVersion, ScorchedProtocolVersion);

	// Check we are in the correct directory
	FILE *checkfile = fopen(getDataFile("data/autoexec.xml"), "r");
	if (!checkfile)
	{
		dialogMessage(
			scorched3dAppName,
			"Error: This game requires the Scorched3D data directory to run.\n"
			"Your machine does not appear to have the Scorched3D data directory in\n"
			"the same directory as the scorched.exe file (set to \"%s\").\n\n"
			"If Scorched3D does not run please re-install Scorched3D.",
			getDataFile("."));
		exit(1);
	}
	else fclose(checkfile);

#ifdef _WIN32
	// For borland compilers disable floating point exceptions. 
	_control87(MCW_EM,MCW_EM);
#endif

#ifndef _WIN32
	// Tells Linux not to issue a sig pipe when writting to a closed socket
	// Why does it have to be dificult!
	signal(SIGPIPE, SIG_IGN);
#endif

	srand((unsigned)time(0));
	// Parse command line
	if (!parseCommandLine(argc, argv)) exit(1);

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

	switch (OptionsParam::instance()->getAction())
	{
	case OptionsParam::ActionRunServer:
	
		// Load the server settings file
		if (!::wxFileExists(OptionsParam::instance()->getServerFile()))
		{
			dialogMessage(scorched3dAppName,
				"Server file \"%s\" does not exist.",
				OptionsParam::instance()->getServerFile());
			exit(1);
		}
		ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
			(char *) OptionsParam::instance()->getServerFile());

		if (OptionsParam::instance()->getConsole())
		{
			consoleServer();
			exit(0);
		}

		// Note: WE DO NOT BREAK
	
	case OptionsParam::ActionNone:
	case OptionsParam::ActionHelp:
		// Run the wxWindows main loop
		// Dialogs are created int CreateDialogs.cpp
#ifdef _WIN32
		wxEntry((WXHINSTANCE) (HINSTANCE) GetModuleHandle(NULL),
			(WXHINSTANCE) NULL, "", SW_SHOWNORMAL);
#else
		wxEntry(argc, argv);
#endif
		break;
	case OptionsParam::ActionRunClient:

#ifdef _WIN32
		{
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
		}
#endif

		// Actually start the client game if that is desired
		wxWindowInit = false;
		if (OptionsParam::instance()->getSDLInitVideo())
		{
			if (setlocale(LC_ALL, "C") == 0)
			{
				dialogMessage(
					scorched3dAppName,
					"Warning: Failed to set client locale");
			}

			// Check if we are connecting to a server
			if (!OptionsParam::instance()->getConnect()[0])
			{
				if (OptionsParam::instance()->getClientFile()[0])
				{
					// If not load the client settings file
					if (!::wxFileExists(OptionsParam::instance()->getClientFile()))
					{
						dialogMessage(scorched3dAppName,
							"Client file \"%s\" does not exist.",
							OptionsParam::instance()->getClientFile());
						exit(1);
					}
					ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
						(char *) OptionsParam::instance()->getClientFile());
				}
				else
				{
					// Or the client saved game
					if (!::wxFileExists(OptionsParam::instance()->getSaveFile()))
					{
						dialogMessage(scorched3dAppName,
							"Client save file \"%s\" does not exist.",
							OptionsParam::instance()->getSaveFile());
						exit(1);
					}
					if (!loadClient(OptionsParam::instance()->getSaveFile()) ||
						!restoreClient(true))
					{
						dialogMessage(scorched3dAppName,
							"Cannot load client save file \"%s\".",
							OptionsParam::instance()->getSaveFile());
						exit(1);
					}
				}
			}

			if (!clientMain()) exit(1);

			// Write display options back to the file
			// in case they have been changed by this client (in game by the console)
			OptionsDisplay::instance()->writeOptionsToFile();
		}
		else
		{
			dialogMessage(
				scorched3dAppName,
				"Error: Cannot start scorched3d client, graphics mode was not initilaized");
		}
		break;
	default:
		dialogMessage(
			scorched3dAppName,
			"Error: You provided incorrect or incompatable parameters");
		exit(1);
	}

	return 0; // exit(0)
}

