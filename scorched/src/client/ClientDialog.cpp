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
#include <client/GLSetup.h>
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsParam.h>
#include <common/Keyboard.h>
#include <common/Display.h>
#include <common/Sound.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/Gamma.h>
#include <GLEXT/GLStateExtension.h>
#include <stdio.h>
#include <time.h>

extern char scorched3dAppName[128];

void setup()
{
	GLStateExtension::getNoExtensions() = 
		OptionsDisplay::instance()->getNoExt();
	GLStateExtension::getNoMultiTex() = 
		OptionsDisplay::instance()->getNoMultiTex();
	GLStateExtension::getNoTexSubImage() =
		OptionsDisplay::instance()->getNoTexSubImage();
	Gamma::instance()->set(
		float(OptionsDisplay::instance()->getBrightness()) / 10.0f);

	GLSetup::setup();

	if (!Keyboard::instance()->init())
	{
		dialogMessage("Scorched3D Keyboard", 
			"SDL failed to aquire keyboard.\n"
#ifdef WIN32
			"Is DirectX 5.0 installed?"
#endif
		);
		exit(1);
	}
	if (!Keyboard::instance()->parseKeyFile(PKGDIR "data/keys.xml"))
	{
		dialogMessage("Scorched3D Keyboard", 
			"Failed to process keyboard file \"data/keys.xml\"");
		exit(1);		
	}

	if (!OptionsDisplay::instance()->getNoSound())
	{
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		{
			dialogMessage("Scorched3D Sound", 
				"Failed to initialise sound.\n"
#ifdef WIN32
				"Is DirectX 5.0 installed?\n"
#endif
				"Is anything else currently using the sound card?");
		}
		if (!Sound::instance()->init())
		{
			dialogMessage("Scorched3D Sound", 
				"Failed to aquire sound.\n"
#ifdef WIN32
				"Is DirectX 5.0 installed?\n"
#endif
				"Is anything else currently using the sound card?");
		}	
	}
}

bool createScorchedWindow()
{
	int width = OptionsDisplay::instance()->getScreenWidth();
	int height = OptionsDisplay::instance()->getScreenHeight();
	bool fullscreen = OptionsDisplay::instance()->getFullScreen();

	if ((width<0) || (height<0)) {
		Display::instance()->autoSettings(width,height,fullscreen);
		OptionsDisplay::instance()->setScreenWidth(width);
		OptionsDisplay::instance()->setScreenHeight(height);
		OptionsDisplay::instance()->setFullScreen(fullscreen);
	}

	SDL_WM_SetCaption(scorched3dAppName, "tank2");
	SDL_WM_SetIcon(SDL_LoadBMP(PKGDIR "data/windows/tank2.bmp"), NULL);

	if (!Display::instance()->init() || 
		!Display::instance()->changeSettings(width,height,fullscreen)) 
	{
		dialogMessage("Scorched 3D Display", 
			"ERROR: Failed to set the display mode.\n"
			"Ensure that no other application is exclusively using the graphics hardware.\n"
			"Ensure that the current desktop mode has at least 24 bits colour depth.\n");
		return FALSE;
	}

	MainCamera::instance()->getCamera().setWindowSize(width,height);
	setup();

	return TRUE;
}
