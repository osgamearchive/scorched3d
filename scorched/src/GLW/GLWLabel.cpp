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

#include <GLW/GLWLabel.h>
#include <GLW/GLWFont.h>

GLWLabel::GLWLabel(float x, float y, char *buttonText) : 
	GLWVisibleWidget(x, y, 0.0f, 20.0f)
{
	if (buttonText) setText(buttonText);
}

GLWLabel::~GLWLabel()
{

}

void GLWLabel::setText(const char *text)
{ 
	buttonText_ = text; 
	w_ = float(buttonText_.size()) * 9.0f;
}

void GLWLabel::draw()
{
	GLWVisibleWidget::draw();

	glColor3f(1.0f, 0.0f, 0.0f);
	GLWFont::instance()->getFont()->draw(
		GLWFont::widgetFontColor, 16,
		x_ + 5.0f, y_ + 5.0f, 0.0f, (char *) buttonText_.c_str());
}