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
#include <tankai/TankAIHuman.h>
#include <weapons/Weapon.h>
#include <landscape/Landscape.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <common/Defines.h>

TankUndoMenu::TankUndoMenu(Tank *tank) :
	tank_(tank)
{
}

TankUndoMenu::~TankUndoMenu()
{
}

void TankUndoMenu::showItems(float x, float y)
{
	static GLWTip useTip("Undo", 
		"Reverts back to the selected rotation,\n"
		"elevtaion and power.\n");

	std::list<GLWSelectorEntry> entries;
	std::vector<TankPhysics::ShotEntry> &oldShots =
		tank_->getPhysics().getOldShots();
	for (int i=0; i<(int) oldShots.size(); i++)
	{
		char buffer[128];
		sprintf(buffer, "%i: Pwr:%.1f Ele:%.1f Rot:%.1f",
			i, oldShots[i].power, oldShots[i].ele,
			(360.0f - oldShots[i].rot));
		entries.push_back(
			GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) 
				((unsigned int) (oldShots.size() - 1 - i))));
	}

	GLWSelector::instance()->showSelector(
		this, x, y, entries,
		ClientState::StatePlaying);
}

void TankUndoMenu::itemSelected(GLWSelectorEntry *entry, int position)
{
	tank_->getPhysics().revertSettings((unsigned int) entry->getUserData());
}

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
		"Click to toggle movement mode.\n"
		"Fuel : %i",
		tank_->getAccessories().getFuel().getNoFuel());
}

void TankFuelTip::showItems(float x, float y)
{
	static GLWTip useTip("Fuel", 
		"Switch movement mode on.\n"
		"In this mode left clicking on the light areas\n"
		"of the landscape will move the tank there.");
	static GLWTip offTip("Fuel Cancel", 
		"Switch back to the normal camera mode.");
	
	std::list<GLWSelectorEntry> entries;
	if (tank_->getAccessories().getFuel().getNoFuel() > 0)
	{
		entries.push_back(GLWSelectorEntry("Move", &useTip, 0, 0, (void *) 1));
	}
	entries.push_back(GLWSelectorEntry("Cancel", &offTip, 0, 0, (void *) 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);		
}

void TankFuelTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	if (entry->getUserData() == 0)
	{
		if (Landscape::instance()->getTextureType() == Landscape::eMovement)
		{
			Landscape::instance()->restoreLandscapeTexture();
		}
	}
	else
	{
		if (Landscape::instance()->getTextureType() != Landscape::eMovement)
		{
			ScorchedClient::instance()->getLandscapeMaps().
				getMMap().calculateForTank(tank_);
			ScorchedClient::instance()->getLandscapeMaps().
				getMMap().movementTexture();
		}
	}
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
		"Click to use some battery(s).\n"
		"Batteries : %i",
		tank_->getAccessories().getBatteries().getNoBatteries());
}

void TankBatteryTip::showItems(float x, float y)
{
	static GLWTip useTip("Battery", 
		"Use some batteries");
	static GLWTip offTip("Battery Cancel", 
		"Don't use any batteries");
	
	std::list<GLWSelectorEntry> entries;
	for (int i=1; i<=MIN(tank_->getAccessories().getBatteries().getNoBatteries(),10); i++)
	{
		char buffer[128];
		sprintf(buffer, "Use %i", i);
		entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) i));
	}
	entries.push_back(GLWSelectorEntry("Cancel", &offTip, 0, 0, (void *) 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);		
}

void TankBatteryTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	for (int i=1; i<=(int) entry->getUserData(); i++)
	{
		if (tank_->getState().getLife() < 100.0f)
		{
			tankAI->useBattery();
		}
	}
}

TankShieldTip::TankShieldTip(Tank *tank) : 
	tank_(tank)
{
}

TankShieldTip::~TankShieldTip()
{
}

void TankShieldTip::showItems(float x, float y)
{
	static GLWTip offTip("Sheild Off", 
		"Don't select a shield or\n"
		"turn off any current shield");

	std::list<GLWSelectorEntry> entries;
	std::map<Shield*, int> &shields = 
		tank_->getAccessories().getShields().getAllShields();
	std::map<Shield*, int>::iterator itor;
	for (itor = shields.begin();
		itor != shields.end();
		itor++)
	{
		char buffer[128];
		int count = (*itor).second;
		if (count >= 0)
		{
			sprintf(buffer, "%s (%i)", 
				(*itor).first->getName(),
				count);
		}
		else
		{
			sprintf(buffer, "%s (In)",
				(*itor).first->getName());
		}
		entries.push_back(GLWSelectorEntry(buffer, &(*itor).first->getToolTip(), 
			0, 0, (*itor).first));
	}
	entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankShieldTip::populate()
{
	if (tank_->getAccessories().getShields().getCurrentShield())
	{
		char buffer[128];
		int count = tank_->getAccessories().getShields().getShieldCount(
			tank_->getAccessories().getShields().getCurrentShield());
		if (count >= 0) sprintf(buffer, "%i", count);
		else sprintf(buffer, "Infinite");

		setText("Shields",
			"Protect the tank from taking shot damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Click to enable/disable shields.\n"
			"Current Shield : %s (%s)\n"
			"Shield Power : %.0f",
			tank_->getAccessories().getShields().getCurrentShield()->getName(),
			buffer,
			tank_->getAccessories().getShields().getShieldPower());
	}
	else
	{
		setText("Shields",
			"Protect the tank from taking shot damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Click to enable/disable shields.\n"
			"Shields Off");
	}
}

void TankShieldTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	if (entry->getUserData() == 0) tankAI->shieldsUpDown(0);
	else tankAI->shieldsUpDown(((Shield *)entry->getUserData())->getAccessoryId());
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
	int count = tank_->getAccessories().getParachutes().getNoParachutes();
	char buffer[128];
	if (count >= 0) sprintf(buffer, "%i", count);
	else sprintf(buffer, "Infinite");
	setText("Parachutes",
		"Prevents the tank from taking damage\n"
		"when falling.  Must be enabled before\n"
		"they take effect.\n"
		"Click to enable/disable parachutes.\n"
		"Number Parachutes : %s\n"
		"Status : %s",
		buffer,
		(tank_->getAccessories().getParachutes().parachutesEnabled()?
		"On":"Off"));
}

void TankParachutesTip::showItems(float x, float y)
{
	static GLWTip useTip("Parachutes On", 
		"Enable parachutes.");
	static GLWTip offTip("Parachutes Off", 
		"Disable parachutes.");
	
	std::list<GLWSelectorEntry> entries;
	int count = tank_->getAccessories().getParachutes().getNoParachutes();
	if (count != 0)
	{
		char buffer[128];
		if (count >= 0) sprintf(buffer, "On (%i)", count);
		else sprintf(buffer, "On (In)");
		entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) 1));
	}
	entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, (void *) 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankParachutesTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	tankAI->parachutesUpDown(entry->getUserData()==0?false:true);
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
		"Click to see auto defense status.\n"
		"Status : %s",
		(tank_->getAccessories().getAutoDefense().haveDefense()?
		"On":"Off (Not Bought)"));
}

void TankAutoDefenseTip::showItems(float x, float y)
{
	static GLWTip useTip("Auto Defense On", 
		"Enable the auto defense.");
	static GLWTip offTip("Auto Defense Off", 
		"Disable the auto defense.");
	
	std::list<GLWSelectorEntry> entries;
	if (tank_->getAccessories().getAutoDefense().haveDefense()) 
		entries.push_back(GLWSelectorEntry("On", &useTip));
	else entries.push_back(GLWSelectorEntry("Off", &offTip));
	GLWSelector::instance()->showSelector(0, x, y, entries,
		ClientState::StatePlaying);
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
			"Click to change weapon.\n"
			"Weapon : %s (%i)\n"
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
			"Click to change weapon.\n"
			"Weapon : %s (Infinite)\n"
			"Description :\n%s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getAccessories().getWeapons().getCurrent()->getDescription());
	}
}

void TankWeaponTip::showItems(float x, float y)
{
	std::list<GLWSelectorEntry> entries;
	std::map<Weapon *, int> &weapons = 
		tank_->getAccessories().getWeapons().getAllWeapons();
	std::map<Weapon *, int>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{
		char buffer[128];
		if ((*itor).second > 0)
		{
			sprintf(buffer, "%s (%i)", 
				(*itor).first->getName(),
				(*itor).second);
		}
		else
		{
			sprintf(buffer, "%s (In)", 
				(*itor).first->getName());
		}
		entries.push_back(GLWSelectorEntry(buffer, &(*itor).first->getToolTip(), 
			0, (*itor).first->getTexture(), (*itor).first));
	}
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankWeaponTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	tank_->getAccessories().getWeapons().setWeapon((Weapon *) entry->getUserData());
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
		"Click to revert back to previous settings.\n"
		"Power : %s",
		tank_->getAccessories().getWeapons().getCurrent()->getName(),
		tank_->getPhysics().getPowerString());
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
		"Click to revert back to previous settings.\n"
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
		"Click to revert back to previous settings.\n"
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
	undoMenu(tank),
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

