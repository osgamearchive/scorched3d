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


#include <actions/TankMove.h>
#include <tank/TankContainer.h>

REGISTER_ACTION_SOURCE(TankMove);

TankMove::TankMove()
{
}

TankMove::TankMove(Vector &position, 
		unsigned int playerId,
		bool useParachute) : 
	position_(position), playerId_(playerId), useParachute_(useParachute)
{
}

TankMove::~TankMove()
{
}

void TankMove::init()
{

}

void TankMove::simulate(float frameTime, bool &remove)
{
	Tank *tank = 
		TankContainer::instance()->getTankById(playerId_);
	if (tank && 
		tank->getState().getState() == TankState::sNormal)
	{
		// Set the new tank position
		tank->getPhysics().setTankPosition(position_);

		// Check if we use parachute
		if (useParachute_)
		{
			tank->getAccessories().getParachutes().useParachutes();
		}
	}

	remove = true;
	ActionMeta::simulate(frameTime, remove);
}

bool TankMove::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	buffer.addToBuffer(useParachute_);
	return true;
}

bool TankMove::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	if (!reader.getFromBuffer(useParachute_)) return false;
	return true;
}
