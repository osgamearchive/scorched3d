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


#if !defined(__INCLUDE_GLVertexTexArrayh_INCLUDE__)
#define __INCLUDE_GLVertexTexArrayh_INCLUDE__

#include <GLEXT/GLVertexArray.h>
#include <GLEXT/GLTexture.h>

class GLVertexTexArray : public GLVertexArray
{
public:
	struct GLVertexArrayTexCoord
	{
		GLfloat a;
		GLfloat b;
	};

	GLVertexTexArray(GLTexture *texture, int noTris);
	virtual ~GLVertexTexArray();

	virtual void draw();
	void setTexCoord(int offset, GLfloat a, GLfloat b);

	GLVertexArrayTexCoord &getTexCoordInfo(int x) { return texCoord_[x]; }

protected:
	GLVertexArrayTexCoord *texCoord_;
	GLTexture *texture_;

	virtual void buildList();
};


#endif
