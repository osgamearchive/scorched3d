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
#include <tank/TankContainer.h>
#include <GLW/GLWFlag.h>
#include <GLW/GLWTextButton.h>
#include <common/WindowManager.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>

AutoDefenseDialog::AutoDefenseDialog() :
	GLWWindow("Auto Defense", 10.0f, 10.0f, 440.0f, 280.0f, 0),
	currentPlayer_(-1), actualPlayer_(0)
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
	currentPlayer_ = -1;
	if (OptionsTransient::instance()->getCurrentGameNo() != 1)
	{
		finished();
	}
	else
	{
		nextPlayer();
	}
}

void AutoDefenseDialog::nextPlayer()
{
	currentPlayer_++;
	Tank *current = TankContainer::instance()->getTankByPos(
		(unsigned int) currentPlayer_);
	if (!current) 
	{
		finished();
	}
	else
	{
		if (current->getTankAI() && current->getTankAI()->isHuman() &&
			current->getAccessories().getAutoDefense().haveDefense())
		{
			actualPlayer_ = current->getPlayerId();
			displayCurrent();
		}
		else
		{
			nextPlayer();
		}
	}
}

void AutoDefenseDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		if (OptionsParam::instance()->getConnectedToServer())
		{
			finished();
		}
		else
		{
			nextPlayer();
		}
	}
}

void AutoDefenseDialog::displayCurrent()
{
	WindowManager::instance()->showWindow(getId());
	Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
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
	Tank *tank = TankContainer::instance()->getTankById(actualPlayer_);
	if (!tank) return;

	if (id == paraId_)
	{
		tank->getTankAI()->parachutesUpDown(pos != 0);
		displayCurrent();
	}
	else if (id == shieldId_)
	{
		if (pos == 0)
		{
			tank->getTankAI()->shieldsUpDown(0);
		}
		else
		{
			tank->getTankAI()->shieldsUpDown(value);
		}
		displayCurrent();
	}
}

void AutoDefenseDialog::finished()
{
	WindowManager::instance()->hideWindow(getId());
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimMain);
}
