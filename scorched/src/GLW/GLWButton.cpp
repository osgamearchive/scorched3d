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
#include <common/Keyboard.h>
#include <GLW/GLWButton.h>

GLWButtonI::~GLWButtonI()
{

}

GLWButton::GLWButton(float x, float y, float w, float h, GLWButtonI *handler,
					 bool ok, bool cancel) : 
	handler_(handler),
	GLWVisibleWidget(x, y, w, h),
	ok_(ok), cancel_(cancel), pressed_(false), startdrag_(false),
	repeatMode_(false), repeatTime_(0.0f)
{

}

GLWButton::~GLWButton()
{

}

void GLWButton::setHandler(GLWButtonI *handler)
{ 
	handler_ = handler; 
}

void GLWButton::draw()
{
	float size = 10.0f;
	if (w_ < 16 || h_ < 16) size = 6.0f;
	else if (w_ < 12 || h_ < 12) size = 4.0f;

	glLineWidth(ok_?2.0f:1.0f);
	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, size, !pressed_);
	glEnd();
	glLineWidth(1.0f);
}

void GLWButton::simulate(float frameTime)
{
	repeatTime_ += frameTime;
	if (repeatMode_)
	{
		const float RepeatTimeInterval = 0.15f;
		while (repeatTime_ > RepeatTimeInterval)
		{
			repeatTime_ -= RepeatTimeInterval;
			if (pressed_ && handler_) handler_->buttonDown(getId());
		}
	}
}

void GLWButton::mouseDown(float x, float y, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;
		startdrag_ = true;
		pressed_ = true;

		if (repeatMode_)
		{
			repeatTime_ = -0.85f;
			if (handler_) handler_->buttonDown(getId());
		}
	}
}

void GLWButton::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	if (startdrag_)
	{
		if (inBox(mx, my, x_, y_, w_, h_))
		{
			pressed_ = true;
			skipRest = true;
		}
		else
		{
			pressed_ = false;
		}
	}
}

void GLWButton::mouseUp(float x, float y, bool &skipRest)
{
	startdrag_ = false;
	if (pressed_)
	{
		pressed_ = false;	
		skipRest = true;

		if (!repeatMode_)
		{
			if (handler_) handler_->buttonDown(getId());
		}
	}
}

void GLWButton::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	if (handler_ && (ok_ || cancel_))
	{
		for (int i=0; i<hisCount; i++)
		{
			DWORD dik = history[i].sdlKey;
			switch (dik)
			{
				case SDLK_KP_ENTER:
				case SDLK_RETURN:
					if (ok_) 
					{ 
						handler_->buttonDown(getId()); 
						skipRest = true; 
					}
					break;
				case SDLK_ESCAPE:
					if (cancel_) 
					{ 
						handler_->buttonDown(getId()); 
						skipRest = true; 
					}
					break;	
			}
		}
	}
}
