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

#include <common/Keyboard.h>
#include <common/SoundStore.h>
#include <common/OptionsDisplay.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <tankai/TankAIHuman.h>
#include <tankgraph/TankModelRenderer.h>
#include <tank/Tank.h>
#include <stdio.h>

static void swapKeys(KeyboardKey *&key1, KeyboardKey *&key2)
{
	KeyboardKey *tmp = key1;
	key1 = key2;
	key2 = key1;
}

TankAIHuman::TankAIHuman(ScorchedContext *context, Tank *tank) : TankAI(context, tank)
{
}

TankAIHuman::~TankAIHuman()
{
}

void TankAIHuman::newGame()
{
}

void TankAIHuman::nextRound()
{
}

void TankAIHuman::tankHurt(Weapon *weapon, unsigned int firer)
{
}

void TankAIHuman::shotLanded(Weapon *weapon, unsigned int firer, 
							 Vector &position)
{
}

void TankAIHuman::playMove(const unsigned state, 
						   float frameTime, char *buffer, 
						   unsigned int keyState)
{
	moveLeftRight(buffer, keyState, frameTime);
	moveUpDown(buffer, keyState, frameTime);
	movePower(buffer, keyState, frameTime);

	KEYBOARDKEY("FIRE_WEAPON", fireKey);
	if (fireKey->keyDown(buffer, keyState, false))
	{
		fireShot();
	}

	KEYBOARDKEY("AUTO_AIM", aimKey);
	if (aimKey->keyDown(buffer, keyState))
	{
		autoAim();
	}

	KEYBOARDKEY("ENABLE_PARACHUTES", parachuteKey);
	if (parachuteKey->keyDown(buffer, keyState, false))
	{
		parachutesUpDown(true);
	}

	KEYBOARDKEY("USE_BATTERY", batteryKey);
	if (batteryKey->keyDown(buffer, keyState, false))
	{
		if (currentTank_->getState().getLife() < 100.0f)
		{
			useBattery();
		}
	}

	KEYBOARDKEY("UNDO_MOVE", undoKey);
	if (undoKey->keyDown(buffer, keyState, false))
	{
		currentTank_->getPhysics().rotateGunXY(
			currentTank_->getPhysics().getOldRotationGunXY(), false);
		currentTank_->getPhysics().rotateGunYZ(
			currentTank_->getPhysics().getOldRotationGunYZ(), false);
		currentTank_->getState().changePower(
			currentTank_->getState().getOldPower(), false);
	}

	KEYBOARDKEY("CHANGE_UP_WEAPON", weaponUpKey);
	KEYBOARDKEY("CHANGE_DOWN_WEAPON", weaponDownKey);
	bool upWeapon = weaponUpKey->keyDown(buffer, keyState, false);
	bool downWeapon = weaponDownKey->keyDown(buffer, keyState, false);
	if (upWeapon || downWeapon)
	{
		if (downWeapon)
		{
			currentTank_->getAccessories().getWeapons().prevWeapon();
		}
		else
		{
			currentTank_->getAccessories().getWeapons().nextWeapon();
		}

		TankModelRendererHUD::setText("Weapon : ", 
			currentTank_->getAccessories().getWeapons().getWeaponString());
	}
}

void TankAIHuman::endPlayMove()
{
	CACHE_SOUND(turn,  PKGDIR "data/wav/movement/turn.wav");
	CACHE_SOUND(elevate,  PKGDIR "data/wav/movement/elevate.wav");

	turn->stop();
	elevate->stop();
}

void TankAIHuman::autoAim()
{
	int x = ScorchedClient::instance()->getGameState().getMouseX();
	int y = ScorchedClient::instance()->getGameState().getMouseY();
	Line direction;
	Vector intersect;

	MainCamera::instance()->getCamera().draw();
	if (MainCamera::instance()->getCamera().
		getDirectionFromPt((GLfloat) x, (GLfloat) y, direction))
	{
		if (context_->landscapeMaps.getHMap().getIntersect(direction, intersect))
        {
			Vector &position = currentTank_->getPhysics().getTankPosition();

			// Calculate direction
			Vector direction = intersect - position;
			float angleXYRads = atan2f(direction[1], direction[0]);
			float angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;
			
			currentTank_->getPhysics().rotateGunXY(angleXYDegs, false);
			leftRightHUD();

			TankModelRendererAIM::setAimPosition(intersect);
		}
	}
}

void TankAIHuman::moveLeftRight(char *buffer, unsigned int keyState, float frameTime)
{
	static bool LRMoving = false;
	bool currentLRMoving = false;

	KEYBOARDKEY("TURN_RIGHT", rightKey);
	KEYBOARDKEY("TURN_RIGHT_SLOW", rightSlowKey);
	KEYBOARDKEY("TURN_RIGHT_FAST", rightFastKey);
	KEYBOARDKEY("TURN_LEFT", leftKey);
	KEYBOARDKEY("TURN_LEFT_SLOW", leftSlowKey);
	KEYBOARDKEY("TURN_LEFT_FAST", leftFastKey);

	bool rightK = rightKey->keyDown(buffer, keyState);
	bool rightKF = rightFastKey->keyDown(buffer, keyState);
	bool rightKS = rightSlowKey->keyDown(buffer, keyState);
	bool leftK = leftKey->keyDown(buffer, keyState);
	bool leftKF = leftFastKey->keyDown(buffer, keyState);
	bool leftKS = leftSlowKey->keyDown(buffer, keyState);

	if (rightK || rightKF || rightKS)
	{
		float mult = frameTime;
		if (rightKF) mult *= 4.0f;
		else if (rightKS) mult *= 0.25f;

		currentTank_->getPhysics().rotateGunXY(-45.0f * mult);
		currentLRMoving = true;

		leftRightHUD();
	}
	else if (leftK || leftKF || leftKS)
	{
		float mult = frameTime;
		if (leftKF) mult *= 4.0f;
		else if (leftKS) mult *= 0.25f;

		currentTank_->getPhysics().rotateGunXY(45.0f * mult);
		currentLRMoving = true;

		leftRightHUD();
	}

	if (LRMoving != currentLRMoving)
	{
		CACHE_SOUND(sound,  PKGDIR "data/wav/movement/movement.wav");
		CACHE_SOUND(turn,  PKGDIR "data/wav/movement/turn.wav");
		if (currentLRMoving)
		{
			sound->play();
			turn->setRepeats();
			turn->play();
		}
		else
		{
			turn->stop();
		}

		LRMoving = currentLRMoving;
	}
} 

void TankAIHuman::leftRightHUD()
{		
	float rot = currentTank_->getPhysics().getRotationGunXY() / 360.0f;
	TankModelRendererHUD::setText("Rot:", 
		currentTank_->getPhysics().getRotationString(), rot * 100.0f);
}

void TankAIHuman::moveUpDown(char *buffer, unsigned int keyState, float frameTime)
{
	static bool UDMoving = false;
	bool currentUDMoving = false;

	KEYBOARDKEY("ROTATE_UP", upKey);
	KEYBOARDKEY("ROTATE_UP_SLOW", upSlowKey);
	KEYBOARDKEY("ROTATE_UP_FAST", upFastKey);
	KEYBOARDKEY("ROTATE_DOWN", downKey);
	KEYBOARDKEY("ROTATE_DOWN_SLOW", downSlowKey);
	KEYBOARDKEY("ROTATE_DOWN_FAST", downFastKey);
	if (OptionsDisplay::instance()->getInvertUpDownKey())
	{
		swapKeys(upKey, downKey);
		swapKeys(upSlowKey, downSlowKey);
		swapKeys(upFastKey, downFastKey);
	}

	bool upK = upKey->keyDown(buffer, keyState);
	bool upKS = upSlowKey->keyDown(buffer, keyState);
	bool upKF = upFastKey->keyDown(buffer, keyState);
	bool downK = downKey->keyDown(buffer, keyState);
	bool downKS = downSlowKey->keyDown(buffer, keyState);
	bool downKF = downFastKey->keyDown(buffer, keyState);

	if (upK || upKS || upKF)
	{
		float mult = frameTime;
		if (upKF) mult *= 4.0f;
		else if (upKS) mult *= 0.25f;

		currentTank_->getPhysics().rotateGunYZ(-45.0f * mult);
		currentUDMoving = true;

		upDownHUD();
	}
	else if (downK || downKS || downKF)
	{
		float mult = frameTime;
		if (downKF) mult *= 4.0f;
		else if (downKS) mult *= 0.25f;

		currentTank_->getPhysics().rotateGunYZ(45.0f * mult);
		currentUDMoving = true;

		upDownHUD();
	}

	if (UDMoving != currentUDMoving)
	{
		CACHE_SOUND(sound,  PKGDIR "data/wav/movement/movement.wav");
		CACHE_SOUND(elevate,  PKGDIR "data/wav/movement/elevate.wav");
		if (currentUDMoving)
		{
			sound->play();
			elevate->setRepeats();
			elevate->play();
		}
		else
		{
			elevate->stop();
		}

		UDMoving = currentUDMoving;
	}
}

void TankAIHuman::upDownHUD()
{
	float rot = currentTank_->getPhysics().getRotationGunYZ() / 90.0f;
	TankModelRendererHUD::setText("Ele:", 
		currentTank_->getPhysics().getElevationString(), rot * 100.0f);
}

void TankAIHuman::movePower(char *buffer, unsigned int keyState, float frameTime)
{
	static bool PMoving = false;
	bool currentPMoving = false;

	KEYBOARDKEY("INCREASE_POWER", incKey);
	KEYBOARDKEY("INCREASE_POWER_SLOW", incSlowKey);
	KEYBOARDKEY("INCREASE_POWER_FAST", incFastKey);
	KEYBOARDKEY("DECREASE_POWER", decKey);
	KEYBOARDKEY("DECREASE_POWER_SLOW", decSlowKey);
	KEYBOARDKEY("DECREASE_POWER_FAST", decFastKey);
	bool incK = incKey->keyDown(buffer, keyState);
	bool incKS = incSlowKey->keyDown(buffer, keyState);
	bool incKF = incFastKey->keyDown(buffer, keyState);
	bool decK = decKey->keyDown(buffer, keyState);
	bool decKS = decSlowKey->keyDown(buffer, keyState);
	bool decKF = decFastKey->keyDown(buffer, keyState);

	if (incK || incKS || incKF) 
	{
		float mult = frameTime;
		if (incKF) mult *= 4.0f;
		else if (incKS) mult *= 0.25f;

		currentTank_->getState().changePower(250.0f * mult);
		currentPMoving = true;

		powerHUD();
	}
	else if (decK || decKS || decKF) 
	{
		float mult = frameTime;
		if (decKF) mult *= 4.0f;
		else if (decKS) mult *= 0.25f;

		currentTank_->getState().changePower(-250.0f * mult);
		currentPMoving = true;

		powerHUD();
	}

	if (PMoving != currentPMoving)
	{
		CACHE_SOUND(elevate,  PKGDIR "data/wav/movement/elevate.wav");
		if (currentPMoving)
		{
			elevate->setRepeats();
			elevate->play();
		}
		else
		{
			elevate->stop();
		}

		PMoving = currentPMoving;
	}
}

void TankAIHuman::powerHUD()
{
	float power = currentTank_->getState().getPower() / 1000.0f;
	TankModelRendererHUD::setText("Pwr:", 
		currentTank_->getState().getPowerString(), power * 100.0f);
}
