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

#include <GLEXT/GLVertexTexArray.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLState.h>
#include <stdlib.h>

GLVertexTexArray::GLVertexTexArray(GLTexture *texture, int noTris) : 
	GLVertexArray(noTris), texture_(texture)
{
	texCoord_ = new GLVertexArrayTexCoord[noTris * 3];
}

GLVertexTexArray::~GLVertexTexArray()
{
	delete texCoord_;
}

void GLVertexTexArray::setTexCoord(int offset, GLfloat a, GLfloat b)
{
	DIALOG_ASSERT(offset < noTris_ * 3);
	GLVertexArrayTexCoord newTexCoord = {a, b};
	texCoord_[offset] = newTexCoord;
}

void GLVertexTexArray::draw()
{
	unsigned int requiredState = GLState::TEXTURE_ON;
	if (texture_->getTexFormat() == GL_RGBA)
	{
		requiredState |= GLState::BLEND_ON;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	}

	GLState textureState(requiredState);
	texture_->draw();
	GLVertexArray::draw();
}

void GLVertexTexArray::buildList()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(GLVertexArrayVertex), &vertices_[0].x);
	glColorPointer(3, GL_FLOAT, sizeof(GLVertexArrayColor), &colors_[0].r);
	glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertexArrayTexCoord), &texCoord_[0].a);

	if (GLStateExtension::glLockArraysEXT())
	{
		GLStateExtension::glLockArraysEXT()(0, noTris_ * 3);
	}

	glNewList(listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
		glDrawArrays(GL_TRIANGLES, 0, noTris_ * 3 );
	glEndList();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
