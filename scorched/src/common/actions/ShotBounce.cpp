////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <actions/ShotBounce.h>
#include <actions/CameraPositionAction.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/WeaponRoller.h>
#include <weapons/AccessoryStore.h>
#include <common/OptionsScorched.h>
#include <tank/TankViewPoints.h>
#include <string.h>

ShotBounce::ShotBounce(WeaponRoller *weapon, 
		FixedVector &startPosition, FixedVector &velocity,
		WeaponFireContext &weaponContext) : 
	PhysicsParticle(weaponContext.getInternalContext().getReferenced()),
	startPosition_(startPosition),
	velocity_(velocity), weapon_(weapon), weaponContext_(weaponContext),
	totalTime_(0), simulateTime_(0),
	vPoint_(0), groups_(0)
{
}

void ShotBounce::init()
{
	PhysicsParticleInfo info(ParticleTypeBounce, weaponContext_.getPlayerId(), this);
	getPhysics().setPhysics(info, *context_, startPosition_, velocity_);
	getPhysics().setForces(weapon_->getWindFactor(*context_), weapon_->getGravityFactor(*context_));
	getPhysics().setOptionRotateOnCollision(weapon_->getRoll());
	getPhysics().setOptionStickyShields(weapon_->getStickyShields());
	getPhysics().setOptionLandscapeCollision(getWeapon()->getLandscapeCollision());
	getPhysics().setOptionShieldCollision(getWeapon()->getShieldCollision());

	stepSize_ = weapon_->getStepSize() * 
		fixed(true, context_->getOptionsGame().getWeaponSpeed());
	weaponTime_ = weapon_->getTime(*context_);
	scale_ = weapon_->getScale(*context_).asFloat();
	if (!context_->getServerMode()) 
	{
		if (!weapon_->getNoCameraTrack())
		{
			vPoint_ = new TankViewPointProvider();
			vPoint_->incrementReference();
			vPoint_->setValues(startPosition_);

			// Point the action camera at this event
			CameraPositionAction *pos = new CameraPositionAction(
				weaponContext_.getPlayerId(), vPoint_, 5, 5, false);
			context_->getActionController().addAction(pos);
		}
	}

	if (weapon_->getLocalGroups().hasGroups() || weapon_->getGlobalGroups().hasGroups())
	{
		groups_ = new ParticleGroup(*context_, this, &weaponContext_);
		weapon_->getLocalGroups().addToGroups(weaponContext_.getInternalContext().getLocalGroups(), groups_);
		weapon_->getGlobalGroups().addToGroups(context_->getObjectGroups(), groups_);
	}
}

ShotBounce::~ShotBounce()
{
	if (vPoint_) vPoint_->decrementReference();
	delete groups_;
}

std::string ShotBounce::getActionDetails()
{
	return S3D::formatStringBuffer("%s %s %s",
		startPosition_.asQuickString(),
		velocity_.asQuickString(),
		weapon_->getParent()->getName());
}

void ShotBounce::collision(PhysicsParticleObject &position, 
	ScorchedCollisionId collisionId)
{
	if (!collision_)
	{
		doCollision();
	}
	PhysicsParticle::collision(position, collisionId);
}

void ShotBounce::simulate(fixed frameTime, bool &remove)
{
	totalTime_ += frameTime;
	if (totalTime_ > weaponTime_)
	{
		doCollision();
		remove = true;
	}

	simulateTime_ += frameTime;
	while (simulateTime_ > stepSize_)
	{
		PhysicsParticle::simulate(stepSize_, remove);
		simulateTime_ -= stepSize_;
	}
}

void ShotBounce::draw()
{
#ifndef S3D_SERVER
	if (!context_->getServerMode()) 
	{
		static float rotMatrix[16];
		getPhysics().getRotationQuat().getOpenGLRotationMatrix(rotMatrix);


		if (vPoint_)
		{
			vPoint_->setValues(getPhysics().getPosition(), lookFrom_);
		}
	}
#endif // #ifndef S3D_SERVER
}

void ShotBounce::doCollision()
{
	WeaponRoller *proj = (WeaponRoller *) weapon_;
	proj->getCollisionAction()->fire(
		*context_, weaponContext_, 
		getPhysics().getPosition(), 
		getPhysics().getVelocity());
}
