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


#include <dialogs/PlayerDialog.h>
#include <engine/GameState.h>
#include <client/ClientState.h>
#include <common/OptionsParam.h>
#include <GLW/GLWTextButton.h>
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
	GLWWindow("Player", 10.0f, 10.0f, 260.0f, 255.0f, 0),
	skipInit_(false)
{
	needCentered_ = true;
	viewer_ = new GLWTankViewer(10.0f, 10.0f);

	// Add buttons
	addWidget(viewer_);
	okId_ = addWidget(new GLWTextButton(" Ok", 195, 10, 55, this, true))->getId();

	// Create player name choice
	playerName_ = (GLWTextBox *) addWidget(new GLWTextBox(10, 220, 240, ""));
}

PlayerDialog::~PlayerDialog()
{
}

void PlayerDialog::draw()
{
	GLWWindow::draw();

	static bool firstDraw = true;
	if (firstDraw)
	{
		const char *playerModel = 
			OptionsParam::instance()->getSkin();
		const char *playerName =
			OptionsParam::instance()->getName();
		if (strlen(playerModel) > 0)
		{
			viewer_->selectModelByName(playerModel);
		}
		if (strlen(playerName) > 0)
		{
			playerName_->setText(playerName);
			buttonDown(okId_);
		}
		firstDraw = false;
	}
}

void PlayerDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		if (!playerName_->getText().empty())
		{
			GameState::instance()->stimulate(ClientState::StimClientConnect);
		}
	}
}
