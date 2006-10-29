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

#include <engine/PhysicsEngine.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>

PhysicsEngine::PhysicsEngine() : 
	world_(0), 
	targetSpace_(0), groundSpace_(0), 
	particleSpace_(0), tankSpace_(0),
	contactgroup_(0), handler_(0)
{
	create();
}

PhysicsEngine::~PhysicsEngine()
{
	destroy();
}

void PhysicsEngine::setCollisionHandler(PhysicsEngineCollision *handler)
{
	handler_ = handler;
}

bool PhysicsEngine::create()
{
	// Create the simultion world
	world_ = dWorldCreate();

	dVector3 staticCenter;
	staticCenter[0] = 128.0;
	staticCenter[1] = 128.0;
	staticCenter[2] = 0.0;
	staticCenter[3] = 0.0;
	dVector3 staticSize;
	staticSize[0] = 256.0;
	staticSize[1] = 256.0;
	staticSize[2] = 128.0;
	staticSize[3] = 0.0;


	typedef dReal dVector3[4];
	targetSpace_ = dHashSpaceCreate(0);/*dQuadTreeSpaceCreate(
		0, // Existing space id
		staticCenter, // Center
		staticSize, // size,
		5); // Depth*/
	tankSpace_ = dHashSpaceCreate(0);
	particleSpace_ = dHashSpaceCreate(0);
	groundSpace_ = dHashSpaceCreate(0);
	contactgroup_ = dJointGroupCreate(0);

	// Setup the world's settings
	dWorldSetGravity(world_,0,0,-10.0); // Set a default gravity (this will change)
	dWorldSetCFM(world_,1e-5);

	return true;
}

void PhysicsEngine::setGravity(Vector &gravity)
{
	dWorldSetGravity(world_, gravity[0], gravity[1], gravity[2]);
}

void PhysicsEngine::setWind(Vector &wind)
{
	wind_ = wind;
}

void PhysicsEngine::destroy()
{
	dJointGroupDestroy(contactgroup_);
	dSpaceDestroy(targetSpace_);
	dSpaceDestroy(tankSpace_);
	dSpaceDestroy(particleSpace_);
	dSpaceDestroy(groundSpace_);
	dWorldDestroy(world_);
}

void PhysicsEngine::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	const int maxContacts = 100;

	PhysicsEngine *engine = (PhysicsEngine *) data;
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) 
	{
		// colliding a space with something
		dSpaceCollide2(o1,o2,data,&nearCallback);

		// collide all geoms internal to the space(s)
		if (dGeomIsSpace(o1))
		{
			dSpaceID s1 = (dSpaceID) o1;
			dSpaceCollide(s1,data,&nearCallback);
		}
		if (dGeomIsSpace(o2))
		{
			dSpaceID s2 = (dSpaceID) o2;
			dSpaceCollide(s2,data,&nearCallback);
		}
	}
    else 
	{
		// exit without doing anything if the two bodies are connected by a joint
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

		// Get the contacts for these two objects
		static dContactGeom contacts[maxContacts];
		int numc = dCollide(o1, o2, maxContacts, contacts, sizeof(dContactGeom));
		if ((numc > 0) && engine->handler_)
		{
			// We have collisions, ask the user to process them
			engine->handler_->collision(o1, o2, contacts, numc);
		}
    }
}

void PhysicsEngine::stepSimulation(float stepSize)
{
	// Simulate the world
	dSpaceCollide2((dGeomID) targetSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) groundSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) tankSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) targetSpace_, (dGeomID) tankSpace_, this, &nearCallback); 
	//dSpaceCollide(space_, this, &nearCallback);
	//dWorldStep(world_, stepSize);
	//dWorldStepFast1(world_, stepSize, 5);
	dWorldQuickStep(world_, stepSize);

	// remove all contact joints
	dJointGroupEmpty(contactgroup_);
}

