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
#include <common/OptionsGame.h>
#include <coms/NetServer.h>
#include <coms/NetMessageHandler.h>
#include <engine/GameState.h>
#include <engine/ActionController.h>
#include <engine/ScorchedCollisionHandler.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/GlobalHMap.h>
#include <server/OptionsServer.h>
#include <server/ServerDialog.h>
#include <server/ServerMessageHandler.h>
#include <server/ServerPlayerReadyHandler.h>
#include <server/ServerSettingsDialog.h>
#include <server/ServerTextHandler.h>
#include <server/ServerPlayedMoveHandler.h>
#include <server/ServerConnectHandler.h>
#include <server/ServerState.h>
#include <SDL/SDL.h>

Timer timer;

#ifdef _WIN32
#include <commctrl.h>
#endif

extern "C" {
#include <ASE/ASEQuerySDK.h>
}

static const char *defaultName = "No Name";
bool publishServer = false;
char *publishAddress = 0;

void suspendPowerManagement()
{
#ifdef _WIN32
    TCHAR szPath[MAX_PATH];
    HINSTANCE hInstKernel32 = NULL;
    typedef EXECUTION_STATE (WINAPI* LPSETTHREADEXECUTIONSTATE)
		( EXECUTION_STATE esFlags );
    LPSETTHREADEXECUTIONSTATE pSetThreadExecutionState = NULL;

    GetSystemDirectory(szPath, MAX_PATH);

    // SetThreadExecutionState() isn't availible on some old OS's, 
    // so do a LoadLibrary to get to it.
    lstrcat(szPath, TEXT("\\kernel32.dll"));
    hInstKernel32 = LoadLibrary(szPath);

    if (hInstKernel32 != NULL)
    {
        pSetThreadExecutionState = 
			(LPSETTHREADEXECUTIONSTATE)
			GetProcAddress(hInstKernel32, "SetThreadExecutionState");
        if( pSetThreadExecutionState != NULL )
        {
            // Tell OS's that have power management to not 
            // sleep, since this app will be using the 
            // network connection and need very little user input
            pSetThreadExecutionState( ES_SYSTEM_REQUIRED | ES_CONTINUOUS );
        }

        FreeLibrary(hInstKernel32);
    }
#endif
}

bool parseArguments(int argc, char *argv[])
{
	// Parse command line settings
	char *configFile = 0;

	// Set some defaults
	OptionsServer::instance()->setServerName((char *) defaultName);
	OptionsServer::instance()->setPortNo(ScorchedPort);
	OptionsGame::instance()->setShotTime(45);
	OptionsGame::instance()->setNoMaxPlayers(10);
	OptionsGame::instance()->setSimultaneous(true);

	ARGParser aParser;
	OptionsGame::instance()->addToArgParser(aParser);
	aParser.addEntry("-publishserver", &publishServer, 
		"Allow other internet users to see this game.  Do not use for LAN games.");
	aParser.addEntry("-publishaddress", &publishAddress,
		"IP address to bind to (auto-detected if not given).");
	aParser.addEntry("-configfile", &configFile,
		"Load all settings from a given configuration file");
	if (!aParser.parse(argc, argv)) return false;

	if (configFile)
	{
		if (!OptionsGame::instance()->readOptionsFromFile(configFile)) return false;
		delete [] configFile;
	}

	return true;
}

bool startServer()
{
#ifndef _NO_SERVER_ASE_
	WORD WSAVerReq = (MAKEWORD((1),(1)));
	WSADATA WSAData;
	if (WSAStartup(WSAVerReq, &WSAData) != 0)
	{
		MessageBox(NULL, 
			"Failed to start windows sockets.",
			"Scorched3D Server",
			MB_OK | MB_ICONWARNING);
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

	// Init physics
	ActionController::instance();
	HeightMapCollision::instance()->create(
		GlobalHMap::instance()->getHMap());
	ScorchedCollisionHandler::instance();

	// Try to start the server
	if (!NetServer::instance()->start(
		OptionsServer::instance()->getPortNo(),
		OptionsGame::instance()->getNoMaxPlayers()))
	{
		MessageBox(NULL, 
			"Failed to start the server.\n\n"
			"Ensure the specified port does not conflict with any other program.",
			"Scorched3D Server",
			MB_OK | MB_ICONWARNING);
		return false;
	}

	ServerState::setupStates();

#ifndef _NO_SERVER_ASE_
	if (!ASEQuery_initialize(OptionsServer::instance()->getPortNo(), 
		(publishServer?1:0), publishAddress))
	{
		MessageBox(NULL, 
			"Failed to initialize the ASE server reporting.",
			"Scorched3D Server",
			MB_OK | MB_ICONWARNING);
		return false;
	}
#endif

	return true;
}

int main(int argc, char *argv[])
{
	// Check we are in the correct directory
	FILE *checkfile = fopen(PKGDIR "data/autoexec.xml", "r");
	if (!checkfile)
	{
		MessageBox(NULL, 
			"This game requires the Scorched3D data directory to run.\n"
			"Your machine does not appear to have the Scorched3D data directory in\n"
			"the same directory as the scorched.exe file.",
			"Scorched 3D",
			MB_OK | MB_ICONWARNING);
		exit(1);
	}
	fclose(checkfile);

#ifdef _WIN32
	// Stop machine from suspending
	suspendPowerManagement();

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

	// Perform application initialization:
	InitCommonControls();
#endif

	if (!parseArguments(argc, argv)) 
	{
		return FALSE;
	}

#ifdef _WIN32
	// Show the settings dialog (if not supplied name)
	if (strcmp(OptionsServer::instance()->getServerName(),
		defaultName) == 0)
	{
		if (!showSettingsDialog())
		{
			return FALSE;
		}
	}

	// Show the main info dialog
	if (!createDialog())
	{
		return FALSE;
	}
#endif

	// Start the server
	if (!startServer()) 
	{
		return FALSE;
	}
	addString("server", "Server started");

	// Main message loop:
#ifdef _WIN32
	MSG msg;
#endif
	for (;;) 
	{
#ifdef _WIN32
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) break;
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
#endif

		if (NetServer::instance()->started())
		{
			if (NetMessageHandler::instance()->processMessages() == 0) SDL_Delay(10);
			GameState::instance()->simulate(timer.getTimeDifference());
#ifndef _NO_SERVER_ASE_
			ASEQuery_check();
#endif
		}
		else SDL_Delay(100); // ooooo nasty
	}

	if (NetServer::instance()->started())
	{
#ifndef _NO_SERVER_ASE_
		ASEQuery_shutdown();
		WSACleanup();
#endif
	}

	return 0;
}
