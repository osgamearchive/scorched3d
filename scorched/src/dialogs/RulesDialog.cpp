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
#include <dialogs/PlayerDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWWindowManager.h>
#include <tank/TankContainer.h>
#include <common/OptionsParam.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <client/ScorchedClient.h>

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
	GLWWindow("Rules", 0.0f, 0.0f, 460.0f, 395.0f, eSmallTitle,
		"Shows the game rules for the game\n"
		"in progress.")
{
	needCentered_ = true;
	okId_ = addWidget(new GLWTextButton("Ok", 395, 10, 55, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();
	listView_ = (GLWListView *) addWidget(new GLWListView(10, 150, 440, 190, 100));
	addWidget(new GLWPanel(45, 345, 370, 40));
	icon_ = (GLWIcon *) addWidget(new GLWIcon(50, 350, 360, 30));
}

RulesDialog::~RulesDialog()
{
}

void RulesDialog::addIcon(GLTexture *texture)
{
	delete icon_->getTexture();
	icon_->setTexture(texture);
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
		if (start[0] != '\0')
		{
			listView_->addLine(start);
		}
	}
	else
	{
		listView_->addLine(text);
	}
}

void RulesDialog::draw()
{
	GLWWindow::draw();

	OptionsTransient &optionsT = ScorchedClient::instance()->getOptionsTransient();
	OptionsGame &options = ScorchedClient::instance()->getOptionsGame();

	GLState newState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	float top = y_ + h_ - 230.0f;
	Vector yellow(0.3f, 0.3f, 0.3f); // Hmm, thats not yellow
	if (ScorchedClient::instance()->getTankContainer().getNoOfNonSpectatorTanks() <
		ScorchedClient::instance()->getOptionsGame().getNoMinPlayers())
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12,
			x_ + 12.0f, top - 30.0f, 0.0f,
			"Game has not started yet");

		int waitingFor = ScorchedClient::instance()->getOptionsGame().getNoMinPlayers() -
			ScorchedClient::instance()->getTankContainer().getNoOfNonSpectatorTanks();

		int mySpectators = 0;
		std::map<unsigned int, Tank *> &playingTanks = 
			ScorchedClient::instance()->getTankContainer().getPlayingTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = playingTanks.begin();
			itor != playingTanks.end();
			itor++)
		{
			Tank *current = (*itor).second;

			if (current->getDestinationId() == 
				ScorchedClient::instance()->getTankContainer().getCurrentDestinationId() &&
				current->getState().getSpectator())
			{
				mySpectators++;
			}
		}

		if (waitingFor == mySpectators)
		{
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow,
				12,
				x_ + 12.0f, top - 45.0f, 0.0f,
				"Players waiting for you");
		}
		else
		{
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow,
				12,
				x_ + 12.0f, top - 45.0f, 0.0f,
				"Waiting for %i more players", 
				waitingFor);
		}
	}
	else
	{
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12,
			x_ + 12.0f, top - 30.0f, 0.0f,
			"Current round : %i", 
			optionsT.getCurrentRoundNo());
		GLWFont::instance()->getSmallPtFont()->draw(
			yellow,
			12,
			x_ + 12.0f, top - 45.0f, 0.0f,
			"Total number of rounds : %i", 
			options.getNoRounds());
	}

	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 75.0f, 0.0f,
		"Mod : %s", 
		ScorchedClient::instance()->getOptionsGame().getMod());
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 90.0f, 0.0f,
		"Game type : %s", 
		ScorchedClient::instance()->getOptionsTransient().getGameType());
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 105.0f, 0.0f,
		((options.getTeams() > 1)?"Teams : %i":"Teams : None"),
		options.getTeams());
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 120.0f, 0.0f,
		"Score Mode : %s", (options.getScoreType() == OptionsGame::ScoreWins)?"Most Wins":
		((options.getScoreType() == OptionsGame::ScoreKills)?"Most Kills":"Most Money"));
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 135.0f, 0.0f,
		((options.getShotTime() > 0)?"Shot time : %i (s)":"Shot time : Unlimited"),
		options.getShotTime());
	GLWFont::instance()->getSmallPtFont()->draw(
		yellow,
		12,
		x_ + 12.0f, top - 150.0f, 0.0f,
		((options.getBuyingTime() > 0)?"Buying time : %i (s)":"Buying time : Unlimited"),
		options.getShotTime());
}

void RulesDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		GLWWindowManager::instance()->hideWindow(id_);	
	}
}

void RulesDialog::windowDisplay()
{
	if (OptionsParam::instance()->getScreenSaverMode())
	{
		GLWWindowManager::instance()->hideWindow(getId());
	}
}

void RulesDialog::windowHide()
{
	GLWWindowManager::instance()->showWindow(
		PlayerDialog::instance()->getId());	
}
