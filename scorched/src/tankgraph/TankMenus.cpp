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


// TankMenus.cpp: implementation of the TankMenus class.
//
//////////////////////////////////////////////////////////////////////

#include <engine/GameState.h>
#include <client/ClientState.h>
#include <tank/TankContainer.h>
#include <tankgraph/TankMenus.h>
#include <common/WindowManager.h>
#include <landscape/Landscape.h>
#include <landscape/GlobalHMap.h>
#include <dialogs/MainMenuDialog.h>
#include <dialogs/QuitDialog.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TankMenus::TankMenus()
{

}

TankMenus::~TankMenus()
{

}

// Player Menus

TankMenus::PlayerMenu::PlayerMenu()
{
	MainMenuDialog::instance()->addMenu("Player", 80, this, 0, 0, this);
	MainMenuDialog::instance()->addMenuItem("Player", "Skip Move");
	MainMenuDialog::instance()->addMenuItem("Player", "Resign Current Round");
	MainMenuDialog::instance()->addMenuItem("Player", "Exit Game");
}

void TankMenus::PlayerMenu::menuSelection(const char* menuName, 
										  const int position, const char *menuItem)
{
	Tank *firstTank = TankContainer::instance()->getCurrentTank();
	if (firstTank)
	{
		TankAI *tankAI = firstTank->getTankAI();
		if (tankAI)
		{
			switch (position)
			{
			case 0:
				tankAI->skipShot();
				break;
			case 1:
				tankAI->resign();
				break;
			case 2:
				WindowManager::instance()->showWindow(
					QuitDialog::instance()->getId());
				break;
			}
		}
	}
}

bool TankMenus::PlayerMenu::getEnabled(const char* menuName)
{
	if (GameState::instance()->getState() != ClientState::StateMain) return false;

	Tank *firstTank = TankContainer::instance()->getCurrentTank();
	if (firstTank)
	{
		return (firstTank->getState().getState() == TankState::sNormal);
	}
	return false;
}

// Accessory Menus

TankMenus::AccessoryMenu::AccessoryMenu()
{
	MainMenuDialog::instance()->addMenu("Weapons", 80, this, 0, this, this);
}

void TankMenus::AccessoryMenu::menuSelection(const char* menuName, 
											 const int position, const char *menuItem)
{
	Tank *firstTank = TankContainer::instance()->getCurrentTank();
	TankAI *tankAI = firstTank->getTankAI();
	if (firstTank && tankAI)
	{
		if (position < (int) menuItems_.size())
		{
			Accessory *accessory = menuItems_[position];

			switch (accessory->getType())
			{
			case Accessory::AccessoryParachute:
				tankAI->parachutesUpDown(
					!firstTank->getAccessories().getParachutes().parachutesEnabled());
				break;
			case Accessory::AccessoryShield:
				// TODO No way of putting the shield off!
				tankAI->shieldsUpDown(accessory->getName());
				break;
			case Accessory::AccessoryWeapon:
				firstTank->getAccessories().getWeapons().setWeapon(
					(Weapon *) accessory);
				break;
			case Accessory::AccessoryBattery:
				if (firstTank->getState().getLife() < 100.0f)
				{
					tankAI->useBattery();
				}
				break;
			case Accessory::AccessoryFuel:
				if (Landscape::instance()->getTextureType() == Landscape::eMovement)
				{
					Landscape::instance()->restoreLandscapeTexture();
				}
				else
				{
					GlobalHMap::instance()->getMMap().calculateForTank(firstTank);
					GlobalHMap::instance()->getMMap().movementTexture();
				}
				break;
			default:
			case Accessory::AccessoryAutoDefense:
				break;
			}
		}
	}
}

void TankMenus::AccessoryMenu::getMenuItems(const char* menuName, 
											std::list<std::string> &result)
{
	menuItems_.clear();
	Tank *firstTank = TankContainer::instance()->getCurrentTank();
	if (!firstTank) return;

	std::list<std::pair<Accessory *, int> > tankAccessories;
	std::list<std::pair<Accessory *, int> >::iterator itor;

	Accessory::AccessoryType lastType = Accessory::AccessoryWeapon;
	firstTank->getAccessories().getAllAccessories(tankAccessories);
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		Accessory *accessory = (*itor).first;
		int accessoryCount = (*itor).second;

		bool sel = false;
		switch (accessory->getType())
		{
		case Accessory::AccessoryParachute:
			sel = firstTank->getAccessories().getParachutes().parachutesEnabled();
			break;
		case Accessory::AccessoryShield:
			sel = (firstTank->getAccessories().getShields().getCurrentShield() == accessory);
			break;
		case Accessory::AccessoryWeapon:
			sel = (firstTank->getAccessories().getWeapons().getCurrent() == accessory);
			break;
		case Accessory::AccessoryFuel:
			sel = (Landscape::instance()->getTextureType() == Landscape::eMovement);
			break;
		case Accessory::AccessoryAutoDefense:
			sel = true;
			break;
		default:
		case Accessory::AccessoryBattery:
			sel = false;
			break;
		}

		if (lastType != accessory->getType())
		{
			lastType = accessory->getType();
			result.push_back(std::string("----------"));
			menuItems_.push_back(0);
		}

		static char buffer[1024];
		if (accessoryCount > 0)
		{
			sprintf(buffer, "%c %s (%i)", 
				(sel?'x':' '),
				accessory->getName(), accessoryCount);
		}
		else
		{
			sprintf(buffer, "%c %s (In)", 
				(sel?'x':' '),
				accessory->getName());
		}
		menuItems_.push_back(accessory);
		result.push_back(std::string(buffer));
	}
}

bool TankMenus::AccessoryMenu::getEnabled(const char* menuName)
{
	if (GameState::instance()->getState() != ClientState::StateMain) return false;

	Tank *firstTank = TankContainer::instance()->getCurrentTank();
	if (firstTank)
	{
		return (firstTank->getState().getState() == TankState::sNormal);
	}
	return false;
}

