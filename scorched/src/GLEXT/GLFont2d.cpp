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


// GLFont2d.cpp: implementation of the GLFont2d class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <GLEXT/GLState.h>
#include <GLEXT/GLFont2d.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLCameraFrustum.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLFont2d::GLFont2d() : fontBase_(0)
{

}

GLFont2d::~GLFont2d()
{
	if (fontBase_)
	{
		glDeleteLists(fontBase_, 256);
	}
}

void GLFont2d::draw(Vector &color, float size, 
					float x, float y, float z, 
					const char *fmt, ...)
{
	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	drawString((GLsizei) strlen(text), color, size, x, y, z, text, false);
}

void GLFont2d::drawLen(unsigned len, Vector &color, float size, 
					   float x, float y, float z, 
					   const char *fmt, ...)
{
	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	GLsizei l = (GLsizei) (MIN(len, strlen(text)));
	drawString(l, color, size, x, y, z, text, false);
}

void GLFont2d::drawBilboard(Vector &color, float size, 
			  float x, float y, float z, 
			  const char *fmt, ...)
{
	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	drawString((GLsizei) strlen(text), color, size, x, y, z, text, true);
}

bool GLFont2d::drawString(unsigned length, Vector &color, float size, 
						  float x, float y, float z, 
						  const char *string,
						  bool bilboard)
{
	if (fontBase_)
	{
		GLState currentState(GLState::BLEND_ON | GLState::TEXTURE_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		fontTexture_.draw(true);

		glColor3fv(color);

		if (bilboard)
		{
			glPushMatrix();
				glTranslatef(x,y,z);
				glScalef(size, size, size);

				for (const char *current = string; *current; current++)
				{
					drawLetter(int(*current) - 32, 0.65f, true);
				}
			glPopMatrix();
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);
				glListBase(fontBase_ - 32);

				glPushMatrix();
					glTranslatef(x,y,z);
					glScalef(size, size, size);
					glCallLists(length, GL_UNSIGNED_BYTE, string);
				glPopMatrix();
			glPopAttrib();
		}

		return true;
	}
	return false;
}

bool GLFont2d::getInit()
{
	return (fontTexture_.textureValid());
}

bool GLFont2d::createFont(char *typeFace, char* typeMap, float moveWidth)
{
	if (!createTexture(typeFace, typeMap)) return false;
	if (!createLetters(moveWidth)) return false;

	return true;
}

bool GLFont2d::createTexture(char *typeFace, char* typeMap)
{
	GLBitmap fontMap(typeFace, typeMap, false);
	if (!fontMap.getBits()) return false;

	fontTexture_.create(fontMap, GL_RGBA);
	fontTexture_.draw(true);
	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

bool GLFont2d::createLetters(float moveWidth)
{
	// create 256 display lists
	fontBase_ = glGenLists(256);

	// creat list for each character
	for (int i = 0; i < 256; i++) 
	{
		glNewList(fontBase_ + i, GL_COMPILE);
			drawLetter(i, moveWidth, false);
		glEndList();
	}
	return true;
}

void GLFont2d::drawLetter(int i, float moveWidth, bool bilboard)
{
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	Vector &bilY = GLCameraFrustum::instance()->getBilboardVectorY();

	const int lettersPerSide = 16;
	const float letterSize = 1.0f / float(lettersPerSide);
	const float mapSize = letterSize - 0.005f;

	float cx = (float) (i % lettersPerSide) / float(lettersPerSide);
	float cy = (float) (i / lettersPerSide) / float(lettersPerSide);
	cy = (1.0f - cy) - letterSize;

	// Draw letter
	glBegin(GL_QUADS);
		glTexCoord2f(cx, cy);
		glVertex2f(0.0f, 0.0f);

		glTexCoord2f(cx + mapSize, cy);
		if (bilboard) glVertex3fv(bilX);
		else glVertex2f(1.0f, 0.0f);

		glTexCoord2f(cx + mapSize, cy + mapSize);
		if (bilboard) glVertex3fv(bilX + bilY);
		else glVertex2f(1.0f, 1.0f);

		glTexCoord2f(cx, cy + mapSize);
		if (bilboard) glVertex3fv(bilY);
		else glVertex2f(0.0f, 1.0f);
	glEnd();

	// move to left of char 
	if (bilboard)
	{
		bilX *= moveWidth;
		glTranslatef(bilX[0], bilX[1], bilX[2]);
	}
	else glTranslatef(moveWidth, 0.0f, 0.0f);
}
