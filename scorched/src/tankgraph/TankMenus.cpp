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
#include <tank/TankContainer.h>
#include <tankgraph/TankMenus.h>
#include <tankgraph/TankModelRenderer.h>
#include <GLW/GLWWindowManager.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <common/FileLogger.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsMessageHandler.h>
#include <coms/ComsTextMessage.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/MovementMap.h>
#include <dialogs/MainMenuDialog.h>
#include <dialogs/QuitDialog.h>
#include <dialogs/SaveDialog.h>
#include <dialogs/ResignDialog.h>
#include <dialogs/SkipDialog.h>
#include <GLEXT/GLConsoleRuleFnIAdapter.h>
#include <GLEXT/GLTexture.h>

TankMenus::TankMenus() : logger_("ClientLog")
{
	new GLConsoleRuleMethodIAdapter<Landscape>(
		Landscape::instance(), &Landscape::updatePlanTexture, "ResetPlan");
	new GLConsoleRuleMethodIAdapter<Landscape>(
		Landscape::instance(), &Landscape::updatePlanATexture, "ResetAPlan");

	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::resetLandscape, "ResetLandscape");
	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::showTankDetails, "TankDetails");
	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::showTextureDetails, "TextureDetails");
	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::logToFile, "LogToFile");
	new GLConsoleRuleMethodIAdapterEx<TankMenus>(
		this, &TankMenus::say, "Say");
	new GLConsoleRuleMethodIAdapterEx<TankMenus>(
		this, &TankMenus::teamsay, "Teamsay");
	new GLConsoleRuleFnIBooleanAdapter(
		"ComsMessageLogging", 
		ScorchedClient::instance()->getComsMessageHandler().getMessageLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"StateLogging", 
		ScorchedClient::instance()->getGameState().getStateLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"ActionLogging",
		ScorchedClient::instance()->getActionController().getActionLogging());
	new GLConsoleRuleMethodIAdapter<TankMenus>(
		this, &TankMenus::showInventory, "TankInventory");
	
	if (OptionsDisplay::instance()->getClientLogToFile())
	{
		logToFile();
	}

	if (false) // Debuging
	{
		logToFile();
		ScorchedServer::instance()->getGameState().getStateLogging() = true;
		ScorchedClient::instance()->getGameState().getStateLogging() = true;
		ScorchedServer::instance()->getComsMessageHandler().getMessageLogging() = true;
		ScorchedClient::instance()->getComsMessageHandler().getMessageLogging() = true;
	}
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

void TankMenus::teamsay(std::list<GLConsoleRuleSplit> list)
{
	list.pop_front();
	if (!list.empty())
	{
		ComsTextMessage message(list.begin()->rule.c_str(),
			ScorchedClient::instance()->getTankContainer().getCurrentPlayerId(),
			false,
			true);
		ComsMessageSender::sendToServer(message);
	}
}

void TankMenus::logToFile()
{
	Logger::addLogger(&logger_);
}

void TankMenus::showTextureDetails()
{
	GLConsole::instance()->addLine(false,
		formatString("%i bytes", GLTexture::getTextureSpace()));
}

void TankMenus::resetLandscape()
{
	Landscape::instance()->reset();
}

void TankMenus::showInventory()
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		GLConsole::instance()->addLine(false,
			formatString("--%s------------------------------------",
			tank->getName()));

		
		std::list<Accessory *> accessories =
			tank->getAccessories().getAllAccessories(true);
		std::list<Accessory *>::iterator aitor;
		for (aitor = accessories.begin();
			aitor != accessories.end();
			aitor++)
		{
			Accessory *accessory = (*aitor);

			GLConsole::instance()->addLine(false,
				formatString("%s - %i", accessory->getName(), 
				tank->getAccessories().getAccessoryCount(accessory)));
		}

		GLConsole::instance()->addLine(false,
			"----------------------------------------------------");
	}
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

		TargetModelId &modelId = tank->getModel();

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
		snprintf(buffer, 1024, "%c %8s - \"%10s\" (%s)", 
			currentTank == tank?'>':' ',
			description,
			tank->getName(), modelId.getTankModelName());
		GLConsole::instance()->addLine(false, buffer);
	}

	GLConsole::instance()->addLine(false,
		"----------------------------------------------------");
}

// Player Menus

TankMenus::PlayerMenu::PlayerMenu()
{
	GLBitmap *map = new GLBitmap(
		formatString(getDataFile("data/windows/screen.bmp")),
		formatString(getDataFile("data/windows/screena.bmp")),
		false);
	DIALOG_ASSERT(map->getBits());
	MainMenuDialog::instance()->addMenu("Player", 32, 
		ClientState::StimPlaying, this, map);

	MainMenuDialog::instance()->addMenuItem("Player", 
		GLMenuItem("Skip Move",
		new GLWTip("Skip Move", 
			"Player forfits this move.")));
	MainMenuDialog::instance()->addMenuItem("Player", 
		GLMenuItem("Resign Round",
		new GLWTip("Resign Round", 
			"Player resigns from this round.\n"
			"Player takes no part in the rest of the round.")));
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
	const int position, GLMenuItem &item)
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
				GLWWindowManager::instance()->showWindow(
					SkipDialog::instance()->getId());
				break;
			case 1:
				GLWWindowManager::instance()->showWindow(
					ResignDialog::instance()->getId());
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
	GLBitmap *map = new GLBitmap(
		formatString(getDataFile("data/windows/screen.bmp")),
		formatString(getDataFile("data/windows/screena.bmp")),
		false);
	DIALOG_ASSERT(map->getBits());
	MainMenuDialog::instance()->addMenu("Weapons", 32, 
		ClientState::StimPlaying, this, map);
}

void TankMenus::AccessoryMenu::menuSelection(const char* menuName, 
	const int position, GLMenuItem &item)
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
			case AccessoryPart::AccessoryParachute:
				tankAI->parachutesUpDown(
					!firstTank->getParachute().parachutesEnabled());
				break;
			case AccessoryPart::AccessoryShield:
				tankAI->shieldsUpDown(accessory->getAccessoryId());
				break;
			case AccessoryPart::AccessoryWeapon:
				firstTank->getAccessories().getWeapons().setWeapon(accessory);
				break;
			case AccessoryPart::AccessoryBattery:
				if (firstTank->getLife().getLife() < 
					firstTank->getLife().getMaxLife())
				{
					tankAI->useBattery();
				}
				break;
			case AccessoryPart::AccessoryFuel:
				if (Landscape::instance()->getTextureType() == Landscape::eMovement)
				{
					Landscape::instance()->restoreLandscapeTexture();
				}
				else
				{
					MovementMap mmap(
						ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth(),
						ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight());
					mmap.calculateForTank(
						firstTank, ScorchedClient::instance()->getContext());
					mmap.movementTexture();
				}
				break;
			default:
			case AccessoryPart::AccessoryAutoDefense:
				break;
			}
		}
	}
}

bool TankMenus::AccessoryMenu::getMenuItems(const char* menuName, 
											std::list<GLMenuItem> &result)
{
	menuItems_.clear();
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!firstTank) return true;

	AccessoryPart::AccessoryType lastType = AccessoryPart::AccessoryWeapon;
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
		case AccessoryPart::AccessoryParachute:
			sel = firstTank->getParachute().parachutesEnabled();
			break;
		case AccessoryPart::AccessoryShield:
			sel = (firstTank->getShield().getCurrentShield() == accessory);
			break;
		case AccessoryPart::AccessoryWeapon:
			sel = (firstTank->getAccessories().getWeapons().getCurrent() == accessory);
			break;
		case AccessoryPart::AccessoryFuel:
			sel = (Landscape::instance()->getTextureType() == Landscape::eMovement);
			break;
		case AccessoryPart::AccessoryAutoDefense:
			sel = true;
			break;
		default:
		case AccessoryPart::AccessoryBattery:
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
			snprintf(buffer, 1024, "%s (%i)", 
				accessory->getName(), accessoryCount);
		}
		else
		{
			snprintf(buffer, 1024, "%s (In)", 
				accessory->getName());
		}
		menuItems_.push_back(accessory);
		result.push_back(
			GLMenuItem(
				buffer, 
				&accessory->getToolTip(), 
				sel,
				accessory->getTexture()));
	}
	return true;
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


