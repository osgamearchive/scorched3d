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

#include <tankgraph/TankModelStore.h>
#include <dialogs/PlayerDialog.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIStrings.h>
#include <common/WindowManager.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <GLW/GLWTextButton.h>
#include <coms/ComsAddPlayerMessage.h>
#include <coms/ComsMessageSender.h>
#include <stdio.h>

PlayerDialog *PlayerDialog::instance_ = 0;

PlayerDialog *PlayerDialog::instance()
{
	if (!instance_)
	{
		instance_ = new PlayerDialog;
	}
	return instance_;
}

PlayerDialog::PlayerDialog() : 
	GLWWindow("Team", 10.0f, 10.0f, 440.0f, 300.0f, eSmallTitle),
	allocatedTeam_(0), cancelId_(0)
{
	needCentered_ = true;
	viewer_ = new GLWTankViewer(10.0f, 25.0f, 4, 3);

	// Add buttons
	okId_ = addWidget(new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();
	if (OptionsParam::instance()->getConnectedToServer())
	{
		cancelId_ = addWidget(new GLWTextButton("Cancel", 280, 10, 85, this, 
			GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
	}
	addWidget(viewer_);

	// Create player name choice
	playerName_ = (GLWTextBox *) addWidget(new GLWTextBox(80, 265, 340, "Player"));
	playerName_->setMaxTextLen(22);
	addWidget(new GLWLabel(10, 265, "Name:"));

	// Create team choice
	addWidget(new GLWLabel(250, 235, "Team:"));
	teamDropDown_ = (GLWDropDown *) addWidget(new GLWDropDown(320, 235, 110));

	// Create computer type choice
	addWidget(new GLWLabel(10, 235, "Type:"));
	typeDropDown_ = (GLWDropDown *) addWidget(new GLWDropDown(80, 235, 110));

}

PlayerDialog::~PlayerDialog()
{
}

void PlayerDialog::draw()
{
	if (ScorchedClient::instance()->getOptionsGame().getTeams() != 1)
	{
		// Auto select the team with the least players
		unsigned int newTeam = 
			ScorchedClient::instance()->getOptionsTransient().getLeastUsedTeam(
			ScorchedClient::instance()->getTankContainer());
		if (newTeam != allocatedTeam_)
		{
			teamDropDown_->setCurrentPosition(newTeam - 1);
			allocatedTeam_ = newTeam;
		}
	}
	GLWWindow::draw();
}

void PlayerDialog::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	GLWWindow::keyDown(buffer, keyState, history, hisCount, skipRest);
	skipRest = true;
}

void PlayerDialog::windowDisplay()
{
	// Add teams
	teamDropDown_->clear();
	if (ScorchedClient::instance()->getOptionsGame().getTeams() == 1)
	{
		teamDropDown_->addText("None");
	}
	else
	{
		for (int i=1; i<=ScorchedClient::instance()->getOptionsGame().getTeams(); i++)
		{
			char buffer[10];
			sprintf(buffer, "Team %i", i);
			teamDropDown_->addText(buffer);
		}	
	}

	// Add player types
	typeDropDown_->clear();
	typeDropDown_->addText("Human");
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		std::list<TankAIComputer *>::iterator aiitor;
		for (aiitor = TankAIStore::instance()->getAis().begin();
			aiitor != TankAIStore::instance()->getAis().end();
			aiitor++)
		{
			typeDropDown_->addText((*aiitor)->getName());
		}
	}
	currentPlayerId_ = 0;
	nextPlayer();
}

void PlayerDialog::nextPlayer()
{
	allocatedTeam_ = 0;
	currentPlayerId_ = getNextPlayer(currentPlayerId_);
	if (currentPlayerId_ == 0)
	{
		WindowManager::instance()->hideWindow(getId());
	}
	else
	{
		Tank *tank = 
			ScorchedClient::instance()->getTankContainer().getTankById(currentPlayerId_);
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If we are connected online then use the online name
			playerName_->setText(OptionsDisplay::instance()->getOnlineUserName());
		}
		else
		{
			// Else use the default names
			if (tank) playerName_->setText(tank->getName());
		}
	}
}

unsigned int PlayerDialog::getNextPlayer(unsigned int current)
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		if ((tank->getDestinationId() == 
			ScorchedClient::instance()->getTankContainer().getCurrentDestinationId()) &&
			(tank->getPlayerId() != 1) &&
			(tank->getState().getState() != TankState::sNormal))
		{
			if (current == 0)
			{
				return tank->getPlayerId();
			}
			else if (tank->getPlayerId() == current) current = 0;
		}
	}
	return 0;
}

void PlayerDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		if (!playerName_->getText().empty())
		{
			// If we are connected online save this players name
			if (OptionsParam::instance()->getConnectedToServer())
			{
				OptionsDisplay::instance()->setOnlineUserName(playerName_->getText().c_str());
			}

			// Get the model type (turns a "Random" choice into a proper name)
			TankModel *model = 
				TankModelStore::instance()->getModelByName(viewer_->getModelName());

			// Get the player type
			const char *playerType = typeDropDown_->getText();

			// Add this player
			ComsAddPlayerMessage message(currentPlayerId_,
				playerName_->getText().c_str(),
				Vector(),
				model->getId().getModelName(),
				ScorchedClient::instance()->getTankContainer().getCurrentDestinationId(),
				((ScorchedClient::instance()->getOptionsGame().getTeams() > 1)?
				teamDropDown_->getCurrentPosition() + 1:0),
				playerType);
			ComsMessageSender::sendToServer(message);

			nextPlayer();
		}
	}
	else if (id == cancelId_)
	{
		WindowManager::instance()->hideWindow(getId());
	}
}
