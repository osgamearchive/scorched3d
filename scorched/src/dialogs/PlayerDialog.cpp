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
#include <common/OptionsParam.h>
#include <common/WindowManager.h>
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
	GLWWindow("", 10.0f, 10.0f, 440.0f, 270.0f, 0),
	dropDown_(0), beenShown_(false)
{
	needCentered_ = true;
	viewer_ = new GLWTankViewer(10.0f, 25.0f, 4, 3);

	// Add buttons
	addWidget(viewer_);
	okId_ = addWidget(new GLWTextButton(" Ok", 365, 10, 55, this, true))->getId();

	// Create player name choice
	playerName_ = (GLWTextBox *) addWidget(new GLWTextBox(70, 235, 240, "Player"));
	addWidget(new GLWLabel(10, 235, "Name:"));

	if (!OptionsParam::instance()->getConnectedToServer())
	{
		// Create computer type choice
		dropDown_ = (GLWDropDown *) addWidget(new GLWDropDown(320, 235, 110));
		dropDown_->addText("Human");
		std::list<TankAIComputer *>::iterator aiitor;
		for (aiitor = TankAIStore::instance()->getAis().begin();
			aiitor != TankAIStore::instance()->getAis().end();
			aiitor++)
		{
			dropDown_->addText((*aiitor)->getName());
		}
	}
}

PlayerDialog::~PlayerDialog()
{
}

void PlayerDialog::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	GLWWindow::keyDown(buffer, keyState, history, hisCount, skipRest);
	skipRest = true;
}

void PlayerDialog::addPlayers()
{
	if (beenShown_) return;
	beenShown_ = true;

	const char *playerModel = 
		OptionsParam::instance()->getSkin();
	const char *playerName =
		OptionsParam::instance()->getName();

	currentPlayerId_ = 0;
	nextPlayer();

	if (playerModel[0])
	{
		viewer_->selectModelByName(playerModel);
	}
	if (playerName[0])
	{
		playerName_->setText(playerName);
		buttonDown(okId_);
	}
}

void PlayerDialog::nextPlayer()
{
	currentPlayerId_ = getNextPlayer(currentPlayerId_);
	if (currentPlayerId_ == 0)
	{
		WindowManager::instance()->hideWindow(getId());
	}
	else
	{
		playerName_->setText(TankAIStrings::instance()->getPlayerName());
		WindowManager::instance()->showWindow(getId());
		Tank *tank = 
			ScorchedClient::instance()->getTankContainer().getTankById(currentPlayerId_);
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
		if (tank->getDestinationId() == 
			ScorchedClient::instance()->getTankContainer().getCurrentDestinationId() &&
			tank->getState().getSpectator())
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
			// Get the model type (turns a "Random" choice into a proper name)
			TankModel *model = 
				TankModelStore::instance()->getModelByName(viewer_->getModelName());

			// Get the player type
			const char *playerType = "Human";
			if (dropDown_) playerType = dropDown_->getText();

			// Add this player
			ComsAddPlayerMessage message(currentPlayerId_,
				playerName_->getText().c_str(),
				Vector(),
				model->getId().getModelName(),
				ScorchedClient::instance()->getTankContainer().getCurrentDestinationId(),
				playerType);
			ComsMessageSender::sendToServer(message);

			nextPlayer();
		}
	}
}
