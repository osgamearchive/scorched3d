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

#include <GLW/GLWScrollPanel.h>
#include <GLW/GLWTranslate.h>
#include <GLEXT/GLState.h>

REGISTER_CLASS_SOURCE(GLWScrollPanel);

GLWScrollPanel::GLWScrollPanel(float x, float y, float w, float h) : 
	GLWPanel(x, y, w, h), scrollW_(x + w - 19, y + 4, h - 10, 0, 0),
	drawScrollBar_(true), maxSee_(0)
{
	scrollW_.setHandler(this);
}

GLWScrollPanel::~GLWScrollPanel()
{

}

void GLWScrollPanel::setH(float h)
{
	GLWidget::setH(h);

	scrollW_.setH(h_ - 10);
}

void GLWScrollPanel::simulate(float frameTime)
{
	scrollW_.simulate(frameTime);
}

void GLWScrollPanel::draw()
{
	drawScrollBar_ = false;
	int canSee = 0;
	glPushMatrix();
	{
		GLWTranslate trans(x_, y_);
		glTranslatef(x_, y_, 0.0f);

		GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

		std::list<GLWPanel::GLWPanelEntry>::iterator itor;
		for (itor = getWidgets().begin();
			itor != getWidgets().end();
			itor++)
		{
			glPushMatrix();
				GLWidget *vw =
					(GLWidget *) (*itor).widget;
				if (vw->getY() < 0.0f || vw->getY() + vw->getH() > h_)
				{
					drawScrollBar_ = true;
				}
				else
				{
					canSee++;
					vw->draw();
				}
			glPopMatrix();
		}
	}
	glPopMatrix();

	// Draw scroll bar if not
	if (drawScrollBar_)
	{
		if (canSee > maxSee_) maxSee_ = canSee;

		scrollW_.setMax((int) getWidgets().size());
		scrollW_.setSee(maxSee_);
		scrollW_.draw();
	}
}

void GLWScrollPanel::mouseUp(float x, float y, bool &skipRest)
{
	if (drawScrollBar_)
	{
		scrollW_.mouseUp(x, y, skipRest);
	}
	if (!skipRest)
	{
		x -= x_;
		y -= y_;

		std::list<GLWPanel::GLWPanelEntry>::iterator itor;
		for (itor = getWidgets().begin();
			itor != getWidgets().end();
			itor++)
		{
			GLWidget *vw =
				(GLWidget *) (*itor).widget;
			if (vw->getY() < 0.0f || vw->getY() + vw->getH() > h_)
			{
			
			}
			else
			{
				vw->mouseUp(x, y, skipRest);
			}
			if (skipRest) break;
		}
	}
}

void GLWScrollPanel::mouseDown(float x, float y, bool &skipRest)
{
	if (drawScrollBar_)
	{
		scrollW_.mouseDown(x, y, skipRest);
	}
	if (!skipRest)
	{
		x -= x_;
		y -= y_;

		std::list<GLWPanel::GLWPanelEntry>::iterator itor;
		for (itor = getWidgets().begin();
			itor != getWidgets().end();
			itor++)
		{
			GLWidget *vw =
				(GLWidget *) (*itor).widget;
			if (vw->getY() < 0.0f || vw->getY() + vw->getH() > h_)
			{
			
			}
			else
			{
				vw->mouseDown(x, y, skipRest);
			}
			if (skipRest) break;
		}
	}
}

void GLWScrollPanel::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	if (drawScrollBar_)
	{
		scrollW_.mouseDrag(mx, my, x, y, skipRest);
	}
	if (!skipRest)
	{
		mx -= x_;
		my -= y_;

		std::list<GLWPanel::GLWPanelEntry>::iterator itor;
		for (itor = getWidgets().begin();
			itor != getWidgets().end();
			itor++)
		{
			GLWidget *vw =
				(GLWidget *) (*itor).widget;
			if (vw->getY() < 0.0f || vw->getY() + vw->getH() > h_)
			{
			
			}
			else
			{
				vw->mouseDrag(mx, my, x, y, skipRest);
			}
			if (skipRest) break;
		}
	}
}

void GLWScrollPanel::positionChange(unsigned int id, int current, int movement)
{
	GLWidget *widget = (GLWidget *) 
		getWidgets().front().widget;

	std::list<GLWPanel::GLWPanelEntry>::iterator itor;
	for (itor = getWidgets().begin();
		itor != getWidgets().end();
		itor++)
	{
		GLWidget *vw =
			(GLWidget *) (*itor).widget;
		vw->setY(vw->getY() - widget->getH() * movement);
	}
}

void GLWScrollPanel::clear()
{
	scrollW_.setCurrent(0);
	GLWPanel::clear();
}
