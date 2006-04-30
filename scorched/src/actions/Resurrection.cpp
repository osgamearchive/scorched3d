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
#include <landscape/DeformLandscape.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <tank/TankContainer.h>

REGISTER_ACTION_SOURCE(Resurrection);

Resurrection::Resurrection()
{
}

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
		LoggerInfo info(LoggerInfo::TypeDeath,
			formatString("\"%s\" was resurrected, %i lives remaining",
				tank->getName(),
				tank->getState().getLives()));
		info.setPlayerId(playerId_);
		Logger::log(info);

		// Rez this tank
		tank->rezTank();
		tank->setTargetPosition(position_);
		DeformLandscape::flattenArea(*context_, position_, 0);
	}

	Action::simulate(frameTime, remove);
}

bool Resurrection::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool Resurrection::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}
