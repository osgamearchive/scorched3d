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


#include <client/ScorchedDialog.h>
#include <client/GLSetup.h>
#include <client/StateSetup.h>
#include <client/WindowSetup.h>
#include <client/MainCamera.h>
#include <client/resource.h>
#include <client/OptionsClient.h>
#include <common/Keyboard.h>
#include <common/Display.h>
#include <common/Sound.h>
#include <common/OptionsTransient.h>
#include <common/Gamma.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/GlobalHMap.h>
#include <tankgraph/TankModelStore.h>
#include <engine/ScorchedCollisionHandler.h>
#include <engine/MainLoop.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLConsoleFileReader.h>
#include <stdio.h>
#include <time.h>

LPCTSTR lpszAppName = "Scorched3D";

void setup()
{
	srand((unsigned) time(NULL));

	GLStateExtension::getNoExtensions() = 
		OptionsClient::instance()->getNoExt();
	GLStateExtension::getNoMultiTex() = 
		OptionsClient::instance()->getNoMultiTex();
	Gamma::instance()->set(
		float(OptionsClient::instance()->getBrightness()) / 10.0f);

	GLSetup::setup();
	MainLoop::instance();

	bool useServer = (strlen(OptionsClient::instance()->getServerName()) > 0);
	StateSetup::setupGameState(useServer);

	ActionController::instance();
	HeightMapCollision::instance()->create(
		GlobalHMap::instance()->getHMap());
	ScorchedCollisionHandler::instance();

	if (!Keyboard::instance()->init())
	{
		MessageBox(NULL, "SDL failed to aquire keyboard.\n"
			"Is DirectX 8.0 installed?", 
			"Direct Input Version 8.0", MB_OK);
		exit(1);
	}

	if (!OptionsClient::instance()->getNoSound())
	{
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		{
			MessageBox(NULL, "Failed to initialise sound.\n"
				"Is DirectX 5.0 installed?\n"
				"Is anything else currently using the sound card?\n",
				"Direct Sound Version 5.0", MB_OK);
			exit(1);
		}
		if (!Sound::instance()->init())
		{
			MessageBox(NULL, "DirectX 8.0 failed to aquire sound.\n"
				"Is DirectX 8.0 installed?\n"
				"Is anything else currently using the sound card?\n",
				"Direct Sound Version 8.0", MB_OK);
			exit(1);
		}	
	}

	if (!TankModelStore::instance()->loadTankMeshes())
	{
			MessageBox(NULL, "Failed to load all tank models",
				"Scorched 3D", MB_OK);		
		exit(1);
	}

	if (useServer)
	{
		OptionsTransient::instance()->getServer() = true;	
	}
	else
	{
		//AccessoryStoreConsole::instance();
		//TankConsole::instance(); // TODO
	}
	
	WindowSetup::setup(useServer);

	std::string errorString;
	if (!GLConsoleFileReader::loadFileIntoConsole(PKGDIR "data/autoexec.xml", errorString))
	{
			MessageBox(NULL, errorString.c_str(),
				"Failed to parse autoexec.xml", MB_OK);
			exit(1);
	}
}

bool createScorchedWindow()
{
	int width = OptionsClient::instance()->getScreenWidth();
	int height = OptionsClient::instance()->getScreenHeight();
	bool fullscreen = OptionsClient::instance()->getFullScreen();

	if ((width<0) || (height<0)) {
		Display::instance()->autoSettings(width,height,fullscreen);
		OptionsClient::instance()->setScreenWidth(width);
		OptionsClient::instance()->setScreenHeight(height);
		OptionsClient::instance()->setFullScreen(fullscreen);
	}

	SDL_WM_SetCaption("Scorched3D", "tank2");
	SDL_WM_SetIcon(SDL_LoadBMP(PKGDIR "data/windows/tank2.bmp"), NULL);

	if (!Display::instance()->changeSettings(width,height,fullscreen)) 
	{
		MessageBox(NULL, "ERROR: Failed to set the display mode", "Display", MB_OK);
		return FALSE;
	}

	MainCamera::instance()->getCamera().setWindowSize(width,height);
	setup();

	return TRUE;
}
