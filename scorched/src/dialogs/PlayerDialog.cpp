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
#include <tank/TankContainer.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>
#include <GLW/GLWWindowManager.h>
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
	GLWWindow("Team", 10.0f, 10.0f, 465.0f, 340.0f, eSmallTitle,
		"Allows the player to make changes to their\n"
		"name, their tank and to change teams."),
	allocatedTeam_(0), cancelId_(0), viewer_(0)
{
	needCentered_ = true;

	// Add buttons
	okId_ = addWidget(new GLWTextButton("Ok", 395, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();
	if (OptionsParam::instance()->getConnectedToServer())
	{
		cancelId_ = addWidget(new GLWTextButton("Cancel", 300, 10, 85, this, 
			GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
	}

	GLWPanel *infoPanel = new GLWPanel(10.0f, 260.0f, 445.0f, 70.0f,
		false, true, true);
	addWidget(infoPanel);

	// Create players avatar choice
	avatarTip1_.setText("Avatar", 
		"The current player's avatar.\n"
		"Click to change.\n");
	avatarTip2_.setText("Avatar", 
		"The current player's avatar.\n"
		"CANNOT be changed while playing,\n"
		"you must quit to change.");
	imageList_ = new GLWImageList(10.0f, 20.0f, getDataFile("data/avatars"));
	imageList_->setCurrent("player.gif");
	imageList_->setToolTip(&avatarTip1_);
	infoPanel->addWidget(imageList_);

	// Create player name choice
	GLWTip *nameTip = new GLWTip("Player Name",
		"The name of this player.\n"
		"Use the backspace or delete key to remove this name.\n"
		"Type in a new player name via the keyboad to change.");
	GLWLabel *nameLabel = (GLWLabel *) 
		infoPanel->addWidget(new GLWLabel(50, 40, "Name:"));
	nameLabel->setToolTip(nameTip);
	playerName_ = (GLWTextBox *) 
		infoPanel->addWidget(new GLWTextBox(120, 40, 320, "Player"));
	playerName_->setMaxTextLen(22);
	playerName_->setToolTip(nameTip);
	
	// Create team choice
	GLWTip *teamTip = new GLWTip("Team Selection",
		"Change the team this player will join.\n"
		"This is only available when playing team games.");
	GLWLabel *teamLabel = (GLWLabel *) 
		infoPanel->addWidget(new GLWLabel(250, 5, "Team:"));
	teamLabel->setToolTip(teamTip);
	teamDropDown_ = (GLWDropDown *) 
		infoPanel->addWidget(new GLWDropDown(320, 5, 120));
	teamDropDown_->setToolTip(teamTip);

	// Create computer type choice
	GLWTip *typeTip = new GLWTip("Player Type",
		"Change between human and computer controlled\n"
		"players.  This is only available when playing\n"
		"single player games.");
	GLWLabel *typeLabel = (GLWLabel *) 
		infoPanel->addWidget(new GLWLabel(50, 5, "Type:"));
	typeLabel->setToolTip(typeTip);
	typeDropDown_ = (GLWDropDown *) 
		infoPanel->addWidget(new GLWDropDown(120, 5, 120));
	typeDropDown_->setHandler(this);
	typeDropDown_->setToolTip(typeTip);
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

void PlayerDialog::select(unsigned int id, const int pos, 
	GLWSelectorEntry value)
{
	if (0 == strcmp("Human", value.getText()))
	{
		imageList_->setCurrent("player.gif");
	}
	else
	{
		imageList_->setCurrent("computer.gif");
	}
}

void PlayerDialog::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	GLWWindow::keyDown(buffer, keyState, history, hisCount, skipRest);
	if (OptionsParam::instance()->getConnectedToServer()) skipRest = true;
}

void PlayerDialog::windowDisplay()
{	
	if (!viewer_)
	{
		GLWPanel *infoPanel = new GLWPanel(10.0f, 40.0f, 445.0f, 210.0f,
			false, true, true);
		viewer_ = new GLWTankViewer(5.0f, 5.0f, 4, 3);
		infoPanel->addWidget(viewer_);
		addWidget(infoPanel);
	}

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

	static AccessoryStore accessoryStore;
	static TankAIStore tankAIStore;

	static bool init = false;
	if (!init)
	{
		init = true;
		accessoryStore.parseFile(
			ScorchedClient::instance()->getOptionsGame());
		tankAIStore.loadAIs(accessoryStore);
	}

	// Add player types
	typeDropDown_->clear();
	typeDropDown_->addText(GLWSelectorEntry("Human", 
		&tankAIStore.getAIByName("Human")->getDescription()));
	if (!OptionsParam::instance()->getConnectedToServer())
	{
		std::list<TankAI *>::iterator aiitor;
		for (aiitor = tankAIStore.getAis().begin();
			aiitor != tankAIStore.getAis().end();
			aiitor++)
		{
			typeDropDown_->addText(
				GLWSelectorEntry((*aiitor)->getName(),
					&(*aiitor)->getDescription()));
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
		GLWWindowManager::instance()->hideWindow(getId());
	}
	else
	{
		Tank *tank = 
			ScorchedClient::instance()->getTankContainer().getTankById(currentPlayerId_);
		if (OptionsParam::instance()->getConnectedToServer())
		{
			// If we are connected online then use the online name
			playerName_->setText(
				OptionsDisplay::instance()->getOnlineUserName());
			viewer_->selectModelByName(
				OptionsDisplay::instance()->getOnlineTankModel());
			if (!imageList_->setCurrent(
				OptionsDisplay::instance()->getOnlineUserIcon()))
			{
				imageList_->setCurrent("player.gif");
			}
		}
		else
		{
			// Else use the default names
			if (tank) playerName_->setText(tank->getName());
		}
		
		if (tank->getState().getSpectator())
		{
			imageList_->setEnabled(true);
			imageList_->setToolTip(&avatarTip1_);
		}
		else
		{
			imageList_->setEnabled(false);
			imageList_->setToolTip(&avatarTip2_);
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
				OptionsDisplay::instance()->setOnlineUserName(
					playerName_->getText().c_str());
				OptionsDisplay::instance()->setOnlineTankModel(
					viewer_->getModelName());
				OptionsDisplay::instance()->setOnlineUserIcon(
					imageList_->getCurrent());
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
			// Add avatar (if not one)
			Tank *tank = ScorchedClient::instance()->getTankContainer().
				getTankById(currentPlayerId_);
			if (tank && !tank->getAvatar().getName()[0])
			{
				if (tank->getAvatar().loadFromFile(imageList_->getCurrent()))
				{
					if (tank->getAvatar().getFile().getBufferUsed() <=
						(unsigned) ScorchedClient::instance()->getOptionsGame().getMaxAvatarSize())
					{
						message.setPlayerIconName(imageList_->getCurrent());
						message.getPlayerIcon().addDataToBuffer(
							tank->getAvatar().getFile().getBuffer(),
							tank->getAvatar().getFile().getBufferUsed());
					}
					else
					{
						Logger::log( "Warning: Avatar too large to send to server");
					}
				}
			}
			ComsMessageSender::sendToServer(message);

			nextPlayer();
		}
	}
	else if (id == cancelId_)
	{
		GLWWindowManager::instance()->hideWindow(getId());
	}
}
