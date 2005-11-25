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
	world_(0), space_(0), 
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
	space_ = dHashSpaceCreate(0);
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

void PhysicsEngine::destroy()
{
	dJointGroupDestroy(contactgroup_);
	dSpaceDestroy(space_);
	dWorldDestroy(world_);
}

void PhysicsEngine::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	const int maxContacts = 100;

	PhysicsEngine *engine = (PhysicsEngine *) data;

	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

	// Get the contacts for these two objects
	static dContactGeom contacts[maxContacts];
	int numc = dCollide(o1, o2, maxContacts, contacts, sizeof(dContactGeom));
	if (numc && engine->handler_)
	{
		// We have collisions, ask the user to process them
		engine->handler_->collision(o1, o2, contacts, numc);
	}
}

void PhysicsEngine::stepSimulation(float stepSize)
{
	// Simulate the world
	dSpaceCollide(space_, this, &nearCallback);
	//dWorldStep(world_, stepSize);
	//dWorldStepFast1(world_, stepSize, 5);
	dWorldQuickStep(world_, stepSize);

	// remove all contact joints
	dJointGroupEmpty(contactgroup_);
}

