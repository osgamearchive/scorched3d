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
#include <tank/TankContainer.h>
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

void TankAIHumanCtrl::keyboardCheck(const unsigned state, float frameTime, 
							char *buffer, int bufCount,
							KeyboardHistory::HistoryElement *history, int hisCount, 
							bool &skipRest)
{
	Tank *currentTank =
		TankContainer::instance()->getCurrentTank();
	if (currentTank)
	{
		if (currentTank->getState().getState() == TankState::sNormal)
		{
			TankAI *ai = currentTank->getTankAI();
			if (ai)
			{
				ai->playMove(state, frameTime, buffer);
			}
		}
		else
		{
			// Stimulate into the shot state
			GameState::instance()->stimulate(ClientState::StimShot);
		}
	}
}
