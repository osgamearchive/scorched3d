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

#include <GLEXT/GLState.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWTextBox.h>
#include <common/Keyboard.h>

GLWTextBox::GLWTextBox(float x, float y, float w, char *startText) :
	GLWVisibleWidget(x, y, w, 25.0f), ctime_(0.0f), text_(startText?startText:""), 
	cursor_(false), maxTextLen_(0)
{

}

GLWTextBox::~GLWTextBox()
{

}

void GLWTextBox::simulate(float frameTime)
{
	ctime_ += frameTime;
	if (ctime_ > 0.5f)
	{
		ctime_ = 0.0f;
		cursor_ = !cursor_;
	}
}

void GLWTextBox::draw()
{
	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 10.0f, false);
	glEnd();

	GLWFont::instance()->getFont()->draw(
		GLWFont::widgetFontColor, 16,
		x_ + 5.0f, y_ + 5.0f, 0.0f, "%s%s", text_.c_str(), cursor_?"":"_");
}

void GLWTextBox::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	for (int i=0; i<hisCount; i++)
	{
		char c = history[i].representedKey;
		DWORD dik = history[i].sdlKey;
		if (dik == SDLK_BACKSPACE || dik == SDLK_DELETE)
		{
			if (!text_.empty())
			{
				text_ = text_.substr(0, text_.length() - 1);
			}
			skipRest = true;
		}
		else if (c >= ' ')
		{
			if ((maxTextLen_==0) || ((int) text_.size() < maxTextLen_))
			{
				if ((int) text_.size() < w_ / 12)
				{
					text_ += c;
				}
			}
			skipRest = true;
		}
	}
}
