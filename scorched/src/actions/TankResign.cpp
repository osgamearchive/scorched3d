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


#include <actions/TankResign.h>
#include <tank/TankContainer.h>
#include <common/Defines.h>
#include <common/Logger.h>

REGISTER_ACTION_SOURCE(TankResign);

TankResign::TankResign() : firstTime_(true)
{
}

TankResign::TankResign(unsigned int playerId) :
	firstTime_(true),
	playerId_(playerId)
{

}

TankResign::~TankResign()
{
}

void TankResign::init()
{
}

void TankResign::simulate(float frameTime, bool &remove)
{
	if (firstTime_)
	{
		firstTime_ = false;
		Tank *tank = 
			TankContainer::instance()->getTankById(playerId_);
		if (tank)
		{
			tank->getState().setState(TankState::sDead);

			Logger::log(tank, "\"%s\" resigned from round", tank->getName());
		}
	}

	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool TankResign::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	return true;
}

bool TankResign::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}
