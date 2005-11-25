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
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <server/ServerMain.h>
#include <engine/ModFiles.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/OptionsHomeDir.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/OptionsTransient.h>
#include <scorched/MainDialog.h>
#include <locale.h>
#include <math.h>
#include <signal.h>
#include <float.h>
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern bool wxWindowInit;
extern bool wxWindowExit;
char scorched3dAppName[128];
static bool allowExceptions = false;

bool parseCommandLine(int argc, char *argv[])
{
	// Read options from command line
	ARGParser aParser;
	if (!OptionEntryHelper::addToArgParser(
		OptionsParam::instance()->getOptions(), aParser)) return false;
	aParser.addEntry("-allowexceptions", &allowExceptions, 
					 "Allows any program exceptions to be thrown (core dumps)");
	if (!aParser.parse(argc, argv)) return false;
	setSettingsDir(OptionsParam::instance()->getSettingsDir());

	// Read display options from a file
	// **This NEEDS to be after the arg parser**
	if (!OptionsDisplay::instance()->readOptionsFromFile())
	{
		return false;
	}

	// Get this host's description and username
	if (!OptionsDisplay::instance()->getHostDescription()[0])
	{
		wxString osDesc = ::wxGetOsDescription();
		OptionsDisplay::instance()->setHostDescription(osDesc.mb_str(wxConvUTF8));

		wxString userName = ::wxGetUserName();
		if (!userName.empty())
		{
			OptionsDisplay::instance()->setOnlineUserName(userName.mb_str(wxConvUTF8));
		}
	}

	// Write the new options back the the file
	OptionsDisplay::instance()->writeOptionsToFile();

	// Read the current home directory options
	OptionsHomeDir::instance()->readOptionsFromFile();
	if (0 != strcmp(OptionsHomeDir::instance()->getDirectoryVersion(),
		ScorchedProtocolVersion))
	{
		// The version has changed move the current mod directories
		if (!s3d_dirExists(getSettingsFile("/oldmods")))
		{
			s3d_dirMake(getSettingsFile("/oldmods"));
		}
		ModDirs dirs;
		dirs.loadModDirs();
		std::list<std::string>::iterator itor;
		for (itor = dirs.getDirs().begin();
			itor != dirs.getDirs().end();
			itor++)
		{
			const char *modDir = (*itor).c_str();
			std::string src = getModFile(modDir);
			std::string dest = getSettingsFile("/oldmods/%s-%u", modDir, time(0));
			if (s3d_dirExists(src.c_str()))
			{
				if (::wxRenameFile(wxString(src.c_str(), wxConvUTF8), 
					wxString(dest.c_str(), wxConvUTF8)))
				{
					dialogMessage("Scorched3D",
						"Mod directory\n"
						"\"%s\"\n"
						"was moved to\n"
						"\"%s\"\n"
						"as it may be incompatable with this version of Scorched3D",
						src.c_str(), dest.c_str());
				}
			}
		}

		OptionsHomeDir::instance()->setDirectoryVersion(ScorchedProtocolVersion);
		OptionsHomeDir::instance()->writeOptionsToFile();
	}

	// Read the game options (allows us to modify any settings only 
	// used by the chooser screens)
	const char *optionsGamePath = getSettingsFile("game.xml");
	ScorchedClient::instance()->getOptionsGame().readOptionsFromFile(
		(char *) optionsGamePath);

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
	snprintf(scorched3dAppName, 128, "Scorched3D - Version %s (%s)", 
		ScorchedVersion, ScorchedProtocolVersion);

	srand((unsigned)time(0));
	// Parse command line
	if (!parseCommandLine(argc, argv)) exit(64);

	// Check we are in the correct directory
	FILE *checkfile = fopen(getDataFile("data/autoexec.xml"), "r");
	if (!checkfile)
	{
		// Perhaps we can get the directory from the executables path name
		char path[1024];
		strcpy(path, argv[0]);
		char *sep = strrchr(path, '/');
		if (sep)
		{
			// Change into this new direcotry
			*sep = '\0';
#ifdef _WIN32
			SetCurrentDirectory(path);
#else
			chdir(path);
#endif // _WIN32
		}

		// Now try again for the correct directory
		checkfile = fopen(getDataFile("data/autoexec.xml"), "r");
		if (!checkfile)
		{	
#ifdef _WIN32
			GetCurrentDirectory(sizeof(path), path);
#else
			getcwd(path, sizeof(path));
#endif // _WIN32
			dialogExit(
				scorched3dAppName,
				"Error: This game requires the Scorched3D data directory to run.\n"
				"Your machine does not appear to have the Scorched3D data directory in\n"
				"the required location.\n"
				"The data directory is set to \"%s\" which does not exist.\n"
				"(Current working directory %s)\n\n"
				"If Scorched3D does not run please re-install Scorched3D.",
				getDataFile("data"), path);
		}
	}
	else fclose(checkfile);

#ifdef _WIN32
	// For borland compilers disable floating point exceptions. 
	_control87(MCW_EM,MCW_EM);
#endif // _WIN32

#ifndef _WIN32
	// Tells Linux not to issue a sig pipe when writting to a closed socket
	// Why does it have to be dificult!
	signal(SIGPIPE, SIG_IGN);
	signal(SIGFPE, SIG_IGN);
#endif

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
		if (!s3d_fileExists(OptionsParam::instance()->getServerFile()))
		{
			dialogExit(scorched3dAppName,
				"Server file \"%s\" does not exist.",
				OptionsParam::instance()->getServerFile());
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
					if (!s3d_fileExists(OptionsParam::instance()->getClientFile()))
					{
						dialogExit(scorched3dAppName,
							"Client file \"%s\" does not exist.",
							OptionsParam::instance()->getClientFile());
					}
					ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
						(char *) OptionsParam::instance()->getClientFile());
				}
				else
				{
					// Or the client saved game
					if (!s3d_fileExists(OptionsParam::instance()->getSaveFile()))
					{
						dialogExit(scorched3dAppName,
							"Client save file \"%s\" does not exist.",
							OptionsParam::instance()->getSaveFile());
					}
					if (!ClientSave::loadClient(OptionsParam::instance()->getSaveFile()) ||
						!ClientSave::restoreClient(true, false))
					{
						dialogExit(scorched3dAppName,
							"Cannot load client save file \"%s\".",
							OptionsParam::instance()->getSaveFile());
					}
				}
			}

			if (!clientMain()) exit(64);

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
		dialogExit(
			scorched3dAppName,
			"Error: You provided incorrect or incompatable parameters");
	}

	return 0; // exit(0)
}

