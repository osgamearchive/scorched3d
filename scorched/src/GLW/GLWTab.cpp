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
#include <GLW/GLWTab.h>

REGISTER_CLASS_SOURCE(GLWTab);

GLWTabI::~GLWTabI()
{

}

GLWTab::GLWTab(char *tabName, float index, float x, float y, float w, float h, float tw) :
	GLWScrollPanel(x, y, w, h), label_(x + 12.0f + index, y + h - 3.0f, tabName),
	tw_(tw),
	index_(index), handler_(0),
	depressed_(true)
{

}

GLWTab::~GLWTab()
{

}

void GLWTab::setH(float h)
{
	GLWScrollPanel::setH(h);
	label_.setY(y_ + h_ - 3.0f);
}

void GLWTab::mouseDown(float x, float y, bool &skipRest)
{
	if (x > x_ + index_ &&
		x < x_ + index_ + tw_ + 40 &&
		y < y_ + h_ + 20 &&
		y > y_ + h_)
	{
		setDepressed();
		if (handler_) handler_->tabDown(getId());
		skipRest = true;
	}

	if (!skipRest && depressed_)
	{
		GLWScrollPanel::mouseDown(x, y, skipRest);
	}
}

void GLWTab::setDepressed()
{
	std::list<GLWidget *>::iterator itor;
	for (itor = parent_->getWidgets().begin();
		itor != parent_->getWidgets().end();
		itor++)
	{
		if ((*itor)->getMetaClassId() == getMetaClassId())
		{
			GLWTab *tab = (GLWTab *) *itor;
			tab->depressed_ = false;
		}
	}

	depressed_ = true;
}

void GLWTab::setParent(GLWPanel *parent)
{
	GLWScrollPanel::setParent(parent);

	setDepressed();
}

void GLWTab::draw()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

	if (depressed_) drawSurround();
	else drawNonSurround();
}

void GLWTab::drawNonSurround()
{
	glBegin(GL_LINES);
		glColor3f(0.4f, 0.4f, 0.6f);
		glVertex2d(x_ + index_, y_ + h_);
		glVertex2d(x_ + index_ + 20.0f, y_ + h_ + 20.0f);

		glVertex2d(x_ + index_ + 20.0f, y_ + h_ + 20.0f);
		glVertex2d(x_ + index_ + 20.0f + tw_, y_ + h_ + 20.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2d(x_ + index_ + 20.0f + tw_, y_ + h_ + 20.0f);
		glVertex2d(x_ + index_ + 40.0f + tw_, y_ + h_);
	glEnd();

	label_.draw();
}

void GLWTab::drawSurround()
{
	glBegin(GL_LINES);
		glColor3f(0.4f, 0.4f, 0.6f);
		glVertex2d(x_, y_);
		glVertex2d(x_ + w_, y_);

		glVertex2d(x_ + w_, y_);
		glVertex2d(x_ + w_, y_ + h_);

		glVertex2d(x_ + index_ + 20.0f + tw_, y_ + h_ + 20.0f);
		glVertex2d(x_ + index_ + 40.0f + tw_, y_ + h_);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2d(x_, y_ + h_);
		glVertex2d(x_ + index_, y_ + h_);

		glVertex2d(x_ + index_, y_ + h_);
		glVertex2d(x_ + index_ + 20.0f, y_ + h_ + 20.0f);

		glVertex2d(x_ + index_ + 20.0f, y_ + h_ + 20.0f);
		glVertex2d(x_ + index_ + 20.0f + tw_, y_ + h_ + 20.0f);

		glVertex2d(x_ + index_ + 40.0f + tw_, y_ + h_);
		glVertex2d(x_ + w_, y_ + h_);

		glVertex2d(x_, y_ + h_);
		glVertex2d(x_, y_);
	glEnd();

	GLWScrollPanel::draw();

	label_.draw();
}
