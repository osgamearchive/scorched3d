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


// GLWTextButton.cpp: implementation of the GLWTextButton class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWTextButton.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWTextButton::GLWTextButton(char *buttonText,float x, float y, float w, GLWButtonI *handler,
							 bool ok, bool cancel) :
	GLWButton(x, y, w, 22.0f, handler, ok ,cancel), label_(x, y -2, buttonText)
{

}

GLWTextButton::~GLWTextButton()
{

}

void GLWTextButton::draw()
{
	GLWButton::draw();

	if (!pressed_)
	{
		label_.setX(x_);
		label_.setY(y_ - 2);
	}
	else
	{
		label_.setX(x_ + 1);
		label_.setY(y_ - 3);
	}

	label_.draw();
}
