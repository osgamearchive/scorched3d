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


// GLVertexArray.h: interface for the GLVertexArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVERTEXARRAY_H__281612E4_C081_45C1_A049_B92631DBA524__INCLUDED_)
#define AFX_GLVERTEXARRAY_H__281612E4_C081_45C1_A049_B92631DBA524__INCLUDED_


#include <GLEXT/GLState.h>

class GLVertexArray  
{
public:
	struct GLVertexArrayVertex 
	{
		GLfloat	x;
		GLfloat	y;
		GLfloat	z;
	};

	struct GLVertexArrayColor
	{
		GLfloat r;
		GLfloat g;
		GLfloat b;
	};

	GLVertexArray(int noTris);
	virtual ~GLVertexArray();

	virtual void draw();
	void setVertex(int offset, GLfloat x, GLfloat y, GLfloat z);
	void setColor(int offset, GLfloat r, GLfloat g, GLfloat b);

	int getNoTris() { return noTris_; }
	GLVertexArrayVertex &getVertexInfo(int x) { return vertices_[x]; }
	GLVertexArrayColor &getColorInfo(int x) { return colors_[x]; }

protected:
	int noTris_;
	GLVertexArrayVertex *vertices_;
	GLVertexArrayColor *colors_;
	GLuint listNo_;

	virtual void buildList();

};

#endif // !defined(AFX_GLVERTEXARRAY_H__281612E4_C081_45C1_A049_B92631DBA524__INCLUDED_)
