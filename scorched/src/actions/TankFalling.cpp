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
#include <actions/TankFallingEnd.h>
#include <target/TargetContainer.h>
#include <target/TargetState.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <weapons/AccessoryStore.h>
#include <weapons/Parachute.h>

REGISTER_ACTION_SOURCE(TankFalling);

TankFallingParticle::TankFallingParticle(TankFalling *tell) : 
	tell_(tell), collisionInfo_(CollisionIdFallingTank)
{

}

void TankFallingParticle::init()
{
	collisionInfo_.data = this;
	physicsObject_.setData(&collisionInfo_);
}

void TankFallingParticle::collision(Vector &position)
{
	if (tell_) tell_->collision();
}

void TankFallingParticle::hadCollision()
{
	collision_ = true;
	tell_ = 0;
}

TankFalling::TankFalling() : remove_(false)
{
}

TankFalling::TankFalling(Weapon *weapon, unsigned int fallingPlayerId,
				   unsigned int firedPlayerId,
				   Parachute *parachute,
				   unsigned int data) :
	weapon_(weapon), remove_(false),
	fallingPlayerId_(fallingPlayerId), firedPlayerId_(firedPlayerId),
	data_(data), parachute_(parachute)
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
	if (current && !current->getTargetState().getFalling())
	{
		current->getTargetState().setFalling(this);

		// Store the start positions
		tankStartPosition_ = current->getTargetPosition();

		for (int i=0; i<4; i++)
		{
			TankFallingParticle *particle = new TankFallingParticle(this);
			particles_.push_back(particle);

			// The sphere is 0.25 radius
			Vector nullVelocity;
			Vector initPos = current->getTargetPosition();	
			switch (i)
			{
			case 0:
				initPos[0] += 0.5f;
				initPos[1] += 0.5f;
				initPos[2] += 0.25;
				break;
			case 1:
				initPos[0] += 0.5f;
				initPos[1] -= 0.5f;
				initPos[2] += 0.25;
				break;
			case 2:
				initPos[0] -= 0.5f;
				initPos[1] -= 0.5f;
				initPos[2] += 0.25;
				break;
			case 3:
				initPos[0] -= 0.5f;
				initPos[1] += 0.5f;
				initPos[2] += 0.25;
				break;
			}

			particle->setScorchedContext(context_);
			particle->setPhysics(initPos, nullVelocity);
			particle->init();

			context_->actionController->addAction(particle);
		}
	}
	else
	{
		remove_ = true;
	}
}

void TankFalling::simulate(float frameTime, bool &remove)
{
	if (!remove_)
	{
		applyForce();

		Vector spherePosition;
		getAllPositions(spherePosition);
		
		// Calcuate the action position
		Vector position(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 0.25f);

		// Move the tank to the new position
		Target *target = context_->targetContainer->getTargetById(fallingPlayerId_);
		if (target && target->getAlive())
		{
			if (position[0] != 0.0f || position[1] != 0.0f || position[2] != 0.0f)
			{
				target->setTargetPosition(position);
			}
		}

		// Simulate the particles
		bool fakeRemove = false;
		std::list<TankFallingParticle *>::iterator itor;
		for (itor = particles_.begin();
			itor != particles_.end();
			itor++)
		{
			TankFallingParticle *part = (*itor);
			part->simulate(frameTime, fakeRemove);
		}
	}
	else
	{
		remove = true;
	}

	ActionMeta::simulate(frameTime, remove);
}

bool TankFalling::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(fallingPlayerId_);
	buffer.addToBuffer(firedPlayerId_);
	buffer.addToBuffer(data_);
	context_->accessoryStore->writeAccessoryPart(buffer, parachute_);
	context_->accessoryStore->writeWeapon(buffer, weapon_);
	return true;
}

bool TankFalling::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(fallingPlayerId_)) return false;
	if (!reader.getFromBuffer(firedPlayerId_)) return false;
	if (!reader.getFromBuffer(data_)) return false;
	parachute_ = (Parachute *) context_->accessoryStore->readAccessoryPart(reader);
	weapon_ = context_->accessoryStore->readWeapon(reader); if (!weapon_) return false;
	return true;
}

void TankFalling::applyForce()
{
	if (!parachute_) return;
	Vector force = parachute_->getSlowForce();

	std::list<TankFallingParticle *>::iterator itor;
	for (itor = particles_.begin();
		itor != particles_.end();
		itor++)
	{
		TankFallingParticle *part = (*itor);
		part->applyForce(force);
	}
}

void TankFalling::getAllPositions(Vector &spherePosition)
{
	std::list<TankFallingParticle *>::iterator itor;
	for (itor = particles_.begin();
		itor != particles_.end();
		itor++)
	{
		TankFallingParticle *part = (*itor);
		spherePosition += part->getCurrentPosition();
	}
	spherePosition /= 4.0f;
}

void TankFalling::remove()
{
	// Make sure we dont get called again when a sub particle has 
	// a collision
	while (!particles_.empty())
	{
		TankFallingParticle *part = particles_.front();
		particles_.pop_front();
		part->hadCollision();
	}

	// This is the end of falling
	remove_ = true;
}

void TankFalling::collision()
{
	// Create end action
	Vector position;
	Target *current = 
		context_->targetContainer->getTargetById(fallingPlayerId_);
	if (current && current->getAlive())
	{
		// Calcuate the end position
		Vector spherePosition;
		getAllPositions(spherePosition);
		position = Vector(spherePosition[0], 
			spherePosition[1], 
			spherePosition[2] - 0.25f);
	}

	// Make sure we dont get called again when a sub particle has 
	// a collision
	// Do this after we get the current position
	while (!particles_.empty())
	{
		TankFallingParticle *part = particles_.front();
		particles_.pop_front();
		part->hadCollision();
	}

	// Say we have ended
	TankFallingEnd *end = new TankFallingEnd(
		weapon_, tankStartPosition_, position,
		fallingPlayerId_, firedPlayerId_, parachute_, data_);
	context_->actionController->addAction(end);
}
