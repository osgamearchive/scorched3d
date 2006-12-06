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

#include <client/ClientParams.h>
#include <client/ScorchedClient.h>
#include <client/ClientSave.h>
#include <client/ClientMain.h>
#include <server/ScorchedServer.h>
#include <engine/ModDirs.h>
#include <common/OptionsGame.h>
#include <common/ARGParser.h>
#include <common/Defines.h>
#include <common/OptionsTransient.h>
#include <graph/OptionsDisplay.h>
#include <locale.h>
#include <math.h>
#include <signal.h>
#include <float.h>
#include <time.h>
#include <common/main.h>

int main(int argc, char *argv[])
{
	// From main.h
	run_main(argc, argv, *ClientParams::instance());

	// Read display options from a file
	// **This NEEDS to be after the arg parser**
	if (!OptionsDisplay::instance()->readOptionsFromFile())
	{
		return false;
	}

	// Get this host's description and username
	if (!OptionsDisplay::instance()->getHostDescription()[0])
	{
		OptionsDisplay::instance()->getHostDescriptionEntry().setValue(s3d_getOSDesc());
		OptionsDisplay::instance()->getOnlineUserNameEntry().setValue("Player");
	}

	// Write the new options back the the file
	OptionsDisplay::instance()->writeOptionsToFile();

	// Init SDL
	unsigned int initFlags = SDL_INIT_VIDEO;
	if (ClientParams::instance()->getAllowExceptions()) initFlags |= SDL_INIT_NOPARACHUTE;
	if (SDL_Init(initFlags) < 0)
	{
		dialogExit(
			scorched3dAppName,
			"This game uses the SDL library to provide graphics.\n"
			"The graphics section of this library failed to initialize.");
	}

	// Check if we are connecting to a server
	if (!ClientParams::instance()->getConnect()[0])
	{
		if (ClientParams::instance()->getStartCustom() ||
			ClientParams::instance()->getClientFile()[0])
		{
			std::string clientFile = ClientParams::instance()->getClientFile();
			if (ClientParams::instance()->getStartCustom())
			{
				clientFile = getSettingsFile("singlecustom.xml");
			}

			// If not load the client settings file
			if (!s3d_fileExists(clientFile.c_str()))
			{
				dialogExit(scorched3dAppName, formatString(
					"Client file \"%s\" does not exist.",
					clientFile.c_str()));
			}
			ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
				(char *) clientFile.c_str());
		}
		else if (ClientParams::instance()->getSaveFile()[0])
		{
			// Or the client saved game
			if (!s3d_fileExists(ClientParams::instance()->getSaveFile()))
			{
				dialogExit(scorched3dAppName, formatString(
					"Client save file \"%s\" does not exist.",
					ClientParams::instance()->getSaveFile()));
			}
			if (!ClientSave::loadClient(ClientParams::instance()->getSaveFile()) ||
				!ClientSave::restoreClient(true, false))
			{
				dialogExit(scorched3dAppName, formatString(
					"Cannot load client save file \"%s\".",
					ClientParams::instance()->getSaveFile()));
			}
		}
		else
		{
			aParser.showArgs();
			exit(64);
		}
	}

	if (!clientMain()) exit(64);

	// Write display options back to the file
	// in case they have been changed by this client (in game by the console)
	OptionsDisplay::instance()->writeOptionsToFile();

	SDL_Quit();
	return 0; // exit(0)
}