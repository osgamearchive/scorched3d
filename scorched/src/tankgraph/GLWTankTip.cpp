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

#include <tankgraph/GLWTankTip.h>
#include <weapons/Weapon.h>

TankFuelTip::TankFuelTip(Tank *tank) : 
	tank_(tank)
{
}

TankFuelTip::~TankFuelTip()
{
}

void TankFuelTip::populate()
{
	setText("Fuel",
		"Allows the tank to move.\n"
		"Fuel : %i",
		tank_->getAccessories().getFuel().getNoFuel());
}

TankBatteryTip::TankBatteryTip(Tank *tank) : 
	tank_(tank)
{
}

TankBatteryTip::~TankBatteryTip()
{
}

void TankBatteryTip::populate()
{
	setText("Batteries",
		"Can be used to recharge life.\n"
		"Each battery gives back 10 life.\n"
		"Batteries : %i",
		tank_->getAccessories().getBatteries().getNoBatteries());
}

TankShieldTip::TankShieldTip(Tank *tank) : 
	tank_(tank)
{
}

TankShieldTip::~TankShieldTip()
{
}

void TankShieldTip::populate()
{
	if (tank_->getAccessories().getShields().getCurrentShield())
	{
		setText("Shields",
			"Protect the tank from taking damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Current Shield : %s\n"
			"Shield Power : %i",
			tank_->getAccessories().getShields().getCurrentShield()->getName(),
			tank_->getAccessories().getShields().getShieldCount(
			tank_->getAccessories().getShields().getCurrentShield()));
	}
	else
	{
		setText("Shields",
			"Protect the tank from taking damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Current Shield : %s\n",
			"Disabled");
	}
}

TankHealthTip::TankHealthTip(Tank *tank) : 
	tank_(tank)
{
}

TankHealthTip::~TankHealthTip()
{
}

void TankHealthTip::populate()
{
	setText("Life",
		"The amount of life this player has.\n"
		"The tank explodes when life reaches 0.\n"
		"Less weapon power is available with less life.\n"
		"Life : %i/100",
		(int) tank_->getState().getLife());
}

TankParachutesTip::TankParachutesTip(Tank *tank) : 
	tank_(tank)
{
}

TankParachutesTip::~TankParachutesTip()
{
}

void TankParachutesTip::populate()
{
	setText("Parachutes",
		"Prevents the tank from taking damage\n"
		"when falling.  Must be enabled before\n"
		"they take effect.\n"
		"Number Parachutes : %i\n"
		"Status : %s",
		tank_->getAccessories().getParachutes().getNoParachutes(),
		(tank_->getAccessories().getParachutes().parachutesEnabled()?
		"Active":"Disabled"));
}

TankAutoDefenseTip::TankAutoDefenseTip(Tank *tank) : 
	tank_(tank)
{
}

TankAutoDefenseTip::~TankAutoDefenseTip()
{
}

void TankAutoDefenseTip::populate()
{
	setText("Auto Defense",
		"Allows the tank to raise shields and\n"
		"activate parachutes before the round\n"
		"starts.\n"
		"Status : %s",
		(tank_->getAccessories().getAutoDefense().haveDefense()?
		"Active":"Disabled"));
}

TankWeaponTip::TankWeaponTip(Tank *tank) : 
	tank_(tank)
{
}

TankWeaponTip::~TankWeaponTip()
{
}

void TankWeaponTip::populate()
{
	if (tank_->getAccessories().getWeapons().getWeaponCount(
		tank_->getAccessories().getWeapons().getCurrent()) > 0)
	{
		setText("Weapon",
			"The currently selected weapon.\n"
			"Weapon : %s\n"
			"Weapon Count : %s\n"
			"Description :\n%s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getAccessories().getWeapons().getWeaponCount(
			tank_->getAccessories().getWeapons().getCurrent()),
			tank_->getAccessories().getWeapons().getCurrent()->getDescription());
	}
	else
	{
		setText("Weapon",
			"The currently selected weapon.\n"
			"Weapon : %s\n"
			"Weapon Count : Infinite\n"
			"Description :\n%s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getAccessories().getWeapons().getCurrent()->getDescription());
	}
}

TankPowerTip::TankPowerTip(Tank *tank) : 
	tank_(tank)
{
}

TankPowerTip::~TankPowerTip()
{
}

void TankPowerTip::populate()
{
	setText("Power",
		"The power used to fire the %s.\n"
		"Power : %s",
		tank_->getAccessories().getWeapons().getCurrent()->getName(),
		tank_->getState().getPowerString());
}

TankRotationTip::TankRotationTip(Tank *tank) : 
	tank_(tank)
{
}

TankRotationTip::~TankRotationTip()
{
}

void TankRotationTip::populate()
{
	setText("Rotation",
		"The rotation of the current player's tank turret.\n"
		"Rotation : %s",
		tank_->getPhysics().getRotationString());
}

TankElevationTip::TankElevationTip(Tank *tank) : 
	tank_(tank)
{
}

TankElevationTip::~TankElevationTip()
{
}

void TankElevationTip::populate()
{
	setText("Elevation",
		"The elevation of the current player's gun.\n"
		"Elevation : %s",
		tank_->getPhysics().getElevationString());
}

TankTip::TankTip(Tank *tank) : 
	tank_(tank)
{
}

TankTip::~TankTip()
{
}

void TankTip::populate()
{
	if (tank_->getAccessories().getShields().getCurrentShield())
	{
		setText(tank_->getName(), 
				"Life   : %.0f/100\n"
				"Shield : %.0f/100\n"
				"Wins   : %i\n"
				"Kills  : %i\n",
				tank_->getState().getLife(),
				tank_->getAccessories().getShields().getShieldPower(),
				tank_->getScore().getWins(),
				tank_->getScore().getKills());
	}
	else
	{
		setText(tank_->getName(), 
				"Life   : %.0f/100\n"
				"Wins   : %i\n"
				"Kills  : %i\n",
				tank_->getState().getLife(),
				tank_->getScore().getWins(),
				tank_->getScore().getKills());
	}
}

GLWTankTips::GLWTankTips(Tank *tank) : 
	tankTip(tank),
	rotationTip(tank),
	elevationTip(tank),
	powerTip(tank),
	weaponTip(tank),
	autodTip(tank),
	paraTip(tank),
	healthTip(tank),
	shieldTip(tank),
	batteryTip(tank),
	fuelTip(tank),
	nameTip("Player Name",
		"Shows the name of the player currently\n"
		"making their move.")
{
}

GLWTankTips::~GLWTankTips()
{
}
