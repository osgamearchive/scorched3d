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
#include <weapons/Weapon.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <tankai/TankAIHuman.h>
#include <tankai/TankAILogic.h>
#include <tankgraph/TankModelRenderer.h>
#include <coms/ComsMessageSender.h>
#include <tank/Tank.h>
#include <stdio.h>

TankAIHuman::TankAIHuman(ScorchedContext *context, Tank *tank) : TankAI(context, tank)
{
}

TankAIHuman::~TankAIHuman()
{
}

void TankAIHuman::newGame()
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
		currentTank_->getPhysics().revertRotation();
		currentTank_->getState().revertPower();
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
	CACHE_SOUND(turn,  (char *) getDataFile("data/wav/movement/turn.wav"));
	CACHE_SOUND(elevate,  (char *) getDataFile("data/wav/movement/elevate.wav"));

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
		CACHE_SOUND(sound, (char *) getDataFile("data/wav/movement/movement.wav"));
		CACHE_SOUND(turn, (char *) getDataFile("data/wav/movement/turn.wav"));
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

	KEYBOARDKEY("ROTATE_UP", staticUpKey);
	KEYBOARDKEY("ROTATE_UP_SLOW", staticUpSlowKey);
	KEYBOARDKEY("ROTATE_UP_FAST", staticUpFastKey);
	KEYBOARDKEY("ROTATE_DOWN", staticDownKey);
	KEYBOARDKEY("ROTATE_DOWN_SLOW", staticDownSlowKey);
	KEYBOARDKEY("ROTATE_DOWN_FAST", staticDownFastKey);

	KeyboardKey *upKey = staticUpKey;
	KeyboardKey *upSlowKey = staticUpSlowKey;
	KeyboardKey *upFastKey = staticUpFastKey;
	KeyboardKey *downKey = staticDownKey;
	KeyboardKey *downSlowKey = staticDownSlowKey;
	KeyboardKey *downFastKey = staticDownFastKey;
	if (OptionsDisplay::instance()->getInvertUpDownKey())
	{
		upKey = staticDownKey;
		upSlowKey = staticDownSlowKey;
		upFastKey = staticDownFastKey;
		downKey = staticUpKey;
		downSlowKey = staticUpSlowKey;
		downFastKey = staticUpFastKey;
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
		CACHE_SOUND(sound, (char *) getDataFile("data/wav/movement/movement.wav"));
		CACHE_SOUND(elevate, (char *) getDataFile("data/wav/movement/elevate.wav"));
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
		CACHE_SOUND(elevate, (char *) getDataFile("data/wav/movement/elevate.wav"));
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


void TankAIHuman::fireShot()
{
	Weapon *currentWeapon = 
		currentTank_->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eShot);
		comsMessage.setShot(
			currentWeapon->getAccessoryId(),
			currentTank_->getPhysics().getRotationGunXY(),
			currentTank_->getPhysics().getRotationGunYZ(),
			currentTank_->getState().getPower());

		// If so we send this move to the server
		ComsMessageSender::sendToServer(comsMessage);

		// Stimulate into the next state waiting for all the shots
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	}
}

void TankAIHuman::skipShot()
{
	// send message saying we are finished with shot
	ComsPlayedMoveMessage comsMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eSkip);

	// Check if we are running in a NET/LAN environment
	// If so we send this move to the server
	ComsMessageSender::sendToServer(comsMessage);

	// Stimulate into the next state waiting for all the shots
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}

void TankAIHuman::resign()
{
	// send message saying we are finished with shot
	ComsPlayedMoveMessage comsMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eResign);

	// Check if we are running in a NET/LAN environment
	// If so we send this move to the server
	ComsMessageSender::sendToServer(comsMessage);
	
	// Stimulate into the next state waiting for all the shots
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}

void TankAIHuman::move(int x, int y)
{
	// send message saying we are finished with shot
	ComsPlayedMoveMessage comsMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eMove);
	comsMessage.setPosition(x, y);

	// If so we send this move to the server
	ComsMessageSender::sendToServer(comsMessage);
	
	// Stimulate into the next state waiting for all the shots
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}

void TankAIHuman::parachutesUpDown(bool on)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		on?ComsDefenseMessage::eParachutesUp:ComsDefenseMessage::eParachutesDown);

	// Check if we are running in a NET/LAN environment
	// If so we send this defense action to the server
	ComsMessageSender::sendToServer(defenseMessage);
}

void TankAIHuman::shieldsUpDown(unsigned int shieldId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(shieldId!=0)?ComsDefenseMessage::eShieldUp:ComsDefenseMessage::eShieldDown,
		shieldId);
	
	// Check if we are running in a NET/LAN environment
	// If so we send this defense action to the server
	ComsMessageSender::sendToServer(defenseMessage);
}

void TankAIHuman::useBattery()
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		ComsDefenseMessage::eBatteryUse);

	// Check if we are running in a NET/LAN environment
	// If so we send this defense action to the server
	ComsMessageSender::sendToServer(defenseMessage);
}
