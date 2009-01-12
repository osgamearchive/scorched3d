////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#include <dialogs/InventoryDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWIcon.h>
#include <GLW/GLWFlag.h>
#include <GLW/GLWWindowManager.h>
#include <GLEXT/GLViewPort.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <common/OptionsScorched.h>
#include <graph/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankContainer.h>
#include <tank/TankScore.h>
#include <tank/TankAccessories.h>
#include <stdio.h>

InventoryDialog *InventoryDialog::instance_ = 0;

InventoryDialog *InventoryDialog::instance()
{
	if (!instance_) instance_ = new InventoryDialog;
	return instance_;
}

InventoryDialog::InventoryDialog() : 
	GLWWindow("Inventory", 10.0f, 10.0f, 440.0f, 300.0f, 0,
		"Show the current weapon inventory")
{
	okId_ = addWidget(new GLWTextButton(LANG_RESOURCE("OK", "Ok"), 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Inv", LANG_RESOURCE("INVENTORY_TAB", "Inv"), 10, 40, 420, 160));
	topPanel_ = (GLWPanel *)
		addWidget(new GLWPanel(10, 265, 420, 50));

	addWidget(new GLWLabel(15, 9, LANG_RESOURCE("SORT_LABEL", "Sort by:")));

	sortDropDown_ = (GLWDropDownText *) addWidget(new GLWDropDownText(100, 9, 100));

	sortDropDown_->addText(LANG_RESOURCE("SORT_NOTHING", "Nothing"), "Nothing");
	sortDropDown_->addText(LANG_RESOURCE("SORT_NAME", "Name"), "Name");
	sortDropDown_->addText(LANG_RESOURCE("SORT_PRICE", "Price"), "Price");
	sortDropDown_->setName("Sort");
	sortDropDown_->setHandler(this);
}

InventoryDialog::~InventoryDialog()
{

}

void InventoryDialog::select(unsigned int id, const int pos, GLWSelectorEntry value)
{
	if (id == sortDropDown_->getId())
	{
		OptionsDisplay *display = OptionsDisplay::instance();
		const char *dataText = value.getDataText();

		if (strcmp(dataText, "Name") == 0)
			display->getAccessorySortKeyEntry().setValue(AccessoryStore::SortName);
		else if (strcmp(dataText, "Price") == 0)
			display->getAccessorySortKeyEntry().setValue(AccessoryStore::SortPrice);
		else
			display->getAccessorySortKeyEntry().setValue(AccessoryStore::SortNothing);

		playerRefresh();
	}
}

void InventoryDialog::display()
{
	sortDropDown_->setHandler(0);

	switch (OptionsDisplay::instance()->getAccessorySortKey())
	{
	case AccessoryStore::SortName:
		sortDropDown_->setCurrentText(LANG_RESOURCE("SORT_NAME", "Name"));
		break;

	case AccessoryStore::SortPrice:
		sortDropDown_->setCurrentText(LANG_RESOURCE("SORT_PRICE", "Price"));
		break;

	case AccessoryStore::SortNothing:
		sortDropDown_->setCurrentText(LANG_RESOURCE("SORT_NOTHING", "Nothing"));
		break;
	}

	sortDropDown_->setHandler(this);

	playerRefresh();
}

void InventoryDialog::setupWindow()
{
	float screenHeight = (float) GLViewPort::getHeight();
	float addition = 0;
	if (screenHeight > 340) addition = screenHeight - 340;
	if (addition > 200) addition = 200;

	setH(300 + addition);
	sellTab_->setH(160 + addition);
	topPanel_->setY(240 + addition);

	needCentered_ = true;
}

void InventoryDialog::addPlayerName()
{
	topPanel_->clear();

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	topPanel_->addWidget(new GLWFlag(tank->getColor(), 5, 15, 60));
	topPanel_->addWidget(new GLWLabel(75, 10, tank->getTargetName()));
	topPanel_->addWidget(new GLWLabel(260, 20, 
		LANG_STRING(S3D::formatStringBuffer("$%i", tank->getScore().getMoney()))));
	topPanel_->addWidget(new GLWLabel(260, 0,
		LANG_RESOURCE_2("ROUND_OF", "Round {0} of {1}",
		S3D::formatStringBuffer("%i", ScorchedClient::instance()->getOptionsTransient().getCurrentRoundNo()),
		S3D::formatStringBuffer("%i", ScorchedClient::instance()->getOptionsGame().getNoRounds()))));
}

void InventoryDialog::addPlayerWeapons()
{
	sellTab_->clear();

	int height = 10;

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	std::list<Accessory *> tankAccessories;
	tank->getAccessories().getAllAccessories(tankAccessories);
	ScorchedClient::instance()->getAccessoryStore().sortList(tankAccessories, 
		OptionsDisplay::instance()->getAccessorySortKey());

	std::list<Accessory *>::reverse_iterator itor;
	for (itor = tankAccessories.rbegin();
		itor != tankAccessories.rend();
		itor++)
	{
		Accessory *current = *itor;
		int count = tank->getAccessories().getAccessoryCount(current);

		GLWPanel *newPanel = (GLWPanel *)
			sellTab_->addWidget(new GLWPanel(10.0f, (float) height, 315.0f, 20.0f, true));
		newPanel->setToolTip(&current->getToolTip());
		newPanel->addWidget(new GLWLabel(0, -2, tank->getAccessories().getAccessoryCountString(current)));
		newPanel->addWidget(new GLWIcon(30, 2, 16, 16, current->getTexture()));
		newPanel->addWidget(new GLWLabel(50, -2, LANG_RESOURCE(current->getName(), current->getName())));
		newPanel->addWidget(new GLWLabel(205, -2, 
			LANG_STRING(S3D::formatStringBuffer("$%i/%i", current->getSellPrice(), 1))));

		height += 24;
	}
}

void InventoryDialog::playerRefresh()
{
	setupWindow();
	addPlayerName();
	addPlayerWeapons();
}

void InventoryDialog::windowInit(const unsigned state)
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (tank)
	{
		playerRefresh();
	}
}

void InventoryDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		GLWWindowManager::instance()->hideWindow(getId());
	}
}
