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


#ifndef _GLFONT2D_H_
#define _GLFONT2D_H_

// GLFont2d.h: interface for the GLFont2d class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>
#include <common/Vector.h>

class GLFont2d  
{
public:
	GLFont2d();
	virtual ~GLFont2d();

	bool createFont(char *typeFace, char *typeMap, float moveWidth = 0.65f);
	bool getInit();

	void draw(Vector &color, float size, 
			  float x, float y, float z, 
			  const char *fmt, ...);
	void drawLen(unsigned len, 
				 Vector &color, float size, 
				 float x, float y, float z, 
				 const char *fmt, ...);
	void drawBilboard(Vector &color, float size, 
			  float x, float y, float z, 
			  const char *fmt, ...);

protected:
	GLTexture fontTexture_;
	GLuint fontBase_;
	
	virtual bool drawString(unsigned len,
							Vector &color, float size, 
							float x, float y, float z, 
							const char *string,
							bool bilboard);

	bool createTexture(char *typeFace, char* typeMap);
	bool createLetters(float moveWidth);
	void drawLetter(int i, float moveWidth, bool bilboard);

};

#endif /* _GLFONT2D_H_ */
