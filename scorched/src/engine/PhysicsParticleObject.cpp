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
#include <common/Defines.h>

PhysicsParticleObject::PhysicsParticleObject() : body_(0), geom_(0)
{

}

PhysicsParticleObject::~PhysicsParticleObject()
{
	if (body_) dBodyDestroy(body_);
	if (geom_) dGeomDestroy(geom_);
	body_ = 0;
	geom_ = 0;
}

void PhysicsParticleObject::applyForce(Vector &force)
{
	if (body_)
	{
		dBodyAddForce(body_, force[0], force[1], force[2]);
	}
}

void PhysicsParticleObject::setPhysics(PhysicsEngine &engine, Vector &position, 
									   Vector &velocity,
									   float sphereSize, float sphereDensity)
{
	if (sphereSize == 0.0f) sphereSize = 0.25f;
	if (sphereDensity == 0.0f) sphereDensity = 5.0f;

	// Create the mass for the object
	dMass m;
	dMassSetSphere(&m,sphereDensity,sphereSize);

	// Create the body for the object
	body_ = dBodyCreate(engine.getWorld());
	dBodySetPosition(body_,
		(dReal) position[0], (dReal) position[1], (dReal) position[2]);
	dBodySetLinearVel(body_,
		(dReal) velocity[0], (dReal) velocity[1], (dReal) velocity[2]);
	dBodySetMass(body_,&m);

	// Create the geom representing the object
	geom_ = dCreateSphere(engine.getSpace(), sphereSize);
	dGeomSetBody(geom_, body_);	
}

void PhysicsParticleObject::setPosition(Vector &position)
{
	DIALOG_ASSERT(body_);
	dBodySetPosition(body_,
		(dReal) position[0], (dReal) position[1], (dReal) position[2]);
}

Vector &PhysicsParticleObject::getPosition()
{
	static Vector position;
	if (body_)
	{
		const dReal *pos = dBodyGetPosition(body_);
		position[0] = (float) pos[0];
		position[1] = (float) pos[1];
		position[2] = (float) pos[2];
	}
	else position.zero();

	return position;
}

Vector &PhysicsParticleObject::getVelocity()
{
	static Vector velocity;
	if (body_)
	{
		const dReal *vel = dBodyGetLinearVel(body_);
		velocity[0] = (float) vel[0];
		velocity[1] = (float) vel[1];
		velocity[2] = (float) vel[2];
	}
	else velocity.zero();

	return velocity;
}

void PhysicsParticleObject::setData(void *data)
{
	dGeomSetData(geom_, data);
}

float *PhysicsParticleObject::getRotationQuat()
{
	static float result[4];
	if (body_)
	{
		const dReal *quat = dBodyGetQuaternion(body_);
		for (int i=0; i<4; i++) result[i] = (float) quat[i];
	}
	return result;
}

