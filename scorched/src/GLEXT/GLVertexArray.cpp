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

#include <GLEXT/GLVertexArray.h>
#include <GLEXT/GLStateExtension.h>
#include <stdlib.h>

GLVertexArray::GLVertexArray(GLenum prim, int noTris, 
	unsigned int type, GLTexture *texture) : 
	prim_(prim), noTris_(noTris), setup_(false),
	verticesVBO_(0), colorsVBO_(0), textureVBO_(0),
	texture_(texture), secondTexture_(false),
	useVBO_(false)
{
	if (type & typeVertex)
	{
		vertices_ = (GLVertexArrayVertex *) 
			malloc(sizeof(GLVertexArrayVertex) * noTris);
	}
	else vertices_ = 0;
	if (type & typeColor)
	{
		colors_ = (GLVertexArrayColor *)
			malloc(sizeof(GLVertexArrayColor) * noTris);
	}
	else colors_ = 0;
	if (type & typeTexture)
	{
		texCoord_ = (GLVertexArrayTexCoord*) 
			malloc(sizeof(GLVertexArrayTexCoord) * noTris);
	}
	else texCoord_ = 0;
}

GLVertexArray::~GLVertexArray()
{
	if (colors_) free(colors_);
	if (vertices_) free(vertices_);
	if (texCoord_) free(texCoord_);
	if (verticesVBO_) GLStateExtension::glDeleteBuffersARB()(1, &verticesVBO_);
	if (colorsVBO_) GLStateExtension::glDeleteBuffersARB()(1, &colorsVBO_);
	if (textureVBO_) GLStateExtension::glDeleteBuffersARB()(1, &textureVBO_);
}

void GLVertexArray::setVertex(int offset, GLfloat x, GLfloat y, GLfloat z)
{
	DIALOG_ASSERT(vertices_);
	DIALOG_ASSERT(offset < noTris_);
	GLVertexArrayVertex newVertex = { x ,y ,z };
	vertices_[offset] = newVertex;
}

void GLVertexArray::setColor(int offset, GLfloat r, GLfloat g, GLfloat b)
{
	DIALOG_ASSERT(colors_);
	DIALOG_ASSERT(offset < noTris_);
	GLVertexArrayColor newColor = { r ,g ,b };
	colors_[offset] = newColor;
}

void GLVertexArray::setTexCoord(int offset, GLfloat a, GLfloat b)
{
	DIALOG_ASSERT(texCoord_);
	DIALOG_ASSERT(offset < noTris_);
	GLVertexArrayTexCoord newTexCoord = { a, b };
	texCoord_[offset] = newTexCoord;
}

void GLVertexArray::setup()
{
	if (useVBO_ && GLStateExtension::glGenBuffersARB())
	{
		useVBO_ = true;
		if (vertices_)
		{
			GLStateExtension::glGenBuffersARB()(1, &verticesVBO_);
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, verticesVBO_);
			GLStateExtension::glBufferDataARB()(GL_ARRAY_BUFFER_ARB, 
				sizeof(GLVertexArrayVertex) * noTris_, 
				vertices_,
				GL_STATIC_DRAW_ARB);
			if (glGetError() == GL_OUT_OF_MEMORY) useVBO_ = false;
		}

		if (colors_)
		{
			GLStateExtension::glGenBuffersARB()(1, &colorsVBO_);
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, colorsVBO_);
			GLStateExtension::glBufferDataARB()(GL_ARRAY_BUFFER_ARB, 
				sizeof(GLVertexArrayColor) * noTris_, 
				colors_,
				GL_STATIC_DRAW_ARB);
			if (glGetError() == GL_OUT_OF_MEMORY) useVBO_ = false;
		}

		if (texCoord_)
		{
			GLStateExtension::glGenBuffersARB()(1, &textureVBO_);
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, textureVBO_);
			GLStateExtension::glBufferDataARB()(GL_ARRAY_BUFFER_ARB, 
				sizeof(GLVertexArrayTexCoord) * noTris_, 
				texCoord_,
				GL_STATIC_DRAW_ARB);
			if (glGetError() == GL_OUT_OF_MEMORY) useVBO_ = false;
		}
	}
	else
	{
		useVBO_ = false;
	}
}

void GLVertexArray::draw()
{
	if (!setup_) 
	{
		setup();
		setup_ = true;
	}

	unsigned int requiredState = 0;
	if (texCoord_)
	{
		requiredState = GLState::TEXTURE_ON;
		if (texture_ && texture_->getTexFormat() == GL_RGBA)
		{
			requiredState |= GLState::BLEND_ON;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		}
	}
	GLState textureState(requiredState);
	if (texCoord_ && texture_) texture_->draw();

	if (!useVBO_)
	{
		if (vertices_)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, &vertices_[0].x);
		}
		if (colors_)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, &colors_[0].r);
		}
		if (texCoord_)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (secondTexture_) GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB); 
			glTexCoordPointer(2, GL_FLOAT, 0, &texCoord_[0].a);
		}
	}
	else
	{
		if (vertices_)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, verticesVBO_);
			glVertexPointer(3, GL_FLOAT, 0, 0);
		}
		if (colors_)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, colorsVBO_);
			glColorPointer(3, GL_FLOAT, 0, 0);
		}
		if (texCoord_)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (secondTexture_) GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB); 
			GLStateExtension::glBindBufferARB()(GL_ARRAY_BUFFER_ARB, textureVBO_);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
		}
	}

	glDrawArrays(prim_, 0, noTris_);

	if (vertices_) glDisableClientState(GL_VERTEX_ARRAY);
	if (colors_) glDisableClientState(GL_COLOR_ARRAY);
	if (texCoord_)
	{	
		if (secondTexture_) GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB); 
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}
