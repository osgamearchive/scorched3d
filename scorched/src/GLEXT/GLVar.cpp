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

#include <GLEXT/GLVar.h>
#include <GLEXT/GLStateExtension.h>
#include <stdlib.h>

GLVar::GLVar(int arraySize) : noTriangles_(0)
{
	currentBuffer_ = bufferOne_ = new GLVarBuffer(arraySize);
	bufferTwo_ = new GLVarBuffer(arraySize);
}

GLVar::~GLVar()
{
	delete bufferOne_;
	delete bufferTwo_;
}

GLVar::GLVarBuffer::GLVarBuffer(GLuint arraySize) : 
	arraySize_(arraySize), arrayUsed_(0)
{
	buffer_ = bufferPos_ = new GLVarStruct[arraySize];
}

GLVar::GLVarBuffer::~GLVarBuffer()
{
	delete [] buffer_;
}

void GLVar::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat txa1, GLfloat tya1,
						GLfloat txb1, GLfloat tyb1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat txa2, GLfloat tya2,
						GLfloat txb2, GLfloat tyb2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat txa3, GLfloat tya3,
						GLfloat txb3, GLfloat tyb3)
{
	if (currentBuffer_->addTriangle(
		x1, y1, z1,
		txa1, tya1,
		txb1, tyb1,
		x2, y2, z2,
		txa2, tya2,
		txb2, tyb2,
		x3, y3, z3,
		txa3, tya3,
		txb3, tyb3))
	{
		swapBuffers();
	}
	noTriangles_++;
}

void GLVar::draw()
{
	//currentBuffer_->draw();
	swapBuffers();
}

void GLVar::swapBuffers()
{
	if (currentBuffer_ == bufferOne_) currentBuffer_ = bufferTwo_;
	else currentBuffer_ = bufferOne_;
}

bool GLVar::GLVarBuffer::addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat txa1, GLfloat tya1,
						GLfloat txb1, GLfloat tyb1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat txa2, GLfloat tya2,
						GLfloat txb2, GLfloat tyb2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat txa3, GLfloat tya3,
						GLfloat txb3, GLfloat tyb3)
{
	GLVarStruct newStruct = 
		{ 
		x1, y1, z1,
		txa1, tya1,
		txb1, tyb1,
		x2, y2, z2,
		txa2, tya2,
		txb2, tyb2,
		x3, y3, z3,
		txa3, tya3,
		txb3, tyb3
		};
	//(*bufferPos_++) = newStruct;

	glBegin(GL_TRIANGLES);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glMultiTextCoord2fARB()
				(GL_TEXTURE1_ARB, txa1, tya1);
			if (GLStateExtension::getTextureUnits() > 2)
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE2_ARB, txb1, tyb1);
		}
		glTexCoord2f(txa1, tya1);
		glVertex3fv(&newStruct.x1);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glMultiTextCoord2fARB()
				(GL_TEXTURE1_ARB, txa2, tya2);
			if (GLStateExtension::getTextureUnits() > 2)
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE2_ARB, txb2, tyb2);
		}
		glTexCoord2f(txa2, tya2);
		glVertex3fv(&newStruct.x2);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glMultiTextCoord2fARB()
				(GL_TEXTURE1_ARB, txa3, tya3);
			if (GLStateExtension::getTextureUnits() > 2)
				GLStateExtension::glMultiTextCoord2fARB()
					(GL_TEXTURE2_ARB, txb3, tyb3);
		}
		glTexCoord2f(txa3, tya3);
		glVertex3fv(&newStruct.x3);
	glEnd();

	if (arrayUsed_++ > arraySize_ - 1) 
	{
		//draw();
		//return true;
	}

	return false;
}

void GLVar::GLVarBuffer::draw()
{	
	const int Stride = sizeof(GLVarStruct) / 3;

	if (arrayUsed_)
	{
		// Enable correct arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, Stride, &buffer_[0].x1);

		if (GLStateExtension::glClientActiveTextureARB())
		{
			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE0_ARB); 
			glTexCoordPointer(2, GL_FLOAT, Stride, &buffer_[0].txa1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

			GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE1_ARB); 
			glTexCoordPointer(2, GL_FLOAT, Stride, &buffer_[0].txa1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (GLStateExtension::getTextureUnits() > 2)
			{
				GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB); 
				glTexCoordPointer(2, GL_FLOAT, Stride, &buffer_[0].txb1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		else
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, Stride, &buffer_[0].txa1);
		}

		// Draw the arrays
		glDrawArrays(GL_TRIANGLES, 0, arrayUsed_ * 3 );

		// Disable the correct arrays
		glDisableClientState(GL_VERTEX_ARRAY);
		if (GLStateExtension::glClientActiveTextureARB())
		{
			if (GLStateExtension::getTextureUnits() > 2)
			{
				GLStateExtension::glClientActiveTextureARB()(GL_TEXTURE2_ARB); 
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

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

