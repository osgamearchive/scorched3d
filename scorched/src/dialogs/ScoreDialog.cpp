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
#include <tank/TankContainer.h>
#include <tank/TankSort.h>
#include <GLW/GLWFont.h>
#include <common/OptionsParam.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <client/ClientState.h>
#include <client/ScorchedClient.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
	GLWWindow("Score", 10.0f, 10.0f, 417.0f, 310.0f, eTransparent | eNoTitle),
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
	std::list<Tank *> sortedTanks;
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		lastScoreValue_ += tank->getScore().getMoney();
		lastWinsValue_ += tank->getScore().getWins();
		sortedTanks.push_back(tank);
	}

	TankSort::getSortedTanks(sortedTanks);
	sortedTanks_.clear();
	std::list<Tank *>::iterator resultitor;
	for (resultitor = sortedTanks.begin();
		resultitor != sortedTanks.end();
		resultitor++)
	{
		sortedTanks_.push_back((*resultitor)->getPlayerId());
	}
}

void ScoreDialog::windowInit(const unsigned state)
{
	needCentered_ = true;
}

void ScoreDialog::draw()
{
	const float lineSpacer = 10.0f;
	float h = sortedTanks_.size() * lineSpacer + 70.0f;
	setH(h);

	static size_t noTanks = 0;
	if (noTanks != sortedTanks_.size())
	{
		needCentered_ = true;
		noTanks = sortedTanks_.size();
	}

	GLWWindow::draw();
	GLState newState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	Vector white(0.8f, 0.8f, 0.8f);
	bool finished = (ScorchedClient::instance()->getGameState().getState() == ClientState::StateScore);
	GLWFont::instance()->getFont()->draw(
			white,
			20,
			x_ + 100.0f, y_ + h_ - 21.0f, 0.0f,
			finished?" Final Rankings":"Current Rankings");

	bool server = (OptionsParam::instance()->getConnectedToServer());
	if (server)
	{
		GLWFont::instance()->getFont()->draw(
				white,
				18,
				x_ + 8.0f, y_ + h_ - 41.0f, 0.0f,
				OptionsGame::instance()->getServerName());
	}

	if (!finished)
	{
		GLWFont::instance()->getFont()->draw(
				white,
				12,
				x_ + 305.0f, y_ + h_ - 35.0f, 0.0f,
				"%i Rounds Left",
				OptionsTransient::instance()->getNoRoundsLeft());
	}

	float y = lineSpacer + 10.0f;
	GLWFont::instance()->getFont()->draw(
			white,
			12,
			x_ + 6.0f, y_ + h_ - y - lineSpacer - 22.0f, 0.0f,
			"   %-24s%5s%10s%10s",
			"Name",
			"Kills",
			"Money",
			"Wins");
	y+= lineSpacer + lineSpacer;

	int pos = 1;
	int tmpLastScoreValue = 0;
	int tmpLastWinsValue = 0;
	std::list<unsigned int>::iterator itor;
	for (itor = sortedTanks_.begin();
		itor != sortedTanks_.end();
		itor ++, pos++)
	{
		unsigned int playerId = (*itor);
		Tank *current = TankContainer::instance()->getTankById(playerId);
		if (current) 
		{
			float textX = x_ + 6.0f;
			float textY  = y_ + h_ - y - lineSpacer - 25.0f;
			bool currentPlayer = false;
	        
			// Print a highlight behind the current clients player
			if (server)
			{
				if (current->getPlayerId() == 
					TankContainer::instance()->getCurrentPlayerId())
				{
					GLState state(GLState::BLEND_ON); 
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
					glBegin(GL_QUADS);
						glVertex2f(x_ + w_ - 3.0f, textY + lineSpacer - 0.0f);
						glVertex2f(x_ + 3.0f, textY + lineSpacer - 0.0f);
						glVertex2f(x_ + 3.0f, textY + 2.0f);
						glVertex2f(x_ + w_ - 3.0f, textY + 2.0f);
					glEnd();
				}
			}

			// Form the name
			static char name[256];
			strcpy(name, current->getName());
			if (current->getState().getState() == TankState::sDead)
			{
				strcat(name, " (DEAD)");
			}
			else if (current->getState().getState() == TankState::sPending)
			{
				strcat(name, " (PENDING)");
			}
			name[26] = '\0'; // Limit length

			// Print the name on the screen
			GLWFont::instance()->getFont()->draw(
				current->getColor(),
				12,
				textX, textY, 0.0f,
				"%-3i%-27s%2i%10i $%8i",
				pos,
				name,
				current->getScore().getKills(),
				current->getScore().getMoney(),
				current->getScore().getWins());
			tmpLastScoreValue += current->getScore().getMoney();
			tmpLastWinsValue += current->getScore().getWins();
		}
		y+= lineSpacer;
	}
	y+= lineSpacer;

	if (tmpLastScoreValue != lastScoreValue_ ||
		tmpLastWinsValue != lastWinsValue_)
	{
		calculateScores();
	}
}
