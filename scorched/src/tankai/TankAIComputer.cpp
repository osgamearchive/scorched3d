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

#include <tankai/TankAIComputer.h>
#include <tankai/TankAIStrings.h>
#include <server/ServerShotHolder.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <actions/TankSay.h>
#include <engine/ActionController.h>
#include <coms/ComsPlayedMoveMessage.h>
#include <coms/ComsDefenseMessage.h>
#include <coms/ComsMessageSender.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TankAIComputer::TankAIComputer() : 
	TankAI(0), primaryShot_(true), name_("<NoName>")
{
	
}

TankAIComputer::~TankAIComputer()
{

}

void TankAIComputer::setTank(Tank *tank)
{
	currentTank_ = tank;
	tankBuyer_.setTank(currentTank_);
}

bool TankAIComputer::parseConfig(XMLNode *node)
{
	XMLNode *nameNode = node->getNamedChild("name");
	if (!nameNode)
	{
		dialogMessage("TankAIComputer",
			"Failed to find name node in tank ai");
		return false;
	}
	name_ = nameNode->getContent();

	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = node->getChildren();
    for (childrenItor = children.begin();
		 childrenItor != children.end();
		 childrenItor++)
    {
		XMLNode *child = (*childrenItor);
		if (strcmp(child->getName(), "weapon")==0)
		{
			XMLNode *wname = child->getNamedChild("name");
			XMLNode *wlevel = child->getNamedChild("level");
			if (!wname || !wlevel) 
			{
				dialogMessage("TankAIComputer",
					"Failed to parse weapon node in tank ai \"%s\"",
					name_.c_str());
				return false;
			}

			if (!tankBuyer_.addAccessory(
				wname->getContent(), 
				atoi(wlevel->getContent()))) return false;
		}
	}

	return true;
}

void TankAIComputer::newGame()
{
	int roundsPlayed = OptionsGame::instance()->getNoRounds() -
		OptionsTransient::instance()->getNoRoundsLeft();
	if (OptionsGame::instance()->getBuyOnRound() - 1 < roundsPlayed)
	{
		buyAccessories();
	}
}

void TankAIComputer::nextRound()
{

}

void TankAIComputer::tankHurt(Weapon *weapon, unsigned int firer)
{
	if (currentTank_->getState().getLife() == 0.0f &&
		currentTank_->getState().getState() == TankState::sNormal)
	{
		const char *line = TankAIStrings::instance()->getDeathLine();
		if (line) say(line);
	}
}

void TankAIComputer::shotLanded(Weapon *weapon, unsigned int firer, Vector &position)
{
	if (primaryShot_ && firer == currentTank_->getPlayerId())
	{
		ourShotLanded(weapon, position);
		primaryShot_ = false;
	}
}

void TankAIComputer::ourShotLanded(Weapon *weapon, Vector &position)
{

}

void TankAIComputer::buyAccessories()
{
	tankBuyer_.buyAccessories(3);
}

void TankAIComputer::fireShot()
{
	primaryShot_ = true;
	TankAI::fireShot();
}

void TankAIComputer::selectFirstShield()
{
	if (currentTank_->getAccessories().getShields().getAllShields().size())
	{
		std::map<Shield *, int>::iterator itor = 
			currentTank_->getAccessories().getShields().getAllShields().begin();
		Shield *shield = (*itor).first;

		if (!currentTank_->getAccessories().getShields().getCurrentShield())
		{
			shieldsUpDown(shield->getName());
		}
	}
}

void TankAIComputer::say(const char *text)
{
	std::string newText(currentTank_->getName());
	newText += ":";
	newText += text;

	ActionController::instance()->addAction(
		new TankSay(currentTank_->getPlayerId(), newText.c_str()));
}
