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


// GLVertexArray.cpp: implementation of the GLVertexArray class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLVertexArray.h>
#include <GLEXT/GLStateExtension.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLVertexArray::GLVertexArray(int noTris) : noTris_(noTris), listNo_(0)
{
	vertices_ = new GLVertexArrayVertex[noTris * 3];
	colors_ = new GLVertexArrayColor[noTris * 3];
}

GLVertexArray::~GLVertexArray()
{
	delete colors_;
	delete vertices_;
	if (glIsList(listNo_)) glDeleteLists(listNo_, 1);
}

void GLVertexArray::setVertex(int offset, GLfloat x, GLfloat y, GLfloat z)
{
	DIALOG_ASSERT(offset < noTris_ * 3);
	GLVertexArrayVertex newVertex = { x ,y ,z };
	vertices_[offset] = newVertex;
}

void GLVertexArray::setColor(int offset, GLfloat r, GLfloat g, GLfloat b)
{
	DIALOG_ASSERT(offset < noTris_ * 3);
	GLVertexArrayColor newColor = { r ,g ,b };
	colors_[offset] = newColor;
}

void GLVertexArray::draw()
{
	if (listNo_)
	{
		glCallList(listNo_);
	}
	else
	{
		buildList();
	}
}

void GLVertexArray::buildList()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(GLVertexArrayVertex), &vertices_[0].x);
	glColorPointer(3, GL_FLOAT, sizeof(GLVertexArrayColor), &colors_[0].r);

	if (GLStateExtension::glLockArraysEXT())
	{
		GLStateExtension::glLockArraysEXT()(0, noTris_ * 3);
	}

	glNewList(listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
		glDrawArrays(GL_TRIANGLES, 0, noTris_ * 3 );
	glEndList();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
