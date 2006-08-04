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
#include <weapons/Accessory.h>
#include <weapons/Shield.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/MovementMap.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>

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
	std::vector<TankPosition::ShotEntry> &oldShots =
		tank_->getPosition().getOldShots();
	for (int i=0; i<(int) oldShots.size(); i++)
	{
		char buffer[128];
		snprintf(buffer, 128, "%s%i: Pwr:%.1f Ele:%.1f Rot:%.1f",
			(oldShots[i].current?"* ":"  "),
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
	tank_->getPosition().revertSettings((unsigned long) entry->getUserData());
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
	std::string extra;
	std::list<Accessory *> entries;
	tank_->getAccessories().getAllAccessoriesByType(
		AccessoryPart::AccessoryFuel, entries);			
	std::list<Accessory *>::iterator itor;
	for (itor = entries.begin();
		itor != entries.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		int count = tank_->getAccessories().getAccessoryCount(accessory);
		if (count == -1)
		{
			extra.append(formatString("\n%s : In", 
				accessory->getName()));
		}
		else
		{
			extra.append(formatString("\n%s : %i", 
				accessory->getName(), count));
		}
	}

	setText("Fuel", formatString(
		"Allows the tank to move.\n"
		"Click to toggle movement mode."
		"%s",
		extra.c_str()));
}

void TankFuelTip::showItems(float x, float y)
{
	static GLWTip offTip("Fuel Off", 
		"Don't select fuel or\n"
		"turn off any fuel.");

	unsigned int currentFuelId = 0;
	if (Landscape::instance()->getTextureType() ==
		Landscape::eMovement)
	{
		currentFuelId = MovementMap::getFuelId();
	}

	std::list<GLWSelectorEntry> entries;
	std::list<Accessory *> fuels;
	tank_->getAccessories().getAllAccessoriesByType(
		AccessoryPart::AccessoryFuel, fuels,
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = fuels.begin();
		itor != fuels.end();
		itor++)
	{
		Accessory *current = (*itor);
		int count = tank_->getAccessories().getAccessoryCount(current);

		char buffer[128];
		if (count >= 0)
		{
			snprintf(buffer, 128, "%s (%i)", 
				current->getName(),
				count);
		}
		else
		{
			snprintf(buffer, 128, "%s (In)",
				current->getName());
		}
		entries.push_back(GLWSelectorEntry(buffer, &current->getToolTip(), 
			(currentFuelId == current->getAccessoryId()), current->getTexture(), current));
	}
	entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankFuelTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	if (Landscape::instance()->getTextureType() == Landscape::eMovement)
	{
		Landscape::instance()->restoreLandscapeTexture();
	}
	if (entry->getUserData() != 0)
	{
		if (Landscape::instance()->getTextureType() != Landscape::eMovement)
		{
			MovementMap mmap(
				ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth(),
				ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight());
			mmap.calculateForTank(tank_,
				((Accessory *)entry->getUserData())->getAccessoryId(),
				ScorchedClient::instance()->getContext());
			mmap.movementTexture();
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
	int count = tank_->getAccessories().getBatteries().getNoBatteries();
	if (count < 0)
	{
		setText("Batteries",
			"Can be used to recharge life.\n"
			"Each battery gives back 10 life.\n"
			"Click to use some battery(s).\n"
			"Batteries : In");
	}
	else
	{
		setText("Batteries", formatString(
			"Can be used to recharge life.\n"
			"Each battery gives back 10 life.\n"
			"Click to use some battery(s).\n"
			"Batteries : %i",
			count));
	}
}

void TankBatteryTip::showItems(float x, float y)
{
	static GLWTip useTip("Battery", 
		"Use some batteries");
	static GLWTip offTip("Battery Cancel", 
		"Don't use any batteries");
	
	int count = tank_->getAccessories().getBatteries().getNoBatteries();
	if (count == -1) count = 10;

	std::list<GLWSelectorEntry> entries;
	for (int i=1; i<=MIN(count,10); i++)
	{
		char buffer[128];
		snprintf(buffer, 128, "Use %i", i);
		entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) i));
	}
	entries.push_back(GLWSelectorEntry("Cancel", &offTip, 0, 0, (void *) 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);		
}

void TankBatteryTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	for (int i=1; i<=(long) entry->getUserData(); i++)
	{
		if (tank_->getLife().getLife() < 
			tank_->getLife().getMaxLife())
		{
			std::list<Accessory *> entries;
			tank_->getAccessories().getAllAccessoriesByType(
				AccessoryPart::AccessoryBattery, entries);			
			if (!entries.empty())
			{
				tankAI->useBattery(entries.front()->getAccessoryId());
			}
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

	Accessory *currentShield = 
		tank_->getShield().getCurrentShield();
	std::list<GLWSelectorEntry> entries;
	std::list<Accessory *> shields;
	tank_->getAccessories().getAllAccessoriesByType(
		AccessoryPart::AccessoryShield, shields,
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = shields.begin();
		itor != shields.end();
		itor++)
	{
		Accessory *current = (*itor);
		int count = tank_->getAccessories().getAccessoryCount(current);

		char buffer[128];
		if (count >= 0)
		{
			snprintf(buffer, 128, "%s (%i)", 
				current->getName(),
				count);
		}
		else
		{
			snprintf(buffer, 128, "%s (In)",
				current->getName());
		}
		entries.push_back(GLWSelectorEntry(buffer, &current->getToolTip(), 
			(currentShield == current), current->getTexture(), current));
	}
	entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankShieldTip::populate()
{
	if (tank_->getShield().getCurrentShield())
	{
		char buffer[128];
		int count = tank_->getAccessories().getAccessoryCount(
			tank_->getShield().getCurrentShield());
		if (count >= 0) snprintf(buffer, 128, "%i", count);
		else snprintf(buffer, 128, "Infinite");

		setText("Shields", formatString(
			"Protect the tank from taking shot damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Click to enable/disable shields.\n"
			"Current Shield : %s (%s)\n"
			"Shield Power : %.0f",
			tank_->getShield().getCurrentShield()->getName(),
			buffer,
			tank_->getShield().getShieldPower()));
	}
	else
	{
		setText("Shields", formatString(
			"Protect the tank from taking shot damage.\n"
			"Shields must be enabled before they take\n"
			"effect.\n"
			"Click to enable/disable shields.\n"
			"Current Shield : Off"));
	}
}

void TankShieldTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	if (entry->getUserData() == 0) tankAI->shieldsUpDown(0);
	else tankAI->shieldsUpDown(((Accessory *)entry->getUserData())->getAccessoryId());
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
	setText("Life", formatString(
		"The amount of life this player has.\n"
		"The tank explodes when life reaches 0.\n"
		"Less weapon power is available with less life.\n"
		"Life : %i/%i",
		(int) tank_->getLife().getLife(),
		(int) tank_->getLife().getMaxLife()));
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
	if (count >= 0) snprintf(buffer, 128, "%i", count);
	else snprintf(buffer, 128, "Infinite");
	setText("Parachutes", formatString(
		"Prevents the tank from taking damage\n"
		"when falling.  Must be enabled before\n"
		"they take effect.\n"
		"Click to enable/disable parachutes.\n"
		"Number Parachutes : %s\n"
		"Status : %s",
		buffer,
		(tank_->getParachute().parachutesEnabled()?
		"On":"Off")));
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
		if (count >= 0) snprintf(buffer, 128, "On (%i)", count);
		else snprintf(buffer, 128, "On (In)");
		entries.push_back(GLWSelectorEntry(buffer, &useTip, 0, 0, (void *) 1));
	}
	entries.push_back(GLWSelectorEntry("Off", &offTip, 0, 0, (void *) 0));
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankParachutesTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();

	if (entry->getUserData()==0)
	{
		tankAI->parachutesUpDown(0);
	}
	else
	{
		std::list<Accessory *> parachutes;
		tank_->getAccessories().getAllAccessoriesByType(
			AccessoryPart::AccessoryParachute, parachutes);
		if (parachutes.size() == 1)
		{
			tankAI->parachutesUpDown(parachutes.front()->getAccessoryId());
		}
	}
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
	setText("Auto Defense", formatString(
		"Allows the tank to raise shields and\n"
		"activate parachutes before the round\n"
		"starts.\n"
		"Click to see auto defense status.\n"
		"Status : %s",
		(tank_->getAccessories().getAutoDefense().haveDefense()?
		"On":"Off (Not Bought)")));
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
	if (tank_->getAccessories().getAccessoryCount(
		tank_->getAccessories().getWeapons().getCurrent()) > 0)
	{
		setText("Weapon", formatString(
			"The currently selected weapon.\n"
			"Click to change weapon.\n"
			"Weapon : %s (%i)\n"
			"Description :\n%s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getAccessories().getAccessoryCount(
			tank_->getAccessories().getWeapons().getCurrent()),
			tank_->getAccessories().getWeapons().getCurrent()->getDescription()));
	}
	else if (tank_->getAccessories().getWeapons().getCurrent())
	{
		setText("Weapon", formatString(
			"The currently selected weapon.\n"
			"Click to change weapon.\n"
			"Weapon : %s (Infinite)\n"
			"Description :\n%s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getAccessories().getWeapons().getCurrent()->getDescription()));
	}
}

void TankWeaponTip::showItems(float x, float y)
{
	std::list<GLWSelectorEntry> entries;

	Accessory *currentWeapon = 
		tank_->getAccessories().getWeapons().getCurrent();
	std::list<Accessory *> weapons;
	tank_->getAccessories().getAllAccessoriesByType(
		AccessoryPart::AccessoryWeapon, weapons,
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{
		Accessory *weapon = (*itor);
		int count = tank_->getAccessories().getAccessoryCount(weapon);

		char buffer[128];
		if (count > 0)
		{
			snprintf(buffer, 128, "%s (%i)", 
				weapon->getName(),
				count);
		}
		else
		{
			snprintf(buffer, 128, "%s (In)", 
				weapon->getName());
		}
		entries.push_back(GLWSelectorEntry(buffer, &weapon->getToolTip(), 
			(currentWeapon == weapon), weapon->getTexture(), weapon));
	}
	GLWSelector::instance()->showSelector(this, x, y, entries,
		ClientState::StatePlaying);
}

void TankWeaponTip::itemSelected(GLWSelectorEntry *entry, int position)
{
	TankAIHuman *tankAI = (TankAIHuman *) tank_->getTankAI();
	tank_->getAccessories().getWeapons().setWeapon((Accessory *) entry->getUserData());
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
	if (tank_->getAccessories().getWeapons().getCurrent())
	{
		setText("Power", formatString(
			"The power used to fire the %s.\n"
			"Click to revert back to previous settings.\n"
			"Power : %s",
			tank_->getAccessories().getWeapons().getCurrent()->getName(),
			tank_->getPosition().getPowerString()));
	}
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
	setText("Rotation", formatString(
		"The rotation of the current player's tank turret.\n"
		"Click to revert back to previous settings.\n"
		"Rotation : %s",
		tank_->getPosition().getRotationString()));
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
	setText("Elevation", formatString(
		"The elevation of the current player's gun.\n"
		"Click to revert back to previous settings.\n"
		"Elevation : %s",
		tank_->getPosition().getElevationString()));
}

static void generateTargetTip(std::string &tip, Target *target)
{
	tip += formatString(
		"Life   : %.0f/%.0f", 
		target->getLife().getLife(), 
		target->getLife().getMaxLife());
	if (target->getShield().getCurrentShield())
	{
		Shield *shield = (Shield*) 
			target->getShield().getCurrentShield()->getAction();
		tip += formatString("\nShield : %.0f/%.0f",
			target->getShield().getShieldPower(),
			shield->getPower());
	}
	if (!target->isTarget())
	{
		Tank *tank = (Tank *) target;

		tip += formatString("\nLives : %i/%i",
			tank->getState().getLives(),
			tank->getState().getMaxLives());
		tip += formatString("\nScore  : %i",
			tank->getScore().getScore());
	}
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
	std::string tip;
	generateTargetTip(tip, tank_);
	setText(tank_->getName(), tip.c_str());
}

TargetTip::TargetTip(Target *target) : 
	target_(target)
{
}

TargetTip::~TargetTip()
{
}

void TargetTip::populate()
{
	std::string tip;
	generateTargetTip(tip, target_);
	setText(target_->getName(), tip.c_str());
}

GLWTargetTips::GLWTargetTips(Target *target) : 
	targetTip(target)
{
}

GLWTargetTips::~GLWTargetTips()
{
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

