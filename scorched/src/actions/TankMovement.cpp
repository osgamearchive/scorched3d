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


#include <actions/TankMovement.h>
#include <actions/TankFalling.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <landscape/GlobalHMap.h>

REGISTER_ACTION_SOURCE(TankMovement);

TankMovement::TankMovement() : timePassed_(0.0f)
{
}

TankMovement::TankMovement(unsigned int playerId) : 
	playerId_(playerId), timePassed_(0.0f)
{
}

TankMovement::~TankMovement()
{
}

void TankMovement::init()
{

}

void TankMovement::simulate(float frameTime, bool &remove)
{
	Tank *tank = 
		TankContainer::instance()->getTankById(playerId_);
	if (tank && 
		tank->getState().getState() == TankState::sNormal)
	{
		// Check to see if this tank is falling
		// If it is then we end the move
		std::set<unsigned int>::iterator findItor =
			TankFalling::fallingTanks.find(playerId_);
		if (findItor == TankFalling::fallingTanks.end())
		{
			// Move the tank one square every 0.25 seconds
			// i.e. 4 squares a second
			timePassed_ += frameTime;
			const float stepsPerFrame = 0.25f;
			while (timePassed_ >= stepsPerFrame)
			{
				timePassed_ -= stepsPerFrame;
				if (!positions_.empty())
				{
					unsigned int pt = positions_.front();
					positions_.pop_front();

					unsigned int x = pt >> 16;
					unsigned int y = pt & 0xffff;
					float z = GlobalHMap::instance()->getHMap().getHeight(
						(int) x, (int) y);
					
					// Form the new tank position
					Vector newPos((float) x, (float) y, (float) z);

					// Check we are not trying to climb to high (this may be due
					// to the landscape changing after we started move)
					Vector &lastPosition = tank->getPhysics().getTankPosition();
					if (newPos[2] - lastPosition[2] > MaxTankClimbHeight)
					{
						positions_.clear();
					}
					else
					{
						// Use up one unit of fuel
						tank->getAccessories().getFuel().rmFuel(1);

						// Actually move the tank
						tank->getPhysics().setTankPosition(newPos);
					}
				}
			}

			if (positions_.empty()) remove = true;
		}
		else
		{
			remove = true;
		}
	}
	else
	{
		remove = true;
	}
	
	ActionMeta::simulate(frameTime, remove);
}

bool TankMovement::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(positions_.size());
	std::list<unsigned int>::iterator itor;
	for (itor = positions_.begin();
		itor != positions_.end();
		itor++)
	{
		buffer.addToBuffer((*itor));
	}

	return true;
}

bool TankMovement::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	unsigned int size, pt;
	if (!reader.getFromBuffer(size)) return false;
	for (;size>0; size--)
	{
		if (!reader.getFromBuffer(pt)) return false;
		positions_.push_back(pt);
	}
	return true;
}
