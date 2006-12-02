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
#include <common/DefinesScorched.h>
#include <sound/Sound.h>
#include <weapons/AccessoryStore.h>
#include <GLEXT/GLConsole.h>
#include <graph/MainCamera.h>
#include <graph/OptionsDisplay.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <tankai/TankAIHuman.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <landscapemap/LandscapeMaps.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsDefenseMessage.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <tank/Tank.h>
#include <tank/TankPosition.h>
#include <tank/TankAccessories.h>
#include <target/TargetShield.h>
#include <target/TargetLife.h>
#include <stdio.h>

TankAIHuman::TankAIHuman() :
	elevateSound_(0), rotateSound_(0), startSound_(0), powerSound_(0)
{
	description_.setText("Human",
		"A human controlled player\n"
		"(turns off any computer controller player).");
}

TankAIHuman::~TankAIHuman()
{
	delete elevateSound_;
	delete rotateSound_;
	delete startSound_;
	delete powerSound_;
}

void TankAIHuman::newGame()
{
}

void TankAIHuman::newMatch()
{
}

void TankAIHuman::tankHurt(Weapon *weapon, unsigned int firer)
{
}

void TankAIHuman::shotLanded(ScorchedCollisionType action,
							 ScorchedCollisionInfo *collision,
							 Weapon *weapon, unsigned int firer, 
							 Vector &position,
							 unsigned int landedCounter)
{
}

void TankAIHuman::playMove(const unsigned state, 
						   float frameTime, char *buffer, 
						   unsigned int keyState)
{
	/*static float totalFrameTime = 0.0f;
	totalFrameTime += frameTime;
	while (totalFrameTime > 0.01f)
	{
		totalFrameTime-=0.01f;
		GLConsole::instance()->addLine(true, 
			"say \"Spam... 123123232131231232131321\"");
	}*/

	if (!elevateSound_) elevateSound_ = 
		new VirtualSoundSource(VirtualSoundPriority::eRotation, true, false);
	elevateSound_->setPosition(currentTank_->getPosition().getTankPosition());
	if (!rotateSound_) rotateSound_ =
		new VirtualSoundSource(VirtualSoundPriority::eRotation, true, false);
	rotateSound_->setPosition(currentTank_->getPosition().getTankPosition());
	if (!startSound_) startSound_ =
		new VirtualSoundSource(VirtualSoundPriority::eRotation, false, false);
	startSound_->setPosition(currentTank_->getPosition().getTankPosition());
	if (!powerSound_) powerSound_ =
		new VirtualSoundSource(VirtualSoundPriority::eRotation, true, false);
	powerSound_->setPosition(currentTank_->getPosition().getTankPosition());

	moveLeftRight(buffer, keyState, frameTime);
	moveUpDown(buffer, keyState, frameTime);
	movePower(buffer, keyState, frameTime);

	KEYBOARDKEY("FIRE_WEAPON", fireKey);
	if (fireKey->keyDown(buffer, keyState, false))
	{
		// Only allow the standard fire key when we are not selecting a point
		// on the landscape.
		Accessory *currentWeapon = 
			currentTank_->getAccessories().getWeapons().getCurrent();
		if (currentWeapon &&
			currentWeapon->getPositionSelect() == Accessory::ePositionSelectNone)
		{
			fireShot();
		}
	}

	KEYBOARDKEY("AUTO_AIM", aimKey);
	if (aimKey->keyDown(buffer, keyState))
	{
		autoAim();
	}

	KEYBOARDKEY("ENABLE_PARACHUTES", parachuteKey);
	if (parachuteKey->keyDown(buffer, keyState, false))
	{
		std::list<Accessory *> &parachutes =
			currentTank_->getAccessories().getAllAccessoriesByType(
				AccessoryPart::AccessoryParachute);
		if (parachutes.size() == 1)
		{
			parachutesUpDown(parachutes.front()->getAccessoryId());
		}
	}

	KEYBOARDKEY("ENABLE_SHIELDS", shieldKey);
	if (shieldKey->keyDown(buffer, keyState, false))
	{
		if (!currentTank_->getShield().getCurrentShield())
		{
			std::list<Accessory *> &shields =
				currentTank_->getAccessories().getAllAccessoriesByType(
					AccessoryPart::AccessoryShield);
			if (shields.size() == 1)
			{
				shieldsUpDown(shields.front()->getAccessoryId());
			}
		}
	}

	KEYBOARDKEY("USE_BATTERY", batteryKey);
	if (batteryKey->keyDown(buffer, keyState, false))
	{
		if (currentTank_->getLife().getLife() < 
			currentTank_->getLife().getMaxLife())
		{
			std::list<Accessory *> &entries =
				currentTank_->getAccessories().getAllAccessoriesByType(
					AccessoryPart::AccessoryBattery);
			if (!entries.empty())
			{
				useBattery(entries.front()->getAccessoryId());
			}
		}
	}

	KEYBOARDKEY("UNDO_MOVE", undoKey);
	if (undoKey->keyDown(buffer, keyState, false))
	{
		currentTank_->getPosition().undo();
	}

	KEYBOARDKEY("CHANGE_UP_WEAPON", weaponUpKey);
	KEYBOARDKEY("CHANGE_DOWN_WEAPON", weaponDownKey);
	bool upWeapon = weaponUpKey->keyDown(buffer, keyState, false);
	bool downWeapon = weaponDownKey->keyDown(buffer, keyState, false);
	if (upWeapon || downWeapon)
	{
		if (downWeapon)
		{
			prevWeapon();
		}
		else
		{
			nextWeapon();
		}

		TargetRendererImplTankHUD::setText("Weapon : ", 
			currentTank_->getAccessories().getWeapons().getWeaponString());
	}
}

void TankAIHuman::nextWeapon()
{
	std::list<Accessory *> &result =
		currentTank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryWeapon);
	ScorchedClient::instance()->getAccessoryStore().sortList(result,
		OptionsDisplay::instance()->getSortAccessories());

	std::list<Accessory *>::iterator itor;
	for (itor = result.begin();
		itor != result.end();
		itor++)
	{
		if (currentTank_->getAccessories().getWeapons().getCurrent() == (*itor))
		{
			if (++itor == result.end())
			{
				itor = result.begin();
			}
			currentTank_->getAccessories().getWeapons().setWeapon(*itor);
			break;
		}
	}
}

void TankAIHuman::prevWeapon()
{
	std::list<Accessory *> &result =
		currentTank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryWeapon);
	ScorchedClient::instance()->getAccessoryStore().sortList(result,
		OptionsDisplay::instance()->getSortAccessories());

	std::list<Accessory *>::iterator itor;
	for (itor = result.begin();
		itor != result.end();
		itor++)
	{
		if (currentTank_->getAccessories().getWeapons().getCurrent() == (*itor))
		{
			if (itor == result.begin())
			{
				itor = result.end();
			}

			--itor;
			currentTank_->getAccessories().getWeapons().setWeapon(*itor);
			break;
		}
	}
}

void TankAIHuman::endPlayMove()
{
	if (elevateSound_) elevateSound_->stop();
	if (rotateSound_) rotateSound_->stop();
	if (powerSound_) powerSound_->stop();
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
		if (ScorchedClient::instance()->getLandscapeMaps().
			getGroundMaps().getIntersect(direction, intersect))
        {
			Vector &position = currentTank_->getPosition().getTankPosition();

			// Calculate direction
			Vector direction = intersect - position;
			float angleXYRads = atan2f(direction[1], direction[0]);
			float angleXYDegs = (angleXYRads / 3.14f) * 180.0f - 90.0f;
			
			currentTank_->getPosition().rotateGunXY(angleXYDegs, false);
			leftRightHUD();

			TargetRendererImplTankAIM::setAimPosition(intersect);
		}
	}
}

void TankAIHuman::moveLeftRight(char *buffer, unsigned int keyState, float frameTime)
{
	static bool LRMoving = false;
	bool currentLRMoving = false;

	KEYBOARDKEY("TURN_RIGHT", rightKey);
	KEYBOARDKEY("TURN_RIGHT_SLOW", rightSlowKey);
	KEYBOARDKEY("TURN_RIGHT_VSLOW", rightVSlowKey);
	KEYBOARDKEY("TURN_RIGHT_FAST", rightFastKey);
	KEYBOARDKEY("TURN_LEFT", leftKey);
	KEYBOARDKEY("TURN_LEFT_SLOW", leftSlowKey);
	KEYBOARDKEY("TURN_LEFT_VSLOW", leftVSlowKey);
	KEYBOARDKEY("TURN_LEFT_FAST", leftFastKey);

	bool rightK = rightKey->keyDown(buffer, keyState);
	bool rightKF = rightFastKey->keyDown(buffer, keyState);
	bool rightKS = rightSlowKey->keyDown(buffer, keyState);
	bool rightKVS = rightVSlowKey->keyDown(buffer, keyState);
	bool leftK = leftKey->keyDown(buffer, keyState);
	bool leftKF = leftFastKey->keyDown(buffer, keyState);
	bool leftKS = leftSlowKey->keyDown(buffer, keyState);
	bool leftKVS = leftVSlowKey->keyDown(buffer, keyState);

	if (rightK || rightKF || rightKS || rightKVS)
	{
		float mult = frameTime;
		if (rightKF) mult *= 4.0f;
		else if (rightKS) mult *= 0.25f;
		else if (rightKVS) mult *= 0.05f;

		currentTank_->getPosition().rotateGunXY(-45.0f * mult);
		currentLRMoving = true;

		leftRightHUD();
	}
	else if (leftK || leftKF || leftKS || leftKVS)
	{
		float mult = frameTime;
		if (leftKF) mult *= 4.0f;
		else if (leftKS) mult *= 0.25f;
		else if (leftKVS) mult *= 0.05f;

		currentTank_->getPosition().rotateGunXY(45.0f * mult);
		currentLRMoving = true;

		leftRightHUD();
	}

	if (LRMoving != currentLRMoving)
	{
		CACHE_SOUND(sound, (char *) getDataFile("data/wav/movement/movement.wav"));
		CACHE_SOUND(turn, (char *) getDataFile("data/wav/movement/turn.wav"));
		if (currentLRMoving)
		{
			startSound_->play(sound);
			rotateSound_->play(turn);
		}
		else
		{
			rotateSound_->stop();
		}

		LRMoving = currentLRMoving;
	}
} 

void TankAIHuman::leftRightHUD()
{		
	float rot = currentTank_->getPosition().getRotationGunXY() / 360.0f;
	TargetRendererImplTankHUD::setText("Rot:", 
		currentTank_->getPosition().getRotationString(), rot * 100.0f);
}

void TankAIHuman::moveUpDown(char *buffer, unsigned int keyState, float frameTime)
{
	static bool UDMoving = false;
	bool currentUDMoving = false;

	KEYBOARDKEY("ROTATE_UP", staticUpKey);
	KEYBOARDKEY("ROTATE_UP_SLOW", staticUpSlowKey);
	KEYBOARDKEY("ROTATE_UP_VSLOW", staticUpVSlowKey);
	KEYBOARDKEY("ROTATE_UP_FAST", staticUpFastKey);
	KEYBOARDKEY("ROTATE_DOWN", staticDownKey);
	KEYBOARDKEY("ROTATE_DOWN_SLOW", staticDownSlowKey);
	KEYBOARDKEY("ROTATE_DOWN_VSLOW", staticDownVSlowKey);
	KEYBOARDKEY("ROTATE_DOWN_FAST", staticDownFastKey);

	KeyboardKey *upKey = staticUpKey;
	KeyboardKey *upSlowKey = staticUpSlowKey;
	KeyboardKey *upVSlowKey = staticUpVSlowKey;
	KeyboardKey *upFastKey = staticUpFastKey;
	KeyboardKey *downKey = staticDownKey;
	KeyboardKey *downSlowKey = staticDownSlowKey;
	KeyboardKey *downVSlowKey = staticDownVSlowKey;
	KeyboardKey *downFastKey = staticDownFastKey;
	if (OptionsDisplay::instance()->getInvertElevation())
	{
		upKey = staticDownKey;
		upSlowKey = staticDownSlowKey;
		upVSlowKey = staticDownVSlowKey;
		upFastKey = staticDownFastKey;
		downKey = staticUpKey;
		downSlowKey = staticUpSlowKey;
		downVSlowKey = staticUpVSlowKey;
		downFastKey = staticUpFastKey;
	}

	bool upK = upKey->keyDown(buffer, keyState);
	bool upKS = upSlowKey->keyDown(buffer, keyState);
	bool upKVS = upVSlowKey->keyDown(buffer, keyState);
	bool upKF = upFastKey->keyDown(buffer, keyState);
	bool downK = downKey->keyDown(buffer, keyState);
	bool downKS = downSlowKey->keyDown(buffer, keyState);
	bool downKVS = downVSlowKey->keyDown(buffer, keyState);
	bool downKF = downFastKey->keyDown(buffer, keyState);

	if (upK || upKS || upKF || upKVS)
	{
		float mult = frameTime;
		if (upKF) mult *= 4.0f;
		else if (upKS) mult *= 0.25f;
		else if (upKVS) mult *= 0.05f;

		currentTank_->getPosition().rotateGunYZ(-45.0f * mult);
		currentUDMoving = true;

		upDownHUD();
	}
	else if (downK || downKS || downKF || downKVS)
	{
		float mult = frameTime;
		if (downKF) mult *= 4.0f;
		else if (downKS) mult *= 0.25f;
		else if (downKVS) mult *= 0.05f;

		currentTank_->getPosition().rotateGunYZ(45.0f * mult);
		currentUDMoving = true;

		upDownHUD();
	}

	if (UDMoving != currentUDMoving)
	{
		CACHE_SOUND(sound, (char *) getDataFile("data/wav/movement/movement.wav"));
		CACHE_SOUND(elevate, (char *) getDataFile("data/wav/movement/elevate.wav"));
		if (currentUDMoving)
		{
			startSound_->play(sound);
			elevateSound_->play(elevate);
		}
		else
		{
			elevateSound_->stop();
		}

		UDMoving = currentUDMoving;
	}
}

void TankAIHuman::upDownHUD()
{
	float rot = currentTank_->getPosition().getRotationGunYZ() / 90.0f;
	TargetRendererImplTankHUD::setText("Ele:", 
		currentTank_->getPosition().getElevationString(), rot * 100.0f);
}

void TankAIHuman::movePower(char *buffer, unsigned int keyState, float frameTime)
{
	static bool PMoving = false;
	bool currentPMoving = false;

	KEYBOARDKEY("INCREASE_POWER", incKey);
	KEYBOARDKEY("INCREASE_POWER_SLOW", incSlowKey);
	KEYBOARDKEY("INCREASE_POWER_VSLOW", incVSlowKey);
	KEYBOARDKEY("INCREASE_POWER_FAST", incFastKey);
	KEYBOARDKEY("DECREASE_POWER", decKey);
	KEYBOARDKEY("DECREASE_POWER_SLOW", decSlowKey);
	KEYBOARDKEY("DECREASE_POWER_VSLOW", decVSlowKey);
	KEYBOARDKEY("DECREASE_POWER_FAST", decFastKey);
	bool incK = incKey->keyDown(buffer, keyState);
	bool incKS = incSlowKey->keyDown(buffer, keyState);
	bool incKVS = incVSlowKey->keyDown(buffer, keyState);
	bool incKF = incFastKey->keyDown(buffer, keyState);
	bool decK = decKey->keyDown(buffer, keyState);
	bool decKS = decSlowKey->keyDown(buffer, keyState);
	bool decKVS = decVSlowKey->keyDown(buffer, keyState);
	bool decKF = decFastKey->keyDown(buffer, keyState);

	if (incK || incKS || incKF || incKVS) 
	{
		float mult = frameTime;
		if (incKF) mult *= 4.0f;
		else if (incKS) mult *= 0.25f;
		else if (incKVS) mult *= 0.05f;

		currentTank_->getPosition().changePower(250.0f * mult);
		currentPMoving = true;

		powerHUD();
	}
	else if (decK || decKS || decKF || decKVS) 
	{
		float mult = frameTime;
		if (decKF) mult *= 4.0f;
		else if (decKS) mult *= 0.25f;
		else if (decKVS) mult *= 0.05f;

		currentTank_->getPosition().changePower(-250.0f * mult);
		currentPMoving = true;

		powerHUD();
	}

	if (PMoving != currentPMoving)
	{
		CACHE_SOUND(power, (char *) getDataFile("data/wav/movement/power.wav"));
		if (currentPMoving)
		{
			powerSound_->play(power);
		}
		else
		{
			powerSound_->stop();
		}

		PMoving = currentPMoving;
	}
}

void TankAIHuman::powerHUD()
{
	float power = currentTank_->getPosition().getPower() / 
		currentTank_->getPosition().getMaxPower();
	TargetRendererImplTankHUD::setText("Pwr:", 
		currentTank_->getPosition().getPowerString(), power * 100.0f);
}


void TankAIHuman::fireShot()
{
	Accessory *currentWeapon = 
		currentTank_->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(currentTank_->getPlayerId(), ComsPlayedMoveMessage::eShot);
		comsMessage.setShot(
			currentWeapon->getAccessoryId(),
			currentTank_->getPosition().getRotationGunXY(),
			currentTank_->getPosition().getRotationGunYZ(),
			currentTank_->getPosition().getPower(),
			currentTank_->getPosition().getSelectPositionX(),
			currentTank_->getPosition().getSelectPositionY());

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

void TankAIHuman::parachutesUpDown(unsigned int paraId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(paraId!=0)?ComsDefenseMessage::eParachutesUp:ComsDefenseMessage::eParachutesDown,
		paraId);
	ComsMessageSender::sendToServer(defenseMessage);
}

void TankAIHuman::shieldsUpDown(unsigned int shieldId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		(shieldId!=0)?ComsDefenseMessage::eShieldUp:ComsDefenseMessage::eShieldDown,
		shieldId);
	ComsMessageSender::sendToServer(defenseMessage);
}

void TankAIHuman::useBattery(unsigned int batteryId)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		ComsDefenseMessage::eBatteryUse,
		batteryId);
	ComsMessageSender::sendToServer(defenseMessage);
}
