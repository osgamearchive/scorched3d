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

#include <landscape/LandscapeObjectsEntryModel.h>
#include <3dsparse/ModelRenderer.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>

LandscapeObjectsEntryModel::LandscapeObjectsEntryModel() :
	model(0), modelburnt(0)
{
}

LandscapeObjectsEntryModel::~LandscapeObjectsEntryModel()
{
	delete model;
	delete modelburnt;
}

void LandscapeObjectsEntryModel::render(float distance)
{
	static Vector point;
	point[0] = posX;
	point[1] = posY;
	point[2] = posZ;
	if (!GLCameraFrustum::instance()->sphereInFrustum(
		point, 2.0f)) return;

	/*{
	GLState state(GLState::TEXTURE_OFF);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
		glVertex3f(posX, posY, posZ);
	glEnd();
	glPointSize(1.0f);
	}*/

	glPushMatrix();
		glTranslatef(posX, posY, posZ);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		glScalef(size, size, size);

		glColor4f(color, color, color, 1.0f);
		if (burnt) modelburnt->drawBottomAligned();
		else model->drawBottomAligned();
	glPopMatrix();
}
