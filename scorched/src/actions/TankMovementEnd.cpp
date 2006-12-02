////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <actions/TankMovementEnd.h>
#include <actions/TankMovement.h>
#include <landscapemap/DeformLandscape.h>
#include <common/OptionsGame.h>
#include <weapons/AccessoryStore.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <target/TargetLife.h>

REGISTER_ACTION_SOURCE(TankMovementEnd);

TankMovementEnd::TankMovementEnd() :
	playerId_(0)
{
}

TankMovementEnd::TankMovementEnd(
	Vector &position,
	unsigned int playerId) :
	position_(position),
	playerId_(playerId)
{
}

TankMovementEnd::~TankMovementEnd()
{
}

void TankMovementEnd::init()
{
}

void TankMovementEnd::simulate(float frameTime, bool &remove)
{
	Tank *current = 
		context_->tankContainer->getTankById(playerId_);
	if (current)
	{
		current->getLife().setRotation(0.0f);
		if (current->getState().getState() == TankState::sNormal)
		{
			// Move the tank to the final position
			current->setTargetPosition(position_);
			DeformLandscape::flattenArea(*context_, position_, 0);
		}
	}

	std::map<unsigned int, TankMovement *>::iterator findItor =
		TankMovement::movingTanks.find(playerId_);
	if (findItor != TankMovement::movingTanks.end())
	{
		(*findItor).second->remove();
	}
	remove=true;
}

bool TankMovementEnd::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool TankMovementEnd::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}
