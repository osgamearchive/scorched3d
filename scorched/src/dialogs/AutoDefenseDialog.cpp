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

#include <dialogs/AutoDefenseDialog.h>
#include <GLW/GLWFlag.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWWindowManager.h>
#include <tankai/TankAIHuman.h>
#include <tankai/TankAIHumanCtrl.h>
#include <common/OptionsTransient.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <coms/ComsMessageSender.h>
#include <client/ClientState.h>
#include <client/ClientDefenseHandler.h>
#include <client/ScorchedClient.h>

AutoDefenseDialog::AutoDefenseDialog() :
	GLWWindow("Auto Defense", 10.0f, 10.0f, 440.0f, 280.0f, 0,
		"Allows the current player to raise and\n"
		"lower defenses before the round starts")
{
	needCentered_ = true;
	okId_ = addWidget(
		new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();
	cancelId_ = addWidget(
		new GLWTextButton("Cancel", 265, 10, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
	topPanel_ = (GLWVisiblePanel *)
		addWidget(new GLWVisiblePanel(10, 245, 420, 30));

	ddpara_ = (GLWDropDown *) addWidget(new GLWDropDown(120, 170, 300));
	ddpara_->setHandler(this);
	ddshields_ = (GLWDropDown *) addWidget(new GLWDropDown(120, 200, 300));
	ddshields_->setHandler(this);
}

AutoDefenseDialog::~AutoDefenseDialog()
{

}

void AutoDefenseDialog::draw()
{
	// Check if the items need updating
	if (messageCount_ != ClientDefenseHandler::instance()->getLocalTankMessageCount())
	{
		displayCurrent();
	}

	GLWWindow::draw();
}

void AutoDefenseDialog::windowInit(const unsigned state)
{
	Tank *current = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!current) 
	{
		finished();
	}
	else
	{
		if (current->getDestinationId() == 
			ScorchedClient::instance()->getTankContainer().getCurrentDestinationId() &&
			current->getAccessories().getAutoDefense().haveDefense())
		{
			displayCurrent();
		}
		else
		{
			finished();
		}
	}
}

void AutoDefenseDialog::buttonDown(unsigned int butid)
{
	if (butid == okId_)
	{
		TankAIHumanCtrl::instance()->setTankAI();
		Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
		if (tank && tank->getTankAI())
		{
			// Set Parachutes on/off
			((TankAIHuman *) tank->getTankAI())->parachutesUpDown(ddpara_->getCurrentPosition() != 0);

			// Set shields on/off
			if (ddshields_->getCurrentPosition() == 0)
			{
				((TankAIHuman *) tank->getTankAI())->shieldsUpDown(0);
			}
			else
			{
				std::map<Shield*, int> &shields = 
					tank->getAccessories().getShields().getAllShields();
				std::map<Shield*, int>::iterator shieldsItor = shields.begin();
				for (int i=1; i<ddshields_->getCurrentPosition() && shieldsItor != shields.end(); i++) shieldsItor++;
				
				if (shieldsItor != shields.end())
				{
					((TankAIHuman *) tank->getTankAI())->shieldsUpDown(
						(*shieldsItor).first->getAccessoryId());
				}
			}
		}


		finished();
	}
	else if (butid == cancelId_)
	{
		finished();
	}
}

void AutoDefenseDialog::displayCurrent()
{
	GLWWindowManager::instance()->showWindow(getId());
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	// Put information at the top of the dialog
	char buffer[256];
	sprintf(buffer, "$%i", tank->getScore().getMoney());
	topPanel_->clear();
	topPanel_->addWidget(new GLWFlag(tank->getColor(), 5, 5, 60));
	topPanel_->addWidget(new GLWLabel(70, 0, (char *) tank->getName()));
	topPanel_->addWidget(new GLWLabel(280, 0, buffer));

	// Put shields info
	ddshields_->clear();
	std::map<Shield*, int>::iterator shieldsItor;
	std::map<Shield*, int> &shields = 
		tank->getAccessories().getShields().getAllShields();
	ddshields_->addText("Shields Off");
	for (shieldsItor = shields.begin();
		shieldsItor != shields.end();
		shieldsItor++)
	{
		Shield *shield = (*shieldsItor).first;
		int shieldcount = (*shieldsItor).second;
		char buffer[256];
		sprintf(buffer, "%s (%i)",
			shield->getName(),
			shieldcount);
		ddshields_->addText(buffer);
	}

	// Put parachutes info
	ddpara_->clear();
	ddpara_->addText("Parachutes Off");
	if (tank->getAccessories().getParachutes().getNoParachutes() > 0)
	{
		char buffer[256];
		sprintf(buffer, "Parachutes On (%i)",
			tank->getAccessories().getParachutes().getNoParachutes());
		ddpara_->addText(buffer);
	}

	// Set the currently shown items
	Shield *currentShield = tank->getAccessories().getShields().getCurrentShield();
	if (currentShield)
	{
		char buffer[256];
		sprintf(buffer, "%s (%i)",
			currentShield->getName(),
			tank->getAccessories().getShields().getShieldCount(currentShield));
		ddshields_->setText(buffer);
	}
	else
	{
		ddshields_->setText("Shields Off");
	}
	if (tank->getAccessories().getParachutes().parachutesEnabled())
	{
		char buffer[256];
		sprintf(buffer, "Parachutes On (%i)",
			tank->getAccessories().getParachutes().getNoParachutes());
		ddpara_->setText(buffer);
	}
	else
	{
		ddpara_->setText("Parachutes Off");
	}

	messageCount_ = ClientDefenseHandler::instance()->getLocalTankMessageCount();
}

void AutoDefenseDialog::select(unsigned int id, 
							   const int pos, 
							   const char *value)
{
	// Nothing to do as we don't actualy set the status
	// until the ok button is pressed
}

void AutoDefenseDialog::finished()
{
	// send message saying we are finished with shot
	ComsPlayedMoveMessage comsMessage(
		ScorchedClient::instance()->getTankContainer().getCurrentPlayerId(), 
		ComsPlayedMoveMessage::eFinishedBuy);
	ComsMessageSender::sendToServer(comsMessage);

	GLWWindowManager::instance()->hideWindow(getId());
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}
