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

PhysicsParticleObject::PhysicsParticleObject() : body_(0), geom_(0)
{

}

PhysicsParticleObject::~PhysicsParticleObject()
{
	if (body_) dBodyDestroy(body_);
	if (geom_) dGeomDestroy(geom_);
}

void PhysicsParticleObject::setPhysics(PhysicsEngine &engine, Vector &position, Vector &velocity)
{
	const dReal DENSITY = 5.0;
	const dReal size = 0.25f;

	// Create the mass for the object
	dMass m;
	dMassSetSphere(&m,DENSITY,size);

	// Create the body for the object
	body_ = dBodyCreate(engine.getWorld());
	dBodySetPosition(body_,
		(dReal) position[0], (dReal) position[1], (dReal) position[2]);
	dBodySetLinearVel(body_,
		(dReal) velocity[0], (dReal) velocity[1], (dReal) velocity[2]);
	dBodySetMass(body_,&m);

    //dRFromAxisAndAngle (R,dRandReal()*2.0-1.0,dRandReal()*2.0-1.0,
	//		 dRandReal()*2.0-1.0,dRandReal()*10.0-5.0);
    //dBodySetRotation (obj[i].body,R);
    //dBodySetData (obj[i].body,(void*) i);

	// Create the geom representing the object
	geom_ = dCreateSphere(engine.getSpace(), size);
	dGeomSetBody(geom_, body_);	
}

Vector &PhysicsParticleObject::getPosition()
{
	static Vector position;
	const dReal *pos = dBodyGetPosition(body_);
	position[0] = (float) pos[0];
	position[1] = (float) pos[1];
	position[2] = (float) pos[2];
	return position;
}

Vector &PhysicsParticleObject::getVelocity()
{
	static Vector velocity;
	const dReal *vel = dBodyGetLinearVel(body_);
	velocity[0] = (float) vel[0];
	velocity[1] = (float) vel[1];
	velocity[2] = (float) vel[2];
	return velocity;
}

void PhysicsParticleObject::setData(void *data)
{
	dGeomSetData(geom_, data);
}