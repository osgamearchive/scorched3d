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

#include <actions/TankFalling.h>
#include <target/TargetContainer.h>
#include <target/TargetState.h>
#include <target/TargetParachute.h>
#include <target/TargetDamageCalc.h>
#include <target/TargetLife.h>
#include <tank/Tank.h>
#include <tank/TankAccessories.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Parachute.h>
#include <common/OptionsGame.h>
#include <landscapemap/DeformLandscape.h>

TankFalling::TankFalling(Weapon *weapon, unsigned int fallingPlayerId,
				   WeaponFireContext &weaponContext,
				   Parachute *parachute) :
	weapon_(weapon),
	fallingPlayerId_(fallingPlayerId),
	weaponContext_(weaponContext), parachute_(parachute)
{
}

TankFalling::~TankFalling()
{
	if (context_)
	{
		Target *target = context_->targetContainer->getTargetById(fallingPlayerId_);
		if (target)
		{
			if (target->getTargetState().getFalling() == this)
			{
				target->getTargetState().setFalling(0);
			}
		}
	}
}

void TankFalling::init()
{
	Target *current = 
		context_->targetContainer->getTargetById(fallingPlayerId_);
	if (current && 
		!current->getTargetState().getFalling() && 
		!current->getTargetState().getNoFalling())
	{
		current->getTargetState().setFalling(this);

		// Store the start positions
		tankStartPosition_ = current->getLife().getTargetPosition();

		Vector velocity(0.0f, 0.0f, 0.0f);
		PhysicsParticleInfo info(ParticleTypeFalling, fallingPlayerId_, this);
		setPhysics(info, tankStartPosition_, velocity, 
			0.0f, 0.0f, 0.0f, false);
	}
	else
	{
		collision_ = true;
	}
}

void TankFalling::simulate(float frameTime, bool &remove)
{
	if (!collision_)
	{
		// Slow falling
		if (parachute_)	applyForce(parachute_->getSlowForce());

		// Move the tank to the new position
		Target *target = context_->targetContainer->getTargetById(fallingPlayerId_);
		if (target && target->getAlive())
		{
			Vector &position = getCurrentPosition();
			if (position[0] != 0.0f || position[1] != 0.0f || position[2] != 0.0f)
			{
				target->getLife().setTargetPosition(position);
			}
		}
		else collision_ = true;
	}

	PhysicsParticleReferenced::simulate(frameTime, remove);
}

void TankFalling::collision(PhysicsParticleObject &position, 
	ScorchedCollisionId collisionId)
{
	Target *current = context_->targetContainer->getTargetById(fallingPlayerId_);
	if (current && current->getAlive())
	{
		// Find how far we have falled to get the total damage
		float dist = (tankStartPosition_ - position.getPosition()).Magnitude();
		float damage = dist * 20.0f;

		// Check we need to cancel the damage
		float minDist = float(context_->optionsGame->
			getMinFallingDistance()) / 10.0f;
		if (dist < minDist)
		{
			// No damage (or parachutes used for tiny falls)
			damage = 0.0f;
		}
		else
		if (parachute_)
		{
			const float ParachuteThreshold = 0.0f;
			if (dist >= ParachuteThreshold)
			{
				// No damage we were using parachutes
				damage = 0.0f;

				// Remove parachutes if we have one
				if (!current->isTarget())
				{
					Tank *currentTank = (Tank *) current;
					currentTank->getAccessories().rm(parachute_->getParent());
					if (currentTank->getAccessories().getAccessoryCount(parachute_->getParent()) == 0)
					{
						current->getParachute().setCurrentParachute(0);
					}
				}
			}
		}

		// Move the tank to the final position
		current->getLife().setTargetPosition(position.getPosition());

		// Flatten the area around tanks
		if (!current->isTarget())
		{
			DeformLandscape::flattenArea(*context_, position.getPosition());
		}

		// Add the damage to the tank
		TargetDamageCalc::damageTarget(
			*context_,
			current, weapon_, 
			weaponContext_, damage, 
			false, false, false);
	}

	PhysicsParticleReferenced::collision(position, collisionId);
}
