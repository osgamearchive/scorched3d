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


// GLWScrollH.cpp: implementation of the GLWScrollH class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWScrollH.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWScrollHI::~GLWScrollHI()
{

}

GLWScrollH::GLWScrollH(float x, float y, float w, int min, int max, int see, GLWScrollHI *handler) :
	GLWVisibleWidget(x, y, w, 15),
	min_(min), max_(max), see_(see), current_(min),
	handler_(handler), dragCurrent_(0),
	leftButton_(x_ + 2.0f, y_ + 2.0f, h_ - 4.0f, h_ - 4.0f),
	rightButton_(x_ + w_ - h_ + 2.0f, y_ + 2.0f, h_ - 4.0f, h_ - 4.0f),
	middleButton_(0.0f, y_ + 2.0f, 0.0f, h_ - 4.0f)
{
	leftButton_.setHandler(this);
	rightButton_.setHandler(this);
	middleButton_.setScrollHandler(this);
}

GLWScrollH::~GLWScrollH()
{

}

void GLWScrollH::draw()
{
	const float buttonWidth = h_ - 4.0f;
	const int possibilites = max_ - min_;

	// Draw scroll indicator
	if (see_ >= possibilites)
	{
		// Can see all possibilites
		middleButton_.setX(x_ + buttonWidth + 4.0f);
		middleButton_.setW(w_ - buttonWidth - buttonWidth - 8.0f);
	}
	else
	{
		float eachwidth = (w_ - buttonWidth - buttonWidth - 8.0f)  / possibilites;
		float left = float(possibilites - see_) * eachwidth;
		float percentage = float(current_) / float(possibilites);
		float pos = left * percentage;

		middleButton_.setX(x_ + buttonWidth + 4.0f + pos);
		middleButton_.setW(see_ * eachwidth);
	}

	leftButton_.draw();
	rightButton_.draw();
	middleButton_.draw();
	glBegin(GL_LINES);
		// Draw surround
		drawBox(x_, y_, w_, h_, false);
	glEnd();

	glColor3f(0.2f, 0.2f, 0.2f);
	float leftOffset = 0.0f;
	if (leftButton_.getPressed()) leftOffset = 1.0f; 
	float rightOffset = 0.0f;
	if (rightButton_.getPressed()) rightOffset = 1.0f; 
	glBegin(GL_TRIANGLES);
		// Left Triangle
		glVertex2f(x_ + buttonWidth + leftOffset, y_ + buttonWidth - leftOffset);
		glVertex2f(x_ + 4.0f + leftOffset, y_ + (buttonWidth / 2.0f) + 2.0f - leftOffset);
		glVertex2f(x_ + buttonWidth + leftOffset, y_ + 4.0f - leftOffset);

		// Right Triangle
		glVertex2f(x_ + w_ - 4.0f + rightOffset, y_ + (buttonWidth / 2.0f) + 2.0f - rightOffset);
		glVertex2f(x_ + w_ - buttonWidth + rightOffset, y_ + buttonWidth - rightOffset);
		glVertex2f(x_ + w_ - buttonWidth + rightOffset, y_ + 4.0f - rightOffset);
	glEnd();
}

void GLWScrollH::mouseDown(float x, float y, bool &skipRest)
{
	leftButton_.mouseDown(x, y, skipRest);
	if (skipRest) return;

	rightButton_.mouseDown(x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseDown(x, y, skipRest);
}

void GLWScrollH::mouseUp(float x, float y, bool &skipRest)
{
	leftButton_.mouseUp(x, y, skipRest);
	if (skipRest) return;

	rightButton_.mouseUp(x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseUp(x, y, skipRest);
}

void GLWScrollH::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	leftButton_.mouseDrag(mx, my, x, y, skipRest);
	if (skipRest) return;

	rightButton_.mouseDrag(mx, my, x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseDrag(mx, my, x, y, skipRest);
}

void GLWScrollH::startDrag(unsigned int id)
{
	dragCurrent_ = current_;
}

void GLWScrollH::buttonDrag(unsigned int id, float x, float y)
{
	const int possibilites = max_ - min_;
	if (see_ < possibilites)
	{
		int diff = int(x / 5.0f);
		int newCurrent = dragCurrent_ + diff;

		if (newCurrent < min_) newCurrent = min_;
		else if (newCurrent > max_) newCurrent = max_;

		if (newCurrent != current_)
		{
			int diffCurrent = current_ - newCurrent;
			current_ = newCurrent;

			if (handler_) handler_->positionChange(getId(), current_, diffCurrent);
		}
	}
}
void GLWScrollH::buttonDown(unsigned int id)
{
	if (id == leftButton_.getId())
	{
		const int possibilites = max_ - min_;
		if (see_ < possibilites)
		{
			if (current_ > min_) 
			{
				current_--;
				if (handler_) handler_->positionChange(getId(), current_, -1);
			}
		}
	}
	else if (id == rightButton_.getId())
	{
		const int possibilites = max_ - min_;
		if (see_ < possibilites)
		{
			if (current_ < max_) 
			{
				current_++;
				if (handler_) handler_->positionChange(getId(), current_, +1);
			}
		}
	}
}
