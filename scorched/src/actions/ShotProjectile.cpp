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

#include <actions/ShotProjectile.h>
#include <sprites/MissileActionRenderer.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <math.h>

REGISTER_ACTION_SOURCE(ShotProjectile);

Vector ShotProjectile::lookatPosition_;
unsigned int ShotProjectile::lookatCount_ = 0;

ShotProjectile::ShotProjectile() : 
	collisionInfo_(CollisionIdShot), vPoint_(0)
{

}

ShotProjectile::ShotProjectile(Vector &startPosition, Vector &velocity,
							   Weapon *weapon, unsigned int playerId,
							   unsigned int flareType,
							   bool under) : 
	collisionInfo_(CollisionIdShot), startPosition_(startPosition),
	velocity_(velocity), weapon_(weapon), playerId_(playerId), 
	flareType_(flareType), under_(under), vPoint_(0)
{

}

void ShotProjectile::init()
{
	if (!context_->serverMode) 
	{
		setActionRender(new MissileActionRenderer(flareType_, weapon_->getScale()));
	}

	vPoint_ = context_->viewPoints.getNewViewPoint(playerId_);
	setPhysics(startPosition_, velocity_);
	collisionInfo_.data = this;
	collisionInfo_.collisionOnSurface = !under_;
	physicsObject_.setData(&collisionInfo_);
}

ShotProjectile::~ShotProjectile()
{
	if (vPoint_) context_->viewPoints.releaseViewPoint(vPoint_);
}

void ShotProjectile::collision(Vector &position)
{
	std::map<unsigned int, Tank *> &tanks = 
		context_->tankContainer.getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;

		TankAI *ai = tank->getTankAI();
		if (ai)
		{		
			if (tank->getState().getState() == TankState::sNormal)
			{
				ai->shotLanded(weapon_, playerId_, position);
			}
		}
	}
	PhysicsParticleMeta::collision(position);
}

void ShotProjectile::simulate(float frameTime, bool &remove)
{
	if (vPoint_)
	{
		vPoint_->setPosition(getCurrentPosition());

		Vector velocity = -getCurrentVelocity();
		velocity[2] = fabs(velocity[2]) + 4.0f;
		vPoint_->setLookFrom(velocity);
	}
	PhysicsParticleMeta::simulate(frameTime, remove);
}

bool ShotProjectile::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(startPosition_[0]);
	buffer.addToBuffer(startPosition_[1]);
	buffer.addToBuffer(startPosition_[2]);
	buffer.addToBuffer(velocity_[0]);
	buffer.addToBuffer(velocity_[1]);
	buffer.addToBuffer(velocity_[2]);
	Weapon::write(buffer, weapon_);
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(flareType_);
	buffer.addToBuffer(under_);
	return true;
}

bool ShotProjectile::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(startPosition_[0])) return false;
	if (!reader.getFromBuffer(startPosition_[1])) return false;
	if (!reader.getFromBuffer(startPosition_[2])) return false;
	if (!reader.getFromBuffer(velocity_[0])) return false;
	if (!reader.getFromBuffer(velocity_[1])) return false;
	if (!reader.getFromBuffer(velocity_[2])) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(flareType_)) return false;
	if (!reader.getFromBuffer(under_)) return false;
	return true;
}
