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

#include <actions/ShotProjectileLeapFrog.h>
#include <actions/Explosion.h>
#include <engine/ScorchedContext.h>

REGISTER_ACTION_SOURCE(ShotProjectileLeapFrog);

ShotProjectileLeapFrog::ShotProjectileLeapFrog()
{

}

ShotProjectileLeapFrog::ShotProjectileLeapFrog(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId,
	float width, int hopsLeft) :
	ShotProjectileExplosion(startPosition, velocity, weapon, playerId, width),
	hopsLeft_(hopsLeft)
{
}

ShotProjectileLeapFrog::~ShotProjectileLeapFrog()
{
}

bool ShotProjectileLeapFrog::writeAction(NetBuffer &buffer)
{
	if (!ShotProjectileExplosion::writeAction(buffer)) return false;
	buffer.addToBuffer(hopsLeft_);
	return true;
}

bool ShotProjectileLeapFrog::readAction(NetBufferReader &reader)
{
	if (!ShotProjectileExplosion::readAction(reader)) return false;
	if (!reader.getFromBuffer(hopsLeft_)) return false;
	return true;
}

void ShotProjectileLeapFrog::collision(Vector &position)
{
	ShotProjectileExplosion::collision(position);

	if (hopsLeft_ > 0)
	{
		Vector newVelocity = velocity_ * 0.6f;
		Action *action = new ShotProjectileLeapFrog(
			position, 
			newVelocity,
			weapon_, playerId_, width_ - 1, hopsLeft_ - 1);
		context_->actionController.addAction(action);
	}
}
