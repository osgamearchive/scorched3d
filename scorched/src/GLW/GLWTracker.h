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

#ifndef _GLWTRACKER_H_
#define _GLWTRACKER_H_

#include <GLW/GLWVisibleWidget.h>

class GLWTrackerI
{
public:
	virtual void currentChanged(unsigned int id, float valueX, float valueY) = 0;
};

class GLWTracker : public GLWVisibleWidget
{
public:
	GLWTracker(float x, float y, float w, float range);
	virtual ~GLWTracker();

	void setHandler(GLWTrackerI *handler) { handler_ = handler; }

	float getCurrentX() { return currentX_; }
	float getCurrentY() { return currentY_; }
	void setCurrentX(float currentx) { currentX_ = currentx; }
	void setCurrentY(float currenty) { currentY_ = currenty; }

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);

METACLASSID

protected:
	GLWTrackerI *handler_;
	bool dragging_;
	float currentX_, currentY_;
	float range_;
	float startX_, startY_, startCurrentX_, startCurrentY_;

};

#endif /* _GLWTRACKER_H_ */
