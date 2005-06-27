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

#include <dialogs/TalkSettingsDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWWindowManager.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <stdio.h>

TalkSettingsDialog *TalkSettingsDialog::instance_ = 0;

TalkSettingsDialog *TalkSettingsDialog::instance()
{
	if (!instance_) instance_ = new TalkSettingsDialog;
	return instance_;
}

TalkSettingsDialog::TalkSettingsDialog() : 
	GLWWindow("", 10.0f, 10.0f, 440.0f, 480.0f, 0,
		"Show the current talk settings")
{
	windowLevel_ = 45000;
	okId_ = addWidget(new GLWTextButton("Ok", 375, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCancel | 
		GLWButton::ButtonFlagCenterX))->getId();
	muteAllId_ = addWidget(new GLWTextButton("Mute All", 10, 10, 120, this, 
		GLWButton::ButtonFlagCenterX))->getId();
	muteNoneId_ = addWidget(new GLWTextButton("Mute None", 140, 10, 120, this, 
		GLWButton::ButtonFlagCenterX))->getId();

	muteTab_ = (GLWTab *)
		addWidget(new GLWTab("Mute", 10, 40, 420, 410));
}

TalkSettingsDialog::~TalkSettingsDialog()
{

}

void TalkSettingsDialog::addPlayers()
{
	muteTab_->clear();
	muteMap_.clear();

	int height = 10;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		GLWPanel *newPanel = (GLWPanel *)
			muteTab_->addWidget(new GLWPanel(10.0f, (float) height, 295.0f, 20.0f, true));
		newPanel->addWidget(new GLWLabel(5, -2, (char *) tank->getName()));
		GLWCheckBoxText *box = (GLWCheckBoxText*)
			newPanel->addWidget(new GLWCheckBoxText(305, 1, "Mute", 
			tank->getState().getMuted()));
		muteMap_[box] = tank->getPlayerId();

		height += 24;
	}
}

void TalkSettingsDialog::windowDisplay()
{
	needCentered_ = true;
	addPlayers();
}

void TalkSettingsDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		std::map<GLWCheckBoxText *, unsigned int>::iterator itor;
		for (itor = muteMap_.begin();
			itor != muteMap_.end();
			itor++)
		{
			GLWCheckBoxText *box = (*itor).first;
			unsigned int playerId = (*itor).second;
			Tank *tank =
				ScorchedClient::instance()->getTankContainer().getTankById(playerId);
			if (tank)
			{
				tank->getState().setMuted(box->getCheckBox().getState());
			}
		}
	
		GLWWindowManager::instance()->hideWindow(getId());
	} 
	else if (id == muteAllId_)
	{
		std::map<GLWCheckBoxText *, unsigned int>::iterator itor;
		for (itor = muteMap_.begin();
			itor != muteMap_.end();
			itor++)
		{
			GLWCheckBoxText *box = (*itor).first;
			box->getCheckBox().setState(true);
		}
	}
	else if (id == muteNoneId_)
	{
		std::map<GLWCheckBoxText *, unsigned int>::iterator itor;
		for (itor = muteMap_.begin();
			itor != muteMap_.end();
			itor++)
		{
			GLWCheckBoxText *box = (*itor).first;
			box->getCheckBox().setState(false);
		}		
	}
}
