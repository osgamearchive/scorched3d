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
#include <tankgraph/RenderTracer.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <tankai/TankAI.h>
#include <common/Defines.h>
#include <engine/ScorchedContext.h>
#include <engine/ViewPoints.h>
#include <weapons/AccessoryStore.h>
#include <math.h>

ShotProjectile::ShotProjectile(Vector &startPosition, Vector &velocity,
							   WeaponProjectile *weapon, WeaponFireContext &weaponContext,
							   unsigned int flareType) :
	startPosition_(startPosition), velocity_(velocity), 
	weapon_(weapon), weaponContext_(weaponContext), 
	flareType_(flareType), vPoint_(0),
	snapTime_(0.2f), up_(false),
	totalTime_(0.0)
{

}

void ShotProjectile::init()
{
#ifndef S3D_SERVER
	if (!context_->serverMode) 
	{
		setActionRender(new MissileActionRenderer(flareType_, weapon_->getScale()));
	}
#endif // #ifndef S3D_SERVER

	vPoint_ = context_->viewPoints->getNewViewPoint(weaponContext_.getPlayerId());
	PhysicsParticleInfo info(ParticleTypeShot, weaponContext_.getPlayerId(), this);
	setPhysics(info, startPosition_, velocity_, 
		0.0f, 0.0f, weapon_->getWindFactor(), getWeapon()->getUnder());
}

ShotProjectile::~ShotProjectile()
{
	if (vPoint_) context_->viewPoints->releaseViewPoint(vPoint_);
}

void ShotProjectile::collision(PhysicsParticleObject &position, 
	ScorchedCollisionId collisionId)
{
	if (!collision_)
	{
		// Tell all AIs about this collision
		std::map<unsigned int, Tank *> tanks = 
			context_->tankContainer->getAllTanks();
		std::map<unsigned int, Tank *>::iterator itor;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			TankAI *ai = tank->getTankAI();
			if (ai)
			{		
				if (tank->getState().getState() == TankState::sNormal &&
					!tank->getState().getSpectator())
				{
					ai->shotLanded(collisionId, 
						getWeapon(), getPlayerId(), 
						getCurrentPosition());
				}
			}
		}

		bool doColl = true;

		// Apex collisions dud if they collide with the ground
		// unless no dud is set
		if (getWeapon()->getApexCollision() && !getWeapon()->getApexNoDud())
		{
			doColl = false;
		}
		if ((getWeapon()->getTimedCollision() > 0.0f) && getWeapon()->getTimedDud())
		{
			doColl = false;
		}

		if (doColl) doCollision(position.getPosition());
	}
	PhysicsParticleReferenced::collision(position, collisionId);
}

void ShotProjectile::simulate(float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (vPoint_)
	{
		vPoint_->setPosition(getCurrentPosition());

		Vector velocity = -getCurrentVelocity();
		velocity[2] = 10.0f;
		vPoint_->setLookFrom(velocity);
	}

	// Apex collision
	if (getWeapon()->getApexCollision())
	{
		if (getCurrentVelocity()[2] > 0.0f) up_ = true;
		else if (up_)
		{
			doCollision(getCurrentPosition());
			remove = true;
		}
	}

	// Timed collision
	if (getWeapon()->getTimedCollision() > 0.0f)
	{
		if (totalTime_ > getWeapon()->getTimedCollision())
		{
			doCollision(getCurrentPosition());
			remove = true;
		}
	}

	// Shot path
	if (getWeapon()->getShowShotPath())
	{
		snapTime_ += frameTime;
		if (snapTime_ > 0.1f || remove)
		{
			Vector up (0.0f, 0.0f, 1.0f);
			RenderTracer::TracerLinePoint point;
			point.position = getCurrentPosition();
			point.cross = (getCurrentVelocity() * up).Normalize();
			positions_.push_back(point);
			snapTime_ = 0.0f;
		}
	}

	PhysicsParticleReferenced::simulate(frameTime, remove);
}

void ShotProjectile::doCollision(Vector &position)
{	
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		if (getWeapon()->getShowShotPath())
		{
			RenderTracer::instance()->
				addSmokeTracer(weaponContext_.getPlayerId(), position, positions_);
		}
		else if (getWeapon()->getShowEndPoint())
		{
			RenderTracer::instance()->
				addTracer(weaponContext_.getPlayerId(), position);
		}
	}
#endif // #ifndef S3D_SERVER

	Vector velocity;
	getWeapon()->getCollisionAction()->fireWeapon(
		*context_, weaponContext_, position, getCurrentVelocity());
}

