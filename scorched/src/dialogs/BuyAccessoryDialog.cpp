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


// BuyAccessoryDialog.cpp: implementation of the BuyAccessoryDialog class.
//
//////////////////////////////////////////////////////////////////////

#include <dialogs/BuyAccessoryDialog.h>
#include <GLW/GLWTextButton.h>
#include <tank/TankContainer.h>
#include <client/ClientState.h>
#include <common/WindowManager.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsBuyAccessoryMessage.h>
#include <weapons/AccessoryStore.h>
#include <GLW/GLWFlag.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuyAccessoryDialog::BuyAccessoryDialog() : 
	GLWWindow("", 10.0f, 10.0f, 440.0f, 280.0f, 0),
	currentPlayer_(-1), actualPlayer_(0)
{
	okId_ = addWidget(new GLWTextButton(" Ok", 375, 10, 55, this, true))->getId();

	buyWeaponTab_ = (GLWTab *)
		addWidget(new GLWTab("Weapons", 0, 10, 40, 420, 160, 80));
	buyOtherTab_ = (GLWTab *)
		addWidget(new GLWTab("Others", 120, 10, 40, 420, 160, 80));
	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Sell", 240, 10, 40, 420, 160, 80));
	topPanel_ = (GLWVisiblePanel *)
		addWidget(new GLWVisiblePanel(10, 245, 420, 30));
}

BuyAccessoryDialog::~BuyAccessoryDialog()
{

}

void BuyAccessoryDialog::setupWindow()
{
	static float fVPort[4];
	glGetFloatv(GL_VIEWPORT, fVPort);
	float screenHeight = fVPort[3];
	float addition = 0;
	if (screenHeight > 340) addition = screenHeight - 340;
	if (addition > 200) addition = 200;

	setH(280 + addition);
	buyWeaponTab_->setH(160 + addition);
	sellTab_->setH(160 + addition);
	buyOtherTab_->setH(160 + addition);
	topPanel_->setY(240 + addition);

	needCentered_ = true;
}

void BuyAccessoryDialog::addPlayerName()
{
	topPanel_->clear();

	Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
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
	Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
	if (!tank) return;

	std::list<Accessory *> weapons;
	if (showWeapons) weapons = AccessoryStore::instance()->getAllWeapons();
	else weapons = AccessoryStore::instance()->getAllOthers();

	char buffer[256];
	int height = 10;

	std::vector<Accessory *> accVector;
	std::list<Accessory *>::iterator itor;
	for (itor = weapons.begin();
		itor != weapons.end();
		itor++)
	{	
		Accessory *current = (*itor);
		if (current->getPrice() <= tank->getScore().getMoney() &&
			(10-current->getArmsLevel()) <= OptionsGame::instance()->getMaxArmsLevel())
		{
			accVector.push_back(current);
		}
	}

	sort(accVector);

	std::vector<Accessory *>::iterator itor2;
	for (itor2 = accVector.begin();
		itor2 != accVector.end();
		itor2++)
	{
		Accessory *current = (*itor2);

		int currentNumber = 
			tank->getAccessories().getAccessoryCount(current);

		GLWVisiblePanel *newPanel = (GLWVisiblePanel *)
			tab->addWidget(new GLWVisiblePanel(10.0f, (float) height, 300.0f, 20.0f, true));
		sprintf(buffer, "%i", (currentNumber>=0?currentNumber:99));
		newPanel->addWidget(new GLWLabel(0, -2, buffer));
		newPanel->addWidget(new GLWLabel(30, -2, (char *) current->getName()));
		sprintf(buffer, "$%i/%i", current->getPrice(), current->getBundle());
		newPanel->addWidget(new GLWLabel(195, -2, buffer));

		if ((!current->singular() && currentNumber >= 0) || (current->singular() && currentNumber==0))
		{
			if (currentNumber < 90)
			{
				GLWidget *button = 
					newPanel->addWidget(new GLWTextButton("Buy", 310, 0, 60, this));
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
	std::list<std::pair<Accessory *, int> > tankAccessories;
	std::list<std::pair<Accessory *, int> >::iterator itor;

	Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
	if (!tank) return;

	tank->getAccessories().getAllAccessories(tankAccessories);
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		GLWVisiblePanel *newPanel = (GLWVisiblePanel *)
			sellTab_->addWidget(new GLWVisiblePanel(10.0f, (float) height, 300.0f, 20.0f, true));
		sprintf(buffer, "%i", (itor->second>=0?itor->second:99));
		newPanel->addWidget(new GLWLabel(0, -2, buffer));
		newPanel->addWidget(new GLWLabel(30, -2, (char *) (itor->first)->getName()));
		sprintf(buffer, "$%i/%i", (itor->first)->getSellPrice(), 1);
		newPanel->addWidget(new GLWLabel(195, -2, buffer));

		if (itor->second >= 0)
		{
			GLWidget *button =
				newPanel->addWidget(new GLWTextButton("Sell", 310, 0, 60, this));
			sellMap_[button->getId()] = itor->first;
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
	int roundsPlayed = OptionsGame::instance()->getNoRounds() -
		OptionsTransient::instance()->getNoRoundsLeft();
	if (OptionsGame::instance()->getBuyOnRound() - 1 >= roundsPlayed ||
		OptionsTransient::instance()->getCurrentGameNo() != 1)
	{
		finished();
	}
	else
	{
		currentPlayer_ = -1;
		nextPlayer();
	}
}

void BuyAccessoryDialog::nextPlayer()
{
	++currentPlayer_;
	Tank *tank = TankContainer::instance()->
		getTankByPos((unsigned int) currentPlayer_);

	if (!tank)
	{
		finished();
	}
	else
	{
		if (tank->getTankAI() && tank->getTankAI()->isHuman())
		{
			actualPlayer_ = tank->getPlayerId();
			buyWeaponTab_->setDepressed();
			playerRefresh();
			WindowManager::instance()->showWindow(getId());
		}
		else
		{
			nextPlayer();
		}
	}
}

void BuyAccessoryDialog::finished()
{
	WindowManager::instance()->hideWindow(getId());
	GameState::instance()->stimulate(ClientState::StimAutoDefense);
}

void BuyAccessoryDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		nextPlayer();
	}
	else
	{
		Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
		if (!tank) return;

		std::map<unsigned int, Accessory *>::iterator itor;
		itor = buyMap_.find(id);
		if (itor != buyMap_.end())
		{
			// Tell the server to add the accessory
			Accessory *acc = itor->second;
			if (OptionsParam::instance()->getConnectedToServer())
			{
				ComsBuyAccessoryMessage buyMessage(tank->getPlayerId(), acc->getName());
				ComsMessageSender::sendToServer(buyMessage);
			}

			// Add the accessory
			tank->getAccessories().add(acc);
			tank->getScore().setMoney(tank->getScore().getMoney() - acc->getPrice());
			playerRefresh();
		}
		else
		{
			itor = sellMap_.find(id);
			if (itor != sellMap_.end())
			{
				// Tell the server to add the accessory
				Accessory *acc = itor->second;
				if (OptionsParam::instance()->getConnectedToServer())
				{
					ComsBuyAccessoryMessage buyMessage(tank->getPlayerId(), acc->getName(), false);
					ComsMessageSender::sendToServer(buyMessage);
				}

				// Add the accessory
				tank->getAccessories().rm(acc);
				tank->getScore().setMoney(tank->getScore().getMoney() + acc->getSellPrice());
				playerRefresh();
			}
		}
	}
}

void BuyAccessoryDialog::sort(std::vector<Accessory *> &accVector)
{
	// Crudely sort by name
	// stl sort method list is broken in visual c 6
	// bubble sort
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0; i<int(accVector.size())-1; i++)
		{
			if (strcmp(accVector[i]->getName(), accVector[i+1]->getName())<0)
			{
				// swap
				Accessory *tmp = accVector[i];
				accVector[i] = accVector[i+1];
				accVector[i+1] = tmp;
				changed = true;
				break;
			}
		}
	} 
}
