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
	GLWWindow("", 10.0f, 10.0f, 440.0f, 300.0f, 0,
		"Allows the current player to buy and sell\n"
		"weapons and other accessories."),
	firstDrawTime_(true)
{
	okId_ = addWidget(new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	buyWeaponTab_ = (GLWTab *)
		addWidget(new GLWTab("Weapons", 10, 40, 420, 160));
	buyOtherTab_ = (GLWTab *)
		addWidget(new GLWTab("Defense", 10, 40, 420, 160));
	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Inv", 10, 40, 420, 160));
	topPanel_ = (GLWPanel *)
		addWidget(new GLWPanel(10, 265, 420, 50));
	sortBox_ = (GLWCheckBox *) addWidget(new GLWCheckBox(10, 10));
	sortBox_->setHandler(this);
	addWidget(new GLWLabel(35, 7, "Sort accessories by name"));
}

BuyAccessoryDialog::~BuyAccessoryDialog()
{
}

void BuyAccessoryDialog::draw()
{
	if (firstDrawTime_)
	{
		firstDrawTime_ = false;
		float screenHeight = (float) GLViewPort::getHeight();
		float addition = 0;
		if (screenHeight > 340) addition = screenHeight - 340;
		if (addition > 200) addition = 200;

		setH(300 + addition);
		buyWeaponTab_->setH(160 + addition);
		sellTab_->setH(160 + addition);
		buyOtherTab_->setH(160 + addition);
		topPanel_->setY(240 + addition);

		needCentered_ = true;
	}
	GLWWindow::draw();
}

void BuyAccessoryDialog::addPlayerName()
{
	topPanel_->clear();

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	topPanel_->addWidget(new GLWFlag(tank->getColor(), 5, 15, 60));
	topPanel_->addWidget(new GLWLabel(75, 10, (char *) tank->getName()));
	topPanel_->addWidget(new GLWLabel(260, 20, (char *)
		formatString("$%i", tank->getScore().getMoney())));
	topPanel_->addWidget(new GLWLabel(260, 0, (char *)
		formatString("Round %i of %i", 
		ScorchedClient::instance()->getOptionsTransient().getCurrentRoundNo(),
		ScorchedClient::instance()->getOptionsGame().getNoRounds())));
}

void BuyAccessoryDialog::addPlayerWeapons()
{
	buyMap_.clear();
	sellMap_.clear();

	sellTab_->clear();
	buyWeaponTab_->clear();
	buyOtherTab_->clear();

	addPlayerWeaponsSell();
	addPlayerWeaponsBuy(buyWeaponTab_, true);
	addPlayerWeaponsBuy(buyOtherTab_, false);
}

void BuyAccessoryDialog::addPlayerWeaponsBuy(GLWTab *tab, bool showWeapons)
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	std::list<Accessory *> weapons;
	if (showWeapons) weapons = ScorchedClient::instance()->
		getAccessoryStore().getAllWeapons(
		OptionsDisplay::instance()->getSortAccessories());
	else weapons = ScorchedClient::instance()->
		getAccessoryStore().getAllOthers(
		OptionsDisplay::instance()->getSortAccessories());

	std::list<Accessory *> accessories;
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{	
		Accessory *current = (*itor);
		if (current->getMaximumNumber() > 0 &&
			10-current->getArmsLevel() <= 
			ScorchedClient::instance()->getOptionsTransient().getArmsLevel())
		{
			accessories.push_back(current);
		}
	}

	float height = 10.0f;
	std::list<Accessory *>::iterator itor2;
	for (itor2 = accessories.begin();
		itor2 != accessories.end();
		itor2++)
	{
		Accessory *current = (*itor2);
		addAccessory(tank, tab, height, current);
		height += 24.0f;
	}
}

void BuyAccessoryDialog::addPlayerWeaponsSell()
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	float height = 10.0f;
	std::list<Accessory *> tankAccessories = 
		tank->getAccessories().getAllAccessories(
			OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		Accessory *current = *itor;
		addAccessory(tank, sellTab_, height, current);
		height += 24.0f;
	}
}

void BuyAccessoryDialog::addAccessory(
	Tank *tank, GLWTab *tab, 
	float height, Accessory *current)
{
	char buffer[256];
	int currentNumber = 
		tank->getAccessories().getAccessoryCount(current);

	GLWPanel *newPanel = (GLWPanel *)
		tab->addWidget(new GLWPanel(5.0f, (float) height, 290.0f, 20.0f, true));
	newPanel->setToolTip(&current->getToolTip());
	sprintf(buffer, "%i", (currentNumber>=0?currentNumber:99));
	newPanel->addWidget(new GLWLabel(5, -2, buffer, 12.0f));
	newPanel->addWidget(new GLWIcon(30, 2, 16, 16, current->getTexture()));
	newPanel->addWidget(new GLWLabel(50, -2, (char *) current->getName(), 12.0f));
	sprintf(buffer, "$%i/%i", current->getPrice(), current->getBundle());
	newPanel->addWidget(new GLWLabel(195, -2, buffer, 12.0f));

	if (currentNumber + current->getBundle() <= current->getMaximumNumber() && // Not exceeded maximum
		current->getStartingNumber() != -1) // Not infinite
	{
		if (current->getPrice() <= tank->getScore().getMoney())
		{
			GLWTextButton *button = (GLWTextButton *)
				newPanel->addWidget(new GLWTextButton("Buy", 295, 0, 45, this, 
				GLWButton::ButtonFlagCenterX, 12.0f));
			button->setH(button->getH() - 2.0f);
			buyMap_[button->getId()] = current;
		}
	}
	if (currentNumber > 0 && 
		current->getStartingNumber() != -1)
	{
		GLWTextButton *button = (GLWTextButton *)
			newPanel->addWidget(new GLWTextButton("Sell", 345, 0, 45, this,
			GLWButton::ButtonFlagCenterX, 12.0f));
		button->setH(button->getH() - 2.0f);
		sellMap_[button->getId()] = current;
	}
}

void BuyAccessoryDialog::playerRefresh()
{
	addPlayerName();
	addPlayerWeapons();
}

void BuyAccessoryDialog::windowInit(const unsigned state)
{
	sortBox_->setState(OptionsDisplay::instance()->getSortAccessories());
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (tank)
	{
		buyWeaponTab_->setDepressed();
		playerRefresh();
	}
}

void BuyAccessoryDialog::stateChange(bool state, unsigned int id)
{
	// The sort accessories check box has been clicked
	OptionsDisplay::instance()->setSortAccessories(state);
	playerRefresh();
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
			int sellCurrent = sellTab_->getScrollBar().getCurrent();
			playerRefresh();
			buyWeaponTab_->getScrollBar().setCurrent(buyCurrent);
			buyOtherTab_->getScrollBar().setCurrent(otherCurrent);
			sellTab_->getScrollBar().setCurrent(sellCurrent);
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
				int buyCurrent = buyWeaponTab_->getScrollBar().getCurrent();
				int otherCurrent = buyOtherTab_->getScrollBar().getCurrent();
				int sellCurrent = sellTab_->getScrollBar().getCurrent();
				playerRefresh();
				buyWeaponTab_->getScrollBar().setCurrent(buyCurrent);
				buyOtherTab_->getScrollBar().setCurrent(otherCurrent);
				sellTab_->getScrollBar().setCurrent(sellCurrent);
			}
		}
	}
}
