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

#include <client/MainCamera.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLConsole.h>
#include <dialogs/MainMenuDialog.h>
#include <common/Keyboard.h>
#include <common/SoundStore.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <math.h>
#include <time.h>

MainCamera *MainCamera::instance_ = 0;

MainCamera *MainCamera::instance()
{
	if (!instance_)
	{
		instance_ = new MainCamera;
	}

	return instance_;
}

MainCamera::MainCamera() : scrollTime_(0.0f)
{
	MainMenuDialog::instance()->addMenu("Camera", 90, this, 0, this);
}

MainCamera::~MainCamera()
{

}

void MainCamera::getMenuItems(const char* menuName, 
	std::list<GLMenuItem> &result)
{
	for (int i=0; i<TargetCamera::getNoCameraNames(); i++)
	{
		result.push_back(GLMenuItem(TargetCamera::getCameraNames()[i], 
			&TargetCamera::getCameraToolTips()[i],
			(targetCam_.getCameraType() == 
			(TargetCamera::CamType) i)));
	}
}

void MainCamera::menuSelection(const char* menuName, 
	const int position, const char *menuItem)
{
	targetCam_.setCameraType((TargetCamera::CamType) position);
}

void MainCamera::simulate(const unsigned state, float frameTime)
{
	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();
	int windowX = targetCam_.getCamera().getCameraWidthInternal();
	int windowY = targetCam_.getCamera().getCameraHeightInternal();

	scrollTime_ += frameTime;
	while (scrollTime_ > 0.0f)
	{
		scrollTime_ -= 0.1f;

		const int scrollWindow = 5;
		if (mouseX < scrollWindow)
		{
			targetCam_.getCamera().scroll(GLCamera::eScrollLeft);
		}
		else if (mouseX > windowX - scrollWindow)
		{
			targetCam_.getCamera().scroll(GLCamera::eScrollRight);
		}
	
		if (mouseY < scrollWindow)
		{
			targetCam_.getCamera().scroll(GLCamera::eScrollDown);
		}
		else if (mouseY > windowY - scrollWindow)
		{
			targetCam_.getCamera().scroll(GLCamera::eScrollUp);
		}
	}

	ScorchedClient::instance()->getContext().viewPoints.simulate(frameTime);
	targetCam_.simulate(frameTime, (state == ClientState::StatePlaying));
}

void MainCamera::draw(const unsigned state)
{
	targetCam_.draw();
}

void MainCamera::mouseDrag(const unsigned state, 
	GameState::MouseButton button, 
	int mx, int my, int x, int y, bool &skipRest)
{
	targetCam_.mouseDrag(button, mx, my, x, y, skipRest);
}

void MainCamera::mouseWheel(const unsigned state, short z, bool &skipRest)
{
	targetCam_.mouseWheel(z, skipRest);
}

void MainCamera::mouseDown(const unsigned state, GameState::MouseButton button, 
	int x, int y, bool &skipRest)
{
	targetCam_.mouseDown(button, x, y, skipRest);
}

void MainCamera::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, 
							   int hisCount, 
							   bool &skipRest)
{
	targetCam_.keyboardCheck(frameTime, buffer,
		keyState, history, hisCount, skipRest);

	KEYBOARDKEY("SAVE_SCREEN", saveScreenKey);
	if (saveScreenKey->keyDown(buffer, keyState, false))
	{
		saveScreen_.saveScreen_ = true;
	}

	KEYBOARDKEY("HUD_ITEMS", hudItemsKey);
	if (hudItemsKey->keyDown(buffer, keyState, false))
	{
		unsigned int currentHudState = 0x0;
		if (OptionsDisplay::instance()->getDrawPlayerNames()) 
			currentHudState |= 0x1;
		if (OptionsDisplay::instance()->getDrawPlayerSight()) 
			currentHudState |= 0x2;
		if (OptionsDisplay::instance()->getDrawPlayerColor()) 
			currentHudState |= 0x4;
		if (OptionsDisplay::instance()->getDrawPlayerHealth()) 
			currentHudState |= 0x8;

		currentHudState++;
		if (currentHudState > 0x1 + 0x2 + 0x4 + 0x8) currentHudState = 0x0;

		OptionsDisplay::instance()->setDrawPlayerNames(
			(currentHudState & 0x1) != 0x0);
		OptionsDisplay::instance()->setDrawPlayerSight(
			(currentHudState & 0x2) != 0x0);
		OptionsDisplay::instance()->setDrawPlayerColor(
			(currentHudState & 0x4) != 0x0);
		OptionsDisplay::instance()->setDrawPlayerHealth(
			(currentHudState & 0x8) != 0x0);
	}
}

void MainCamera::SaveScreen::draw(const unsigned state)
{
	if (!saveScreen_) return;
	saveScreen_ = false;

	static unsigned counter = 0;
	time_t currentTime = time(0);
	char *fileName = (char *) 
		getHomeFile("ScreenShot-%i-%i.bmp", currentTime, counter++);

	GLBitmap screenMap;
	screenMap.grabScreen();
	screenMap.writeToFile(fileName);

	// Don't print to banner otherwise this message will be in
	// the screenshot!
	GLConsole::instance()->addLine(false, 
		"Screen shot saved as file \"%s\"", fileName);

	// snapshot sound
	CACHE_SOUND(sound,  (char *) getDataFile("data/wav/misc/camera.wav"));
	sound->play();
}

