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


#ifndef _GLFONTBANNER_H_
#define _GLFONTBANNER_H_
// GLFontBanner.h: interface for the GLFontBanner class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWidget.h>
#include <common/Vector.h>

class GLFontBannerEntry
{
public:
	GLFontBannerEntry();
	virtual ~GLFontBannerEntry();

	bool decrementTime(float time);

	char *getText() { return textLine_; }
	void setText(char *text);

	void setColor(Vector &color) { color_ = color; }
	Vector &getColor() { return color_; }

protected:
	char textLine_[1024];
	Vector color_;
	float timeRemaining_;
};

class GLFontBanner : public GLWidget
{
public:
	static Vector defaultColor;

	GLFontBanner(GLfloat x, GLfloat y, GLfloat w, int lines);
	virtual ~GLFontBanner();

	virtual void simulate(float frameTime);
	virtual void draw();
	void addLine(Vector &color, const char *fmt, ...);
	
	void setX(float x) { x_ = x; }
	void setY(float y) { y_ = y; }

METACLASSID

protected:
	GLfloat x_, y_, w_;
	int totalLines_;
	int startLine_, usedLines_;

	GLFontBannerEntry *textLines_;
};

#endif /* _GLFONTBANNER_H_ */
