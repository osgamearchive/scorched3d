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

#include <tankai/TankAIComputerPShark.h>
#include <engine/ScorchedContext.h>
#include <math.h>

TankAIComputerPShark::TankAIComputerPShark()
{

}

TankAIComputerPShark::~TankAIComputerPShark()
{
}

void TankAIComputerPShark::tankHurt(Weapon *weapon, unsigned int firer)
{
	// Don't take a grudge against ourselves
	if (firer != currentTank_->getPlayerId())
	{
		// Add tanks we take a grudge against
		hitlist_.push_front(firer);
	}
	TankAIComputerTosser::tankHurt(weapon, firer);
}

void TankAIComputerPShark::newGame()
{
	hitlist_.clear();
	TankAIComputerTosser::newGame();
}

Tank *TankAIComputerPShark::findTankToShootAt()
{
	while (!hitlist_.empty())
	{
		unsigned int hit = hitlist_.front();
		hitlist_.pop_front();

		Tank *t = context_->tankContainer.getTankById(hit);
		if (t && t->getState().getState() == TankState::sNormal)
		{
			bool sameTeam = false;
			if (currentTank_->getTeam() > 0)
			{
				sameTeam = (currentTank_->getTeam() == t->getTeam());
			}

			if (!sameTeam)
			{
				hitlist_.push_back(hit);
				return t;
			}
		}
	}

	return TankAIComputerTosser::findTankToShootAt();
}
