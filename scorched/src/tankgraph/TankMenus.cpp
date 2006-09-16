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
#include <sound/SoundUtils.h>
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
	new GLConsoleRuleMethodIAdapterEx<TankMenus>(
		this, &TankMenus::whisper, "Whisper");
	new GLConsoleRuleFnIBooleanAdapter(
		"ComsMessageLogging", 
		ScorchedClient::instance()->getComsMessageHandler().getMessageLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"StateLogging", 
		ScorchedClient::instance()->getGameState().getStateLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"ActionLogging",
		ScorchedClient::instance()->getActionController().getActionLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"ActionProfiling",
		ScorchedServer::instance()->getActionController().getActionProfiling());
	new GLConsoleRuleFnIBooleanAdapter(
		"StateLogging",
		ScorchedClient::instance()->getGameState().getStateLogging());
	new GLConsoleRuleFnIBooleanAdapter(
		"StateTimeLogging",
		ScorchedClient::instance()->getGameState().getStateTimeLogging());
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

void TankMenus::whisper(std::list<GLConsoleRuleSplit> list)
{
	list.pop_front();
	if (!list.empty())
	{
		const char *playerName = list.begin()->rule.c_str();
		Tank *tank = 
			ScorchedClient::instance()->getTankContainer().
				getTankByName(playerName);
		if (!tank)
		{
			Logger::log(formatString("Failed to find a player name \"%s\"",
				playerName));
		}
		else
		{
			list.pop_front();
			if (!list.empty())
			{
				ComsTextMessage message(list.begin()->rule.c_str(),
					ScorchedClient::instance()->getTankContainer().getCurrentPlayerId(),
					false,
					false,
					0,
					tank->getPlayerId());
				ComsMessageSender::sendToServer(message);
			}
		}
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
		ScorchedClient::instance()->getTankContainer().getAllTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		GLConsole::instance()->addLine(false,
			formatString("--%s------------------------------------",
			tank->getName()));

		std::list<Accessory *> accessories;
		tank->getAccessories().getAllAccessories(accessories);
		ScorchedClient::instance()->getAccessoryStore().sortList(accessories, true);

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
		ScorchedClient::instance()->getTankContainer().getAllTanks();
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
			tank->getName(), 
			tank->getModelContainer().getTankModelName());
		GLConsole::instance()->addLine(false, buffer);
	}

	GLConsole::instance()->addLine(false,
		"----------------------------------------------------");
}

// Player Menus

TankMenus::PlayerMenu::PlayerMenu()
{
	GLBitmap *map = new GLBitmap(
		formatString(getDataFile("data/windows/setting.bmp")),
		formatString(getDataFile("data/windows/settinga.bmp")),
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
		formatString(getDataFile("data/windows/bomb.bmp")),
		formatString(getDataFile("data/windows/bomba.bmp")),
		false);
	DIALOG_ASSERT(map->getBits());
	MainMenuDialog::instance()->addMenu("Weapons", 32, 
		ClientState::StimPlaying, this, map);
}

void TankMenus::AccessoryMenu::menuSelection(const char* menuName, 
	const int position, GLMenuItem &item)
{
	Accessory *accessory = (Accessory *) item.getUserData();
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (firstTank && accessory)
	{
		firstTank->getAccessories().activate(accessory);
	}
}

bool TankMenus::AccessoryMenu::getMenuItems(const char* menuName, 
											std::list<GLMenuItem> &result)
{
	Tank *firstTank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!firstTank) return true;

	bool firstIteration = true;
	AccessoryPart::AccessoryType lastType = AccessoryPart::AccessoryWeapon;
	std::string lastGroup = "";

	std::list<Accessory *> tankAccessories;
	firstTank->getAccessories().getAllAccessories(tankAccessories);
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
			sel = (firstTank->getParachute().getCurrentParachute() == accessory);
			break;
		case AccessoryPart::AccessoryShield:
			sel = (firstTank->getShield().getCurrentShield() == accessory);
			break;
		case AccessoryPart::AccessoryWeapon:
			sel = (firstTank->getAccessories().getWeapons().getCurrent() == accessory);
			break;
		case AccessoryPart::AccessoryAutoDefense:
			sel = true;
			break;
		default:
		case AccessoryPart::AccessoryBattery:
			sel = false;
			break;
		}

		if (!firstIteration &&
			(lastType != accessory->getType() ||
			0 != strcmp(lastGroup.c_str(), accessory->getGroupName())))
		{
			result.push_back("----------");
		}
		lastType = accessory->getType();
		lastGroup = accessory->getGroupName();
		firstIteration = false;

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

		result.push_back(
			GLMenuItem(
				buffer, 
				&accessory->getToolTip(), 
				sel,
				accessory->getTexture(),
				accessory));
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


