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


// GLWScrollH.h: interface for the GLWScrollH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWSCROLLH_H__2F106F2D_E9C8_4283_93F7_AE849ECCBEFD__INCLUDED_)
#define AFX_GLWSCROLLH_H__2F106F2D_E9C8_4283_93F7_AE849ECCBEFD__INCLUDED_


#include <GLW/GLWScrollButton.h>

class GLWScrollHI
{
public:
	virtual ~GLWScrollHI();

	virtual void positionChange(unsigned int id, int current, int movement) = 0;
};

class GLWScrollH  : 
	public GLWVisibleWidget, 
	public GLWButtonI,
	public GLWScrollButtonI
{
public:
	GLWScrollH(float x, float y, float w, int min, int max, int see = 1, GLWScrollHI *handler = 0);
	virtual ~GLWScrollH();

	void setHandler(GLWScrollHI *handler = 0) { handler_ = handler; }
	void setMin(int min) { min_ = min; }
	void setMax(int max) { max_ = max; }
	void setSee(int see) { see_ = see; }
	void setCurrent(int c) { current_ = c; }

	int getCurrent() { return current_; }
	int getMin() { return min_; }
	int getMax() { return max_; }

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void buttonDown(unsigned int id);

	virtual void startDrag(unsigned int id);
	virtual void buttonDrag(unsigned int id, float x, float y);

METACLASSID
protected:
	GLWScrollHI *handler_;
	GLWButton leftButton_;
	GLWButton rightButton_;
	GLWScrollButton middleButton_;
	int min_, max_, see_;
	int current_, dragCurrent_;

};

#endif // !defined(AFX_GLWSCROLLH_H__2F106F2D_E9C8_4283_93F7_AE849ECCBEFD__INCLUDED_)
