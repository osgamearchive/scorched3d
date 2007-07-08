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

#include <engine/PhysicsParticleObject.h>
#include <engine/ScorchedContext.h>
#include <engine/SyncCheck.h>
#include <engine/ActionController.h>
#include <landscapemap/LandscapeMaps.h>
#include <target/TargetSpace.h>
#include <target/TargetShield.h>
#include <target/TargetLife.h>
#include <tank/TankContainer.h>
#include <tank/TankPosition.h>
#include <weapons/Accessory.h>
#include <weapons/Shield.h>
#include <weapons/ShieldRoundReflective.h>
#include <weapons/ShieldSquareReflective.h>
#include <weapons/ShieldRoundMag.h>
#include <actions/ShotBounce.h>
#include <actions/ShotProjectile.h>
#include <actions/ShieldHit.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/OptionsTransient.h>

PhysicsParticleObject::PhysicsParticleObject() : 
	handler_(0), context_(0), underGroundCollision_(false), iterations_(0),
	info_(ParticleTypeNone, 0, 0), rotateOnCollision_(false)
{
}

PhysicsParticleObject::~PhysicsParticleObject()
{
}

void PhysicsParticleObject::applyForce(Vector &force)
{
	velocity_ += force;
}

void PhysicsParticleObject::setPhysics(
	PhysicsParticleInfo info,
	ScorchedContext &context, 
	Vector &position, Vector &velocity,
	float sphereSize, float sphereDensity, float windFactor,
	bool underGroundCollision, bool rotateOnCollision)
{
	info_ = info;
	context_ = &context;
	underGroundCollision_ = underGroundCollision;
	rotateOnCollision_ = rotateOnCollision;

	Vector zaxis(0.0f, 0.0f, 1.0f);
	rotation_.setQuatFromAxisAndAngle(zaxis, 0.0f);
	position_ = position;
	velocity_ = velocity * 1.2f;
	windFactor_ = 
		context_->optionsTransient->getWindDirection() * 
		context_->optionsTransient->getWindSpeed() / 
		2.5f * windFactor;

	/*SyncCheck::instance()->addString(*context_, 
		formatString("Init :%f %f,%f,%f %f",
		windFactor,
		context_->optionsTransient->getWindDirection()[0], 
		context_->optionsTransient->getWindDirection()[1], 
		context_->optionsTransient->getWindDirection()[2],
		context_->optionsTransient->getWindSpeed()));*/

	Vector gravity(0.0f, 0.0f, (float) context_->optionsGame->getGravity());
	windFactor_ += gravity;
	windFactor_ /= 70.0f;
}

void PhysicsParticleObject::simulate(float frameTime)
{
	iterations_++;
	velocity_ += windFactor_;
	position_ += velocity_ / 100.0f;

	if (rotateOnCollision_)
	{
		rotation_ += avelocity_;
		rotation_.Normalize();
	}

	/*SyncCheck::instance()->addString(*context_, 
		formatString("Move : %u P%f,%f,%f W%f,%f,%f",
			iterations_,
			position_[0], position_[1], position_[2],
			windFactor_[0], windFactor_[1], windFactor_[2]));*/

	if (!handler_ || !context_) return;

	checkCollision();
}

void PhysicsParticleObject::checkCollision()
{
	CollisionInfo collision;
	collision.collisionId = CollisionIdNone;

	// Find if we have had a collision
	Target *target = context_->targetSpace->getCollision(position_);
	if (getTargetBounceCollision(collision, target) ||
		getTargetCollision(collision, target) ||
		getShieldCollision(collision, target) ||
		getLandscapeCollision(collision) ||
		getRoofCollision(collision) ||
		getWallCollision(collision))
	{
		/*SyncCheck::instance()->addString(*context_, 
			formatString("Collision : %u %i P%f,%f,%f W%f,%f,%f N%f,%f,%f",
				iterations_,
				(int) collision.collisionId,
				position_[0], position_[1], position_[2],
				windFactor_[0], windFactor_[1], windFactor_[2],
				collision.normal[0], collision.normal[1], collision.normal[2]));*/

		// We have had a collision
		// Check what each of these shot types does with different collision type
		CollisionAction action = CollisionActionNone;
		switch (info_.type_)
		{
		case ParticleTypeShot:
			action = checkShotCollision(collision, target);
			break;
		case ParticleTypeBounce:
			action = checkBounceCollision(collision, target);
			break;
		case ParticleTypeFalling:
			action = checkFallingCollision(collision, target);
			break;
		}

		// Perform the result of the collision
		switch (action)
		{
		case CollisionActionCollision:
			handler_->collision(*this, collision.collisionId);
			break;
		case CollisionActionBounce:
			{
				float strength = velocity_.Magnitude();
				Vector direction = velocity_ / strength;
				float dotp = -collision.normal.dotP(direction);
				direction = direction + collision.normal * (2.0f * dotp);
				velocity_ = direction * strength * collision.deflectFactor;

				float landHeight = 
					context_->landscapeMaps->getGroundMaps().
						getInterpHeight(position_[0], position_[1]);
				float particleHeight = position_[2] - landHeight;
				if (underGroundCollision_)
				{
					if (particleHeight > 0.5f) position_[2] = landHeight + 0.5f;
				}
				else
				{
					if (particleHeight < -0.5f) position_[2] = landHeight - 0.5f;
				}

				if (rotateOnCollision_)
				{
					Vector up(0.0f, 0.0f, -1.0f);
					Vector rotAxis = velocity_ * up;
					rotAxis.StoreNormalize();
					avelocity_.setQuatFromAxisAndAngle(rotAxis, velocity_.Magnitude() * 5.0f);
				}
			}
			break;
		}
	}
	else
	{
		if (info_.type_ == ParticleTypeBounce)
		{
			velocity_[2] = MIN(velocity_[2], 10.0f);
		}
	}
}

PhysicsParticleObject::CollisionAction PhysicsParticleObject::checkShotCollision(
	CollisionInfo &collision, Target *target)
{
	switch(collision.collisionId)
	{
	case CollisionIdLandscape:
	case CollisionIdRoof:
	case CollisionIdTarget:
		return CollisionActionCollision;
		break;
	case CollisionIdShield:
		if (target && target->getShield().getCurrentShield())
		{
			shotShieldHit(target);

			if (target->getShield().getCurrentShield()) // shotShieldHit may have removed shield
			{
				Shield *shield = (Shield *) target->getShield().getCurrentShield()->getAction();
				switch (shield->getShieldType())
				{
				case Shield::ShieldTypeRoundNormal:
				case Shield::ShieldTypeSquareNormal:
					return CollisionActionCollision;
				case Shield::ShieldTypeRoundReflective:
				case Shield::ShieldTypeSquareReflective:
					return CollisionActionBounce;
				case Shield::ShieldTypeRoundMag:
					{
						ShieldRoundMag *magShield = (ShieldRoundMag *) shield;
						Vector force(0.0f, 0.0f, magShield->getDeflectPower() / 50.0f);
						velocity_ += force;
					}
					return CollisionActionNone;
				}
			}
		}
		break;
	case CollisionIdWallLeft:
	case CollisionIdWallRight:
	case CollisionIdWallTop:
	case CollisionIdWallBottom:

		shotWallHit(collision);

		handler_->wallCollision(*this, collision.collisionId);

		switch (context_->optionsTransient->getWallType())
		{
		case OptionsTransient::wallBouncy:
			return CollisionActionBounce;
		case OptionsTransient::wallWrapAround:

			switch (collision.collisionId)
			{
			case CollisionIdWallLeft:
				{
					int landscapeWidth = context_->landscapeMaps->getGroundMaps().getMapWidth();
					position_[0] = landscapeWidth - 10.0f;
				}
				break;
			case CollisionIdWallRight:
				position_[0] = 10.0f;
				break;
			case CollisionIdWallTop:
				{
					int landscapeHeight = context_->landscapeMaps->getGroundMaps().getMapHeight();
					position_[1] = landscapeHeight - 10.0f;
				}
				break;
			case CollisionIdWallBottom:
				position_[1] = 10.0f;
				break;
			}
			return CollisionActionNone;
		case OptionsTransient::wallConcrete:
			return CollisionActionCollision;
		}
		break;
	}
	return CollisionActionNone;
}

PhysicsParticleObject::CollisionAction PhysicsParticleObject::checkBounceCollision(
	CollisionInfo &collision, Target *target)
{
	switch(collision.collisionId)
	{
	case CollisionIdRoof:
		return CollisionActionNone;
		break;
	case CollisionIdLandscape:
	case CollisionIdWallLeft:
	case CollisionIdWallRight:
	case CollisionIdWallTop:
	case CollisionIdWallBottom:
		return CollisionActionBounce;
		break;
	case CollisionIdShield:
		bounceShieldHit(target);
		return CollisionActionBounce;
		break;
	case CollisionIdTarget:
		return CollisionActionCollision;
		break;
	}
	return CollisionActionNone;
}

PhysicsParticleObject::CollisionAction PhysicsParticleObject::checkFallingCollision(
	CollisionInfo &collision, Target *target)
{
	switch(collision.collisionId)
	{
	case CollisionIdTarget:
	case CollisionIdShield:
		return CollisionActionNone;
		break;
	case CollisionIdRoof:
	case CollisionIdWallLeft:
	case CollisionIdWallRight:
	case CollisionIdWallTop:
	case CollisionIdWallBottom:
		return CollisionActionBounce;
		break;
	case CollisionIdLandscape:
		return CollisionActionCollision;
		break;
	}
	return CollisionActionNone;
}

bool PhysicsParticleObject::getLandscapeCollision(CollisionInfo &collision)
{
	// Check for collision with the ground
	// (or underground collision if applicable)
	float landHeight = 
		context_->landscapeMaps->getGroundMaps().
			getInterpHeight(position_[0], position_[1]);
	if (underGroundCollision_)
	{
		if (position_[2] <= 0.0f) 
		{
			collision.collisionId = CollisionIdLandscape;
			collision.deflectFactor = 1.0f;
			collision.normal = Vector(0.0f, 0.0f, -1.0f);
			return true;
		}
		if (position_[2] >= landHeight)
		{
			collision.collisionId = CollisionIdLandscape;
			collision.deflectFactor = 1.0f;
			context_->landscapeMaps->getGroundMaps().
				getInterpNormal(position_[0], position_[1], collision.normal);
			collision.normal = -collision.normal;
			return true;
		}
	}
	else
	{
		if (position_[2] <= 0.0f)
		{
			collision.collisionId = CollisionIdLandscape;
			collision.deflectFactor = 1.0f;
			collision.normal = Vector(0.0f, 0.0f, 1.0f);
			return true;
		}
		if (position_[2] <= landHeight)
		{
			collision.collisionId = CollisionIdLandscape;
			collision.deflectFactor = 1.0f;
			context_->landscapeMaps->getGroundMaps().
				getInterpNormal(position_[0], position_[1], collision.normal);
			return true;
		}
	}
	return false;
}

bool PhysicsParticleObject::getRoofCollision(CollisionInfo &collision)
{
	// This will return MAX_FLT when there is no roof
	float maxHeight = context_->landscapeMaps->getRoofMaps().getInterpRoofHeight(
		position_[0], position_[1]);
	if (position_[2] >= maxHeight)
	{
		collision.collisionId = CollisionIdRoof;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(0.0f, 0.0f, -1.0f);
		return true;
	}
	return false;
}

bool PhysicsParticleObject::getWallCollision(CollisionInfo &collision)
{
	// Check for collision with the walls (if enabled)
	if (context_->optionsTransient->getWallType() == OptionsTransient::wallNone)
	{
		return false;
	}

	int landscapeWidth = context_->landscapeMaps->getGroundMaps().getMapWidth();
	int landscapeHeight = context_->landscapeMaps->getGroundMaps().getMapHeight();
	if (position_[0] <= 0.0f)
	{
		collision.collisionId = CollisionIdWallLeft;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(1.0f, 0.0f, 0.0f);
		return true;
	}
	else if (position_[0] >= landscapeWidth)
	{
		collision.collisionId = CollisionIdWallRight;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(-1.0f, 0.0f, 0.0f);
		return true;
	}
	else if (position_[1] <= 0.0f)
	{
		collision.collisionId = CollisionIdWallTop;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(0.0f, 1.0f, 0.0f);
		return true;
	}
	else if (position_[1] >= landscapeHeight)
	{
		collision.collisionId = CollisionIdWallBottom;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(0.0f, -1.0f, 0.0f);
		return true;
	}
	return false;
}

bool PhysicsParticleObject::getShieldCollision(CollisionInfo &collision, Target *target)
{
	if (!target) return false;

	// Get the shield
	Accessory *shieldAcc = target->getShield().getCurrentShield();			
	if (!shieldAcc) return false;
	Shield *shield = (Shield *) shieldAcc->getAction();

	// Is this tank in the shield
	// This should always be a tank as it is the one firing
	Tank *shotTank = context_->tankContainer->getTankById(info_.playerId_);
	if (shotTank)
	{
		Vector offset = shotTank->getPosition().getTankPosition() -
			target->getLife().getTargetPosition();
		if (shield->tankInShield(offset))
		{
			// We can ignore this shield as this tank is in the shield
			return false;
		}
	}

	// Check we are in this shield
	Vector direction = position_ - target->getLife().getTargetPosition();
	if (!shield->inShield(direction)) return false;

	// Perform the shield action
	switch (shield->getShieldType())
	{
	case Shield::ShieldTypeRoundMag:
		collision.collisionId = CollisionIdShield;
		collision.deflectFactor = 1.0f;
		collision.normal = Vector(0.0f, 0.0f, 1.0f);
		return true;
	case Shield::ShieldTypeRoundNormal:
	case Shield::ShieldTypeRoundReflective:
		{
			// Find the deflect factor
			float deflectFactor = 1.0f;
			if (shield->getShieldType() == Shield::ShieldTypeRoundReflective)
			{
				ShieldRoundReflective *squareRoundReflective = 
					(ShieldRoundReflective *) shield;
				deflectFactor = squareRoundReflective->getDeflectFactor();
			}

			collision.collisionId = CollisionIdShield;
			collision.deflectFactor = deflectFactor;
			collision.normal = (position_ - target->getLife().getTargetPosition()).Normalize();
		}
		return true;
	case Shield::ShieldTypeSquareReflective:
	case Shield::ShieldTypeSquareNormal:
		{
			// Find the deflect factor
			float deflectFactor = 1.0f;
			if (shield->getShieldType() == Shield::ShieldTypeSquareReflective)
			{
				ShieldSquareReflective *squareSquareReflective = 
					(ShieldSquareReflective *) shield;
				deflectFactor = squareSquareReflective->getDeflectFactor();
			}

			// Find the smallest penetration side
			ShieldSquare *squareShield = (ShieldSquare *) shield;
			Vector &size = squareShield->getSize();

			float diff0 = size[0] - fabs(direction[0]);
			float diff1 = size[1] - fabs(direction[1]);
			float diff2 = size[2] - fabs(direction[2]);
			if (diff0 <= diff1 && diff0 <= diff2)
			{
				if (direction[0] > 0.0f) collision.normal = Vector(-1.0f, 0.0f, 0.0f);
				else collision.normal = Vector(1.0f, 0.0f, 0.0f);
			}
			if (diff1 <= diff0 && diff1 <= diff2)
			{
				if (direction[1] > 0.0f) collision.normal = Vector(0.0f, -1.0f, 0.0f);
				else collision.normal = Vector(0.0f, 1.0f, 0.0f);
			}
			if (diff2 <= diff0 && diff2 <= diff1)
			{
				if (direction[2] > 0.0f) collision.normal = Vector(0.0f, 0.0f, -1.0f);
				else collision.normal = Vector(0.0f, 0.0f, 1.0f);
			}

			collision.collisionId = CollisionIdShield;
			collision.deflectFactor = deflectFactor;
		}
		return true;
	}

	return false;
}

bool PhysicsParticleObject::getTargetCollision(CollisionInfo &collision, Target *target)
{
	if (!target) return false;

	// We cannot collide with ourselves
	if (target->getPlayerId() == info_.playerId_) return false;

	// Check we are in this target
	if (target->getLife().collision(position_))
	{
		collision.normal = (position_ - target->getLife().getCenterPosition()).Normalize();	
		collision.deflectFactor = 1.0f;
		collision.collisionId = CollisionIdTarget;
		return true;
	}
	
	return false;
}

bool PhysicsParticleObject::getTargetBounceCollision(CollisionInfo &collision, Target *target)
{
	if (info_.type_ != ParticleTypeBounce) return false;

	// A special case, to add some width to the bounce particle to make it easier
	// to hit targets with
	std::map<unsigned int, Target *> collisionTargets;
	context_->targetSpace->getCollisionSet(position_, 3.0f, collisionTargets, false);
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = collisionTargets.begin();
		itor != collisionTargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		if (target->getLife().collision(position_) ||
			target->getLife().collisionDistance(position_) < 3.0f)
		{
			collision.normal = (position_ - target->getLife().getCenterPosition()).Normalize();	
			collision.deflectFactor = 1.0f;
			collision.collisionId = CollisionIdTarget;
			return true;
		}
	}

	return false;
}

void PhysicsParticleObject::shotShieldHit(Target *target)
{
}

void PhysicsParticleObject::bounceShieldHit(Target *target)
{
}

void PhysicsParticleObject::shotWallHit(CollisionInfo &collision)
{
}

PhysicsParticleActionObject::PhysicsParticleActionObject()
{
}

PhysicsParticleActionObject::~PhysicsParticleActionObject()
{
}

void PhysicsParticleActionObject::shotShieldHit(Target *target)
{
	Shield *shield = (Shield *) target->getShield().getCurrentShield()->getAction();
	ShotProjectile *shot = (ShotProjectile *) info_.data_;
	float hurtFactor = shot->getWeapon()->getShieldHurtFactor(*context_);
	if (hurtFactor > 0.0f)
	{
		if (shield->getShieldType() != Shield::ShieldTypeRoundMag)
		{
			context_->actionController->addAction(
				new ShieldHit(target->getPlayerId(),
					position_,
					hurtFactor));
		}
		else
		{
			target->getShield().setShieldPower(
				target->getShield().getShieldPower() -
				shield->getHitRemovePower() * hurtFactor);
		}
	}
}

void PhysicsParticleActionObject::bounceShieldHit(Target *target)
{
	Shield *shield = (Shield *) target->getShield().getCurrentShield()->getAction();
	ShotBounce *shot = (ShotBounce *) info_.data_;
	float hurtFactor = shot->getWeapon()->getShieldHurtFactor(*context_);
	if (shield->getShieldType() != Shield::ShieldTypeRoundMag && hurtFactor > 0.0f)
	{
		context_->actionController->addAction(
			new ShieldHit(target->getPlayerId(),
				position_,
				hurtFactor));
	}
}

#ifndef S3D_SERVER

#include <client/ScorchedClient.h>
#include <graph/ParticleEmitter.h>

static void addWallCollisionParticle(Vector &position, ScorchedCollisionId collisionId)
{
	Vector color(1.0f, 1.0f, 1.0f);
	ParticleEmitter emitter;
	emitter.setAttributes(
		8.5f, 8.0f, // Life
		0.2f, 0.5f, // Mass
		0.01f, 0.02f, // Friction
		Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), // Velocity
		color, 1.0f, // StartColor1
		color, 1.0f, // StartColor2
		color, 0.0f, // EndColor1
		color, 0.0f, // EndColor2
		2.0f, 2.0f, 2.0f, 2.0f, // Start Size
		2.0f, 2.0f, 2.0f, 2.0f, // EndSize
		Vector(0.0f, 0.0f, 0.0f), // Gravity
		false,
		false);

	switch (collisionId)
	{
	case CollisionIdWallLeft:
		emitter.emitWallHit(position,
			ScorchedClient::instance()->getParticleEngine(),
			OptionsTransient::LeftSide);
		break;
	case CollisionIdWallRight:
		emitter.emitWallHit(position,
			ScorchedClient::instance()->getParticleEngine(),
			OptionsTransient::RightSide);
		break;
	case CollisionIdWallTop:
		emitter.emitWallHit(position,
			ScorchedClient::instance()->getParticleEngine(),
			OptionsTransient::TopSide);
		break;
	case CollisionIdWallBottom:
		emitter.emitWallHit(position,
			ScorchedClient::instance()->getParticleEngine(),
			OptionsTransient::BotSide);
		break;
	}
}
#endif

void PhysicsParticleActionObject::shotWallHit(CollisionInfo &collision)
{
#ifndef S3D_SERVER
		addWallCollisionParticle(position_, collision.collisionId);
#endif
}
