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
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankTeamScore.h>
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
		int roundDiff,
		int scoreBonusDiff) :
	firstTime_(true),
	playerId_(playerId), moneyDiff_(moneyDiff),
	killDiff_(killDiff), roundDiff_(roundDiff),
	scoreBonusDiff_(scoreBonusDiff)
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
		if (!context_->serverMode && moneyDiff_ != 0) 
		{
			Vector position = tank->getPosition().getTankPosition();
			position[0] += RAND * 5.0f - 2.5f;
			position[1] += RAND * 5.0f - 2.5f;
			position[2] += RAND * 5.0f - 2.5f;

			Vector yellowColor(0.75f, 0.75f, 0.0f);
			context_->actionController->addAction(
				new SpriteAction(
					new TextActionRenderer(
						position,
						yellowColor,
						formatString("$ %+i", moneyDiff_))));
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
			int score = scoreBonusDiff_;

			tank->getScore().addTotalMoneyEarnedStat(moneyDiff_);
			tank->getScore().setMoney(tank->getScore().getMoney() + moneyDiff_);
			tank->getScore().setKills(tank->getScore().getKills() + killDiff_);
			tank->getScore().setWins(tank->getScore().getWins() + roundDiff_);
			tank->getScore().setScore(tank->getScore().getScore() + score);

			if (tank->getTeam() > 0)
			{
				context_->tankTeamScore->addScore(score, tank->getTeam());
			}
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
	buffer.addToBuffer(scoreBonusDiff_);
	return true;
}

bool TankScored::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(moneyDiff_)) return false;
	if (!reader.getFromBuffer(killDiff_)) return false;
	if (!reader.getFromBuffer(roundDiff_)) return false;
	if (!reader.getFromBuffer(scoreBonusDiff_)) return false;
	return true;
}
