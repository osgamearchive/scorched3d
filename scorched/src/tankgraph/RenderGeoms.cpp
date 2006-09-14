////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <tankgraph/RenderGeoms.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <engine/ActionController.h>
#include <GLEXT/GLState.h>

RenderGeoms *RenderGeoms::instance()
{
	static RenderGeoms instance;
	return &instance;
}

RenderGeoms::RenderGeoms()
{
}

RenderGeoms::~RenderGeoms()
{
}

void RenderGeoms::draw(const unsigned state)
{
	if (!OptionsDisplay::instance()->getDrawCollisionGeoms()) return;

	GLState glState(GLState::TEXTURE_OFF);
	glColor3f(1.0f, 1.0f, 1.0f);

	//drawSpace(ScorchedClient::instance()->getActionController().
	//	getPhysics().getTargetSpace());
	drawSpace(ScorchedClient::instance()->getActionController().
		getPhysics().getTankSpace());
	drawSpace(ScorchedClient::instance()->getActionController().
		getPhysics().getParticleSpace());
}

void RenderGeoms::drawSpace(dSpaceID space)
{
	int enabledGeoms = 0;
	int geoms = dSpaceGetNumGeoms(space);
	for (int i=0; i<geoms; i++)
	{
		dGeomID geom = dSpaceGetGeom(space, i);
		if (dGeomIsEnabled(geom) != 1) continue;

		int geomClass = dGeomGetClass(geom);
		if (geomClass == dSphereClass)
		{
			dReal radius = dGeomSphereGetRadius(geom);
			const dReal *position = dGeomGetPosition(geom);

			static GLUquadric *obj = 0;
			if (!obj)
			{
				obj = gluNewQuadric();
				gluQuadricDrawStyle(obj, GLU_LINE);
			}

			glPushMatrix();
				glTranslated(position[0], position[1], position[2]);
				gluSphere(obj, radius, 6, 6);
			glPopMatrix();
		}
		else if (geomClass == dBoxClass)
		{
			// Position
			const dReal *position = dGeomGetPosition(geom);

			// Size
			dVector3 size;
			dGeomBoxGetLengths(geom, size);
			double wid = size[0];
			double hgt = size[1];
			double dep = size[2];

			// Rotation
			static double rotMatrix[16];
			dQuaternion quat;
			dGeomGetQuaternion(geom, quat);

			// And I thought this would be easy!!
			dMatrix3 matrix;
			dQtoR(quat, matrix);
			rotMatrix[0] = matrix[0];
			rotMatrix[1] = matrix[4];
			rotMatrix[2] = matrix[8];
			rotMatrix[4] = matrix[1];
			rotMatrix[5] = matrix[5];
			rotMatrix[6] = matrix[9];
			rotMatrix[8] = matrix[2];
			rotMatrix[9] = matrix[6];
			rotMatrix[10] = matrix[10];
			rotMatrix[3] = rotMatrix[7] = rotMatrix[11] = 0.0;
			rotMatrix[15] = 1.0;
			rotMatrix[12] = rotMatrix[13] = rotMatrix[14] = 0.0;	

			glPushMatrix();
				glTranslated(position[0], position[1], position[2]);
				glMultMatrixd(rotMatrix);
				glBegin(GL_LINE_LOOP);
					// Top
					glNormal3d(0,1,0);
					glVertex3d(-wid/2,hgt/2,dep/2);
					glVertex3d(wid/2,hgt/2,dep/2);
					glVertex3d(wid/2,hgt/2,-dep/2);
					glVertex3d(-wid/2,hgt/2,-dep/2);
				glEnd();
				glBegin(GL_LINE_LOOP);
					// Back
					glNormal3d(0,0,1);
					glVertex3d(-wid/2,hgt/2,dep/2);
					glVertex3d(-wid/2,-hgt/2,dep/2);
					glVertex3d(wid/2,-hgt/2,dep/2);
					glVertex3d(wid/2,hgt/2,dep/2);
				glEnd();
				glBegin(GL_LINE_LOOP);
					// Front
					glNormal3d(0,0,-1);
					glVertex3d(-wid/2,hgt/2,-dep/2);
					glVertex3d(wid/2,hgt/2,-dep/2);
					glVertex3d(wid/2,-hgt/2,-dep/2);
					glVertex3d(-wid/2,-hgt/2,-dep/2);
				glEnd();
				glBegin(GL_LINE_LOOP);
					// Left
					glNormal3d(1,0,0);
					glVertex3d(wid/2,hgt/2,-dep/2);
					glVertex3d(wid/2,hgt/2,dep/2);
					glVertex3d(wid/2,-hgt/2,dep/2);
					glVertex3d(wid/2,-hgt/2,-dep/2);
				glEnd();
				glBegin(GL_LINE_LOOP);
					// Right
					glNormal3d(-1,0,0);
					glVertex3d(-wid/2,hgt/2,dep/2);
					glVertex3d(-wid/2,hgt/2,-dep/2);
					glVertex3d(-wid/2,-hgt/2,-dep/2);
					glVertex3d(-wid/2,-hgt/2,dep/2);
				glEnd();
				glBegin(GL_LINE_LOOP);
					// Bottom
					glNormal3d(0,-1,0);
					glVertex3d(-wid/2,-hgt/2,dep/2);
					glVertex3d(-wid/2,-hgt/2,-dep/2);
					glVertex3d(wid/2,-hgt/2,-dep/2);
					glVertex3d(wid/2,-hgt/2,dep/2);
				glEnd();
			glPopMatrix();
		}
	}
}
