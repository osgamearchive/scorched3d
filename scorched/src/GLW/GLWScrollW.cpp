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


// GLWScrollW.cpp: implementation of the GLWScrollW class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWScrollW.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWScrollWI::~GLWScrollWI()
{

}

GLWScrollW::GLWScrollW(float x, float y, float h, int min, int max, int see, GLWScrollWI *handler) :
	GLWVisibleWidget(x, y, 15.0f, h),
	min_(min), max_(max), see_(see), current_(min),
	handler_(handler), dragCurrent_(0),
	bottomButton_(x_ + 2.0f, y_ + 2.0f, w_ - 4.0f, w_ - 4.0f),
	topButton_(x_ + 2.0f, y_ + h_ - w_ + 2.0f, w_ - 4.0f, w_ - 4.0f),
	middleButton_(x_ + 2.0f, 0.0f, w_ - 4.0f, 0.0f)
{
	bottomButton_.setHandler(this);
	topButton_.setHandler(this);
	middleButton_.setScrollHandler(this);
}

GLWScrollW::~GLWScrollW()
{

}

void GLWScrollW::setX(float x)
{
	x_ = x; 
	topButton_.setX(x_+2.0f);
	bottomButton_.setX(x_+2.0f);
	middleButton_.setX(x_+2.0f);
}

void GLWScrollW::setH(float h)
{
	GLWVisibleWidget::setH(h);
	topButton_.setY(y_ + h_ - w_ + 2.0f);
}

void GLWScrollW::draw()
{
	const float buttonWidth = w_ - 4.0f;
	const int possibilites = max_ - min_;

	// Draw scroll indicator
	if (see_ >= possibilites)
	{
		// Can see all possibilites
		middleButton_.setY(y_ + buttonWidth + 4.0f);
		middleButton_.setH(h_ - buttonWidth - buttonWidth - 8.0f);
	}
	else
	{
		float eachwidth = (h_ - buttonWidth - buttonWidth - 8.0f)  / possibilites;
		float bottom = float(possibilites - see_) * eachwidth;
		float percentage = float(current_) / float(possibilites);
		float pos = bottom * percentage;

		middleButton_.setY(y_ + buttonWidth + 4.0f + pos);
		middleButton_.setH(see_ * eachwidth);
	}

	bottomButton_.draw();
	topButton_.draw();
	middleButton_.draw();

	glBegin(GL_LINES);
		// Draw surround
		drawBox(x_, y_, w_, h_, false);
	glEnd();

	glColor3f(0.2f, 0.2f, 0.2f);
	float bottomOffset = 0.0f;
	if (bottomButton_.getPressed()) bottomOffset = 1.0f; 
	float topOffset = 0.0f;
	if (topButton_.getPressed()) topOffset = 1.0f; 
	glBegin(GL_TRIANGLES);
		// Bottom Triangle
		glVertex2f(x_ + (buttonWidth / 2.0f) + 2.0f + bottomOffset, y_ + 4.0f - bottomOffset);
		glVertex2f(x_ + buttonWidth + bottomOffset, y_ + buttonWidth - 2.0f - bottomOffset);
		glVertex2f(x_ + 4 + bottomOffset, y_ + buttonWidth - 2.0f - bottomOffset);

		// Top Triangle
		glVertex2f(x_ + buttonWidth + topOffset, y_ + 2.0f + h_ - buttonWidth - topOffset);
		glVertex2f(x_ + (buttonWidth / 2.0f) + 2.0f + topOffset, y_ - 4.0f + h_ - topOffset);
		glVertex2f(x_ + 4 + topOffset, y_ + 2.0f + h_ - buttonWidth - topOffset);
	glEnd();
}

void GLWScrollW::mouseDown(float x, float y, bool &skipRest)
{
	topButton_.mouseDown(x, y, skipRest);
	if (skipRest) return;

	bottomButton_.mouseDown(x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseDown(x, y, skipRest);
}

void GLWScrollW::mouseUp(float x, float y, bool &skipRest)
{
	topButton_.mouseUp(x, y, skipRest);
	if (skipRest) return;

	bottomButton_.mouseUp(x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseUp(x, y, skipRest);
}

void GLWScrollW::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	topButton_.mouseDrag(mx, my, x, y, skipRest);
	if (skipRest) return;

	bottomButton_.mouseDrag(mx, my, x, y, skipRest);
	if (skipRest) return;

	middleButton_.mouseDrag(mx, my, x, y, skipRest);
}

void GLWScrollW::startDrag(unsigned int id)
{
	dragCurrent_ = current_;
}

void GLWScrollW::buttonDrag(unsigned int id, float x, float y)
{
	const int possibilites = max_ - min_;
	if (see_ < possibilites)
	{
		int diff = int(y / 5.0f);
		int newCurrent = dragCurrent_ + diff;

		if (newCurrent < min_) newCurrent = min_;
		else if (newCurrent > max_) newCurrent = max_;

		if (newCurrent != current_)
		{
			int diffCurrent = current_ - newCurrent;
			current_ = newCurrent;

			if (handler_) handler_->positionChange(getId(), current_, -diffCurrent);
		}
	}
}

void GLWScrollW::buttonDown(unsigned int id)
{
	if (id == bottomButton_.getId())
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
	else if (id == topButton_.getId())
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
