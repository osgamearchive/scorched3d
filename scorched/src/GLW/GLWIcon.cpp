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


#include <GLW/GLWIcon.h>
#include <GLEXT/GLState.h>

GLWIcon::GLWIcon(float x, float y, float w, float h, GLTexture *texture) : 
	GLWVisibleWidget(x, y, w, h),
	texture_(texture)
{
}

GLWIcon::~GLWIcon()
{
}

void GLWIcon::draw()
{
	if (texture_)
	{
		GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(1.0f, 1.0f, 1.0f);
		texture_->draw();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(x_, y_);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(x_ + w_, y_);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(x_ + w_, y_ + h_);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(x_, y_ + h_);
		glEnd();
	}

	GLWVisibleWidget::draw();
}
