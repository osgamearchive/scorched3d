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

#include <weapons/Weapon.h>
#include <engine/GameState.h>
#include <engine/ActionController.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <server/ScorchedServer.h>
#include <tankai/TankAIHuman.h>
#include <tankai/TankAIComputer.h>
#include <tankgraph/TankMenus.h>
#include <tankgraph/TankModelRenderer.h>
#include <GLW/GLWWindowManager.h>
#include <common/OptionsParam.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsTextMessage.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <dialogs/MainMenuDialog.h>
#include <dialogs/QuitDialog.h>
#include <dialogs/SaveDialog.h>
#include <GLEXT/GLConsoleRuleFnIAdapter.h>

TankMenus::TankMenus() : logger_("ClientLog")
{
	new GLConsoleRuleMethodIAdapter<Landscape>(
		Landscape::instance(), &Landscape::reset, "ResetLandscape");
	new GLConsoleRuleMethodIAdapter<Landscape>(
		Landscape::instance(), &Landscape::updatePlanTexture, "RecalculatePlan");
	new GLConsoleRuleMethodIAdapter<Landscape>(
		Landscape::instance(), &Landscape::updatePlanATexture, "RecalculateAPlan");

	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::showTankDetails, "TankDetails");
	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::logToFile, "LogToFile");
	new GLConsoleRuleMethodIAdapterEx<TankMenus>(
		this, &TankMenus::say, "Say");
	new GLConsoleRuleFnIBooleanAdapter(
		"ComsMessageLogging", 
		ScorchedClient::instance()->getComsMessageHandler().getMessageLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"StateLogging", 
		ScorchedClient::instance()->getGameState().getStateLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"ActionLogging",
		ScorchedClient::instance()->getActionController().getActionLogging());
}

TankMenus::~TankMenus()
{

}

void TankMenus::say(std::list<GLConsoleRuleSplit> list)
{
	list.pop_front();
	if (!list.empty())
	{
		ComsTextMessage message(list.begin()->rule.c_str(),
			ScorchedClient::instance()->getTankContainer().getCurrentPlayerId(),
			false,
			false);
		ComsMessageSender::sendToServer(message);
	}
}

void TankMenus::logToFile()
{
	Logger::addLogger(&logger_);
}

void TankMenus::showTankDetails()
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	Tank *currentTank = 
		ScorchedClient::instance()->getTankContainer().getCurrentTank();

	GLConsole::instance()->addLine(false,
		"--Tank Dump-----------------------------------------");
		
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		TankModelId &modelId = tank->getModel();

		const char *description = "Unknown";
		Tank *otherTank = ScorchedServer::instance()->
			getTankContainer().getTankById(tank->getPlayerId());
		if (tank->getTankAI())
		{
			description = "Human";
		}
		else if (otherTank && otherTank->getTankAI())
		{
			description = 
				((TankAIComputer *)otherTank->getTankAI())->getName();
		}

		char buffer[1024];
		sprintf(buffer, "%c %8s - \"%10s\" (%s)", 
			currentTank == tank?'>':' ',
			description,
			tank->getName(), modelId.getModelName());
		GLConsole::instance()->addLine(false, buffer);
	}

	GLConsole::instance()->addLine(false,
		"----------------------------------------------------");
}

// Player Menus

TankMenus::PlayerMenu::PlayerMenu()
{
	MainMenuDialog::instance()->addMenu("Player", 90, this, 0, 0, this);
	MainMenuDialog::instance()->addMenuItem("Player", 
		GLMenuItem("Skip Move",
		new GLWTip("Skip Move", 
			"Player forfits this move.")));
	MainMenuDialog::instance()->addMenuItem("Player", 
		GLMenuItem("Resign Round",
		new GLWTip("Resign Round", 
			"Player resigns from this round.\nPlayer takes no part in the rest of the round.")));
	MainMenuDialog::instance()->addMenuItem("Player", 
		GLMenuItem("Exit Game",
		new GLWTip("Exit Game", "Stop Playing Scorched.")));
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		MainMenuDialog::instance()->addMenuItem("Player",
			GLMenuItem("Mass Tank Kill",
			new GLWTip("Mass Tank Kill",
				"Kill all tanks.\nStarts the next round.")));
		MainMenuDialog::instance()->addMenuItem("Player",
			GLMenuItem("Save",
			new GLWTip("Save", "Save this game.")));
	}
}

void TankMenus::PlayerMenu::menuSelection(const char* menuName, 
										  const int position, const char *menuItem)
{
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (firstTank)
	{
		TankAIHuman *tankAI = (TankAIHuman *) firstTank->getTankAI();
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
				GLWWindowManager::instance()->showWindow(
					QuitDialog::instance()->getId());
				break;
			case 3:
				GLWWindowManager::instance()->showWindow(
					QuitDialog::instance()->getId());
				break;
			case 4:
				GLWWindowManager::instance()->showWindow(
					SaveDialog::instance()->getId());
				break;
			}
		}
	}
}

bool TankMenus::PlayerMenu::getEnabled(const char* menuName)
{
	if (ScorchedClient::instance()->getGameState().getState() 
		!= ClientState::StatePlaying) return false;

	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (firstTank)
	{
		return (firstTank->getState().getState() == TankState::sNormal);
	}
	return false;
}

// Accessory Menus

TankMenus::AccessoryMenu::AccessoryMenu()
{
	MainMenuDialog::instance()->addMenu("Weapons", 90, this, 0, this, this);
}

void TankMenus::AccessoryMenu::menuSelection(const char* menuName, 
											 const int position, const char *menuItem)
{
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	TankAIHuman *tankAI = (TankAIHuman *) firstTank->getTankAI();
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
				tankAI->shieldsUpDown(accessory->getAccessoryId());
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
					ScorchedClient::instance()->getLandscapeMaps().getMMap().calculateForTank(firstTank,
						ScorchedClient::instance()->getContext());
					ScorchedClient::instance()->getLandscapeMaps().getMMap().movementTexture();
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
											std::list<GLMenuItem> &result)
{
	menuItems_.clear();
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!firstTank) return;

	Accessory::AccessoryType lastType = Accessory::AccessoryWeapon;
	std::list<Accessory *> tankAccessories = 
		firstTank->getAccessories().getAllAccessories();
	std::list<Accessory *>::iterator itor;
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		int accessoryCount = 
			firstTank->getAccessories().getAccessoryCount(accessory);

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
			result.push_back("----------");
			menuItems_.push_back(0);
		}

		static char buffer[1024];
		if (accessoryCount > 0)
		{
			sprintf(buffer, "%s (%i)", 
				accessory->getName(), accessoryCount);
		}
		else
		{
			sprintf(buffer, "%s (In)", 
				accessory->getName());
		}
		menuItems_.push_back(accessory);
		result.push_back(GLMenuItem(buffer, &accessory->getToolTip(), sel));
	}
}

bool TankMenus::AccessoryMenu::getEnabled(const char* menuName)
{
	if (ScorchedClient::instance()->getGameState().getState() != 
		ClientState::StatePlaying) return false;

	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (firstTank)
	{
		return (firstTank->getState().getState() == TankState::sNormal);
	}
	return false;
}

