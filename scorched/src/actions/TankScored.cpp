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

#include <actions/TankScored.h>
#include <sprites/TextActionRenderer.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>
#include <common/OptionsParam.h>
#include <common/Logger.h>

REGISTER_ACTION_SOURCE(TankScored);

TankScored::TankScored() : firstTime_(true)
{
}

TankScored::TankScored(unsigned int playerId,
		int moneyDiff,
		int killDiff,
		int roundDiff) :
	firstTime_(true),
	playerId_(playerId), moneyDiff_(moneyDiff),
	killDiff_(killDiff), roundDiff_(roundDiff)
{

}

TankScored::~TankScored()
{
}

void TankScored::init()
{
	Tank *tank = 
		context_->tankContainer->getTankById(playerId_);
	if (tank)
	{
		if (!context_->serverMode) 
		{
			Vector position = tank->getPhysics().getTankPosition();
			position[0] += RAND * 5.0f - 2.5f;
			position[1] += RAND * 5.0f - 2.5f;
			position[2] += RAND * 5.0f - 2.5f;

			Vector greenColor(0.0f, 0.75f, 0.0f);
			Vector redColor(0.75f, 0.0f, 0.0f);
			Vector yellowColor(0.75f, 0.75f, 0.0f);

			char buffer[256];
			sprintf(buffer, "$ %+i", moneyDiff_);
			setActionRender(
				new TextActionRenderer(
					position,
					(moneyDiff_>0.0f)?greenColor:((moneyDiff_<0.0f)?redColor:yellowColor),
					buffer));
		}
	}
}

void TankScored::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		Tank *tank = 
			context_->tankContainer->getTankById(playerId_);
		if (tank)
		{
			tank->getScore().setMoney(tank->getScore().getMoney() + moneyDiff_);
			tank->getScore().setKills(tank->getScore().getKills() + killDiff_);
			tank->getScore().setWins(tank->getScore().getWins() + roundDiff_);
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool TankScored::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(moneyDiff_);
	buffer.addToBuffer(killDiff_);
	buffer.addToBuffer(roundDiff_);
	return true;
}

bool TankScored::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(moneyDiff_)) return false;
	if (!reader.getFromBuffer(killDiff_)) return false;
	if (!reader.getFromBuffer(roundDiff_)) return false;
	return true;
}
