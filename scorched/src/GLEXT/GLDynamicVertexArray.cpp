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

#include <GLEXT/GLDynamicVertexArray.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLInfo.h>

GLDynamicVertexArray *GLDynamicVertexArray::instance_ = 0;

GLDynamicVertexArray *GLDynamicVertexArray::instance()
{
	if (!instance_) 
	{
		instance_ = new GLDynamicVertexArray();
	}
	return instance_;
}

GLDynamicVertexArray::GLDynamicVertexArray() : 
	capacity_(3000), used_(0)
{
	array_ = new GLfloat[capacity_];
}

GLDynamicVertexArray::~GLDynamicVertexArray()
{
	delete [] array_;
}

void GLDynamicVertexArray::drawROAM()
{
	if (used_ == 0) return;

	int stride = 5;
	if (GLStateExtension::glClientActiveTextureARB())
	{
		if (GLStateExtension::getTextureUnits() > 2)
		{
			stride = 7;
		}
	}

	// Vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_);

	// Tex Coord 0
	GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);

	if (GLStateExtension::glClientActiveTextureARB())
	{
		// Tex Coord 1
		GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);

		if (GLStateExtension::getTextureUnits() > 2)
		{
			// Tex Coord 2
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 5);
		}
	}

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, used_ / stride);
	GLInfo::addNoTriangles(used_ / stride / 3);

	// No Vertices
	glDisableClientState(GL_VERTEX_ARRAY);

	if (GLStateExtension::glClientActiveTextureARB())
	{
		// No Tex Coord 1
		GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (GLStateExtension::getTextureUnits() > 2)
		{
			// No Tex Coord 2
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	// No Tex Coord 0
	GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	used_ = 0;
}

void GLDynamicVertexArray::drawQuadStrip(bool useColor)
{
	int stride = 5;
	if (useColor) stride = 8;

	if (used_ < 4 * stride)
	{
		used_ = 0;
		return;	
	}

	// Vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_);

	// Tex Coord 0
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);

	// Color
	if (useColor)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 5);
	}

	// Draw
	glDrawArrays(GL_QUAD_STRIP, 0, used_ / stride);
	GLInfo::addNoTriangles((used_ / stride) - 2);

	// No Vertices
	glDisableClientState(GL_VERTEX_ARRAY);

	// No Tex Coord 0
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// No Color
	if (useColor) glDisableClientState(GL_COLOR_ARRAY);

	used_ = 0;
}
