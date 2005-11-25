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

#include <client/ScorchedClient.h>
#include <client/ClientDefenseHandler.h>
#include <tankai/TankAILogic.h>
#include <tank/TankContainer.h>
#include <coms/ComsDefenseMessage.h>
#include <common/Defines.h>
#include <sound/SoundUtils.h>
#include <weapons/AccessoryStore.h>

ClientDefenseHandler *ClientDefenseHandler::instance_ = 0;

ClientDefenseHandler *ClientDefenseHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientDefenseHandler;
	}
	return instance_;
}

ClientDefenseHandler::ClientDefenseHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsDefenseMessage",
		this);
}

ClientDefenseHandler::~ClientDefenseHandler()
{
}

bool ClientDefenseHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Decode the connect message
	ComsDefenseMessage message;
	if (!message.readMessage(reader)) return false;

	// Check tank exists and is alive
	Tank *tank = ScorchedClient::instance()->getTankContainer().getTankById(message.getPlayerId());
	if (!tank || tank->getState().getState() != TankState::sNormal)
	{
		return true;
	}

	// Actually perform the required action
	switch (message.getChange())
	{
	case ComsDefenseMessage::eBatteryUse:
		{
			Accessory *battery = 
				ScorchedClient::instance()->getAccessoryStore().
					findByAccessoryType(AccessoryPart::AccessoryBattery);
			if (battery)
			{
				SoundBuffer *batSound = 
					Sound::instance()->fetchOrCreateBuffer((char *)
						getDataFile("data/wav/%s", battery->getActivationSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					batSound, tank->getPhysics().getTankPosition());
			}

			if (tank->getDestinationId() != 
				ScorchedClient::instance()->getTankContainer().getCurrentDestinationId())
			{
				tank->getAccessories().getBatteries().addBatteries(1);
			}

			tank->getState().setLife(tank->getState().getLife() + 10.0f);
			tank->getAccessories().getBatteries().rmBatteries(1);
		}
		break;
	case ComsDefenseMessage::eShieldUp:
		{
			Accessory *accessory = 
				ScorchedClient::instance()->getAccessoryStore().
					findByAccessoryId(message.getInfoId());
			if (accessory->getType() == AccessoryPart::AccessoryShield)
			{
				SoundBuffer *activateSound = 
					Sound::instance()->fetchOrCreateBuffer((char *)
						getDataFile("data/wav/%s", accessory->getActivationSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					activateSound, tank->getPhysics().getTankPosition());

				if (tank->getDestinationId() != 
					ScorchedClient::instance()->getTankContainer().getCurrentDestinationId())
				{
					// Make sure tank has this shield
					tank->getAccessories().getShields().addShield(accessory, 1);
				}
				tank->getAccessories().getShields().setCurrentShield(accessory);
			}
		}
		break;
	case ComsDefenseMessage::eShieldDown:
		{
			tank->getAccessories().getShields().setCurrentShield(0);
		}	
		break;
	case ComsDefenseMessage::eParachutesUp:
		{
			Accessory *parachute = 
				ScorchedClient::instance()->getAccessoryStore().
					findByAccessoryType(AccessoryPart::AccessoryParachute);
			if (parachute)
			{
				SoundBuffer *paraSound = 
					Sound::instance()->fetchOrCreateBuffer((char *)
						getDataFile("data/wav/%s", parachute->getActivationSound()));
				SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
					paraSound, tank->getPhysics().getTankPosition());
			}

			tank->getAccessories().getParachutes().setParachutesEnabled(true);
		}
		break;
	case ComsDefenseMessage::eParachutesDown:
		{
			tank->getAccessories().getParachutes().setParachutesEnabled(false);
		}
		break;
	}

	return true;
}
