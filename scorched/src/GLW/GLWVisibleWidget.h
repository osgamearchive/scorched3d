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


// GLWVisibleWidget.h: interface for the GLWVisibleWidget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWVISIBLEWIDGET_H__1EB53BAC_EE59_4434_805E_F9FAA72BEF37__INCLUDED_)
#define AFX_GLWVISIBLEWIDGET_H__1EB53BAC_EE59_4434_805E_F9FAA72BEF37__INCLUDED_


#include <GLW/GLWidget.h>

class GLWVisibleWidget : public GLWidget
{
public:
	GLWVisibleWidget(float x, float y, float w, float h);
	virtual ~GLWVisibleWidget();

	virtual float getX() { return x_; }
	virtual float getY() { return y_; }
	virtual float getW() { return w_; }
	virtual float getH() { return h_; }

	virtual void setX(float x) { x_ = x; }
	virtual void setY(float y) { y_ = y; }
	virtual void setW(float w) { w_ = w; }
	virtual void setH(float h) { h_ = h; }

METACLASSID

	static bool inBox(float posX, float posY, float x, float y, float w, float h);
	static void drawRoundBox(float x, float y, float w, float h, bool large);
	static void drawCircle(int startA, int endA, float posX, float posY, bool size);
	static void drawBox(float x, float y, float w, float h, bool depressed);

protected:
	float x_, y_, w_, h_;

};

#endif // !defined(AFX_GLWVISIBLEWIDGET_H__1EB53BAC_EE59_4434_805E_F9FAA72BEF37__INCLUDED_)
