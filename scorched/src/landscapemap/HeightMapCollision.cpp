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

#include <landscapemap/HeightMapCollision.h>
#include <landscapemap/LandscapeMaps.h>
#include <common/OptionsGame.h>
#include <common/Triangle.h>
#include <common/Defines.h>

static ScorchedContext *staticContext_ = 0;

HeightMapCollision::HeightMapCollision(ScorchedContext *context) : 
	landscapeClass_(0), context_(context),
	groundInfo_(CollisionIdGround),
	wallInfoTop_(CollisionIdWallTop),
	wallInfoBottom_(CollisionIdWallBottom),
	wallInfoLeft_(CollisionIdWallLeft),
	wallInfoRight_(CollisionIdWallRight),
	info_(CollisionIdLandscape),
	sidea_(0), sideb_(0), 
	sidec_(0), sided_(0)
{
	create();
}

HeightMapCollision::~HeightMapCollision()
{
}

void HeightMapCollision::setContext(ScorchedContext *context)
{
	staticContext_ = context;
}

void HeightMapCollision::create()
{
	PhysicsEngine &engine = context_->actionController->getPhysics();

	// Create the walls and ground
	// Create the ground 
	{
		dGeomID ground = dCreatePlane(engine.getGroundSpace(),0,0,1,0);
		dGeomSetData(ground, (void *) &groundInfo_);
	}

	// Create the land scape stuff
	dGeomClass c;

	c.bytes = 0;
	c.collider = &dLandscapeGetColliderFn;
	c.aabb = &dInfiniteAABB;
	c.aabb_test = &dCollideLAABB;
	c.dtor = 0;
	landscapeClass_ = dCreateGeomClass(&c);

	dGeomID geom = dCreateGeom(landscapeClass_);
	dGeomSetData(geom, (void *) &info_);
	dSpaceAdd(engine.getGroundSpace(), geom);
}

void HeightMapCollision::generate()
{
	if (sidea_)
	{
		dGeomDestroy(sidea_);
		dGeomDestroy(sideb_);
		dGeomDestroy(sidec_);
		dGeomDestroy(sided_);
	
		sidea_ = 0;
		sideb_ = 0;
		sidec_ = 0;
		sided_ = 0;
	}

	if (context_->optionsGame->getWallType() != OptionsGame::WallNone)
	{
		PhysicsEngine &engine = context_->actionController->getPhysics();

		// Create the side walls
		{
			sidea_ = dCreatePlane(engine.getGroundSpace(),0.0f,1.0f,0.0f,5.0f);
			dGeomSetData(sidea_, (void *) &wallInfoTop_);
		}
		{
			int mapHeight = 
				context_->landscapeMaps->getGroundMaps().getMapHeight();
			sideb_ = dCreatePlane(engine.getGroundSpace(),0.0f,-1.0f,0.0f,-mapHeight + 5.0f);
			dGeomSetData(sideb_, (void *) &wallInfoBottom_);
		}
		{
			sidec_ = dCreatePlane(engine.getGroundSpace(),1.0f,0.0f,0.0f,5.0f);
			dGeomSetData(sidec_, (void *) &wallInfoLeft_);
		}
		{
			int mapWidth = 
				context_->landscapeMaps->getGroundMaps().getMapWidth();
			sided_ = dCreatePlane(engine.getGroundSpace(),-1.0f,0.0f,0.0f,-mapWidth + 5.0f);
			dGeomSetData(sided_, (void *) &wallInfoRight_);
		}
	}
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

	// See if any heights in this area are in the bounding box
	for (int x=minX; x<=maxX; x++)
	{
		for (int y=minY; y<=maxY; y++)
		{
			if (info2->collisionOnSurface && info1->collisionOnSurface)
			{
				if (staticContext_->landscapeMaps->getGroundMaps().
					getHeight(x, y) > aabb2[4]) return 1;
			}
			else
			{
				if (staticContext_->landscapeMaps->getGroundMaps().
					getHeight(x, y) < aabb2[5]) return 1;
			}
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
		info2->id == CollisionIdBounce ||
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
			staticContext_->landscapeMaps->getGroundMaps().
				getInterpHeight(sphereCentre[0], sphereCentre[1]);
		
		bool landscapeCollision = false;
		if (info2->collisionOnSurface)
		{
			landscapeCollision = (dist < 0.0f);
		}
		else
		{
			landscapeCollision = (dist > 0.0f);
		}


		if (landscapeCollision)
		{
			Vector interN;
			staticContext_->landscapeMaps->getGroundMaps().
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

	// Check all squares for contacts
	int num_contacts = 0;
	for (int x=minX; x<=maxX; x++)
	{
		for (int y=minY; y<=maxY; y++)
		{
			static Triangle triA;
			triA.setPointComponents(
				float(x), float(y), staticContext_->landscapeMaps->getGroundMaps().getHeight(x, y),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x, y),
				float(x + 1), float(y), staticContext_->landscapeMaps->getGroundMaps().getHeight(x + 1, y),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x+1, y),
				float(x + 1), float(y + 1), staticContext_->landscapeMaps->getGroundMaps().getHeight(x + 1, y + 1),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x+1, y+1));

			static Triangle triB;
			triB.setPointComponents(
				float(x + 1), float(y + 1), staticContext_->landscapeMaps->getGroundMaps().getHeight(x + 1, y + 1),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x+1, y+1),
				float(x), float(y + 1), staticContext_->landscapeMaps->getGroundMaps().getHeight(x, y + 1),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x, y+1),
				float(x), float(y), staticContext_->landscapeMaps->getGroundMaps().getHeight(x, y),
				staticContext_->landscapeMaps->getGroundMaps().getNormal(x, y));

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
