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


// TankAIHuman.cpp: implementation of the TankAIHuman class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Keyboard.h>
#include <common/SoundStore.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <client/OptionsClient.h>
#include <client/StateSetup.h>
#include <tank/TankAIHuman.h>
#include <tank/Tank.h>
#include <engine/GameState.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TankAIHuman::TankAIHuman(Tank *tank) : currentTank_(tank)
{

}

TankAIHuman::~TankAIHuman()
{

}

void TankAIHuman::playMove(const unsigned state, 
						   float frameTime, char *buffer)
{
	float mult = frameTime;
	if (KEYDOWN(buffer, SDLK_LCTRL))
	{
		mult *= 4.0f;

		//MainLowerBanner::instance()->resetTimer();
	}
	else if (KEYDOWN(buffer, SDLK_LSHIFT))
	{
		if (frameTime < 0.01f)
		{
			mult *= 0.25f;
		}
		else
		{
			mult *= 0.05f;
		}

		//MainLowerBanner::instance()->resetTimer();
	}

	moveLeftRight(buffer, mult);
	moveUpDown(buffer, mult);
	movePower(buffer, mult);

	bool fireShotKey = false;
	KEYPRESS_START(buffer, SDLK_SPACE)
		fireShotKey = true;
	KEYPRESS_END
	KEYPRESS_START(buffer, SDLK_f)
		fireShotKey = true;
	KEYPRESS_END
	if (fireShotKey) 
	{
		fireShot();
	}

	KEYPRESS_START(buffer, SDLK_p)
		//activateParachute(!
			//currentTank_->getAccessories().getParachutes().parachutesEnabled());
	KEYPRESS_END

	KEYPRESS_START(buffer, SDLK_b)
		//useBatteries(1);
	KEYPRESS_END

	KEYPRESS_START(buffer, SDLK_u)
		currentTank_->getPhysics().rotateGunXY(
			currentTank_->getPhysics().getOldRotationGunXY(), false);
		currentTank_->getPhysics().rotateGunYZ(
			currentTank_->getPhysics().getOldRotationGunYZ(), false);
		currentTank_->getState().changePower(
			currentTank_->getState().getOldPower(), false);
	KEYPRESS_END

	KEYPRESS_START(buffer,  SDLK_d)
	/* TODO
		if (WindowManager::instance()->
			windowVisible(TankDefenseDialog::instance()->getId()))
		{
			WindowManager::instance()->
				hideWindow(TankDefenseDialog::instance()->getId());
		}
		else
		{
			WindowManager::instance()->
				showWindow(TankDefenseDialog::instance()->getId());
		}*/
	KEYPRESS_END

	KEYPRESS_START(buffer, SDLK_r)
	/* TODO
		if (WindowManager::instance()->
		windowVisible(ResignDialog::instance()->getId()))
		{
			WindowManager::instance()->
			hideWindow(ResignDialog::instance()->getId());
		}
		else
		{
			WindowManager::instance()->
			showWindow(ResignDialog::instance()->getId());
		}
		*/
	KEYPRESS_END

	KEYPRESS_START(buffer, SDLK_TAB)
	/*
		if (mult != frameTime)
		{
			currentTank_->getAccessories().getWeapons().prevWeapon();
		}
		else
		{
			currentTank_->getAccessories().getWeapons().nextWeapon();
		}

		MainLowerBanner::instance()->resetTimer(0.0f, MainLowerBanner::OpWeapon);		
		*/
	KEYPRESS_END
}

void TankAIHuman::moveLeftRight(char *buffer, float mult)
{
	static bool LRMoving = false;
	bool currentLRMoving = false;

	static char messageBuffer[255];
	if (KEYDOWN(buffer, SDLK_RIGHT)) 
	{	
		float rot = currentTank_->getPhysics().
			rotateGunXY(-45.0f * mult) / 360.0f;
		sprintf(messageBuffer, "%.2f Deg", rot * 360.0f);

		currentLRMoving = true;
		//MainLowerBanner::instance()->resetTimer(rot, MainLowerBanner::OpRotation);
	}
	else if (KEYDOWN(buffer, SDLK_LEFT)) 
	{
		float rot = currentTank_->getPhysics().
			rotateGunXY(45.0f * mult) / 360.0f;
		sprintf(messageBuffer, "%.2f Deg", rot * 360.0f);

		currentLRMoving = true;
		//MainLowerBanner::instance()->resetTimer(rot, MainLowerBanner::OpRotation);
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

void TankAIHuman::moveUpDown(char *buffer, float mult)
{
	static bool UDMoving = false;
	bool currentUDMoving = false;

	static char messageBuffer[255];
	unsigned upKey = SDLK_UP;
	unsigned downKey = SDLK_DOWN;
	if (OptionsClient::instance()->getInvertUpDownKey())
	{
		upKey = SDLK_DOWN;
		downKey = SDLK_UP;
	}

	if (KEYDOWN(buffer, upKey)) 
	{
		float rot = currentTank_->getPhysics().
			rotateGunYZ(-45.0f * mult)  / 90.0f;
		sprintf(messageBuffer, "%.2f Deg", rot * 90.0f);

		currentUDMoving = true;
		//MainLowerBanner::instance()->resetTimer(rot, MainLowerBanner::OpElevation);
	}
	else if (KEYDOWN(buffer, downKey)) 
	{
		float rot = currentTank_->getPhysics().
			rotateGunYZ(45.0f * mult)  / 90.0f;
		sprintf(messageBuffer, "%.2f Deg", rot * 90.0f);

		currentUDMoving = true;
		//MainLowerBanner::instance()->resetTimer(rot, MainLowerBanner::OpElevation);
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

void TankAIHuman::movePower(char *buffer, float mult)
{
	static bool PMoving = false;
	bool currentPMoving = false;

	static char messageBuffer[255];
	if (KEYDOWN(buffer, SDLK_EQUALS) ||
		KEYDOWN(buffer, SDLK_PLUS)) 
	{
		float power = currentTank_->getState().
			changePower(250.0f * mult) / 1000.0f;
		sprintf(messageBuffer, "%.2f", power * 1000.0f);

		currentPMoving = true;
		//MainLowerBanner::instance()->resetTimer(power, MainLowerBanner::OpPower);
	}
	else if (KEYDOWN(buffer, SDLK_MINUS)) 
	{
		float power = currentTank_->getState().
			changePower(-250.0f * mult) / 1000.0f;
		sprintf(messageBuffer, "%.2f", power * 1000.0f);

		currentPMoving = true;
		//MainLowerBanner::instance()->resetTimer(power, MainLowerBanner::OpPower);
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

void TankAIHuman::fireShot()
{
	/*ShotProjectileExplosion *newShot = new ShotProjectileExplosion;
	newShot->aim(
		currentTank_->getPhysics().getTankGunPosition(), 
		currentTank_->getPhysics().getRotationGunXY(),
		currentTank_->getPhysics().getRotationGunYZ(),
		currentTank_->getState().getPower());

	ActionController::instance()->addAction(newShot);*/

	Weapon *currentWeapon = 
		currentTank_->getAccessories().getWeapons().getCurrent();
	if (currentWeapon)
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(ComsPlayedMoveMessage::eShot);
		comsMessage.setShot(
			currentWeapon->getName(),
			currentTank_->getPhysics().getRotationGunXY(),
			currentTank_->getPhysics().getRotationGunYZ(),
			currentTank_->getState().getPower());
		ComsMessageSender::sendToServer(comsMessage);

		// Stimulate into the next state waiting for all the shots
		GameState::instance()->stimulate(MainGameState::StimShot);
	}
}
