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


// TankMesh.h: interface for the TankMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKMESH_H__CB857C65_A22F_4FBC_9344_EFF22F8A4EEA__INCLUDED_)
#define AFX_TANKMESH_H__CB857C65_A22F_4FBC_9344_EFF22F8A4EEA__INCLUDED_

#include <common/Vector.h>
#include <GLEXT/GLVertexArray.h>

class ASEFile;
class TankMesh  
{
public:
	TankMesh(ASEFile &aseTank, bool useTextures, float detail);
	virtual ~TankMesh();

	void draw(bool drawS, float angle, Vector &position, 
		float fireOffSet, float rotXY, float rotXZ);
	float getTurretHeight() { return turretHeight_; }
	int getNoTris();

protected:
	static GLuint sightList_;

	float turretHeight_;
	GLVertexArray *gunArray_;
	GLVertexArray *turretArray_;
	GLVertexArray *otherArray_;

	void drawSight();
	void drawGun(bool drawS, float fireOffset, float rotXY, float rotXZ);
	void createArrays(ASEFile &aseTank, bool useTextures, float detail);

};



#endif // !defined(AFX_TANKMESH_H__CB857C65_A22F_4FBC_9344_EFF22F8A4EEA__INCLUDED_)
