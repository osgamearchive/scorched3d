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

#include <dialogs/TalkDialog.h>
#include <dialogs/TalkSettingsDialog.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWLabel.h>
#include <GLEXT/GLConsole.h>
#include <GLEXT/GLViewPort.h>

TalkDialog *TalkDialog::instance_ = 0;

TalkDialog *TalkDialog::instance()
{
	if (!instance_)
	{
		instance_ = new TalkDialog;
	}
	return instance_;
}

TalkDialog::TalkDialog() : 
	GLWWindow("Talk", 10.0f, 10.0f, 300.0f, 70.0f, eSmallTitle,
		"Allows the current player to send messages\n"
		"to other online players.")
{
	windowLevel_ = 50000;
	needCentered_ = true;

	// Add buttons
	ok_ = (GLWTextButton *) 
		addWidget(new GLWTextButton("Ok", 235, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX));
	cancel_ = (GLWTextButton *) 
		addWidget(new GLWTextButton("Cancel ", 120, 10, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX));
	mute_ = (GLWTextButton *) 
		addWidget(new GLWTextButton("Mute", 150, 10, 65, this, 
		GLWButton::ButtonFlagCenterX));

	addWidget(new GLWLabel(30, 7, "Team Say"));
	teamBox_ = (GLWCheckBox *)
		addWidget(new GLWCheckBox(10, 10, false));

	// Create player name choice
	talkText_ = (GLWTextBox *) 
		addWidget(new GLWTextBox(10, 40, 280, ""));
}

TalkDialog::~TalkDialog()
{
}

void TalkDialog::windowDisplay()
{
	teamBox_->getState() = false;
}

void TalkDialog::setTeamTalk()
{
	teamBox_->getState() = true;
}

void TalkDialog::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	GLWWindow::keyDown(buffer, keyState, history, hisCount, skipRest);
	skipRest = true;
}

void TalkDialog::draw()
{
	float width = (float) GLViewPort::getWidth();
	if (width < 320) width = 320;
	if (width > 640) width = 640;

	setW(width - 20);
	talkText_->setW(width - 40);
	ok_->setX(width - 85);
	cancel_->setX(width - 200);

	GLWWindow::draw();
}

void TalkDialog::buttonDown(unsigned int id)
{
	if (id == ok_->getId())
	{
		if (!talkText_->getText().empty())
		{
			char *text = (char *) talkText_->getText().c_str();
			while (char *c = strchr(text, '"'))
			{
				*c = '\'';
			}

			GLConsole::instance()->addLine(true, "%s \"%s\"",
				(teamBox_->getState()?"Teamsay":"Say"),
				text);
		}

		talkText_->setText("");
		GLWWindowManager::instance()->hideWindow(id_);
	}
	else if (id == cancel_->getId())
	{
		talkText_->setText("");
		GLWWindowManager::instance()->hideWindow(id_);
	}
	else if (id == mute_->getId())
	{
		GLWWindowManager::instance()->showWindow(
				TalkSettingsDialog::instance()->getId());
	}
}

TeamTalkDialog *TeamTalkDialog::instance_ = 0;

TeamTalkDialog *TeamTalkDialog::instance()
{
	if (!instance_)
	{
		instance_ = new TeamTalkDialog;
	}
	return instance_;
}

TeamTalkDialog::TeamTalkDialog() : 
	GLWWindow("Team Talk", 10.0f, 10.0f, 300.0f, 70.0f, eSmallTitle,
		"Allows the current player to send messages\n"
		"to other online players (in the same team).")
{
	needCentered_ = true;
}

TeamTalkDialog::~TeamTalkDialog()
{
}

void TeamTalkDialog::windowDisplay()
{
	GLWWindowManager::instance()->hideWindow(getId());
	GLWWindowManager::instance()->showWindow(
		TalkDialog::instance()->getId());
	TalkDialog::instance()->setTeamTalk();
}
