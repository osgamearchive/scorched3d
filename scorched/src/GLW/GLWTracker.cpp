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

#include <GLW/GLWTracker.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLState.h>
#include <common/Keyboard.h>
#include <common/OptionsDisplay.h>

REGISTER_CLASS_SOURCE(GLWTracker);

GLWTracker::GLWTracker(float x, float y, float w, float range) :
	GLWVisibleWidget(x, y, w, w), 
	range_(range),
	dragging_(false), handler_(0), currentX_(0.0f), currentY_(0.0f)
{

}

GLWTracker::~GLWTracker()
{

}

void GLWTracker::draw()
{
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_LINE_LOOP);
		drawRoundBox(x_, y_, w_, w_, 5.0f);
	glEnd();
	glBegin(GL_LINES);
	float size = 0;
	while (size < w_ / 2.0f)
	{
		glVertex2f(size + x_ + w_ / 2.0f, y_ + h_ / 2.0f - 2.0f);
		glVertex2f(size + x_ + w_ / 2.0f, y_ + h_ / 2.0f + 2.0f);

		glVertex2f(-size + x_ + w_ / 2.0f, y_ + h_ / 2.0f - 2.0f);
		glVertex2f(-size + x_ + w_ / 2.0f, y_ + h_ / 2.0f + 2.0f);
		size += 10.0f;
	}
	glEnd();
}

void GLWTracker::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		dragging_ = true;
		startX_ = x;
		startY_ = y;
		startCurrentX_ = currentX_;
		startCurrentY_ = currentY_;
	}
}

void GLWTracker::mouseUp(float x, float y, bool &skipRest)
{
	dragging_ = false;
}

void GLWTracker::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	if (dragging_)
	{            
		float rangeMult = 1.0f;
		unsigned int keyState = 
			Keyboard::instance()->getKeyboardState();
		if (keyState & KMOD_LSHIFT) rangeMult = 0.5f;

		currentX_ = startCurrentX_ - ((mx - startX_)/w_ * range_ * rangeMult);
		if (OptionsDisplay::instance()->getInvertUpDownKey())
		{
			rangeMult = -rangeMult;
		}
		currentY_ = startCurrentY_ - ((my - startY_)/w_ * range_ * rangeMult);
		if (handler_) handler_->currentChanged(getId(), currentX_, currentY_);

		skipRest = true;
	}
}
