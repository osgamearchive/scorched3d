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


// TankAccessories.cpp: implementation of the TankAccessories class.
//
//////////////////////////////////////////////////////////////////////

#include <weapons/AccessoryStore.h>
#include <tank/TankAccessories.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TankAccessories::TankAccessories()
{

}

TankAccessories::~TankAccessories()
{

}

void TankAccessories::reset()
{
	tankWeapon_.reset();
	tankPara_.reset();
	tankShield_.reset();
	tankAuto_.reset();
	tankBatteries_.reset();
	tankFuel_.reset();
}

void TankAccessories::newGame()
{
	tankPara_.newGame();
	tankShield_.newGame();
}

void TankAccessories::getAllAccessories(std::list<std::pair<Accessory *, int> > &result)
{
	// Add all weapons
	std::map<Weapon *, int> &tankWeapons = tankWeapon_.getAllWeapons();
	std::map<Weapon *, int>::iterator itor;
	for (itor = tankWeapons.begin();
		itor != tankWeapons.end();
		itor++)
	{
		result.push_back(*itor);
	}

	// Add all shields
	std::map<Shield *, int> &tankShields = tankShield_.getAllShields();
	std::map<Shield *, int>::iterator shitor;
	for (shitor = tankShields.begin();
		shitor != tankShields.end();
		shitor++)
	{
		result.push_back(*shitor);
	}

	// Add parachutes
	if (tankPara_.getNoParachutes() != 0)
	{
		Accessory *accessory = AccessoryStore::instance()->
			findByAccessoryType(Accessory::AccessoryParachute);
		result.push_back(std::pair<Accessory *, int>
			(accessory, tankPara_.getNoParachutes()));
	}

	// Add auto defense
	if (tankAuto_.haveDefense())
	{
		Accessory *accessory = AccessoryStore::instance()->
			findByAccessoryType(Accessory::AccessoryAutoDefense);
		result.push_back(std::pair<Accessory *, int>
			(accessory, 1));
	}

	// Add batteries
	if (tankBatteries_.getNoBatteries())
	{
		Accessory *accessory = AccessoryStore::instance()->
			findByAccessoryType(Accessory::AccessoryBattery);
		result.push_back(std::pair<Accessory *, int>
			(accessory, tankBatteries_.getNoBatteries()));
	}

	// Add fuel
	if (tankFuel_.getNoFuel())
	{
		Accessory *accessory = AccessoryStore::instance()->
			findByAccessoryType(Accessory::AccessoryFuel);
		result.push_back(std::pair<Accessory *, int>
			(accessory, tankFuel_.getNoFuel()));
	}
}

int TankAccessories::getAccessoryCount(Accessory *accessory)
{
	switch (accessory->getType())
	{
	case Accessory::AccessoryAutoDefense:
		return (tankAuto_.haveDefense()?1:0);
		break;
	case Accessory::AccessoryWeapon:
		return tankWeapon_.getWeaponCount((Weapon*) accessory);
		break;
	case Accessory::AccessoryParachute:
		return tankPara_.getNoParachutes();
		break;
	case Accessory::AccessoryShield:
		return tankShield_.getShieldCount((Shield *) accessory);
		break;
	case Accessory::AccessoryBattery:
		return tankBatteries_.getNoBatteries();
		break;
	case Accessory::AccessoryFuel:
		return tankFuel_.getNoFuel();
		break;
	}

	return 0;
}

void TankAccessories::add(Accessory *accessory)
{
	switch (accessory->getType())
	{
	case Accessory::AccessoryAutoDefense:
		tankAuto_.addDefense();
		break;
	case Accessory::AccessoryWeapon:
		tankWeapon_.addWeapon((Weapon*) accessory, accessory->getBundle());
		break;
	case Accessory::AccessoryParachute:
		tankPara_.addParachutes(accessory->getBundle());
		break;
	case Accessory::AccessoryShield:
		tankShield_.addShield((Shield *) accessory, accessory->getBundle());
		break;
	case Accessory::AccessoryBattery:
		tankBatteries_.addBatteries(accessory->getBundle());
		break;
	case Accessory::AccessoryFuel:
		tankFuel_.addFuel(accessory->getBundle());
		break;
	}
}

void TankAccessories::rm(Accessory *accessory)
{
	switch (accessory->getType())
	{
	case Accessory::AccessoryAutoDefense:
		tankAuto_.rmDefense();
		break;
	case Accessory::AccessoryWeapon:
		tankWeapon_.rmWeapon((Weapon*) accessory, 1);
		break;
	case Accessory::AccessoryParachute:
		tankPara_.useParachutes(1);
		break;
	case Accessory::AccessoryShield:
		tankShield_.rmShield((Shield *) accessory, 1);
		break;
	case Accessory::AccessoryBattery:
		tankBatteries_.rmBatteries(1);
		break;
	case Accessory::AccessoryFuel:
		tankFuel_.rmFuel(1);
		break;
	}
}

bool TankAccessories::writeMessage(NetBuffer &buffer)
{
	if (!tankWeapon_.writeMessage(buffer)) return false;
	if (!tankPara_.writeMessage(buffer)) return false;
	if (!tankShield_.writeMessage(buffer)) return false;
	if (!tankAuto_.writeMessage(buffer)) return false;
	if (!tankBatteries_.writeMessage(buffer)) return false;
	if (!tankFuel_.writeMessage(buffer)) return false;
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
