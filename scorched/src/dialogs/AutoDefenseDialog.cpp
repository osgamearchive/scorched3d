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
#include <tankai/TankAIHuman.h>
#include <common/WindowManager.h>
#include <common/OptionsTransient.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <coms/ComsMessageSender.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>

AutoDefenseDialog::AutoDefenseDialog() :
	GLWWindow("Auto Defense", 10.0f, 10.0f, 440.0f, 280.0f, 0)
{
	needCentered_ = true;
	okId_ = addWidget(
		new GLWTextButton(" Ok", 375, 10, 55, this, true))->getId();

	topPanel_ = (GLWVisiblePanel *)
		addWidget(new GLWVisiblePanel(10, 245, 420, 30));
}

AutoDefenseDialog::~AutoDefenseDialog()
{

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

void AutoDefenseDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		finished();
	}
}

void AutoDefenseDialog::displayCurrent()
{
	WindowManager::instance()->showWindow(getId());
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	topPanel_->clear();

	// Put information at the top of the dialog
	char buffer[256];
	sprintf(buffer, "$%i", tank->getScore().getMoney());
	topPanel_->addWidget(new GLWFlag(tank->getColor(), 5, 5, 60));
	topPanel_->addWidget(new GLWLabel(70, 0, (char *) tank->getName()));
	topPanel_->addWidget(new GLWLabel(280, 0, buffer));

	// Put shields info
	GLWDropDown *ddshields = (GLWDropDown *) 
		topPanel_->addWidget(new GLWDropDown(120, -110, 300));
	ddshields->setHandler(this);
	shieldId_ = ddshields->getId();

	std::map<Shield*, int>::iterator shieldsItor;
	std::map<Shield*, int> &shields = 
		tank->getAccessories().getShields().getAllShields();
	ddshields->addText("Shields Off");
	for (shieldsItor = shields.begin();
		shieldsItor != shields.end();
		shieldsItor++)
	{
		Shield *shield = (*shieldsItor).first;
		ddshields->addText(shield->getName());
	}
	if (tank->getAccessories().getShields().getCurrentShield())
	{
		ddshields->setText(
			tank->getAccessories().getShields().getCurrentShield()->getName());
	}
	else
	{
		ddshields->setText("Shields Off");
	}


	// Put parachutes info
	GLWDropDown *ddpara = (GLWDropDown *) 
		topPanel_->addWidget(new GLWDropDown(120, -70, 300));
	ddpara->setHandler(this);
	paraId_ = ddpara->getId();

	ddpara->addText("Parachutes Off");
	if (tank->getAccessories().getParachutes().getNoParachutes() > 0)
	{
		ddpara->addText("Parachutes On");
	}
	if (tank->getAccessories().getParachutes().parachutesEnabled())
	{
		ddpara->setText("Parachutes On");
	}
	else
	{
		ddpara->setText("Parachutes Off");
	}
}

void AutoDefenseDialog::select(unsigned int id, 
							   const int pos, 
							   const char *value)
{
	Tank *tank = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!tank) return;

	if (id == paraId_)
	{
		((TankAIHuman *) tank->getTankAI())->parachutesUpDown(pos != 0);
		displayCurrent();
	}
	else if (id == shieldId_)
	{
		if (pos == 0)
		{
			((TankAIHuman *) tank->getTankAI())->shieldsUpDown(0);
		}
		else
		{
			((TankAIHuman *) tank->getTankAI())->shieldsUpDown(value);
		}
		displayCurrent();
	}
}

void AutoDefenseDialog::finished()
{
	// send message saying we are finished with shot
	ComsPlayedMoveMessage comsMessage(
		ScorchedClient::instance()->getTankContainer().getCurrentPlayerId(), 
		ComsPlayedMoveMessage::eFinishedBuy);

	// Check if we are running in a NET/LAN environment
	// If so we send this move to the server
	ComsMessageSender::sendToServer(comsMessage);

	WindowManager::instance()->hideWindow(getId());
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
}
