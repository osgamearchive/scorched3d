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

#include <dialogs/RulesDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWFont.h>
#include <common/WindowManager.h>
#include <common/OptionsParam.h>
#include <client/ScorchedClient.h>
#include <server/TurnController.h>

RulesDialog *RulesDialog::instance_ = 0;

RulesDialog *RulesDialog::instance()
{
	if (!instance_)
	{
		instance_ = new RulesDialog();
	}
	return instance_;
}

RulesDialog::RulesDialog() : 
	GLWWindow("Rules", 0.0f, 0.0f, 300.0f, 280.0f, 0), firstShow_(true)
{
	needCentered_ = true;
	okId_ = addWidget(new GLWTextButton(" Ok", 235, 10, 55, this, true))->getId();
	listView_ = (GLWListView *) addWidget(new GLWListView(10, 130, 280, 140, 100));
}

RulesDialog::~RulesDialog()
{
}

void RulesDialog::addMOTD(const char *text)
{
	listView_->clear();

	// Add single or multiple lines
	char *found = strchr(text, '\n');
	char *start = (char *) text;
	if (found)
	{
		while (found)
		{
			*found = '\0';
			listView_->addLine(start);
			start = found;
			start++;

			found = strchr(start, '\n');
		}
		if (start[0] != '\0') listView_->addLine(start);
	}
	else
	{
		listView_->addLine(text);
	}
}

void RulesDialog::windowInit(const unsigned state)
{
	if (firstShow_)
	{
		firstShow_ = false;
		WindowManager::instance()->showWindow(id_);
	}
}

void RulesDialog::draw()
{
	GLWWindow::draw();

	OptionsTransient &optionsT = ScorchedClient::instance()->getOptionsTransient();
	OptionsGame &options = ScorchedClient::instance()->getOptionsGame();

	GLState newState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	float top = y_ + h_ - 150.0f;
	Vector yellow(0.3f, 0.3f, 0.3f); // Hmm, thats not yellow
	if (ScorchedClient::instance()->getTankContainer().getNoOfNonSpectatorTanks() <
		ScorchedClient::instance()->getOptionsGame().getNoMinPlayers())
	{
		GLWFont::instance()->getFont()->draw(
			yellow,
			14,
			x_ + 12.0f, top - 30.0f, 0.0f,
			"Game has not started yet");
		GLWFont::instance()->getFont()->draw(
			yellow,
			14,
			x_ + 12.0f, top - 45.0f, 0.0f,
			"Waiting for %i more players", 
			ScorchedClient::instance()->getOptionsGame().getNoMinPlayers()-
			ScorchedClient::instance()->getTankContainer().getNoOfNonSpectatorTanks());
	}
	else
	{
		GLWFont::instance()->getFont()->draw(
			yellow,
			14,
			x_ + 12.0f, top - 30.0f, 0.0f,
			"Total number of rounds : %i", 
			options.getNoRounds());
		GLWFont::instance()->getFont()->draw(
			yellow,
			14,
			x_ + 12.0f, top - 45.0f, 0.0f,
			"Rounds left : %i", 
			optionsT.getNoRoundsLeft());
	}

	GLWFont::instance()->getFont()->draw(
		yellow,
		14,
		x_ + 12.0f, top - 75.0f, 0.0f,
		"Game type : %s", 
		TurnController::instance()->getGameType(options.getTurnType()));
	GLWFont::instance()->getFont()->draw(
		yellow,
		14,
		x_ + 12.0f, top - 90.0f, 0.0f,
		((options.getShotTime() > 0)?"Shot time : %i (s)":"No shot time limit"),
		options.getShotTime());
	GLWFont::instance()->getFont()->draw(
		yellow,
		14,
		x_ + 12.0f, top - 105.0f, 0.0f,
		((options.getBuyingTime() > 0)?"Buying time : %i (s)":"No buying time limit"),
		options.getShotTime());
}

void RulesDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		WindowManager::instance()->hideWindow(id_);
	}
}
