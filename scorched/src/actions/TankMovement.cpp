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
#include <actions/TankMove.h>
#include <actions/ShotProjectile.h>
#include <engine/ScorchedContext.h>

static const int NoMovementTransitions = 4;

REGISTER_ACTION_SOURCE(TankMovement);

TankMovement::TankMovement() : timePassed_(0.0f), vPoint_(0)
{
}

TankMovement::TankMovement(unsigned int playerId) : 
	playerId_(playerId), timePassed_(0.0f), vPoint_(0)
{
}

TankMovement::~TankMovement()
{
	if (vPoint_) context_->viewPoints.releaseViewPoint(vPoint_);
}

void TankMovement::init()
{
	vPoint_ = context_->viewPoints.getNewViewPoint(playerId_);
	std::list<unsigned int>::iterator itor;
	for (itor = positions_.begin();
		itor != positions_.end();)
	{
		unsigned int fistpt = (*itor);
		itor++;

		if (itor != positions_.end())
		{
			unsigned int secpt = (*itor);

			float firstx = float(fistpt >> 16);
			float firsty = float(fistpt & 0xffff);
			float *firstHeight = 
				&context_->landscapeMaps.getHMap().getHeight(int(firstx), int(firsty));
			float secx = float(secpt >> 16);
			float secy = float(secpt & 0xffff);
			float *secondHeight = 
				&context_->landscapeMaps.getHMap().getHeight(int(secx), int(secy));

			float diffX = secx - firstx;
			float diffY = secy - firsty;
			float ang = (atan2f(diffY, diffX) / 3.14f * 180.0f) - 90.0f;

			for (int i=0; i<NoMovementTransitions; i++)
			{
				float currentX = firstx + diffX/float(NoMovementTransitions)*float(i+1);
				float currentY = firsty + diffY/float(NoMovementTransitions)*float(i+1);
				expandedPositions_.push_back(
					PositionEntry(
						currentX, currentY, 
						ang, (i==(NoMovementTransitions-1)),
						firstHeight, secondHeight));
			}
		}
	}
}

void TankMovement::simulate(float frameTime, bool &remove)
{
	Tank *tank = 
		context_->tankContainer.getTankById(playerId_);
	if (tank)
	{
		if (vPoint_) vPoint_->setPosition(tank->getPhysics().getTankPosition());

		if (tank->getState().getState() == TankState::sNormal)
		{
			// Check to see if this tank is falling
			// If it is then we end the move
			std::set<unsigned int>::iterator findItor =
				TankFalling::fallingTanks.find(playerId_);
			if (findItor == TankFalling::fallingTanks.end())
			{
				// Move the tank one position every 0.1 seconds
				// i.e. 10 positions a second
				timePassed_ += frameTime;
				const float stepsPerFrame = 0.05f;
				while (timePassed_ >= stepsPerFrame)
				{
					timePassed_ -= stepsPerFrame;
					if (!expandedPositions_.empty())
					{
						moveTank(tank);
					}
					else break;
				}

				if (expandedPositions_.empty()) remove = true;
			}
			else remove = true;
		}
		else remove = true;

		// If this is the very last movement made
		// Ensure all tanks always end in the same place
		if (remove)
		{
			tank->getPhysics().rotateTank(0.0f);
			context_->actionController.addAction(
				new TankMove(tank->getPhysics().getTankPosition(),
					tank->getPlayerId(), false));
		}
	}
	else remove = true;
	
	ActionMeta::simulate(frameTime, remove);
}

void TankMovement::moveTank(Tank *tank)
{
	float x = expandedPositions_.front().x;
	float y = expandedPositions_.front().y;
	float a = expandedPositions_.front().ang;
	bool useF = expandedPositions_.front().useFuel;
	float *firstz = expandedPositions_.front().heighta;
	float *secondz = expandedPositions_.front().heightb;
	float z = context_->landscapeMaps.getHMap().getInterpHeight(x, y);
	expandedPositions_.pop_front();

	// Form the new tank position
	Vector newPos(x, y, z);

	// Check we are not trying to climb to high (this may be due
	// to the landscape changing after we started move)
	if (*secondz - *firstz > MaxTankClimbHeight)
	{
		expandedPositions_.clear();
	}
	else
	{
		// Use up one unit of fuel
		if (useF) tank->getAccessories().getFuel().rmFuel(1);

		// Actually move the tank
		tank->getPhysics().rotateTank(a);
		tank->getPhysics().setTankPosition(newPos);
	}
}

bool TankMovement::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer((unsigned int) positions_.size());
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
