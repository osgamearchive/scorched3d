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

#include <GLW/GLWDropDown.h>
#include <GLW/GLWTranslate.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWToolTip.h>
#include <client/ScorchedClient.h>

GLWDropDownI::~GLWDropDownI()
{

}

REGISTER_CLASS_SOURCE(GLWDropDown);

GLWDropDown::GLWDropDown(float x, float y, float w) :
	GLWVisibleWidget(x, y, w - 21, 25.0f), text_("None"), 
	button_(x + w - 20.0f, y, 20.0f, 25.0f),
	handler_(0)
{

}

GLWDropDown::~GLWDropDown()
{

}

void GLWDropDown::setHandler(GLWDropDownI *handler)
{
	handler_ = handler;
}

void GLWDropDown::clear()
{
	texts_.clear();
}

int GLWDropDown::getCurrentPosition()
{
	std::list<GLWDropDownEntry>::iterator itor;
	int pos = 0;
	for (itor = texts_.begin();
		itor != texts_.end();
		itor++, pos++)
	{
		GLWDropDownEntry &entry = *itor;
		if (strcmp(text_.c_str(), entry.text_.c_str()) == 0)
		{
			return pos;
		}
	}

	return -1;
}

void GLWDropDown::setCurrentPosition(int pos)
{
	int position = 0;
	std::list<GLWDropDownEntry>::iterator itor;
	for (itor = texts_.begin();
		itor != texts_.end();
		itor++)
	{
		GLWDropDownEntry &entry = *itor;
		text_ = entry.text_;
		if (position++ >= pos) break;
	}
}

void GLWDropDown::addText(GLWDropDownEntry text)
{
	if (texts_.empty())
	{
		text_ = text.text_;
	}
	texts_.push_back(text);
}

void GLWDropDown::draw()
{
	GLWVisibleWidget::draw();

	float mouseX = float(ScorchedClient::instance()->getGameState().getMouseX());
	mouseX -= GLWTranslate::getPosX();
	float mouseY = float(ScorchedClient::instance()->getGameState().getMouseY());
	mouseY -= GLWTranslate::getPosY();

	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 10.0f, false);
	glEnd();

	button_.draw();
	float offset = 0.0f;
	if(button_.getPressed()) offset = 1.0f;

	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex2d(x_ + w_ + 16.0f + offset, y_ + 17.0f - offset);
		glVertex2d(x_ + w_ + 5.0f + offset, y_ + 17.0f - offset);
		glVertex2d(x_ + w_ + 10.0f + offset, y_ + 7.0f - offset);
	glEnd();

	GLWFont::instance()->getLargePtFont()->draw(
		GLWFont::widgetFontColor, 14,
		x_ + 5.0f, y_ + 5.0f, 0.0f, text_.c_str());

	if (button_.getPressed())
	{
		glColor3f(0.8f, 0.8f, 1.0f);
		float dropSize = float(20.0f * texts_.size());

		GLWToolTip::instance()->clearToolTip(
			GLWTranslate::getPosX() + x_, 
			GLWTranslate::getPosY() + y_ - dropSize, 
			w_, dropSize);

		glBegin(GL_QUADS);
			glVertex2f(x_, y_ - dropSize - 1);
			glVertex2f(x_ + w_, y_ - dropSize - 1);
			glVertex2f(x_ + w_, y_ - 1);
			glVertex2f(x_, y_ - 1);
		glEnd();
		glBegin(GL_LINE_LOOP);
			drawShadedRoundBox(x_, y_ - dropSize - 1, w_, dropSize, 10.0f, true);
		glEnd();

		float top = y_ - 24.0f;
		std::list<GLWDropDownEntry>::iterator itor;
		for (itor = texts_.begin();
			itor != texts_.end();
			itor++)
		{
			GLWDropDownEntry &entry = *itor;
			static Vector selectedColor(0.7f, 0.7f, 0.7f);
			bool selected = 
				inBox(mouseX, mouseY, x_, top, w_, 19.0f);

			if (entry.tip_)
			{
				GLWToolTip::instance()->addToolTip(entry.tip_, 
					GLWTranslate::getPosX() + x_, 
					GLWTranslate::getPosY() + top, 
					w_, 19.0f);
			}

			GLWFont::instance()->getLargePtFont()->draw(
				selected?selectedColor:GLWFont::widgetFontColor, 14,
				x_ + 5.0f, top + 5.0f, 0.0f, entry.text_.c_str());
			top -= 20.0f;
		}
	}
}

void GLWDropDown::mouseDown(float x, float y, bool &skipRest)
{
	button_.mouseDown(x, y, skipRest);
	if (!skipRest)
	{
		if (button_.getPressed())
		{
			button_.getPressed() = false;
			skipRest = true;

			float dropSize = float(20.0f * texts_.size());
			if (inBox(x, y, x_, y_ - dropSize - 1, w_, dropSize))
			{
				int pos = 0;
				float top = y_ - 24.0f;
				std::list<GLWDropDownEntry>::iterator itor;
				for (itor = texts_.begin();
					itor != texts_.end();
					itor++)
				{
					GLWDropDownEntry &entry = *itor;
					if (inBox(x, y, x_, top, w_, 19.0f))
					{
						text_ = entry.text_.c_str();
						if (handler_)
						{
							handler_->select(id_, pos, entry);
							return;
						}
					}

					pos++;
					top -= 20.0f;
				}
			}
		}
	}
}

void GLWDropDown::mouseUp(float x, float y, bool &skipRest)
{
	button_.mouseUp(x, y, skipRest);
}
