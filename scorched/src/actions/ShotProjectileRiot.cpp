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

#include <actions/ShotProjectileRiot.h>
#include <actions/Explosion.h>
#include <engine/ScorchedContext.h>

REGISTER_ACTION_SOURCE(ShotProjectileRiot);

ShotProjectileRiot::ShotProjectileRiot()
{
}

ShotProjectileRiot::ShotProjectileRiot(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId, float width) :
		ShotProjectile(startPosition, velocity, weapon, playerId), 
		width_(width)
{

}

ShotProjectileRiot::~ShotProjectileRiot()
{

}

bool ShotProjectileRiot::writeAction(NetBuffer &buffer)
{
	if (!ShotProjectile::writeAction(buffer)) return false;
	buffer.addToBuffer(width_);
	return true;
}

bool ShotProjectileRiot::readAction(NetBufferReader &reader)
{
	if (!ShotProjectile::readAction(reader)) return false;
	if (!reader.getFromBuffer(width_)) return false;
	return true;
}

void ShotProjectileRiot::collision(Vector &position)
{
	context_->actionController.addAction(
		new Explosion(position, width_, weapon_, playerId_, 
			false)); // Hurt = false

	ShotProjectile::collision(position);
}
