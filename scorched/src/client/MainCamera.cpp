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


// MainCamera.cpp: implementation of the MainCamera class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLConsole.h>
#include <engine/ActionController.h>
#include <actions/ShotProjectile.h>
#include <client/MainCamera.h>
#include <dialogs/MainMenuDialog.h>
#include <tank/TankContainer.h>
#include <landscape/GlobalHMap.h>
#include <landscape/Landscape.h>
#include <common/Keyboard.h>
#include <common/SoundStore.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const char *menuItems[] = 
{
	"Top",
	"AboveTank",
	"Tank",
	"Shot",
	"Left",
	"Right",
	"LeftFar",
	"RightFar",
	"Spectator",
	"Free"
};
static const int noMenuItems = sizeof(menuItems) / sizeof(char *);

MainCamera *MainCamera::instance_ = 0;

MainCamera *MainCamera::instance()
{
	if (!instance_)
	{
		instance_ = new MainCamera;
	}

	return instance_;
}

MainCamera::MainCamera() : mainCam_(300, 300), 
	cameraPos_(CamSpectator)
{
	MainMenuDialog::instance()->addMenu("Camera", 80, this, 0, this);

	resetCam();
	mainCam_.setHeightFunc(heightFunc);
}

MainCamera::~MainCamera()
{

}

void MainCamera::resetCam()
{
	Vector at(128.0f, 128.0f, 5.0f);
	mainCam_.setLookAt(at, true);
	cameraPos_ = CamSpectator;
}

float MainCamera::heightFunc(int x, int y)
{
	if (instance_->cameraPos_ == CamGun)
	{
		if ((instance_->mainCam_.getCurrentPos() - 
			instance_->mainCam_.getLookAt()).Magnitude() < 5.0f) return 0.0f;
	}

	const float heightMin = 5.0f;
	float addition = 5.0f;

	float h = 0;
	if (x >= 0 && x < GlobalHMap::instance()->getHMap().getWidth() &&
		y >= 0 && y < GlobalHMap::instance()->getHMap().getWidth())
	{
		h = GlobalHMap::instance()->getHMap().getHeight(x, y) + addition;
	}

	return (h>heightMin + addition?h:heightMin + addition);
}

void MainCamera::getMenuItems(const char* menuName, 
							  std::list<GLMenuItem> &result)
{
	for (int i=0; i<noMenuItems; i++)
	{
		static char buffer[128];
		sprintf(buffer, "%c %s",
			((cameraPos_ == (CamType) i)?'x':' '), menuItems[i]);
		result.push_back(buffer);
	}
}

void MainCamera::menuSelection(const char* menuName, const int position, const char *menuItem)
{
	cameraPos_ = (CamType) position;
}

void MainCamera::simulate(const unsigned state, float frameTime)
{
	moveCamera(frameTime);
	mainCam_.simulate(frameTime);
}

void MainCamera::draw(const unsigned state)
{
	mainCam_.draw();
}

void MainCamera::moveCamera(float frameTime)
{
	Vector position(128.0f, 128.0f, 0.0f);
	float currentRotation = 0.0f;

	Tank *currentTank = TankContainer::instance()->getCurrentTank();
	if (currentTank && 
		currentTank->getState().getState() == TankState::sNormal)
	{
		position = currentTank->getPhysics().getTankTurretPosition();
		currentRotation = (180.0f - currentTank->getPhysics().getRotationGunXY()) / 57.32f;
	}

	switch (cameraPos_)
	{
	case CamTop:
		mainCam_.setLookAt(position);
		mainCam_.movePosition(currentRotation, 0.174f, 50.f);
		break;
	case CamTank:
		{
			Vector newPos(
				sinf(currentRotation) * 8.0f, 
				cosf(currentRotation) * 8.0f,
				0.0f);

			Vector newPos2 = position - newPos;
			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation, 1.48f, 15.f);
		}
		break;
	case CamGun:
		if (!ActionController::instance()->noReferencedActions() &&
			ShotProjectile::getLookAtCount() > 0)
		{
			ShotProjectile::getLookAtPosition() /= (float) ShotProjectile::getLookAtCount();
			mainCam_.setLookAt(ShotProjectile::getLookAtPosition());
			mainCam_.movePosition(currentRotation + 0.3f, 1.0f, 10.0f);
		}
		else
		{
			if (currentTank && currentTank->getState().getState() == TankState::sNormal)
			{
				float currentElevation = (currentTank->getPhysics().getRotationGunYZ()) / 160.0f;
				Vector newPos = currentTank->getPhysics().getTankGunPosition();
				Vector diff = newPos - position;
				Vector newPos2 = position + (diff);
				newPos2 += Vector(0.0f, 0.0f, 0.5f);

				mainCam_.setLookAt(newPos2);
				mainCam_.movePosition(currentRotation, currentElevation + 1.57f, 3.0f);
			}
		}
		break;
	case CamBehind:
		{
			Vector newPos(
				sinf(currentRotation) * 25.0f, 
				cosf(currentRotation) * 25.0f,
				0.0f);
			Vector newPos2 = position - newPos;

			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation, 1.0f, 60.f);
		}
		break;
	case CamLeftFar:
		{
			Vector newPos(
				sinf(currentRotation) * 65.0f, 
				cosf(currentRotation) * 65.0f,
				0.0f);
			Vector newPos2 = position - newPos;

			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation + HALFPI, 1.0f, 100.f);
		}
		break;
	case CamRightFar:
		{
			Vector newPos(
				sinf(currentRotation) * 65.0f, 
				cosf(currentRotation) * 65.0f,
				0.0f);
			Vector newPos2 = position - newPos;

			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation - HALFPI, 1.4f, 100.f);
		}
		break;
	case CamLeft: 
		{
			Vector newPos(
				sinf(currentRotation) * 10.0f, 
				cosf(currentRotation) * 10.0f,
				0.0f);
			Vector newPos2 = position - newPos;

			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation + HALFPI, 1.4f, 20.f);
		}
		break;
	case CamRight:
		{
			Vector newPos(
				sinf(currentRotation) * 10.0f, 
				cosf(currentRotation) * 10.0f,
				0.0f);
			Vector newPos2 = position - newPos;

			mainCam_.setLookAt(newPos2);
			mainCam_.movePosition(currentRotation - HALFPI, 0.8f, 20.f);
		}
		break;
	case CamSpectator:
		{
			Vector at(128.0f, 128.0f, 0.0f);
			mainCam_.setLookAt(at);
			mainCam_.movePosition(HALFPI, 0.7f, 200.f);
		}
		break;
	default:
		break;
	}

	ShotProjectile::getLookAtCount() = 0;
	ShotProjectile::getLookAtPosition() = Vector();
}

void MainCamera::mouseDrag(const unsigned state, GameState::MouseButton button, 
						   int mx, int my, int x, int y, bool &skipRest)
{
	cameraPos_ = CamFree;
	if (button == GameState::MouseButtonRight)
	{
		const float QPI = 3.14f / 180.0f;
		mainCam_.movePositionDelta(
			(GLfloat) (x) * QPI,
			(GLfloat) (-y) * QPI,
			0.0f);
	}
	else
	{
		mainCam_.movePositionDelta(
			0.0f, 0.0f,
			(GLfloat) (y));
	}
}

void MainCamera::mouseWheel(const unsigned state, short z, bool &skipRest)
{
	cameraPos_ = CamFree;
	mainCam_.movePositionDelta(
			0.0f, 0.0f,
			((GLfloat) z) / 10.0f);
}

void MainCamera::mouseDown(const unsigned state, GameState::MouseButton button, int x, int y, bool &skipRest)
{
	mainCam_.draw();

	Vector intersect;
	Line direction;
	if (mainCam_.getDirectionFromPt((GLfloat) x, (GLfloat) y, direction))
	{
		if (GlobalHMap::instance()->getHMap().getIntersect(direction, intersect))
		{
			skipRest = true;

			// Does the click on the landscape mean we move there
			// or do we just want to look there
			// This depends on the mode the landscape texture is in...
			if (Landscape::instance()->getTextureType() == Landscape::eMovement)
			{
				// Try to move the tank to the position on the landscape
				Tank *currentTank = TankContainer::instance()->getCurrentTank();
				if (currentTank && currentTank->getState().getState() == TankState::sNormal)
				{
					int posX = (int) intersect[0];
					int posY = (int) intersect[1];
					if (posX > 0 && posX < GlobalHMap::instance()->getHMap().getWidth() &&
						posY > 0 && posY < GlobalHMap::instance()->getHMap().getWidth())
					{
						MovementMap::MovementMapEntry &entry =
							GlobalHMap::instance()->getMMap().getEntry(posX, posY);
						if (entry.type == MovementMap::eMovement &&
							entry.dist < currentTank->getAccessories().getFuel().getNoFuel())
						{
							TankAI *ai = currentTank->getTankAI();
							if (ai) ai->move(posX, posY);
						}
					}
				}
			}
			else
			{
				// Just look at the point on the landscape
				cameraPos_ = CamFree;
				mainCam_.setLookAt((Vector &) intersect);
			}
		}
	}	
}

void MainCamera::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, 
							   int hisCount, 
							   bool &skipRest)
{
	KEYBOARDKEY("CAMERA_TOP_VIEW", topViewKey);
	KEYBOARDKEY("CAMERA_BEHIND_VIEW", behindViewKey);
	KEYBOARDKEY("CAMERA_TANK_VIEW", tankViewKey);
	KEYBOARDKEY("CAMERA_GUN_VIEW", gunViewKey);
	KEYBOARDKEY("CAMERA_LEFT_VIEW", leftViewKey);
	KEYBOARDKEY("CAMERA_RIGHT_VIEW", rightViewKey);
	KEYBOARDKEY("CAMERA_LEFTFAR_VIEW", leftFarViewKey);
	KEYBOARDKEY("CAMERA_RIGHTFAR_VIEW", rightFarViewKey);
	KEYBOARDKEY("CAMERA_SPECTATOR_VIEW", spectatorViewKey);

	if (topViewKey->keyDown(buffer, keyState)) cameraPos_ = CamTop;
	else if (behindViewKey->keyDown(buffer, keyState)) cameraPos_ = CamBehind;
	else if (tankViewKey->keyDown(buffer, keyState)) cameraPos_ = CamTank;
	else if (gunViewKey->keyDown(buffer, keyState)) cameraPos_ = CamGun;
	else if (leftViewKey->keyDown(buffer, keyState)) cameraPos_ = CamLeft;
	else if (rightViewKey->keyDown(buffer, keyState)) cameraPos_ = CamRight;
	else if (leftFarViewKey->keyDown(buffer, keyState)) cameraPos_ = CamLeftFar;
	else if (rightFarViewKey->keyDown(buffer, keyState)) cameraPos_ = CamRightFar;
	else if (spectatorViewKey->keyDown(buffer, keyState)) cameraPos_ = CamSpectator;

	const float QPI = 3.14f / 4.0f;
	KEYBOARDKEY("CAMERA_ROTATE_LEFT", leftKey);
	KEYBOARDKEY("CAMERA_ROTATE_RIGHT", rightKey);
	if (leftKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(QPI * frameTime, 0.0f, 0.0f);
	}
	else if (rightKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(-QPI * frameTime, 0.0f, 0.0f);
	}

	KEYBOARDKEY("CAMERA_ROTATE_DOWN", downKey);
	KEYBOARDKEY("CAMERA_ROTATE_UP", upKey);
	if (upKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(0.0f, -QPI * frameTime, 0.0f);
	}
	else if (downKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(0.0f, QPI * frameTime, 0.0f);
	}

	KEYBOARDKEY("CAMERA_ZOOM_IN", inKey);
	KEYBOARDKEY("CAMERA_ZOOM_OUT", outKey);
	if (inKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(0.0f, 0.0f, -100.0f * frameTime);
	}
	else if (outKey->keyDown(buffer, keyState))
	{
		cameraPos_ = CamFree;
		mainCam_.movePositionDelta(0.0f, 0.0f, 100.0f * frameTime);
	}

	KEYBOARDKEY("CAMERA_NOLIMIT", limitKey);
	if (limitKey->keyDown(buffer, keyState))
	{	
		mainCam_.setUseHeightFunc(false);
	}
	else  mainCam_.setUseHeightFunc(true);

	KEYBOARDKEY("SAVE_SCREEN", saveScreenKey);
	if (saveScreenKey->keyDown(buffer, keyState, false))
	{
		saveScreen_.saveScreen_ = true;
	}
}

void MainCamera::SaveScreen::draw(const unsigned state)
{
	if (!saveScreen_) return;
	saveScreen_ = false;

	static unsigned counter = 0;
	time_t currentTime = time(0);
	char fileName[256];
	sprintf(fileName, PKGDIR "ScreenShot-%i-%i.bmp", currentTime, counter++);

	GLBitmap screenMap;
	screenMap.grabScreen();
	screenMap.writeToFile(fileName);

	// Don't print to banner otherwise this message will be in
	// the screenshot!
	GLConsole::instance()->addLine(false, 
		"Screen shot saved as file \"%s\"", fileName);

	// snapshot sound
	CACHE_SOUND(sound,  PKGDIR "data/wav/misc/camera.wav");
	sound->play();
}
