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


// GLVar.h: interface for the GLVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_)
#define AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_


#include <GLEXT/GLState.h>

class GLVar  
{
public:
	GLVar(int size);
	virtual ~GLVar();

	void addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat tx1, GLfloat ty1,
						GLfloat r1, GLfloat g1, GLfloat b1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat tx2, GLfloat ty2,
						GLfloat r2, GLfloat g2, GLfloat b2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat tx3, GLfloat ty3,
						GLfloat r3, GLfloat g3, GLfloat b3);
	void draw();
	unsigned long getNoTriangles() { unsigned long t = noTriangles_; noTriangles_ = 0; return t; }

protected:
	struct GLVarStruct
	{
		GLfloat	x1, y1, z1;
		GLfloat tx1, ty1;
		GLfloat r1, g1, b1;
		GLfloat	x2, y2, z2;
		GLfloat tx2, ty2;
		GLfloat r2, g2, b2;
		GLfloat	x3, y3, z3;
		GLfloat tx3, ty3;
		GLfloat r3, g3, b3;
	};
	
	class GLVarBuffer
	{
	public:
		GLVarBuffer(GLVarStruct *buffer, GLuint arraySize);
		virtual ~GLVarBuffer();

		bool addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat tx1, GLfloat ty1,
						GLfloat r1, GLfloat g1, GLfloat b1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat tx2, GLfloat ty2,
						GLfloat r2, GLfloat g2, GLfloat b2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat tx3, GLfloat ty3,
						GLfloat r3, GLfloat g3, GLfloat b3);
		void draw();

	protected:
		// The buffers start and current positions
		GLVarStruct *buffer_;
		GLVarStruct *bufferPos_;

		// The total number of entries allowed and used
		GLuint arraySize_;
		GLuint arrayUsed_;

		void waitForFinish();
	};

	// Array used to store all memory allocated on card
	GLVarStruct *array_;

	// Pointers to buffer objects and current buffer
	GLVarBuffer *bufferOne_;
	GLVarBuffer *bufferTwo_;
	GLVarBuffer *currentBuffer_;
	unsigned long noTriangles_;

	void swapBuffers();

};

#endif // !defined(AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_)
