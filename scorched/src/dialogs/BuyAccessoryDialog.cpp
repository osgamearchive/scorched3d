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

#include <dialogs/BuyAccessoryDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWIcon.h>
#include <GLW/GLWFlag.h>
#include <GLEXT/GLViewPort.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <common/OptionsGame.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsBuyAccessoryMessage.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankContainer.h>
#include <stdio.h>

BuyAccessoryDialog::BuyAccessoryDialog() : 
	GLWWindow("", 10.0f, 10.0f, 440.0f, 280.0f, 0,
		"Allows the current player to buy and sell\n"
		"weapons and other accessories.")
{
	okId_ = addWidget(new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	buyWeaponTab_ = (GLWTab *)
		addWidget(new GLWTab("Weapons", 0, 10, 40, 420, 160, 80));
	buyOtherTab_ = (GLWTab *)
		addWidget(new GLWTab("Others", 120, 10, 40, 420, 160, 80));
	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Sell", 240, 10, 40, 420, 160, 80));
	topPanel_ = (GLWPanel *)
		addWidget(new GLWPanel(10, 245, 420, 30));
	sortBox_ = (GLWCheckBox *) addWidget(new GLWCheckBox(10, 10));
	sortBox_->setHandler(this);
	addWidget(new GLWLabel(35, 7, "Sort accessories by name"));
}

BuyAccessoryDialog::~BuyAccessoryDialog()
{

}

void BuyAccessoryDialog::stateChange(bool state, unsigned int id)
{
	OptionsDisplay::instance()->setSortAccessories(state);
	playerRefresh();
}

void BuyAccessoryDialog::setupWindow()
{
	float screenHeight = (float) GLViewPort::getHeight();
	float addition = 0;
	if (screenHeight > 340) addition = screenHeight - 340;
	if (addition > 200) addition = 200;

	setH(280 + addition);
	buyWeaponTab_->setH(160 + addition);
	sellTab_->setH(160 + addition);
	buyOtherTab_->setH(160 + addition);
	topPanel_->setY(240 + addition);

	needCentered_ = true;

	sortBox_->setState(OptionsDisplay::instance()->getSortAccessories());
}

void BuyAccessoryDialog::addPlayerName()
{
	topPanel_->clear();

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	char buffer[256];
	sprintf(buffer, "$%i", tank->getScore().getMoney());
	topPanel_->addWidget(new GLWFlag(tank->getColor(), 5, 5, 60));
	topPanel_->addWidget(new GLWLabel(75, 0, (char *) tank->getName()));
	topPanel_->addWidget(new GLWLabel(280, 0, buffer));
}

void BuyAccessoryDialog::addPlayerWeapons()
{
	addPlayerWeaponsSell();

	buyMap_.clear();
	addPlayerWeaponsBuy(buyWeaponTab_, true);
	addPlayerWeaponsBuy(buyOtherTab_, false);
}

void BuyAccessoryDialog::addPlayerWeaponsBuy(GLWTab *tab, bool showWeapons)
{
	tab->clear();
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	std::list<Accessory *> weapons;
	if (showWeapons) weapons = ScorchedClient::instance()->
		getAccessoryStore().getAllWeapons(
		OptionsDisplay::instance()->getSortAccessories());
	else weapons = ScorchedClient::instance()->
		getAccessoryStore().getAllOthers(
		OptionsDisplay::instance()->getSortAccessories());

	char buffer[256];
	int height = 10;

	std::list<Accessory *> accessories;
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{	
		Accessory *current = (*itor);
		if (10-current->getArmsLevel() <= 
			ScorchedClient::instance()->getOptionsGame().getMaxArmsLevel())
		{
			accessories.push_back(current);
		}
	}

	std::list<Accessory *>::iterator itor2;
	for (itor2 = accessories.begin();
		itor2 != accessories.end();
		itor2++)
	{
		Accessory *current = (*itor2);

		int currentNumber = 
			tank->getAccessories().getAccessoryCount(current);

		GLWPanel *newPanel = (GLWPanel *)
			tab->addWidget(new GLWPanel(10.0f, (float) height, 315.0f, 20.0f, true));
		newPanel->setToolTip(&current->getToolTip());
		sprintf(buffer, "%i", (currentNumber>=0?currentNumber:99));
		newPanel->addWidget(new GLWLabel(0, -2, buffer));
		newPanel->addWidget(new GLWIcon(30, 2, 16, 16, current->getTexture()));
		newPanel->addWidget(new GLWLabel(50, -2, (char *) current->getName()));
		sprintf(buffer, "$%i/%i", current->getPrice(), current->getBundle());
		newPanel->addWidget(new GLWLabel(205, -2, buffer));

		if ((!current->singular() && currentNumber >= 0) || (current->singular() && currentNumber==0))
		{
			if (currentNumber < ScorchedClient::instance()->getOptionsGame().getMaxNumberWeapons() &&
				current->getPrice() <= tank->getScore().getMoney())
			{
				GLWidget *button = 
					newPanel->addWidget(new GLWTextButton("Buy", 325, 0, 60, this, 
					GLWButton::ButtonFlagCenterX));
				buyMap_[button->getId()] = current;
			}
		}

		height += 24;
	}
}

void BuyAccessoryDialog::addPlayerWeaponsSell()
{
	sellTab_->clear();
	sellMap_.clear();

	char buffer[256];
	int height = 10;

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	std::list<Accessory *> tankAccessories = 
		tank->getAccessories().getAllAccessories(
			OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		Accessory *current = *itor;
		int count = tank->getAccessories().getAccessoryCount(current);

		GLWPanel *newPanel = (GLWPanel *)
			sellTab_->addWidget(new GLWPanel(10.0f, (float) height, 315.0f, 20.0f, true));
		newPanel->setToolTip(&current->getToolTip());
		if (count >= 0) sprintf(buffer, "%i", count);
		else sprintf(buffer, "In");
		newPanel->addWidget(new GLWLabel(0, -2, buffer));
		newPanel->addWidget(new GLWIcon(30, 2, 16, 16, current->getTexture()));
		newPanel->addWidget(new GLWLabel(50, -2, (char *) current->getName()));
		sprintf(buffer, "$%i/%i", current->getSellPrice(), 1);
		newPanel->addWidget(new GLWLabel(205, -2, buffer));

		if (count >= 0)
		{
			GLWidget *button =
				newPanel->addWidget(new GLWTextButton("Sell", 325, 0, 60, this,
				GLWButton::ButtonFlagCenterX));
			sellMap_[button->getId()] = current;
		}

		height += 24;
	}
}

void BuyAccessoryDialog::playerRefresh()
{
	setupWindow();
	addPlayerName();
	addPlayerWeapons();
}

void BuyAccessoryDialog::windowInit(const unsigned state)
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (tank)
	{
		buyWeaponTab_->setDepressed();
		playerRefresh();
	}
}

void BuyAccessoryDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimAutoDefense);
	}
	else
	{
		Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
		if (!tank) return;

		std::map<unsigned int, Accessory *>::iterator itor;
		itor = buyMap_.find(id);
		if (itor != buyMap_.end())
		{
			// Tell the server to add the accessory
			Accessory *acc = itor->second;
			ComsBuyAccessoryMessage buyMessage(tank->getPlayerId(), acc->getAccessoryId());
			ComsMessageSender::sendToServer(buyMessage);

			// Add the accessory
			tank->getAccessories().add(acc);
			tank->getScore().setMoney(tank->getScore().getMoney() - acc->getPrice());

			// Refresh the window
			int buyCurrent = buyWeaponTab_->getScrollBar().getCurrent();
			int otherCurrent = buyOtherTab_->getScrollBar().getCurrent();
			playerRefresh();
			buyWeaponTab_->getScrollBar().setCurrent(buyCurrent);
			buyOtherTab_->getScrollBar().setCurrent(otherCurrent);
		}
		else
		{
			itor = sellMap_.find(id);
			if (itor != sellMap_.end())
			{
				// Tell the server to add the accessory
				Accessory *acc = itor->second;
				ComsBuyAccessoryMessage buyMessage(tank->getPlayerId(), acc->getAccessoryId(), false);
				ComsMessageSender::sendToServer(buyMessage);

				// Add the accessory
				tank->getAccessories().rm(acc);
				tank->getScore().setMoney(tank->getScore().getMoney() + acc->getSellPrice());

				// Refresh the window
				int sellCurrent = sellTab_->getScrollBar().getCurrent();
				playerRefresh();
				sellTab_->getScrollBar().setCurrent(sellCurrent);
			}
		}
	}
}
