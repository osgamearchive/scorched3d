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

#include <tankai/TankAIHumanCtrl.h>
#include <tankai/TankAIHuman.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>

TankAIHumanCtrl * TankAIHumanCtrl::instance_ = 0;

TankAIHumanCtrl * TankAIHumanCtrl::instance()
{
	if (!instance_)
	{
		instance_ = new TankAIHumanCtrl;
	}

	return instance_;
}

TankAIHumanCtrl::TankAIHumanCtrl()
{

}

TankAIHumanCtrl::~TankAIHumanCtrl()
{

}

void TankAIHumanCtrl::enterState(const unsigned state)
{
	if (state == ClientState::StateShot)
	{
		Tank *currentTank =
			ScorchedClient::instance()->getTankContainer().getCurrentTank();
		if (currentTank)
		{
			if (currentTank->getState().getState() == TankState::sNormal)
			{
				TankAI *ai = currentTank->getTankAI();
				if (ai)
				{
					ai->endPlayMove();
				}
			}
		}
	}
}

void TankAIHumanCtrl::keyboardCheck(const unsigned state, float frameTime, 
							char *buffer, unsigned int keyState,
							KeyboardHistory::HistoryElement *history, int hisCount, 
							bool &skipRest)
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			setTankAI(); // Ensure this tank has an AI
			TankAI *ai = currentTank->getTankAI();
			if (ai)
			{
				ai->playMove(state, frameTime, buffer, keyState);
			}
		}
		else
		{
			DIALOG_ASSERT(0);
		}
	}
}

void TankAIHumanCtrl::setTankAI()
{
	Tank *currentTank =
		ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getDestinationId() == 
			ScorchedClient::instance()->getTankContainer().getCurrentDestinationId() &&
			!currentTank->getTankAI()) 
		{
			// If this is a player local to this destination then add the human ai
			TankAI *ai = new TankAIHuman(&ScorchedClient::instance()->getContext(), currentTank);
			currentTank->setTankAI(ai);
		}
	}
}
