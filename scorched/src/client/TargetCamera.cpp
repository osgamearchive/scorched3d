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

#include <client/TargetCamera.h>
#include <client/ScorchedClient.h>
#include <actions/CameraPositionAction.h>
#include <common/OptionsDisplay.h>
#include <landscape/Landscape.h>
#include <tankai/TankAIHuman.h>
#include <common/Keyboard.h>
#include <common/Defines.h>
#include <math.h>

static const char *cameraNames[] = 
{
	"Top",
	"AboveTank",
	"Tank",
	"Shot",
	"Action",
	"Left",
	"Right",
	"LeftFar",
	"RightFar",
	"Spectator",
	"Free"
};
static const int noCameraNames = sizeof(cameraNames) / sizeof(char *);

static GLWTip *cameraToolTips = 0;
static const char *cameraDescriptions[] = 
{
	"Look directly down on the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from above and behind the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from directly behind the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from the current tanks gun turret.\n"
	"Follows any shots the current tank makes.\n"
	"Tracks the current tanks rotation.",
	"Automaticaly tracks any action around the\n"
	"island by moving to view any explosions\n"
	"and deaths.",
	"Look from at the left of the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from at the right of the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from at the left of the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from at the right of the current tank.\n"
	"Tracks the current tanks rotation.",
	"Look from at the island from afar.",
	"A custom camera position has been made by the\n"
	"user."
};
static const int noCameraDescriptions = sizeof(cameraDescriptions) / sizeof(char *);

TargetCamera::TargetCamera() : mainCam_(300, 300), cameraPos_(CamSpectator)
{
	resetCam();
	mainCam_.setHeightFunc(heightFunc, this);
	DIALOG_ASSERT(noCameraDescriptions == noCameraNames);
}

TargetCamera::~TargetCamera()
{

}

void TargetCamera::resetCam()
{
	Vector at(128.0f, 128.0f, 5.0f);
	mainCam_.setLookAt(at, true);
	cameraPos_ = CamSpectator;
}

const char **TargetCamera::getCameraNames()
{
	return cameraNames;
}

GLWTip *TargetCamera::getCameraToolTips()
{
	if (cameraToolTips == 0)
	{
		cameraToolTips = new GLWTip[noCameraDescriptions];
		for (int i=0; i<noCameraDescriptions; i++)
		{
			cameraToolTips[i].setText(getCameraNames()[i],
				cameraDescriptions[i]);
		}
	}
	return cameraToolTips;
}

int TargetCamera::getNoCameraNames()
{
	return noCameraNames;
}

float TargetCamera::heightFunc(int x, int y, void *data)
{
	TargetCamera *instance = (TargetCamera *) data;
	if (instance->cameraPos_ == CamGun)
	{
		if ((instance->mainCam_.getCurrentPos() - 
			instance->mainCam_.getLookAt()).Magnitude() < 5.0f) return 0.0f;
	}

	const float heightMin = 5.0f;
	float addition = 5.0f;

	float h = 0;
	if (x >= 0 && x < ScorchedClient::instance()->
			getLandscapeMaps().getHMap().getWidth() &&
		y >= 0 && y < ScorchedClient::instance()->
			getLandscapeMaps().getHMap().getWidth())
	{
		h = ScorchedClient::instance()->getLandscapeMaps().
			getHMap().getHeight(x, y) + addition;
	}

	return (h>heightMin + addition?h:heightMin + addition);
}

void TargetCamera::simulate(float frameTime, bool playing)
{
	if (moveCamera(frameTime, playing))
	{
		mainCam_.simulate(frameTime);
	}
}

void TargetCamera::draw()
{
	mainCam_.draw();
}

bool TargetCamera::moveCamera(float frameTime, bool playing)
{
	bool simulateCamera = true;
	Vector position(128.0f, 128.0f, 0.0f);
	float currentRotation = 0.0f;

	Tank *currentTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank && 
		currentTank->getState().getState() == TankState::sNormal)
	{
		position = currentTank->getPhysics().getTankTurretPosition();
		currentRotation = (180.0f - currentTank->getPhysics().getRotationGunXY()) / 57.32f;
	}

	bool viewFromBehindTank = false;
	switch (cameraPos_)
	{
	case CamAction:
		{
			CameraPositionAction *action = 
				CameraPositionActionRegistry::getCurrentAction();
			if (action)
			{
				mainCam_.setLookAt(action->getShowPosition());
				mainCam_.movePosition(currentRotation + 0.3f, 0.7f, 80.0f);
			}
			else viewFromBehindTank = true;
		}
		break;
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
		if (ScorchedClient::instance()->getContext().viewPoints.getLookAtCount() > 0)
		{
			Vector lookatPos, lookfromPos;
			ScorchedClient::instance()->getContext().viewPoints.getValues(lookatPos, lookfromPos);
			mainCam_.setLookAt(lookatPos, true);
			mainCam_.setOffSet(lookfromPos, true);
			//simulateCamera = false;
		}
		else viewFromBehindTank = true;
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
	
	if (viewFromBehindTank)
	{
			if (playing &&
				currentTank && currentTank->getState().getState() == TankState::sNormal)
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

	return simulateCamera;
}

void TargetCamera::mouseDrag(GameState::MouseButton button, 
	int mx, int my, int x, int y, bool &skipRest)
{
	cameraPos_ = CamFree;
	if (button == GameState::MouseButtonRight)
	{
		if (OptionsDisplay::instance()->getInvertMouse())
		{
			y = -y;
		}

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

void TargetCamera::mouseWheel(short z, bool &skipRest)
{
	cameraPos_ = CamFree;
	mainCam_.movePositionDelta(
			0.0f, 0.0f,
			((GLfloat) z) / 10.0f);
}

void TargetCamera::mouseDown(GameState::MouseButton button, 
	int x, int y, bool &skipRest)
{
	mainCam_.draw();

	Vector intersect;
	Line direction;
	if (mainCam_.getDirectionFromPt((GLfloat) x, (GLfloat) y, direction))
	{
		if (ScorchedClient::instance()->getLandscapeMaps().getHMap().
			getIntersect(direction, intersect))
		{
			skipRest = true;

			// Does the click on the landscape mean we move there
			// or do we just want to look there
			// This depends on the mode the landscape texture is in...
			if (Landscape::instance()->getTextureType() == Landscape::eMovement)
			{
				// Try to move the tank to the position on the landscape
				Tank *currentTank = ScorchedClient::instance()->
					getTankContainer().getCurrentTank();
				if (currentTank && currentTank->getState().getState() == TankState::sNormal)
				{
					int posX = (int) intersect[0];
					int posY = (int) intersect[1];
					if (posX > 0 && posX < ScorchedClient::instance()->
							getLandscapeMaps().getHMap().getWidth() &&
						posY > 0 && posY < ScorchedClient::instance()->
							getLandscapeMaps().getHMap().getWidth())
					{
						MovementMap::MovementMapEntry &entry =
							ScorchedClient::instance()->getLandscapeMaps().getMMap().
							getEntry(posX, posY);
						if (entry.type == MovementMap::eMovement &&
							entry.dist < currentTank->getAccessories().getFuel().getNoFuel())
						{
							TankAIHuman *ai = (TankAIHuman *) currentTank->getTankAI();
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

void TargetCamera::keyboardCheck(float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, 
							   int hisCount, 
							   bool &skipRest)
{
	KEYBOARDKEY("CAMERA_TOP_VIEW", topViewKey);
	KEYBOARDKEY("CAMERA_BEHIND_VIEW", behindViewKey);
	KEYBOARDKEY("CAMERA_TANK_VIEW", tankViewKey);
	KEYBOARDKEY("CAMERA_GUN_VIEW", gunViewKey);
	KEYBOARDKEY("CAMERA_ACTION_VIEW", actionViewKey);
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
	else if (actionViewKey->keyDown(buffer, keyState)) cameraPos_ = CamAction;

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
}
