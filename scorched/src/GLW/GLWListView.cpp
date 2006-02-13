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

#include <GLW/GLWListView.h>
#include <GLW/GLWFont.h>
#include <common/Defines.h>
#include <stdio.h>

REGISTER_CLASS_SOURCE(GLWListView);

GLWListView::GLWListView(float x, float y, float w, float h, int maxLen) :
	GLWidget(x, y, w, h), 
	scroll_(x + w - 17, y, h, 0, 1), maxLen_(maxLen)
{
	scroll_.setMax((int) lines_.size());
	scroll_.setSee((int) (h_ / 12.0f));
	scroll_.setCurrent(scroll_.getMax());
}

GLWListView::~GLWListView()
{
}

void GLWListView::draw()
{
	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 6.0f, false);
	glEnd();

	{
		// Stops each font draw from changing state every time
		GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);

		float posY = y_ + h_ - 10.0f;

		int pos = (scroll_.getMax() - scroll_.getSee()) - scroll_.getCurrent();
		for (int i=pos; i<(int) scroll_.getMax(); i++)
		{
			if (i >= 0 && i < (int) lines_.size())
			{
				GLWFont::instance()->getSmallPtFont()->drawWidth(
					(int) w_ - 20,
					GLWFont::widgetFontColor, 10,
					x_ + 5.0f, posY, 0.0f, 
					formatString("%s", lines_[i].c_str()));
				posY -= 12.0f;
			}

			if (posY < y_) break;
		}
	}

	scroll_.draw();
}

void GLWListView::simulate(float frameTime)
{
	scroll_.simulate(frameTime);
}

void GLWListView::mouseDown(float x, float y, bool &skipRest)
{
	scroll_.mouseDown(x, y, skipRest);
}

void GLWListView::mouseUp(float x, float y, bool &skipRest)
{
	scroll_.mouseUp(x, y, skipRest);
}

void GLWListView::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	scroll_.mouseDrag(mx, my, x, y, skipRest);
}

void GLWListView::clear()
{
	lines_.clear();
	scroll_.setCurrent(0);
}

void GLWListView::addLine(const char *text)
{
	// Remove extra lines
	if (maxLen_ > 0)
	{
		if (lines_.size() > (unsigned int) maxLen_) lines_.clear();
	}

	// Generate the line to add
	lines_.push_back(text);

	// Setup the current scroll position
	int view = (int) (h_ / 12.0f);

	scroll_.setMax((int) lines_.size());
	scroll_.setSee(view);
	scroll_.setCurrent(0);
}
