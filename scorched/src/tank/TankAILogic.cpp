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


#include <tank/TankAILogic.h>
#include <tank/Tank.h>
#include <landscape/GlobalHMap.h>
#include <actions/TankMovement.h>
#include <actions/TankResign.h>
#include <actions/TankFired.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Battery.h>
#include <weapons/Parachute.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/SoundStore.h>
#include <engine/ActionController.h>

void TankAILogic::processPlayedMoveMessage(ComsPlayedMoveMessage &message, Tank *tank)
{
	switch (message.getType())
	{
		case ComsPlayedMoveMessage::eShot:
			processFiredMessage(message, tank);
			break;
		case ComsPlayedMoveMessage::eSkip:
			// Just do nothing as the player has requested
			// That they skip their move
			break;
		case ComsPlayedMoveMessage::eResign:
			processResignMessage(message, tank);
			break;
		case ComsPlayedMoveMessage::eMove:
			processMoveMessage(message, tank);
			break;
		default:
			// add other types
			break;
	}
}

void TankAILogic::processMoveMessage(ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the is alive
	if (tank->getState().getState() != TankState::sNormal) return;

	int posX = message.getPositionX();
	int posY = message.getPositionY();
	if (posX > 0 && posX < GlobalHMap::instance()->getHMap().getWidth() &&
		posY > 0 && posY < GlobalHMap::instance()->getHMap().getWidth())
	{
		GlobalHMap::instance()->getMMap().calculateForTank(tank);
		MovementMap::MovementMapEntry entry =
			GlobalHMap::instance()->getMMap().getEntry(posX, posY);
		if (entry.type == MovementMap::eMovement &&
			entry.dist < tank->getAccessories().getFuel().getNoFuel())
		{
			TankMovement *move = new TankMovement(tank->getPlayerId());

			// Add the end (destination) point to the list of points for the tank
			// to visit
			unsigned int pt = (posX << 16) | (posY & 0xffff);
			move->getPositions().push_front(pt);

			// Work backward to the source point and pre-pend them onto the
			// this of points
			while (entry.srcEntry)
			{
				pt = entry.srcEntry;
				unsigned int x = pt >> 16;
				unsigned int y = pt & 0xffff;
				move->getPositions().push_front(pt);
				entry = GlobalHMap::instance()->getMMap().getEntry(x, y);
			}

			// Action to perform "walking" the list of points
			ActionController::instance()->addAction(move);
		}
	}
}

void TankAILogic::processResignMessage(ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the is alive
	if (tank->getState().getState() == TankState::sNormal)
	{
		// Tank resign action
		TankResign *resign = new TankResign(tank->getPlayerId());
		ActionController::instance()->addAction(resign);
	}
}

void TankAILogic::processFiredMessage(ComsPlayedMoveMessage &message, Tank *tank)
{
	// Check the tank is alive
	if (tank->getState().getState() == TankState::sNormal)
	{
		// Check the weapon name exists and is a weapon
		Accessory *accessory = 
			AccessoryStore::instance()->findByAccessoryName(
			message.getWeaponName());
		if (accessory && accessory->getType() == Accessory::AccessoryWeapon)
		{
			// Check this tank has these weapons
			Weapon *weapon = (Weapon *) accessory;
			int count = 
				tank->getAccessories().getWeapons().getWeaponCount(weapon);
			if (count > 0 || count == -1)
			{
				if ((10 - weapon->getArmsLevel()) <=
					OptionsGame::instance()->getMaxArmsLevel())
				{
					// Actually use up one of the weapons
					tank->getAccessories().getWeapons().rmWeapon(weapon, 1);

					// shot fired action
					TankFired *fired = new TankFired(tank->getPlayerId(), 
						weapon,
						message.getRotationXY(), message.getRotationYZ());
					ActionController::instance()->addAction(fired);

					// Set the tank to have the correct rotation etc..
					tank->getPhysics().rotateGunXY(
						message.getRotationXY(), false);
					tank->getPhysics().rotateGunYZ(
						message.getRotationYZ(), false);
					tank->getState().changePower(
						message.getPower(), false);

					// Create the action for the weapon and
					// add it to the action controller
					Vector velocity = tank->getPhysics().getVelocityVector() *
						tank->getState().getPower();
					Vector position = tank->getPhysics().getTankGunPosition();

					Action *action = weapon->fireWeapon(tank->getPlayerId(), position, velocity);
					if (action)
					{
						ActionController::instance()->addAction(action);
					}
				}
			}
		}
	}
}

bool TankAILogic::processDefenseMessage(ComsDefenseMessage &message, Tank *tank)
{
	// Actually perform the required action
	switch (message.getChange())
	{
	case ComsDefenseMessage::eBatteryUse:
		if (tank->getAccessories().getBatteries().getNoBatteries() > 0)
		{
			if (!OptionsParam::instance()->getOnServer()) 
			{
				static Battery battery;
				SoundBuffer *batSound = 
					SoundStore::instance()->fetchOrCreateBuffer(
						(char *) battery.getActivatedSound());
				batSound->play();
			}

			tank->getAccessories().getBatteries().rmBatteries(1);
			tank->getState().setLife(tank->getState().getLife() + 10.0f);
		}
		else return false;
		break;
	case ComsDefenseMessage::eShieldUp:
		{
			Accessory *accessory = 
				AccessoryStore::instance()->findByAccessoryName(message.getInfo());
			if (accessory->getType() == Accessory::AccessoryShield)
			{
				Shield *shield = (Shield *) accessory;
				if (tank->getAccessories().getShields().getShieldCount(shield) > 0)
				{
					if (!OptionsParam::instance()->getOnServer()) 
					{
						SoundBuffer *activateSound = 
							SoundStore::instance()->fetchOrCreateBuffer(
								(char *) shield->getActivatedSound());
						activateSound->play();
					}

					tank->getAccessories().getShields().setCurrentShield(shield);
				}
				else return false;
			}
			else return false;
		}
		break;
	case ComsDefenseMessage::eShieldDown:
		{
			Shield *shield =
				tank->getAccessories().getShields().getCurrentShield();
			if (shield)
			{
				tank->getAccessories().getShields().setCurrentShield(0);
			}
			else return false;
		}	
		break;
	case ComsDefenseMessage::eParachutesUp:
		if (tank->getAccessories().getParachutes().getNoParachutes() > 0)
		{
			if (!OptionsParam::instance()->getOnServer()) 
			{
				static Parachute para;
				SoundBuffer *activateSound = 
					SoundStore::instance()->fetchOrCreateBuffer(
						(char *) para.getActivatedSound());
				activateSound->play();
			}

			tank->getAccessories().getParachutes().setParachutesEnabled(true);
		}
		else return false;
		break;
	case ComsDefenseMessage::eParachutesDown:
		if (tank->getAccessories().getParachutes().parachutesEnabled())
		{
			tank->getAccessories().getParachutes().setParachutesEnabled(false);
		}
		else return false;
		break;
	}

	return true;
}
