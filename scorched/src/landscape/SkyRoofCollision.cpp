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

// Checks for collision between a Landscape and an AABB.
int SkyRoofCollision::dCollideLAABB(dGeomID o1, dGeomID o2, dReal aabb2[6])
{
	// Check we have a roof
	if (!staticContext_->landscapeMaps->getRoof()) return 0;

    // This function checks for contact between the Landscape and an AABB.
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
	int minX = (int)floor(aabb2[0]);
	int maxX = (int)ceil(aabb2[1]);
	int minY = (int)floor(aabb2[2]);
	int maxY = (int)ceil(aabb2[3]);

	int X = (minX + maxX) / 8;
	int Y = (minY + maxY) / 8;
	if (X > staticContext_->landscapeMaps->getRMap().getWidth()) 
		X = staticContext_->landscapeMaps->getRMap().getWidth();
	if (X < 0) X = 0;
	if (Y > staticContext_->landscapeMaps->getRMap().getWidth()) 
		Y = staticContext_->landscapeMaps->getRMap().getWidth();
	if (Y < 0) Y = 0;

	// See if any heights in this area are in the bounding box
	if (staticContext_->landscapeMaps->getRMap().
		getHeight(X, Y) < aabb2[4]) return 1;

	return 0;
}

// Checks for collision between a Landscape and a Sphere
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
		staticContext_->landscapeMaps->getHMap().
			getInterpHeight(sphereCentre[0] / 4.0f, sphereCentre[1] / 4.0f);
		
	int num_contacts = 0;
	bool landscapeCollision = (dist > 0.0f);
	if (landscapeCollision)
	{
		Vector interN;
		staticContext_->landscapeMaps->getHMap().
			getInterpNormal(sphereCentre[0] / 4.0f, sphereCentre[1] / 4.0f, interN);

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