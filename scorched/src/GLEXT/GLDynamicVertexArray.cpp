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
#include <GLEXT/GLInfo.h>
#include <common/OptionsDisplay.h>

GLDynamicVertexArray *GLDynamicVertexArray::instance_ = 0;

GLDynamicVertexArray::GLDynamicVertexArray() : 
	capacity_(3000), used_(0), array_(0)
{
	array_ = new GLfloat[capacity_];
}

GLDynamicVertexArray::~GLDynamicVertexArray()
{
	delete [] array_;
}

void GLDynamicVertexArray::drawROAM()
{
	int stride = 5;
	if (GLStateExtension::glClientActiveTextureARB())
	{
		if (GLStateExtension::getTextureUnits() > 2)
		{
			stride = 7;
		}
	}
	if (used_ < stride * 3)
	{
		used_ = 0;
		return;
	}

	if (OptionsDisplay::instance()->getNoVBO())
	{
		GLfloat *start = array_;
		glBegin(GL_TRIANGLES);
		for (int i=0; i<used_; i+=stride)
		{
			glTexCoord2f(start[3], start[4]); 
			if (GLStateExtension::glClientActiveTextureARB())
			{
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE1_ARB, start[3], start[4]);
				if (GLStateExtension::getTextureUnits() > 2)
				{
					GLStateExtension::glMultiTextCoord2fARB()
						(GL_TEXTURE2_ARB, start[5], start[6]);
				}
			}

			glVertex3fv(start); 
			start += stride;
		}
		glEnd();
	}
	else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (GLStateExtension::getTextureUnits() > 2)
			{
				GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_);
		glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB);
			glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);
			if (GLStateExtension::getTextureUnits() > 2)
			{
				GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB);
				glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 5);
			}
		}

		glDrawArrays(GL_TRIANGLES, 0, used_ / stride);

		glDisableClientState(GL_VERTEX_ARRAY);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			if (GLStateExtension::getTextureUnits() > 2)
			{
				GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	GLInfo::addNoTriangles((used_ / stride) / 3);
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

	if (OptionsDisplay::instance()->getNoVBO())
	{
		GLfloat *start = array_;
		glBegin(GL_QUAD_STRIP);
		for (int i=0; i<used_; i+=stride)
		{
			glTexCoord2fv(start + 3);
			if (useColor) glColor3fv(start + 5);
			glVertex3fv(start);
			start += stride;
		}
		glEnd();
	}
	else
	{
		// Define	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (useColor) glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_);
		glTexCoordPointer(2, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 3);
		if (useColor) glColorPointer(3, GL_FLOAT, stride * sizeof(GL_FLOAT), array_ + 5);

		// Draw
		glDrawArrays(GL_QUAD_STRIP, 0, used_ / stride);
		
		// Undefine
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if (useColor) glDisableClientState(GL_COLOR_ARRAY);
	}

	GLInfo::addNoTriangles((used_ / stride) - 2);
	used_ = 0;
}
