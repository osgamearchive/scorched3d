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
#include <GLW/GLWCheckBoxText.h>
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
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();

	muteTab_ = (GLWTab *)
		addWidget(new GLWTab("Mute", 0, 10, 40, 420, 410, 100));
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
		newPanel->addWidget(new GLWLabel(0, -2, (char *) tank->getName()));
		GLWCheckBoxText *box = (GLWCheckBoxText*)
			newPanel->addWidget(new GLWCheckBoxText(305, 1, "Mute", 
			tank->getState().getMuted()));
		muteMap_[box->getId()] = tank->getPlayerId();
		box->getCheckBox().setHandler(this);

		height += 24;
	}
}

void TalkSettingsDialog::windowInit(const unsigned state)
{
	needCentered_ = true;
	addPlayers();
}

void TalkSettingsDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		GLWWindowManager::instance()->hideWindow(getId());
	}
}

void TalkSettingsDialog::stateChange(bool state, unsigned int id)
{
	std::map<unsigned int, unsigned int>::iterator findItor =
		muteMap_.find(id);
	if (findItor != muteMap_.end())
	{
		unsigned int playerId = (*findItor).second;
		Tank *tank =
			ScorchedClient::instance()->getTankContainer().getTankById(playerId);
		if (tank)
		{
			tank->getState().setMuted(state);
		}
	}
}

