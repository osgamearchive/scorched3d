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


// ShotProjectileMirv.cpp: implementation of the ShotProjectileMirv class.
//
//////////////////////////////////////////////////////////////////////

#include <actions/ShotProjectileMirv.h>
#include <actions/ShotProjectileExplosion.h>
#include <engine/ActionController.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

REGISTER_ACTION_SOURCE(ShotProjectileMirv);

ShotProjectileMirv::ShotProjectileMirv() : up_(false)
{

}

ShotProjectileMirv::ShotProjectileMirv(
		Vector &startPosition, Vector &velocity,
		Weapon *weapon, unsigned int playerId, float width, 
		int noWarheads, bool spread) :
	ShotProjectile(startPosition, velocity, weapon, playerId),
	width_(width), noWarheads_(noWarheads), spread_(spread),
	up_(false)
{

}

ShotProjectileMirv::~ShotProjectileMirv()
{

}

bool ShotProjectileMirv::writeAction(NetBuffer &buffer)
{
	if (!ShotProjectile::writeAction(buffer)) return false;
	buffer.addToBuffer(width_);
	buffer.addToBuffer(noWarheads_);
	buffer.addToBuffer(spread_);
	return true;
}

bool ShotProjectileMirv::readAction(NetBufferReader &reader)
{
	if (!ShotProjectile::readAction(reader)) return false;
	if (!reader.getFromBuffer(width_)) return false;
	if (!reader.getFromBuffer(noWarheads_)) return false;
	if (!reader.getFromBuffer(spread_)) return false;
	return true;
}

void ShotProjectileMirv::simulate(float timepassed, bool &remove)
{
	if (getCurrentVelocity()[2] > 0.0f) up_ = true;
	else if (up_)
	{
		apex(getCurrentPosition(),
			getCurrentVelocity());
		remove = true;
	}

	ShotProjectile::simulate(timepassed, remove);
}

void ShotProjectileMirv::apex(Vector &position, Vector &currentVelocity)
{
	// Add a shot that will fall where the original was aimed
	ShotProjectileExplosion *newShot = 
		new ShotProjectileExplosion(
			position, currentVelocity, 
			weapon_, playerId_, width_);
	ActionController::instance()->addAction(newShot);

	// Add all of the sub warheads that have a random spread
	for (int i=0; i<noWarheads_ - 1; i++)
	{
		Vector newDiff = currentVelocity;
		newDiff[2] = 0.0f;
		if (spread_)
		{
			Vector diff = newDiff;
			diff[2] -= 1.0f;
			Vector perp = newDiff * diff;

			newDiff += (perp * ((RAND * 1.0f) - 0.5f));
		}
		newDiff[2] += (float(i - (noWarheads_ / 2)) / float(noWarheads_ / 2)) * 5.0f;

		ShotProjectileExplosion *newShot = 
			new ShotProjectileExplosion(position, newDiff, 
			weapon_, playerId_, width_);

		ActionController::instance()->addAction(newShot);
	}

	ShotProjectile::collision(position);
}
