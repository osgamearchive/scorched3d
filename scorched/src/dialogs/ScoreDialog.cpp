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

#include <dialogs/ScoreDialog.h>
#include <tank/TankSort.h>
#include <GLW/GLWFont.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <stdio.h>

static const float rankLeft = 5.0f;
static const float nameLeft = 20.0f;
static const float killsLeft = 205.0f;
static const float moneyLeft = 260.0f;
static const float winsLeft = 360.0f;
static const float readyLeft = 390.0f;
static const float lineSpacer = 10.0f;

ScoreDialog *ScoreDialog::instance_ = 0;
ScoreDialog *ScoreDialog::instance2_ = 0;

ScoreDialog *ScoreDialog::instance()
{
	if (!instance_)
	{
		instance_ = new ScoreDialog;
	}

	return instance_;
}

ScoreDialog *ScoreDialog::instance2()
{
	if (!instance2_)
	{
		instance2_ = new ScoreDialog;
	}

	return instance2_;
}

ScoreDialog::ScoreDialog() :
	GLWWindow("Score", 10.0f, 10.0f, 417.0f, 310.0f, eTransparent |eSmallTitle),
	lastScoreValue_(0), lastWinsValue_(0)
{

}

ScoreDialog::~ScoreDialog()
{

}

void ScoreDialog::windowDisplay()
{
	calculateScores();
}

void ScoreDialog::calculateScores()
{
	lastScoreValue_ = lastWinsValue_ = 0;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		lastScoreValue_ += tank->getScore().getMoney();
		lastWinsValue_ += tank->getScore().getWins();
	}

	sortedTanks_.clear();
	TankSort::getSortedTanksIds(
		ScorchedClient::instance()->getContext(), sortedTanks_);
}

void ScoreDialog::windowInit(const unsigned state)
{
	needCentered_ = true;
}

void ScoreDialog::draw()
{
	float h = sortedTanks_.size() * lineSpacer + 80.0f;
	if (ScorchedClient::instance()->getOptionsGame().getTeams() > 1) h += lineSpacer * 4.0f;
	setH(h);

	static size_t noTanks = 0;
	if (noTanks != sortedTanks_.size())
	{
		needCentered_ = true;
		noTanks = sortedTanks_.size();
	}

	GLWWindow::draw();
	GLState newState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	Vector white(0.9f, 0.9f, 1.0f);
	bool finished = (ScorchedClient::instance()->getGameState().getState() == ClientState::StateScore);
	GLWFont::instance()->getLargePtFont()->draw(
			white,
			20,
			x_ + 8.0f, y_ + h_ - 21.0f, 0.0f,
			finished?" Final Rankings":"Current Rankings");

	bool server = (OptionsParam::instance()->getConnectedToServer());
	if (server)
	{
		GLWFont::instance()->getLargePtFont()->draw(
				white,
				18,
				x_ + 8.0f, y_ + h_ - 41.0f, 0.0f,
				ScorchedClient::instance()->getOptionsGame().getServerName());
	}

	if (!finished)
	{
		char moves[256];
		moves[0] = '\0';
		if (ScorchedClient::instance()->getOptionsGame().getNoMaxRoundTurns() > 0)
		{
			int movesLeft = ScorchedClient::instance()->getOptionsGame().getNoMaxRoundTurns() -
				ScorchedClient::instance()->getOptionsTransient().getCurrentGameNo();
			sprintf(moves, ", %i Moves", movesLeft);
		}

		GLWFont::instance()->getSmallPtFont()->draw(
				white,
				10,
				x_ + 260.0f, y_ + h_ - 21.0f, 0.0f,
				"%i Rounds%s",
				ScorchedClient::instance()->getOptionsTransient().getNoRoundsLeft(),
				moves);
	}

	float y = lineSpacer + 10.0f;
	GLWFont::instance()->getSmallPtFont()->draw(
			white,
			12,
			x_ + nameLeft, y_ + h_ - y - lineSpacer - 26.0f, 0.0f,
			"Name");
	GLWFont::instance()->getSmallPtFont()->draw(
			white,
			12,
			x_ + killsLeft, y_ + h_ - y - lineSpacer - 26.0f, 0.0f,
			"Kills");
	GLWFont::instance()->getSmallPtFont()->draw(
			white,
			12,
			x_ + moneyLeft, y_ + h_ - y - lineSpacer - 26.0f, 0.0f,
			"Money");
	GLWFont::instance()->getSmallPtFont()->draw(
			white,
			12,
			x_ + winsLeft, y_ + h_ - y - lineSpacer - 26.0f, 0.0f,
			"Wins");
	y+= lineSpacer + lineSpacer;

	int tmpLastScoreValue = 0;
	int tmpLastWinsValue = 0;
	if (ScorchedClient::instance()->getOptionsGame().getTeams() > 1)
	{
		int winningTeam = TankSort::getWinningTeam(
			ScorchedClient::instance()->getContext());

		bool teamOne = false;
		bool teamTwo = false;
		int winsOne = 0, killsOne = 0, moneyOne = 0;
		int winsTwo = 0, killsTwo = 0, moneyTwo = 0;
		std::list<unsigned int>::iterator itor;
		// Team 1
		for (itor = sortedTanks_.begin();
			itor != sortedTanks_.end();
			itor ++)
		{
			unsigned int playerId = (*itor);
			Tank *current = ScorchedClient::instance()->getTankContainer().getTankById(playerId);
			if (current && current->getTeam() == 1 && !current->getState().getSpectator()) 
			{
				teamOne = true;
				addLine(current, y, (char *)(winningTeam!=2?"1":"2"));
				winsOne += current->getScore().getWins();
				killsOne += current->getScore().getKills();
				moneyOne += current->getScore().getMoney();
				tmpLastScoreValue += current->getScore().getMoney();
				tmpLastWinsValue += current->getScore().getWins();
				y+= lineSpacer;
			}
		}
		if (teamOne)
		{
			addScoreLine(y, killsOne,moneyOne,winsOne);
			y+= lineSpacer;
			y+= lineSpacer;
		}
		// Team 2
		for (itor = sortedTanks_.begin();
			itor != sortedTanks_.end();
			itor ++)
		{
			unsigned int playerId = (*itor);
			Tank *current = ScorchedClient::instance()->getTankContainer().getTankById(playerId);
			if (current && current->getTeam() == 2 && !current->getState().getSpectator()) 
			{
				teamTwo = true;
				addLine(current, y, (char *)(winningTeam!=1?"1":"2"));
				winsTwo += current->getScore().getWins();
				killsTwo += current->getScore().getKills();
				moneyTwo += current->getScore().getMoney();
				tmpLastScoreValue += current->getScore().getMoney();
				tmpLastWinsValue += current->getScore().getWins();
				y+= lineSpacer;
			}	
		}
		if (teamTwo)
		{
			addScoreLine(y, killsTwo,moneyTwo,winsTwo);
			y+= lineSpacer;
			y+= lineSpacer;
		}
	}
	else
	{
		// No Team
		int rank = 1;
		char strrank[10];
		std::list<unsigned int>::iterator itor;
		for (itor = sortedTanks_.begin();
			itor != sortedTanks_.end();
			itor ++, rank++)
		{
			unsigned int playerId = (*itor);
			Tank *current = ScorchedClient::instance()->getTankContainer().getTankById(playerId);
			if (current && !current->getState().getSpectator()) 
			{
				sprintf(strrank, "%i", rank);

				addLine(current, y, strrank);
				tmpLastScoreValue += current->getScore().getMoney();
				tmpLastWinsValue += current->getScore().getWins();
				y+= lineSpacer;
			}
		}	
	}
	y+= lineSpacer / 1.5f;
	// Spectators
	std::list<unsigned int>::iterator itor;
	for (itor = sortedTanks_.begin();
		itor != sortedTanks_.end();
		itor ++)
	{
		unsigned int playerId = (*itor);
		Tank *current = ScorchedClient::instance()->getTankContainer().getTankById(playerId);
		if (current && current->getState().getSpectator()) 
		{
			addLine(current, y, " ");
			y+= lineSpacer;
		}
	}	

	if (tmpLastScoreValue != lastScoreValue_ ||
		tmpLastWinsValue != lastWinsValue_)
	{
		calculateScores();
	}
}

void ScoreDialog::addScoreLine(float y, int kills, int money, int wins)
{
	float textX = x_;
	float textY  = y_ + h_ - y - lineSpacer - 30.0f;
	Vector white(0.9f, 0.9f, 1.0f);

	// Print the name on the screen
	GLWFont::instance()->getSmallPtFont()->draw(
		white,
		10,
		textX + killsLeft, textY, 0.0f,
		"%i",
		kills);	
	GLWFont::instance()->getSmallPtFont()->draw(
		white,
		10,
		textX + moneyLeft, textY, 0.0f,
		"$%i",
		money);	
	GLWFont::instance()->getSmallPtFont()->draw(
		white,
		10,
		textX + winsLeft, textY, 0.0f,
		"%i",
		wins);	
}

void ScoreDialog::addLine(Tank *current, float y, char *rank)
{
	float textX = x_;
	float textY  = y_ + h_ - y - lineSpacer - 25.0f;
	bool currentPlayer = false;
	
	// Print a highlight behind the current clients player
	if (current->getDestinationId() == 
		ScorchedClient::instance()->getTankContainer().getCurrentDestinationId())
	{
		GLState state(GLState::BLEND_ON); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
		glBegin(GL_QUADS);
			glVertex2f(x_ + w_ - 3.0f, textY + lineSpacer - 1.0f);
			glVertex2f(x_ + 3.0f, textY + lineSpacer - 1.0f);
			glVertex2f(x_ + 3.0f, textY - 1.0f);
			glVertex2f(x_ + w_ - 3.0f, textY - 1.0f);
		glEnd();
	}

	// Form the name
	static char name[256];
	strcpy(name, current->getName());
	if (current->getState().getState() != TankState::sNormal)
	{
		strcat(name, " (");
		strcat(name, current->getState().getSmallStateString());
		strcat(name, ")");
	}
	name[26] = '\0'; // Limit length

	if (current->getState().getSpectator())
	{
		// Print the name on the screen
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + nameLeft, textY, 0.0f,
			name);
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + readyLeft, textY, 0.0f,
			"%2s",
			((current->getState().getReadyState() == TankState::SNotReady)?"*":" "));
	}
	else
	{
		// Print the name on the screen
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + rankLeft, textY, 0.0f,
			"%s",
			rank);
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + nameLeft, textY, 0.0f,
			"%s",
			name);
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + killsLeft, textY, 0.0f,
			"%i",
			current->getScore().getKills());
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + moneyLeft, textY, 0.0f,
			"$%i",
			current->getScore().getMoney());
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + winsLeft, textY, 0.0f,
			"%i",
			current->getScore().getWins());
		GLWFont::instance()->getSmallPtFont()->draw(
			current->getColor(),
			10,
			textX + readyLeft, textY, 0.0f,
			"%s",
			((current->getState().getReadyState() == TankState::SNotReady)?"*":" "));
	}
}
