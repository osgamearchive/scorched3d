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

GLWTankWeapon::GLWTankWeapon(Tank *tank) : 
	tank_(tank)
{
}

GLWTankWeapon::~GLWTankWeapon()
{
}

void GLWTankWeapon::populate()
{
	setText("Weapon",
		"The currently selected weapon.\n"
		"Weapon : %s\n"
		"%s",
		tank_->getAccessories().getWeapons().getCurrent()->getName(),
		tank_->getAccessories().getWeapons().getCurrent()->getDescription());
}

GLWTankPower::GLWTankPower(Tank *tank) : 
	tank_(tank)
{
}

GLWTankPower::~GLWTankPower()
{
}

void GLWTankPower::populate()
{
	setText("Power",
		"The power used to fire the %s.\n"
		"Power : %s",
		tank_->getAccessories().getWeapons().getCurrent()->getName(),
		tank_->getState().getPowerString());
}

GLWTankRotation::GLWTankRotation(Tank *tank) : 
	tank_(tank)
{
}

GLWTankRotation::~GLWTankRotation()
{
}

void GLWTankRotation::populate()
{
	setText("Rotation",
		"The rotation of the current player's tank turret.\n"
		"Rotation : %s",
		tank_->getPhysics().getRotationString());
}

GLWTankElevation::GLWTankElevation(Tank *tank) : 
	tank_(tank)
{
}

GLWTankElevation::~GLWTankElevation()
{
}

void GLWTankElevation::populate()
{
	setText("Elevation",
		"The elevation of the current player's gun.\n"
		"Elevation : %s",
		tank_->getPhysics().getElevationString());
}

GLWTankTip::GLWTankTip(Tank *tank) : 
	tank_(tank)
{
}

GLWTankTip::~GLWTankTip()
{
}

void GLWTankTip::populate()
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
	nameTip("Player Name",
		"Shows the name of the player currently\n"
		"making their move.")
{
}

GLWTankTips::~GLWTankTips()
{
}
