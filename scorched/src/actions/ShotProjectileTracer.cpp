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

#include <actions/ShotProjectileTracer.h>
#include <sprites/TracerActionRenderer.h>
#include <common/OptionsParam.h>
#include <engine/ScorchedContext.h>
#include <tankgraph/TankRenderer.h>

REGISTER_ACTION_SOURCE(ShotProjectileTracer);

ShotProjectileTracer::ShotProjectileTracer() : snapTime_(0.2f)
{

}

ShotProjectileTracer::ShotProjectileTracer(
	Vector &startPosition, Vector &velocity,
	Weapon *weapon, unsigned int playerId, bool smokeTracer) :
		ShotProjectile(startPosition, velocity, weapon, playerId),
		smokeTracer_(smokeTracer), snapTime_(0.2f)
{

}

ShotProjectileTracer::~ShotProjectileTracer()
{

}

void ShotProjectileTracer::init()
{
	ShotProjectile::init();

	if (!context_->serverMode)
	{
		setActionRender(new TracerActionRenderer);
	}
}

bool ShotProjectileTracer::writeAction(NetBuffer &buffer)
{
	if (!ShotProjectile::writeAction(buffer)) return false;
	buffer.addToBuffer(smokeTracer_);
	return true;
}

bool ShotProjectileTracer::readAction(NetBufferReader &reader)
{
	if (!ShotProjectile::readAction(reader)) return false;
	if (!reader.getFromBuffer(smokeTracer_)) return false;
	return true;
}

void ShotProjectileTracer::simulate(float frametime, bool &remove)
{
	if (smokeTracer_)
	{
		snapTime_ += frametime;
		if (snapTime_ > 0.1f)
		{
			positions_.push_back(getCurrentPosition());
			snapTime_ = 0.0f;
		}
	}
	ShotProjectile::simulate(frametime, remove);
}

void ShotProjectileTracer::collision(Vector &position)
{
	if (!context_->serverMode)
	{
		if (smokeTracer_)
		{
			TankRenderer::instance()->getTracerStore().
				addSmokeTracer(playerId_, position, positions_);
		}
		else
		{
			TankRenderer::instance()->getTracerStore().
				addTracer(playerId_, position);
		}
	}
	ShotProjectile::collision(position);
}
