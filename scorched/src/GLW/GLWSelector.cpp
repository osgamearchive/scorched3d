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

#include <GLW/GLWSelector.h>

GLWSelector::Entry::Entry(const char *text, 
			GLWTip *tooltip, 
			bool selected,
			GLTexture *icon) : 
	text_(text),
	tip_(tooltip),
	selected_(selected),
	icon_(icon)
{

}

GLWSelector *GLWSelector::instance_ = 0;

GLWSelector *GLWSelector::instance()
{
	if (!instance_)
	{
		instance_ = new GLWSelector();
	}
	return instance_;
}

GLWSelector::GLWSelector() :
	GLWWindow("", 0.0f, 0.0f, 0.0f, 0.0f, 0, ""), 
	visible_(false)
{
}

GLWSelector::~GLWSelector()
{
}

void GLWSelector::draw()
{

}

void GLWSelector::showSelector(GLWSelectorI *user,
							   float x, float y,
							   std::list<GLWSelector::Entry> &entries)
{
	drawX_ = x;
	drawY_ = y;
	user_ = user;
	entries_ = entries;
	visible_ = true;
	w_ = 100000.0f;
	h_ = 100000.0f;
}
