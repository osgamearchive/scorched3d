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


// TankAIComputer.cpp: implementation of the TankAIComputer class.
//
//////////////////////////////////////////////////////////////////////

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

TankAIComputer::TankAIComputer(Tank *tank) : 
	TankAI(tank), primaryShot_(true)
{
	tankBuyer_.setTank(currentTank_);
}

TankAIComputer::~TankAIComputer()
{

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
		say(TankAIStrings::instance()->getDeathLine());
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
