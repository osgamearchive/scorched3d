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


#include <tank/TankAI.h>
#include <tank/Tank.h>
#include <tank/TankAILogic.h>
#include <common/OptionsParam.h>
#include <coms/ComsMessageSender.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <server/ServerShotHolder.h>

TankAI::TankAI(Tank *tank) : currentTank_(tank)
{
}

TankAI::~TankAI()
{
}

void TankAI::endPlayMove()
{
}

void TankAI::fireShot()
{
	if (!OptionsParam::instance()->getOnServer())
	{
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

			// Check if we are running in a NET/LAN environment
			if (OptionsParam::instance()->getConnectedToServer())
			{
				// If so we send this move to the server
				ComsMessageSender::sendToServer(comsMessage);
			}
			else
			{
				// Else we just play the move
				TankAILogic::processPlayedMoveMessage(comsMessage, currentTank_);
			}

			// Stimulate into the next state waiting for all the shots
			ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
		}
	}
	else
	{
		Weapon *currentWeapon = 
			currentTank_->getAccessories().getWeapons().getCurrent();
		if (currentWeapon)
		{
			ComsPlayedMoveMessage *message = 
				new ComsPlayedMoveMessage(ComsPlayedMoveMessage::eShot);
			message->setShot(
				currentTank_->getAccessories().getWeapons().getCurrent()->getName(),
				currentTank_->getPhysics().getRotationGunXY(),
				currentTank_->getPhysics().getRotationGunYZ(),
				currentTank_->getState().getPower());
		
			ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
		}
	}
}

void TankAI::skipShot()
{
	if (!OptionsParam::instance()->getOnServer())
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(ComsPlayedMoveMessage::eSkip);

		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this move to the server
			ComsMessageSender::sendToServer(comsMessage);
		}
		else
		{
			// Else we just play the move
			TankAILogic::processPlayedMoveMessage(comsMessage, currentTank_);
		}

		// Stimulate into the next state waiting for all the shots
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
	}
	else
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(ComsPlayedMoveMessage::eSkip);
		ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
	}
}

void TankAI::resign()
{
	if (!OptionsParam::instance()->getOnServer())
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(ComsPlayedMoveMessage::eResign);

		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this move to the server
			ComsMessageSender::sendToServer(comsMessage);
		}
		else
		{
			// Else we just play the move
			TankAILogic::processPlayedMoveMessage(comsMessage, currentTank_);
		}

		// Stimulate into the next state waiting for all the shots
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
	}
	else
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(ComsPlayedMoveMessage::eResign);
		ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
	}
}

void TankAI::move(int x, int y)
{
	if (!OptionsParam::instance()->getOnServer())
	{
		// send message saying we are finished with shot
		ComsPlayedMoveMessage comsMessage(ComsPlayedMoveMessage::eMove);
		comsMessage.setPosition(x, y);

		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this move to the server
			ComsMessageSender::sendToServer(comsMessage);
		}
		else
		{
			// Else we just play the move
			TankAILogic::processPlayedMoveMessage(comsMessage, currentTank_);
		}

		// Stimulate into the next state waiting for all the shots
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimShot);
	}
	else
	{
		ComsPlayedMoveMessage *message = 
			new ComsPlayedMoveMessage(ComsPlayedMoveMessage::eMove);
		message->setPosition(x, y);
		ServerShotHolder::instance()->addShot(currentTank_->getPlayerId(), message);
	}
}

void TankAI::parachutesUpDown(bool on)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		on?ComsDefenseMessage::eParachutesUp:ComsDefenseMessage::eParachutesDown);
	if (!OptionsParam::instance()->getOnServer())
	{
		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this defense action to the server
			ComsMessageSender::sendToServer(defenseMessage);
		}
		else
		{
			// Else we just perform the action
			TankAILogic::processDefenseMessage(defenseMessage, currentTank_);
		}
	}
	else
	{
		if (TankAILogic::processDefenseMessage(defenseMessage, currentTank_))
		{
			ComsMessageSender::sendToAllPlayingClients(defenseMessage);
		}
	}
}

void TankAI::shieldsUpDown(const char *name)
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		name?ComsDefenseMessage::eShieldUp:ComsDefenseMessage::eShieldDown,
		name?name:"");
	if (!OptionsParam::instance()->getOnServer()) 
	{
		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this defense action to the server
			ComsMessageSender::sendToServer(defenseMessage);
		}
		else
		{
			// Else we just perform the action
			TankAILogic::processDefenseMessage(defenseMessage, currentTank_);
		}
	} 
	else 
	{
		if (TankAILogic::processDefenseMessage(defenseMessage, currentTank_))
		{
			ComsMessageSender::sendToAllPlayingClients(defenseMessage);
		}
	}
}

void TankAI::useBattery()
{
	ComsDefenseMessage defenseMessage(
		currentTank_->getPlayerId(),
		ComsDefenseMessage::eBatteryUse);
	if (!OptionsParam::instance()->getOnServer()) 
	{
		// Check if we are running in a NET/LAN environment
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If so we send this defense action to the server
			ComsMessageSender::sendToServer(defenseMessage);
		}
		else
		{
			// Else we just perform the action
			TankAILogic::processDefenseMessage(defenseMessage, currentTank_);
		}
	}
	else 
	{
		if (TankAILogic::processDefenseMessage(defenseMessage, currentTank_))
		{
			ComsMessageSender::sendToAllPlayingClients(defenseMessage);
		}
	}
}
