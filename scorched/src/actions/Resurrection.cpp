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

#include <actions/Resurrection.h>
#include <engine/ActionController.h>
#include <engine/ScorchedContext.h>
#include <landscapemap/DeformLandscape.h>
#include <common/ChannelManager.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <target/TargetLife.h>

Resurrection::Resurrection(
	unsigned int playerId,
	Vector &position) :
	playerId_(playerId),
	position_(position)
{

}

Resurrection::~Resurrection()
{
}

void Resurrection::init()
{
}

void Resurrection::simulate(float frameTime, bool &remove)
{
	remove = true;

	Tank *tank = context_->tankContainer->getTankById(playerId_);
	if (tank)
	{
#ifndef S3D_SERVER
		if (!context_->serverMode)
		{
			ChannelText text("combat",
				formatString("\"%s\" was resurrected, %i lives remaining",
					tank->getName(),
					tank->getState().getLives()));
			//info.setPlayerId(playerId_);
			ChannelManager::showText(text);
		}
#endif

		// Rez this tank
		tank->rezTank();
		tank->getLife().setTargetPosition(position_);
		DeformLandscape::flattenArea(*context_, position_);
	}

	Action::simulate(frameTime, remove);
}
