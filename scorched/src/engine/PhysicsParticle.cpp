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

#include <engine/PhysicsParticle.h>
#include <engine/ScorchedContext.h>

PhysicsParticle::PhysicsParticle()  : collision_(false), totalTime_(0)
{

}

PhysicsParticle::~PhysicsParticle()
{

}

void PhysicsParticle::setPhysics(Vector &position, Vector &velocity)
{
	physicsObject_.setPhysics(context_->actionController.getPhysics(), position, velocity);
}

void PhysicsParticle::collision(Vector &position)
{
	collision_ = true;
}

void PhysicsParticle::setData(void *data)
{
	physicsObject_.setData(data);
}

Vector &PhysicsParticle::getCurrentPosition()
{
	return physicsObject_.getPosition();
}

Vector &PhysicsParticle::getCurrentVelocity()
{
	return physicsObject_.getVelocity();
}

void PhysicsParticle::setCurrentPosition(Vector &position)
{
	physicsObject_.setPosition(position);
}

void PhysicsParticle::simulate(float frameTime, bool &remove)
{
	Action::simulate(frameTime, remove);
	if (collision_) remove = true;
	totalTime_ += frameTime;
	if (totalTime_ > 30.0f) remove = true;
}

PhysicsParticleMeta::PhysicsParticleMeta()  : collision_(false), totalTime_(0)
{

}

PhysicsParticleMeta::~PhysicsParticleMeta()
{

}

void PhysicsParticleMeta::setPhysics(Vector &position, Vector &velocity,
									 float sphereSize, float sphereDensity)
{
	physicsObject_.setPhysics(context_->actionController.getPhysics(), 
		position, velocity,
		sphereSize, sphereDensity);
}

void PhysicsParticleMeta::collision(Vector &position)
{
	collision_ = true;
}

void PhysicsParticleMeta::setData(void *data)
{
	physicsObject_.setData(data);
}

Vector &PhysicsParticleMeta::getCurrentPosition()
{
	return physicsObject_.getPosition();
}

Vector &PhysicsParticleMeta::getCurrentVelocity()
{
	return physicsObject_.getVelocity();
}

void PhysicsParticleMeta::setCurrentPosition(Vector &position)
{
	physicsObject_.setPosition(position);
}

void PhysicsParticleMeta::simulate(float frameTime, bool &remove)
{
	Action::simulate(frameTime, remove);
	if (collision_) remove = true;
	totalTime_ += frameTime;
	if (totalTime_ > 30.0f) remove = true;
}
