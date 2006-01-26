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

#include <weapons/AccessoryStore.h>
#include <tank/TankAccessories.h>
#include <tank/TankType.h>
#include <tank/Tank.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsGame.h>

TankAccessories::TankAccessories(ScorchedContext &context) :
	context_(context),
	tankFuel_(context),
	tankWeapon_(context),
	tankPara_(context),
	tankShield_(context),
	tankAuto_(context),
	tankBatteries_(context)
{
}

TankAccessories::~TankAccessories()
{
}

void TankAccessories::setTank(Tank *tank)
{
	tank_ = tank;
	tankPara_.setTank(tank);
}

void TankAccessories::newMatch()
{
	tankWeapon_.newMatch();
	tankPara_.newMatch();
	tankShield_.newMatch();
	tankAuto_.newMatch();
	tankBatteries_.newMatch();
	tankFuel_.newMatch();

	// Add all the accessories the tank should start with
	// this is the accessories from the global accessories file
	// and also if give all accessories is set
	{
		std::list<Accessory *> accessories = 
			context_.accessoryStore->getAllAccessories();
		std::list<Accessory *>::iterator itor;
		for (itor = accessories.begin();
			itor != accessories.end();
			itor++)
		{
			Accessory *accessory = (*itor);
			if (accessory->getMaximumNumber() > 0)
			{
				int startingNumber = accessory->getStartingNumber();
				if (context_.optionsGame->getGiveAllWeapons())
				{
					startingNumber = -1;
				}

				if (startingNumber != 0)
				{
					add(accessory, startingNumber);
				}
			}
		}
	}

	// Add all of the accessories that come from the tank's type
	{
		TankType *type = tank_->getModel().getTankType(context_);
		std::map<Accessory *, int> accessories = type->getAccessories();
		std::map<Accessory *, int>::iterator itor;
		for (itor = accessories.begin();
			itor != accessories.end();
			itor++)
		{
			Accessory *accessory = (*itor).first;
			int count = (*itor).second;

			add(accessory, count);
		}
	}
}

std::list<Accessory *> TankAccessories::getAllAccessories(bool sort)
{
	std::list<Accessory *> result;

	// Add all weapons
	std::list<Accessory *> tankWeapons = 
		tankWeapon_.getAllWeapons(
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = tankWeapons.begin();
		itor != tankWeapons.end();
		itor++)
	{
		result.push_back(*itor);
	}

	// Add all shields
	std::list<Accessory *> tankShields = 
		tankShield_.getAllShields(
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator shitor;
	for (shitor = tankShields.begin();
		shitor != tankShields.end();
		shitor++)
	{
		result.push_back(*shitor);
	}

	// Add parachutes
	if (tankPara_.getNoParachutes() != 0)
	{
		Accessory *accessory = context_.accessoryStore->
			findByAccessoryType(AccessoryPart::AccessoryParachute);
		result.push_back(accessory);
	}

	// Add auto defense
	if (tankAuto_.haveDefense())
	{
		Accessory *accessory = context_.accessoryStore->
			findByAccessoryType(AccessoryPart::AccessoryAutoDefense);
		result.push_back(accessory);
	}

	// Add batteries
	if (tankBatteries_.getNoBatteries())
	{
		Accessory *accessory = context_.accessoryStore->
			findByAccessoryType(AccessoryPart::AccessoryBattery);
		result.push_back(accessory);
	}

	// Add fuel
	if (tankFuel_.getNoFuel())
	{
		Accessory *accessory = context_.accessoryStore->
			findByAccessoryType(AccessoryPart::AccessoryFuel);
		result.push_back(accessory);
	}

	if (sort) AccessoryStore::sortList(result);
	return result;
}

int TankAccessories::getAccessoryCount(Accessory *accessory)
{
	switch (accessory->getType())
	{
	case AccessoryPart::AccessoryAutoDefense:
		return (tankAuto_.haveDefense()?1:0);
		break;
	case AccessoryPart::AccessoryWeapon:
		return tankWeapon_.getWeaponCount(accessory);
		break;
	case AccessoryPart::AccessoryParachute:
		return tankPara_.getNoParachutes();
		break;
	case AccessoryPart::AccessoryShield:
		return tankShield_.getShieldCount(accessory);
		break;
	case AccessoryPart::AccessoryBattery:
		return tankBatteries_.getNoBatteries();
		break;
	case AccessoryPart::AccessoryFuel:
		return tankFuel_.getNoFuel();
		break;
	}

	return 0;
}

void TankAccessories::add(Accessory *accessory, int count)
{
	switch (accessory->getType())
	{
	case AccessoryPart::AccessoryAutoDefense:
		tankAuto_.addDefense();
		break;
	case AccessoryPart::AccessoryWeapon:
		tankWeapon_.addWeapon(accessory, count);
		break;
	case AccessoryPart::AccessoryParachute:
		tankPara_.addParachutes(count);
		break;
	case AccessoryPart::AccessoryShield:
		tankShield_.addShield(accessory, count);
		break;
	case AccessoryPart::AccessoryBattery:
		tankBatteries_.addBatteries(count);
		break;
	case AccessoryPart::AccessoryFuel:
		tankFuel_.addFuel(count);
		break;
	}
}

void TankAccessories::rm(Accessory *accessory)
{
	switch (accessory->getType())
	{
	case AccessoryPart::AccessoryAutoDefense:
		tankAuto_.rmDefense();
		break;
	case AccessoryPart::AccessoryWeapon:
		tankWeapon_.rmWeapon(accessory, 1);
		break;
	case AccessoryPart::AccessoryParachute:
		tankPara_.useParachutes(1);
		break;
	case AccessoryPart::AccessoryShield:
		tankShield_.rmShield(accessory, 1);
		break;
	case AccessoryPart::AccessoryBattery:
		tankBatteries_.rmBatteries(1);
		break;
	case AccessoryPart::AccessoryFuel:
		tankFuel_.rmFuel(1);
		break;
	}
}

bool TankAccessories::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (!tankWeapon_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankPara_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankShield_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankAuto_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankBatteries_.writeMessage(buffer, writeAccessories)) return false;
	if (!tankFuel_.writeMessage(buffer, writeAccessories)) return false;
	return true;
}

bool TankAccessories::readMessage(NetBufferReader &reader)
{
	if (!tankWeapon_.readMessage(reader)) return false;
	if (!tankPara_.readMessage(reader)) return false;
	if (!tankShield_.readMessage(reader)) return false;
	if (!tankAuto_.readMessage(reader)) return false;
	if (!tankBatteries_.readMessage(reader)) return false;
	if (!tankFuel_.readMessage(reader)) return false;
	return true;
}

