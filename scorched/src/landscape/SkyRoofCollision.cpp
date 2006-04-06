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

#include <landscape/SkyRoofCollision.h>
#include <landscape/LandscapeMaps.h>
#include <common/Triangle.h>
#include <common/Defines.h>

static ScorchedContext *staticContext_ = 0;

SkyRoofCollision::SkyRoofCollision(ScorchedContext *context) : 
	roofClass_(0), context_(context),
	info_(CollisionIdRoof)
{
	create();
}

SkyRoofCollision::~SkyRoofCollision()
{
}

void SkyRoofCollision::setContext(ScorchedContext *context)
{
	staticContext_ = context;
}

void SkyRoofCollision::create()
{
	PhysicsEngine &engine = context_->actionController->getPhysics();

	// Create the land scape stuff
	dGeomClass c;

	c.bytes = 0;
	c.collider = &dLandscapeGetColliderFn;
	c.aabb = &dInfiniteAABB;
	c.aabb_test = &dCollideLAABB;
	c.dtor = 0;
	roofClass_ = dCreateGeomClass(&c);

	dGeomID geom = dCreateGeom(roofClass_);
	dGeomSetData(geom, (void *) &info_);
	dSpaceAdd(engine.getSpace(), geom);
}

dColliderFn * SkyRoofCollision::dLandscapeGetColliderFn(int num)
{
    if (num == dSphereClass)    return (dColliderFn *) &dCollideLS;
    //if (num == dBoxClass)       return (dColliderFn *) &dCollideLB;
    return 0;
}

// Checks for collision between a roof and an AABB.
int SkyRoofCollision::dCollideLAABB(dGeomID o1, dGeomID o2, dReal aabb2[6])
{
    // This function checks for contact between the roof and an AABB.
    // It checks a larger region of terrain than strictly necessary and
    // will sometimes return true even if there is no collision, but will never
    // return false if there is a collision.
    // If the AABB is below the terrain it will also return true.

	// Check what we have collided with
	// We are only interested in Shots, FallingTanks and Sprites
	// as all other objects are static to the landscape and should
	// not be checked for collisions
	ScorchedCollisionInfo *info1 = 
		(ScorchedCollisionInfo *) dGeomGetData(o1);
	ScorchedCollisionInfo *info2 = 
		(ScorchedCollisionInfo *) dGeomGetData(o2);
	DIALOG_ASSERT(info1 && info2);
	if (info2->id != CollisionIdShot &&
		info2->id != CollisionIdFallingTank &&
		info2->id != CollisionIdBounce &&
		info2->id != CollisionIdSprite && 
		info1->id != CollisionIdShot &&
		info1->id != CollisionIdFallingTank &&
		info1->id != CollisionIdBounce &&
		info1->id != CollisionIdSprite)
	{
		return 0;
	}

	// Now we have something we should actualy be checking the collision
	// for get the extents of the bounding box
	float X = float(aabb2[0] + aabb2[1]) / 2.0f;
	float Y = float(aabb2[2] + aabb2[3]) / 2.0f;

	// See if any heights in this area are in the bounding box
	if (staticContext_->landscapeMaps->getRoofMaps().
		getInterpRoofHeight(X, Y) < aabb2[5]) return 1;
	// aabb2[5] == maxz (the highest part of the sphere)

	return 0;
}

// Checks for collision between a Roof and a Sphere
int SkyRoofCollision::dCollideLS (dGeomID o1, dGeomID o2, int flags,
        dContactGeom *basecontact, int skip)
{
	ScorchedCollisionInfo *info2 = 
		(ScorchedCollisionInfo *) dGeomGetData(o2);
	DIALOG_ASSERT(info2);

	// Get the centre and radius of the sphere
	const dReal *centre = dGeomGetPosition(o2);
	float radius = (float) dGeomSphereGetRadius(o2);
	Vector sphereCentre;
	sphereCentre[0] = (float) centre[0];
	sphereCentre[1] = (float) centre[1];
	sphereCentre[2] = (float) centre[2];

	// Check if the center of the spehere is in the landscape
	float dist = sphereCentre[2] - 
		staticContext_->landscapeMaps->getRoofMaps().
			getInterpRoofHeight(sphereCentre[0], sphereCentre[1]); 
		
	int num_contacts = 0;
	bool rootCollision = (dist > 0.0f);
	if (rootCollision)
	{
		Vector interN(0.0f, 0.0f, -1.0f);

		char *ptr = (char *) basecontact;
		ptr += (num_contacts * skip);
		dContactGeom *contact = (dContactGeom *) ptr;

		// This is probably not correct, but it is close
		// enough + nothing currently uses the collision info
		contact->depth = -dist;
		contact->normal[0] = -interN[0];
		contact->normal[1] = -interN[1];
		contact->normal[2] = -interN[2];
		contact->pos[0] = sphereCentre[0] - (interN[0] * radius);
		contact->pos[1] = sphereCentre[1] - (interN[1] * radius);
		contact->pos[2] = sphereCentre[2] - (interN[2] * radius);

		num_contacts++;
	}

	return num_contacts;
}
