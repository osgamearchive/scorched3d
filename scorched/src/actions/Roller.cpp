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

#include <landscape/Landscape.h>
#include <engine/ScorchedContext.h>
#include <tank/TankController.h>
#include <actions/Roller.h>
#include <actions/Explosion.h>
#include <sprites/RollerRenderer.h>
#include <common/OptionsParam.h>

REGISTER_ACTION_SOURCE(Roller);

Roller::Roller() : totalTime_(0.0f), rollerTime_(0.0f)
{
}

Roller::Roller(int x, int y, Weapon *weapon, unsigned int playerId) :
	x_(x), y_(y), 
	weapon_((WeaponRoller *) weapon), 
	playerId_(playerId), 
	totalTime_(0.0f), rollerTime_(0.0f)
{

}

Roller::~Roller()
{

}

void Roller::init()
{
	if (!context_->serverMode) 
	{
		setActionRender(new RollerRenderer);
	}
}

void Roller::simulate(float frameTime, bool &remove)
{
	// Move roller for the period of the time interval
	// once the time interval has expired then start taking it away
	// Once all Roller has disapeared the simulation is over
	const float StepTime = weapon_->getStepTime();

	totalTime_ += frameTime;
	while (totalTime_ > StepTime)
	{
		totalTime_ -= StepTime;
		rollerTime_ += StepTime;

		if (!simulateMoveStep() ||
			rollerTime_ > weapon_->getRollerTime())
		{
			remove = true;
			finished();
			break;
		}
	}

	Action::simulate(frameTime, remove);
}

float Roller::getHeight(int x, int y)
{
	LandscapeMaps *hmap = &context_->landscapeMaps;
	if (x < 0 || y < 0 ||
		x > hmap->getHMap().getWidth() ||
		y > hmap->getHMap().getWidth())
	{
		// The height at the sides of the landscape is huge
		// so we will never go there with the roller
		return 9999999.0f;
	}

	unsigned int pos = (x << 16) | (y & 0xffff);
	if (usedPoints_.find(pos) != usedPoints_.end())
	{
		return 9999999.0f;
	}

	// Return the correct height the square
	return hmap->getHMap().getHeight(x, y);
}

bool Roller::simulateMoveStep()
{
	const float rollerHeight = weapon_->getRollerHeight();

	// Get the height of this point
	float height = getHeight(x_, y_);

	// Roller does not go under water (for now)
	// Return finished
	if (height < Landscape::instance()->getWater().getHeight())
	{
		return false;
	}

	// Add this point to the list of used points
	// so we don't go here again
	unsigned int pos = (x_ << 16) | (y_ & 0xffff);
	usedPoints_.insert(pos);

	// Add on some height so the roller can cover small rises
	height += rollerHeight;

	// Calculate every time as the landscape may change
	// due to other actions
	float heightL = getHeight(x_-1, y_);
	float heightR = getHeight(x_+1, y_);
	float heightU = getHeight(x_, y_+1);
	float heightD = getHeight(x_, y_-1);

	// Find the new point to move to (if any)
	// This point must be lower than the current point
	if (heightL < height &&
		heightL < heightR &&
		heightL < heightU &&
		heightL < heightD)
	{
		// Move left
		x_ -= 1;
	}
	else if (heightR < height &&
		heightR < heightL &&
		heightR < heightU &&
		heightR < heightD)
	{
		// Move right
		x_ += 1;
	}
	else if (heightU < height &&
		heightU < heightL &&
		heightU < heightR &&
		heightU < heightD)
	{
		// Move up
		y_ += 1;
	}
	else if (heightD < height)
	{
		// Move down
		y_ -= 1;
	}
	else
	{
		// None of the landscape is currently lower than the current point
		return false;
	}

	return true;
}

void Roller::finished()
{
	float z = context_->landscapeMaps.getHMap().getHeight(x_, y_);
	Vector position(x_, y_, (int) z);

	context_->actionController.addAction(
		new Explosion(position, (float) weapon_->getSize(), weapon_, playerId_));
}

bool Roller::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(x_);
	buffer.addToBuffer(y_);
	Weapon::write(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	return true;
}

bool Roller::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(x_)) return false;
	if (!reader.getFromBuffer(y_)) return false;
	weapon_ = (WeaponRoller *) Weapon::read(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	return true;
}
