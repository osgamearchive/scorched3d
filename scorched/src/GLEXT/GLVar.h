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

#if !defined(AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_)
#define AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_

#include <GLEXT/GLState.h>

class GLVar  
{
public:
	GLVar(int size);
	virtual ~GLVar();

	void addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat txa1, GLfloat tya1,
						GLfloat txb1, GLfloat tyb1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat txa2, GLfloat tya2,
						GLfloat txb2, GLfloat tyb2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat txa3, GLfloat tya3,
						GLfloat txb3, GLfloat tyb3);
	void draw();
	unsigned int getNoTriangles() 
		{ unsigned int t = noTriangles_; noTriangles_ = 0; return t; }

protected:
	struct GLVarStruct
	{
		GLfloat	x1, y1, z1;
		GLfloat txa1, tya1;
		GLfloat txb1, tyb1;
		GLfloat	x2, y2, z2;
		GLfloat txa2, tya2;
		GLfloat txb2, tyb2;
		GLfloat	x3, y3, z3;
		GLfloat txa3, tya3;
		GLfloat txb3, tyb3;
	};
	
	class GLVarBuffer
	{
	public:
		GLVarBuffer(GLuint arraySize);
		virtual ~GLVarBuffer();

		bool addTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
						GLfloat txa1, GLfloat tya1,
						GLfloat txb1, GLfloat tyb1,
						GLfloat x2, GLfloat y2, GLfloat z2,
						GLfloat txa2, GLfloat tya2,
						GLfloat txb2, GLfloat tyb2,
						GLfloat x3, GLfloat y3, GLfloat z3,
						GLfloat txa3, GLfloat tya3,
						GLfloat txb3, GLfloat tyb3);
		void draw();

	protected:
		// The buffers start and current positions
		GLVarStruct *buffer_;
		GLVarStruct *bufferPos_;

		// The total number of entries allowed and used
		GLuint arraySize_;
		GLuint arrayUsed_;
	};

	// Pointers to buffer objects and current buffer
	GLVarBuffer *bufferOne_;
	GLVarBuffer *bufferTwo_;
	GLVarBuffer *currentBuffer_;
	unsigned int noTriangles_;

	void swapBuffers();

};

#endif // !defined(AFX_GLVAR_H__8A531272_F0A5_4F29_B551_93777C031540__INCLUDED_)
