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


// HeightMapCollision.cpp: implementation of the HeightMapCollision class.
//
//////////////////////////////////////////////////////////////////////

#include <landscape/HeightMapCollision.h>
#include <engine/ScorchedCollisionIds.h>
#include <common/Triangle.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int dSphereClass;
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HeightMapCollision *HeightMapCollision::instance_ = 0;

HeightMapCollision *HeightMapCollision::instance()
{
	if (!instance_)
	{
		instance_ = new HeightMapCollision;
	}
	return instance_;
}

HeightMapCollision::HeightMapCollision() : landscapeClass_(0), hMap_(0)
{
 
}

HeightMapCollision::~HeightMapCollision()
{
}

void HeightMapCollision::create(HeightMap &map)
{
	PhysicsEngine &engine = ActionController::instance()->getPhysics();

	// Create the walls and ground
	// Create the ground 
	{
		static ScorchedCollisionInfo groundInfo(CollisionIdGround);
		dGeomID ground = dCreatePlane(engine.getSpace(),0,0,1,0);
		dGeomSetData(ground, (void *) &groundInfo);
	}

	// Create the side walls
	{
		static ScorchedCollisionInfo wallInfoTop(CollisionIdWallTop);
		dGeomID sidea = dCreatePlane(engine.getSpace(),0,1,0,5);
		dGeomSetData(sidea, (void *) &wallInfoTop);
	}
	{
		static ScorchedCollisionInfo wallInfoBottom(CollisionIdWallBottom);
		dGeomID sideb = dCreatePlane(engine.getSpace(),0,-1,0,-250);
		dGeomSetData(sideb, (void *) &wallInfoBottom);
	}
	{
		static ScorchedCollisionInfo wallInfoLeft(CollisionIdWallLeft);
		dGeomID sidec = dCreatePlane(engine.getSpace(),1,0,0,5);
		dGeomSetData(sidec, (void *) &wallInfoLeft);
	}
	{
		static ScorchedCollisionInfo wallInfoRight(CollisionIdWallRight);
		dGeomID sided = dCreatePlane(engine.getSpace(),-1,0,0,-250);
		dGeomSetData(sided, (void *) &wallInfoRight);
	}

	// Create the land scape stuff
	hMap_ = &map;
	dGeomClass c;

	c.bytes = 0;
	c.collider = &dLandscapeGetColliderFn;
	c.aabb = &dInfiniteAABB;
	c.aabb_test = &dCollideLAABB;
	c.dtor = 0;
	landscapeClass_ = dCreateGeomClass(&c);

	dGeomID geom = dCreateGeom(landscapeClass_);
	static ScorchedCollisionInfo info(CollisionIdLandscape);
	dGeomSetData(geom, (void *) &info);
	dSpaceAdd(engine.getSpace(), geom);
}

dColliderFn * HeightMapCollision::dLandscapeGetColliderFn(int num)
{
    if (num == dSphereClass)    return (dColliderFn *) &dCollideLS;
    //if (num == dBoxClass)       return (dColliderFn *) &dCollideLB;
    return 0;
}

// Checks for collision between a Landscape and an AABB.
int HeightMapCollision::dCollideLAABB(dGeomID o1, dGeomID o2, dReal aabb2[6])
{
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
		info2->id != CollisionIdSprite && 
		info1->id != CollisionIdShot &&
		info1->id != CollisionIdFallingTank &&
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

	if (maxX >= instance_->hMap_->getWidth()) maxX = instance_->hMap_->getWidth() - 1;
	if (minX < 0) minX = 0;
	if (maxY >= instance_->hMap_->getWidth()) maxY = instance_->hMap_->getWidth() - 1;
	if (minY < 0) minY = 0;

	// See if any heights in this area are in the bounding box
	for (int x=minX; x<=maxX; x++)
	{
		for (int y=minY; y<=maxY; y++)
		{
			if (instance_->hMap_->getHeight(x, y) > aabb2[4]) return 1;
		}
	}
	return 0;
}

// Checks for collision between a Landscape and a Sphere
int HeightMapCollision::dCollideLS (dGeomID o1, dGeomID o2, int flags,
        dContactGeom *basecontact, int skip)
{
	ScorchedCollisionInfo *info2 = 
		(ScorchedCollisionInfo *) dGeomGetData(o2);
	DIALOG_ASSERT(info2);

	// Only use the sphere collision for shots and falling tanks
	int num_contacts = 0;
	if (info2->id == CollisionIdShot ||
		info2->id == CollisionIdFallingTank)
	{
		// Do we actually need to do this??
		// This is very intensive for the actual precision we need
		// we could probably get away with the sphere center check!
		num_contacts = dCollideLSSphere(o1, o2, flags, basecontact, skip);
	}

 	// Now check the actual center of the sphere is not in the landscape
	if (!num_contacts)
	{
		// Get the centre and radius of the sphere
		const dReal *centre = dGeomGetPosition(o2);
		float radius = (float) dGeomSphereGetRadius(o2);
		Vector sphereCentre;
		sphereCentre[0] = (float) centre[0];
		sphereCentre[1] = (float) centre[1];
		sphereCentre[2] = (float) centre[2];

		// Check if the center of the spehere is in the landscape
		float dist = sphereCentre[2] - 
			instance_->hMap_->
				getInterpHeight(sphereCentre[0], sphereCentre[1]);
		if (dist < 0.0f)
		{
			Vector interN;
			instance_->hMap_->
				getInterpNormal(sphereCentre[0], sphereCentre[1], interN);

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
	}

	return num_contacts;
}

int HeightMapCollision::dCollideLSSphere (dGeomID o1, dGeomID o2, int flags,
        dContactGeom *basecontact, int skip)
{
    // Get the centre & radius of the sphere
    const dReal *centre = dGeomGetPosition(o2);
	Vector sphereCentre;
	sphereCentre[0] = (float) centre[0];
	sphereCentre[1] = (float) centre[1];
	sphereCentre[2] = (float) centre[2];
	float radius = (float) dGeomSphereGetRadius(o2);

	// Get the bounding box of the sphere
    dReal aabb2[6];
    dGeomGetAABB (o2, aabb2);

	// Get the extents of the bounding box
    int minX = (int)floor(aabb2[0]);
    int maxX = (int)ceil(aabb2[1]);
    int minY = (int)floor(aabb2[2]);
    int maxY = (int)ceil(aabb2[3]);

	if (maxX >= instance_->hMap_->getWidth()) maxX = instance_->hMap_->getWidth() - 1;
	if (minX < 0) minX = 0;
	if (maxY >= instance_->hMap_->getWidth()) maxY = instance_->hMap_->getWidth() - 1;
	if (minY < 0) minY = 0;

	// Check all squares for contacts
	int num_contacts = 0;
	for (int x=minX; x<=maxX; x++)
	{
		for (int y=minY; y<=maxY; y++)
		{
			static Triangle triA;
			triA.setPointComponents(
				float(x), float(y), instance_->hMap_->getHeight(x, y),
				instance_->hMap_->getNormal(x, y),
				float(x + 1), float(y), instance_->hMap_->getHeight(x + 1, y),
				instance_->hMap_->getNormal(x+1, y),
				float(x + 1), float(y + 1), instance_->hMap_->getHeight(x + 1, y + 1),
				instance_->hMap_->getNormal(x+1, y+1));

			static Triangle triB;
			triB.setPointComponents(
				float(x + 1), float(y + 1), instance_->hMap_->getHeight(x + 1, y + 1),
				instance_->hMap_->getNormal(x+1, y+1),
				float(x), float(y + 1), instance_->hMap_->getHeight(x, y + 1),
				instance_->hMap_->getNormal(x, y+1),
				float(x), float(y), instance_->hMap_->getHeight(x, y),
				instance_->hMap_->getNormal(x, y));

			static Vector newInter, interN;
			float dist;
			if (triA.sphereIntersect(sphereCentre, radius, newInter, interN, dist))
			{
				char *ptr = (char *) basecontact;
				ptr += (num_contacts * skip);
				dContactGeom *contact = (dContactGeom *) ptr;

				contact->depth = dist;
				contact->normal[0] = -interN[0];
				contact->normal[1] = -interN[1];
				contact->normal[2] = -interN[2];
				contact->pos[0] = sphereCentre[0] - (interN[0] * radius);
				contact->pos[1] = sphereCentre[1] - (interN[1] * radius);
				contact->pos[2] = sphereCentre[2] - (interN[2] * radius);

				num_contacts++;
			}
			if (triB.sphereIntersect(sphereCentre, radius, newInter, interN, dist))
			{
				char *ptr = (char *) basecontact;
				ptr += (num_contacts * skip);
				dContactGeom *contact = (dContactGeom *) ptr;

				contact->depth = dist;
				contact->normal[0] = -interN[0];
				contact->normal[1] = -interN[1];
				contact->normal[2] = -interN[2];
				contact->pos[0] = sphereCentre[0] - (interN[0] * radius);
				contact->pos[1] = sphereCentre[1] - (interN[1] * radius);
				contact->pos[2] = sphereCentre[2] - (interN[2] * radius);

				num_contacts++;
			}

			if (num_contacts > 3) break;
		}
	}

    return num_contacts;
}
