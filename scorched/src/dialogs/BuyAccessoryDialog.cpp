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
#include <common/Defines.h>
#include <coms/ComsMessageSender.h>
#include <coms/ComsBuyAccessoryMessage.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankContainer.h>
#include <stdio.h>

BuyAccessoryDialog::BuyAccessoryDialog() : 
	GLWWindow("", 10.0f, 10.0f, 465.0f, 300.0f, 0,
		"Allows the current player to buy and sell\n"
		"weapons and other accessories."),
	firstDrawTime_(true)
{
	okId_ = addWidget(new GLWTextButton("Ok", 400, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	buyWeaponTab_ = (GLWTab *)
		addWidget(new GLWTab("Weapons", 10, 40, 450, 160));
	buyOtherTab_ = (GLWTab *)
		addWidget(new GLWTab("Defense", 10, 40, 450, 160));
	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Inv", 10, 40, 450, 160));
	favouritesTab_ = (GLWTab *)
		addWidget(new GLWTab("Favorites", 10, 40, 450, 160));
	topPanel_ = (GLWPanel *)
		addWidget(new GLWPanel(10, 265, 450, 50));

	buyWeaponTab_->setHandler(this);
	buyOtherTab_->setHandler(this);
	sellTab_->setHandler(this);
	favouritesTab_->setHandler(this);

	defaultTab_ = (GLWCheckBoxText *) 
		addWidget(new GLWCheckBoxText(18.0f, 230.0f, "Default Tab", false, 3.0f));
	defaultTab_->getCheckBox().setW(12);
	defaultTab_->getCheckBox().setH(12);
	defaultTab_->getLabel().setSize(10);
	defaultTab_->getCheckBox().setHandler(this);

	sortBox_ = (GLWCheckBox *) addWidget(new GLWCheckBox(10, 14));
	sortBox_->setHandler(this);
	sortBox_->setW(14);
	sortBox_->setH(14);
	GLWLabel *label = (GLWLabel *)
		addWidget(new GLWLabel(30, 9, "Sort by name"));
	label->setSize(12);
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
		buyWeaponTab_->setH(165 + addition);
		sellTab_->setH(165 + addition);
		buyOtherTab_->setH(165 + addition);
		favouritesTab_->setH(165 + addition);
		topPanel_->setY(240 + addition);
		defaultTab_->setY(190 + addition);

		needCentered_ = true;
	}

	GLWWindow::draw();

	Vector red(0.7f, 0.0f, 0.0f);
	Vector green(0.0f, 0.4f, 0.0f);
	GLWFont::instance()->getLargePtFont()->draw(
		green, 12.0f, x_ + 260.0f, y_ + topPanel_->getY() - 50.0f, 0.0f, "Buy");
	GLWFont::instance()->getLargePtFont()->draw(
		red, 12.0f, x_ + 360.0f, y_ + topPanel_->getY() - 50.0f, 0.0f, "Sell");
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

void BuyAccessoryDialog::playerRefresh()
{
	addPlayerName();
	addPlayerWeapons();
}

void BuyAccessoryDialog::playerRefreshKeepPos()
{
	int buyCurrent = buyWeaponTab_->getScrollBar().getCurrent();
	int otherCurrent = buyOtherTab_->getScrollBar().getCurrent();
	int sellCurrent = sellTab_->getScrollBar().getCurrent();
	int favCurrent = favouritesTab_->getScrollBar().getCurrent();
	addPlayerName();
	addPlayerWeapons();
	buyWeaponTab_->getScrollBar().setCurrent(buyCurrent);
	buyOtherTab_->getScrollBar().setCurrent(otherCurrent);
	sellTab_->getScrollBar().setCurrent(sellCurrent);
	favouritesTab_->getScrollBar().setCurrent(favCurrent);
}

void BuyAccessoryDialog::addPlayerWeapons()
{
	buyMap_.clear();
	sellMap_.clear();
	favMap_.clear();

	favouritesTab_->clear();
	sellTab_->clear();
	buyWeaponTab_->clear();
	buyOtherTab_->clear();

	addPlayerFavorites();
	addPlayerWeaponsSell();
	addPlayerWeaponsBuy(buyWeaponTab_, true);
	addPlayerWeaponsBuy(buyOtherTab_, false);
}

void BuyAccessoryDialog::addPlayerFavorites()
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	float height = 10.0f;
	std::list<Accessory *> acessories = 
		ScorchedClient::instance()->
			getAccessoryStore().getAllAccessories(
				OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = acessories.begin();
		itor != acessories.end();
		itor++)
	{
		Accessory *current = *itor;
		if (favorites_.find(current->getName()) != favorites_.end())
		{
			if (addAccessory(tank, favouritesTab_, height, current)) height += 24.0f;
		}
	}
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
		accessories.push_back(current);
	}

	float height = 10.0f;
	std::list<Accessory *>::iterator itor2;
	for (itor2 = accessories.begin();
		itor2 != accessories.end();
		itor2++)
	{
		Accessory *current = (*itor2);
		if (addAccessory(tank, tab, height, current)) height += 24.0f;
	}
}

void BuyAccessoryDialog::addPlayerWeaponsSell()
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	float height = 10.0f;
	std::list<Accessory *> tankAccessories;
	tank->getAccessories().getAllAccessories(
		tankAccessories,
		OptionsDisplay::instance()->getSortAccessories());
	std::list<Accessory *>::iterator itor;
	for (itor = tankAccessories.begin();
		itor != tankAccessories.end();
		itor++)
	{
		Accessory *current = *itor;
		if (addAccessory(tank, sellTab_, height, current)) height += 24.0f;
	}
}

bool BuyAccessoryDialog::addAccessory(
	Tank *tank, GLWTab *tab, 
	float height, Accessory *current)
{
	if (current->getMaximumNumber() == 0) return false;

	int currentNumber = 
		tank->getAccessories().getAccessoryCount(current);

	// Panel
	GLWPanel *newPanel = (GLWPanel *)
		tab->addWidget(new GLWPanel(5.0f, (float) height, 415.0f, 22.0f, true));
	newPanel->setToolTip(&current->getToolTip());

	// Favorites checkbox
	GLWCheckBox *sortBox = (GLWCheckBox *) newPanel->addWidget(new GLWCheckBox(2, 4, false));
	sortBox->setHandler(this);
	sortBox->setW(14);
	sortBox->setH(14);
	sortBox->setToolTip(new GLWTip("Favorite Weapon",
		"Set/unset this weapon as a favorite.\n"
		"Favorite weapons will show in the\n"
		"favorites tab."));
	sortBox->setState(favorites_.find(current->getName()) != favorites_.end());
	favMap_[sortBox->getId()] = current;
	
	// Others
	newPanel->addWidget(new GLWLabel(20, 0, (char *)
		formatString((currentNumber == -1?"In":"%i"), currentNumber), 12.0f));
	newPanel->addWidget(new GLWIcon(45, 4, 16, 16, current->getTexture()));
	newPanel->addWidget(new GLWLabel(65, 0, (char *) current->getName(), 12.0f));

	// Buy Button
	if (tank->getAccessories().accessoryAllowed(current, current->getBundle()) && 
		current->getPrice() <= tank->getScore().getMoney())
	{
		GLWTextButton *button = (GLWTextButton *)
			newPanel->addWidget(new GLWTextButton(
				(char *) formatString("$%i/%i",
					current->getPrice(), current->getBundle()), 
					210, 2, 100, this, 
			GLWButton::ButtonFlagCenterX, 12.0f));
		button->setColor(Vector(0.0f, 0.4f, 0.0f));
		button->setToolTip(new GLWTip("Buy", 
			formatString("Buy %i %s(s) for $%i", 
				current->getBundle(), current->getName(), current->getPrice())));
		button->setH(button->getH() - 2.0f);
		buyMap_[button->getId()] = current;
	}
	else
	{
		char *text = (char *) formatString("$%i/%i",
			current->getPrice(), current->getBundle());
		GLWLabel *label = (GLWLabel *)
			newPanel->addWidget(new GLWLabel(
				260, 0, text, 12.0f));
		label->setX(label->getX() - label->getW() / 2);
		label->setColor(Vector(0.4f, 0.4f, 0.4f));
	}

	// Sell Button
	if (currentNumber > 0)
	{
		GLWTextButton *button = (GLWTextButton *)
			newPanel->addWidget(new GLWTextButton(
				(char *) formatString("$%i/%i",
					current->getSellPrice(), 1), 
					312, 2, 100, this,
			GLWButton::ButtonFlagCenterX, 12.0f));
		button->setColor(Vector(0.7f, 0.0f, 0.0f));
		button->setToolTip(new GLWTip("Sell", 
			formatString("Sell 1 %s for $%i", 
				current->getName(), current->getSellPrice())));
		button->setH(button->getH() - 2.0f);
		sellMap_[button->getId()] = current;
	}

	return true;
}

void BuyAccessoryDialog::windowInit(const unsigned state)
{
	loadFavorites();

	sortBox_->setState(OptionsDisplay::instance()->getSortAccessories());
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (tank)
	{
		buyWeaponTab_->setDepressed();
		const char *buyTab = OptionsDisplay::instance()->getBuyTab();
		std::list<GLWPanel::GLWPanelEntry>::iterator itor;
		for (itor = getWidgets().begin();
			itor != getWidgets().end();
			itor++)
		{
			GLWPanel::GLWPanelEntry &entry = (*itor);
			if (entry.widget->getMetaClassId() == buyWeaponTab_->getMetaClassId())
			{
				GLWTab *tab = (GLWTab *) entry.widget;
				if (0 == strcmp(buyTab, tab->getName()))
				{
					tab->setDepressed();
					break;
				}
			}
		}
		tabDown(0);
	
		playerRefresh();
	}
}

void BuyAccessoryDialog::tabDown(unsigned int id)
{
	const char *buyTab = OptionsDisplay::instance()->getBuyTab();
	std::list<GLWPanel::GLWPanelEntry>::iterator itor;
	for (itor = getWidgets().begin();
		itor != getWidgets().end();
		itor++)
	{
		GLWPanel::GLWPanelEntry &entry = (*itor);
		if (entry.widget->getMetaClassId() == buyWeaponTab_->getMetaClassId())
		{
			GLWTab *tab = (GLWTab *) entry.widget;
			if (tab->getDepressed())
			{
				if (0 == strcmp(buyTab, tab->getName()))
				{
					defaultTab_->getCheckBox().setState(true);
				}
				else
				{
					defaultTab_->getCheckBox().setState(false);
				}
				break;
			}
		}
	}
}

void BuyAccessoryDialog::stateChange(bool state, unsigned int id)
{
	if (id == sortBox_->getId())
	{
		// The sort accessories check box has been clicked
		OptionsDisplay::instance()->getSortAccessoriesEntry().setValue(state);
		playerRefreshKeepPos();
	}
	else if (id == defaultTab_->getCheckBox().getId())
	{
		if (defaultTab_->getCheckBox().getState())
		{
			std::list<GLWPanel::GLWPanelEntry>::iterator itor;
			for (itor = getWidgets().begin();
				itor != getWidgets().end();
				itor++)
			{
				GLWPanel::GLWPanelEntry &entry = (*itor);
				if (entry.widget->getMetaClassId() == buyWeaponTab_->getMetaClassId())
				{
					GLWTab *tab = (GLWTab *) entry.widget;
					if (tab->getDepressed())
					{
						OptionsDisplay::instance()->getBuyTabEntry().setValue(tab->getName());
						break;
					}
				}
			}
		}
		else
		{
			OptionsDisplay::instance()->getBuyTabEntry().setValue("");
		}
	}
	else
	{
		// The favorites check box has been clicked
		std::map<unsigned int, Accessory *>::iterator findItor
			= favMap_.find(id);
		if (findItor != favMap_.end())
		{
			if (state)
			{
				favorites_.insert((*findItor).second->getName());
			}
			else
			{
				favorites_.erase((*findItor).second->getName());
			}
		}
		playerRefreshKeepPos();
	}
}

void BuyAccessoryDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		saveFavorites();
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
			tank->getAccessories().add(acc, acc->getBundle());
			tank->getScore().setMoney(tank->getScore().getMoney() - acc->getPrice());

			// Refresh the window
			playerRefreshKeepPos();
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
				playerRefreshKeepPos();
			}
		}
	}
}

void BuyAccessoryDialog::loadFavorites()
{
	const char *filename = 
		getSettingsFile(formatString("weaponfavorites-%s.xml", 
			ScorchedClient::instance()->getOptionsGame().getMod()));

	favorites_.clear();
	XMLFile file;
	if (!file.readFile(filename))
	{
		dialogMessage("BuyAccessoryDialog", formatString(
					  "Failed to parse \"%s\"\n%s", 
					  filename,
					  file.getParserError()));
		return;
	}
	if (!file.getRootNode()) return; // Empty File

	std::string accessory;
	while (file.getRootNode()->getNamedChild("accessory", accessory, false))
	{
		favorites_.insert(accessory);
	}
}

void BuyAccessoryDialog::saveFavorites()
{
	const char *filename = 
		getSettingsFile(formatString("weaponfavorites-%s.xml", 
			ScorchedClient::instance()->getOptionsGame().getMod()));

	XMLNode node("accessories");
	std::set<std::string>::iterator itor;
	for (itor = favorites_.begin();
		itor != favorites_.end();
		itor++)
	{
		std::string accessory = *itor;

		XMLNode *accessoryNode = 
			new XMLNode("accessory", accessory.c_str());
		node.addChild(accessoryNode);
	}
	node.writeToFile(filename);
}
