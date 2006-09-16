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

#include <dialogs/InventoryDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWIcon.h>
#include <GLW/GLWFlag.h>
#include <GLW/GLWWindowManager.h>
#include <GLEXT/GLViewPort.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <common/OptionsGame.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankContainer.h>
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
	okId_ = addWidget(new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	sellTab_ = (GLWTab *)
		addWidget(new GLWTab("Inventory", 10, 40, 420, 160));
	topPanel_ = (GLWPanel *)
		addWidget(new GLWPanel(10, 265, 420, 50));
	sortBox_ = (GLWCheckBox *) addWidget(new GLWCheckBox(10, 10));
	sortBox_->setHandler(this);
	addWidget(new GLWLabel(35, 7, "Sort accessories by name"));
}

InventoryDialog::~InventoryDialog()
{

}

void InventoryDialog::stateChange(bool state, unsigned int id)
{
	OptionsDisplay::instance()->getSortAccessoriesEntry().setValue(state);
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

	sortBox_->setState(OptionsDisplay::instance()->getSortAccessories());
}

void InventoryDialog::addPlayerName()
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

void InventoryDialog::addPlayerWeapons()
{
	sellTab_->clear();

	char buffer[256];
	int height = 10;

	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	std::list<Accessory *> tankAccessories;
	tank->getAccessories().getAllAccessories(tankAccessories);
	ScorchedClient::instance()->getAccessoryStore().sortList(tankAccessories, 
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
		if (count >= 0) snprintf(buffer, 256, "%i", count);
		else snprintf(buffer, 256, "In");
		newPanel->addWidget(new GLWLabel(0, -2, buffer));
		newPanel->addWidget(new GLWIcon(30, 2, 16, 16, current->getTexture()));
		newPanel->addWidget(new GLWLabel(50, -2, (char *) current->getName()));
		snprintf(buffer, 256, "$%i/%i", current->getSellPrice(), 1);
		newPanel->addWidget(new GLWLabel(205, -2, buffer));

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

