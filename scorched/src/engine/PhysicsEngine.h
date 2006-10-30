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

#if !defined(__INCLUDE_PhysicsEngineh_INCLUDE__)
#define __INCLUDE_PhysicsEngineh_INCLUDE__

#include <ode/ode.h>
#include <common/Vector.h>
#include <list>

class PhysicsEngineCollision
{
public:
	virtual void collision(dGeomID o1, dGeomID o2, 
		dContactGeom *contacts, int noContacts) = 0;
};

class PhysicsEngine{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void setCollisionHandler(PhysicsEngineCollision *handler);
	void setGravity(Vector &gravity);
	void setWind(Vector &wind);
	Vector &getWind() { return wind_; }

	dWorldID &getWorld() { return world_; }
	dSpaceID &getTargetSpace() { return targetSpace_; }
	dSpaceID &getParticleSpace() { return particleSpace_; }
	dSpaceID &getTankSpace() { return tankSpace_; }
	dSpaceID &getGroundSpace() { return groundSpace_; }
	dJointGroupID &getContactGroup() { return contactgroup_; }

	void destroyGeom(dGeomID geom); // Dispose of a geom outside collision loop

	virtual void stepSimulation(float frameTime);

protected:
	PhysicsEngineCollision *handler_;
	dWorldID world_;
	dSpaceID targetSpace_;
	dSpaceID tankSpace_;
	dSpaceID particleSpace_;
	dSpaceID groundSpace_;
	dJointGroupID contactgroup_;
	std::list<dGeomID> deadGeoms_;
	Vector wind_;
	bool create();
	void destroy();

	static void nearCallback(void *data, dGeomID o1, dGeomID o2);

};


#endif
