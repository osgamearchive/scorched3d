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


// GLVar.cpp: implementation of the GLVar class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLVar.h>
#include <GLEXT/GLStateExtension.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLVar::GLVar(int arraySize) : noTriangles_(0)
{
	// Allocate array memory
	int size = (arraySize * sizeof(GLVarStruct) * 2);
	array_ = (GLVarStruct *) new GLVarStruct[size];
	DIALOG_ASSERT(array_); 

	currentBuffer_ = bufferOne_ = new GLVarBuffer(array_, arraySize);
	bufferTwo_ = new GLVarBuffer(array_ + arraySize, arraySize);
}

GLVar::~GLVar()
{
	delete [] array_;
	delete bufferOne_;
	delete bufferTwo_;
}

GLVar::GLVarBuffer::GLVarBuffer(GLVarStruct *buffer, GLuint arraySize) : 
	arraySize_(arraySize), arrayUsed_(0), 
	buffer_(buffer), bufferPos_(buffer)
{

}

GLVar::GLVarBuffer::~GLVarBuffer()
{

}

void GLVar::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat tx1, GLfloat ty1,
						GLfloat r1, GLfloat g1, GLfloat b1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat tx2, GLfloat ty2,
						GLfloat r2, GLfloat g2, GLfloat b2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat tx3, GLfloat ty3,
						GLfloat r3, GLfloat g3, GLfloat b3)
{
	if (currentBuffer_->addTriangle(
		x1, y1, z1,
		tx1, ty1,
		r1, g1, b1,
		x2, y2, z2,
		tx2, ty2,
		r2, g2, b2,
		x3, y3, z3,
		tx3, ty3,
		r3, g3, b3))
	{
		swapBuffers();
	}
	noTriangles_++;
}

void GLVar::draw()
{
	currentBuffer_->draw();
	swapBuffers();
}

void GLVar::swapBuffers()
{
	if (currentBuffer_ == bufferOne_) currentBuffer_ = bufferTwo_;
	else currentBuffer_ = bufferOne_;
}

bool GLVar::GLVarBuffer::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat tx1, GLfloat ty1,
						GLfloat r1, GLfloat g1, GLfloat b1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat tx2, GLfloat ty2,
						GLfloat r2, GLfloat g2, GLfloat b2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat tx3, GLfloat ty3,
						GLfloat r3, GLfloat g3, GLfloat b3)
{
	if (!arrayUsed_) waitForFinish();

	GLVarStruct newStruct = 
		{ x1, y1, z1,
		  tx1, ty1,
		  r1, g1, b1, 
		  x2,y2,z2,
		  tx2, ty2,
		  r2, g2, b2, 
		  x3,y3,z3,
		  tx3, ty3,
		  r3, g3, b3
		};
	(*bufferPos_++) = newStruct;

	if (arrayUsed_++ > arraySize_ - 1) 
	{
		draw();
		return true;
	}

	return false;
}

void GLVar::GLVarBuffer::draw()
{	
	if (arrayUsed_)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB); 
			glTexCoordPointer(2, GL_FLOAT, sizeof(GLVarStruct) / 3, &buffer_[0].tx1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB); 
			glTexCoordPointer(2, GL_FLOAT, sizeof(GLVarStruct) / 3, &buffer_[0].tx1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		else
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(GLVarStruct) / 3, &buffer_[0].tx1);
		}

		glVertexPointer(3, GL_FLOAT, sizeof(GLVarStruct) / 3, &buffer_[0].x1);
		glColorPointer(3, GL_FLOAT, sizeof(GLVarStruct) / 3, &buffer_[0].r1);
		
		glDrawArrays(GL_TRIANGLES, 0, arrayUsed_ * 3 );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB); 
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB); 
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		else
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		arrayUsed_ = 0;
		bufferPos_ = buffer_;
	}
}

void GLVar::GLVarBuffer::waitForFinish()
{

}
