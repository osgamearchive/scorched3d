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
	contactgroup_(0), handler_(0),
	contactGeomsCount_(0), contactInfosCount_(0),
	maxContacts_(1000)
{
	contactGeoms_ = new dContactGeom[maxContacts_];
	contactInfos_ = new dContactInfo[maxContacts_];
	create();
}

PhysicsEngine::~PhysicsEngine()
{
	destroy();
	delete [] contactGeoms_;
	delete [] contactInfos_;
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
	dWorldSetCFM(world_,1e-5f);

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
	PhysicsEngine *engine = (PhysicsEngine *) data;	
	engine->realNearCallback(data, o1, o2);
}

void PhysicsEngine::realNearCallback(void *data, dGeomID o1, dGeomID o2)
{
	if (dGeomIsEnabled(o1) == 0 ||
		dGeomIsEnabled(o1) == 0)
	{
		// Check that both bodies should actualy count in the collision
		// Does it seem odd that I have to do this, I think so!
		return;
	}

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

		// Find how many contacts we are allowed from the pool
		unsigned int contactsLeft = maxContacts_ - contactGeomsCount_;
		if (contactsLeft > 0)
		{
			// We have contacts left in the pool
			// Get the contacts for these two objects
			int numc = dCollide(o1, o2, 
				contactsLeft, &contactGeoms_[contactGeomsCount_], 
				sizeof(dContactGeom));
			if (numc > 0)
			{
				// We have a collision
				// Record this collision for processing
				dContactInfo &info = contactInfos_[contactInfosCount_];
				info.o1 = o1;
				info.o2 = o2;
				info.contacts = &contactGeoms_[contactGeomsCount_];
				info.contactCount = numc;

				contactInfosCount_ += 1; // We've used one collision
				contactGeomsCount_ += numc; // We've used numc contacts
			}
		}
    }
}

void PhysicsEngine::stepSimulation(float stepSize)
{
	// Find collisions
	contactGeomsCount_ = 0;
	contactInfosCount_ = 0;
	dSpaceCollide2((dGeomID) targetSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) groundSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) tankSpace_, (dGeomID) particleSpace_, this, &nearCallback); 
	dSpaceCollide2((dGeomID) targetSpace_, (dGeomID) tankSpace_, this, &nearCallback); 

	// Perform collision actions
	if (handler_ && contactInfosCount_ > 0)
	{
		for (unsigned int c=0; c<contactInfosCount_; c++)
		{
			dContactInfo &info = contactInfos_[c];

			// We have collisions, ask the user to process them
			handler_->collision(info.o1, info.o2, info.contacts, info.contactCount);
		}
	}

	// Simulate the world
	dWorldQuickStep(world_, stepSize);

	// remove all contact joints
	dJointGroupEmpty(contactgroup_);

	// Destroy all dead geoms
	while (!deadGeoms_.empty())
	{
		dGeomID geom = deadGeoms_.back();
		deadGeoms_.pop_back();
		dGeomDestroy(geom);
	}
}

void PhysicsEngine::destroyGeom(dGeomID geom)
{
	dGeomDisable(geom);
	deadGeoms_.push_back(geom);
}
