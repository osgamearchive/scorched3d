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

#if !defined(AFX_SKYROOFCOLLISION_H__D335723A_9576_42EB_A750_29F20EFDF7AD__INCLUDED_)
#define AFX_SKYROOFCOLLISION_H__D335723A_9576_42EB_A750_29F20EFDF7AD__INCLUDED_

#include <engine/ScorchedContext.h>
#include <engine/ScorchedCollisionIds.h>
#include <engine/ActionController.h>

class SkyRoofCollision
{
public:
	SkyRoofCollision(ScorchedContext *context);
	virtual ~SkyRoofCollision();

	static void setContext(ScorchedContext *context);

protected:
	int roofClass_;
	ScorchedContext *context_;
	ScorchedCollisionInfo info_;

	void create();

	static dColliderFn *dLandscapeGetColliderFn(int num);
	static int dCollideLAABB(dGeomID o1, dGeomID o2, dReal aabb2[6]);
	static int dCollideLS (dGeomID o1, dGeomID o2, int flags,
        dContactGeom *contact, int skip);

};

#endif // !defined(AFX_SKYROOFCOLLISION_H__D335723A_9576_42EB_A750_29F20EFDF7AD__INCLUDED_)
