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


// GLWScrollPanel.cpp: implementation of the GLWScrollPanel class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWScrollPanel.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWScrollPanel::GLWScrollPanel(float x, float y, float w, float h) : 
	GLWVisiblePanel(x, y, w, h), scrollW_(x + w - 19, y + 4, h - 10, 0, 0),
	drawScrollBar_(true), maxSee_(0)
{
	scrollW_.setHandler(this);
}

GLWScrollPanel::~GLWScrollPanel()
{

}

void GLWScrollPanel::setH(float h)
{
	GLWVisibleWidget::setH(h);

	scrollW_.setH(h_ - 10);
}

void GLWScrollPanel::draw()
{
	drawScrollBar_ = false;
	int canSee = 0;
	glPushMatrix();
		glTranslatef(x_, y_, 0.0f);

		GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

		std::list<GLWidget *>::iterator itor;
		for (itor = panel_.getWidgets().begin();
			itor != panel_.getWidgets().end();
			itor++)
		{
			glPushMatrix();
				GLWVisibleWidget *vw =
					(GLWVisibleWidget *) (*itor);
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
	glPopMatrix();

	// Draw scroll bar if not
	if (drawScrollBar_)
	{
		if (canSee > maxSee_) maxSee_ = canSee;

		scrollW_.setMax((int) panel_.getWidgets().size());
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

		std::list<GLWidget *>::iterator itor;
		for (itor = panel_.getWidgets().begin();
			itor != panel_.getWidgets().end();
			itor++)
		{
			GLWVisibleWidget *vw =
				(GLWVisibleWidget *) (*itor);
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

		std::list<GLWidget *>::iterator itor;
		for (itor = panel_.getWidgets().begin();
			itor != panel_.getWidgets().end();
			itor++)
		{
			GLWVisibleWidget *vw =
				(GLWVisibleWidget *) (*itor);
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

		std::list<GLWidget *>::iterator itor;
		for (itor = panel_.getWidgets().begin();
			itor != panel_.getWidgets().end();
			itor++)
		{
			GLWVisibleWidget *vw =
				(GLWVisibleWidget *) (*itor);
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
	GLWVisibleWidget *widget = (GLWVisibleWidget *) panel_.getWidgets().front();

	std::list<GLWidget *>::iterator itor;
	for (itor = panel_.getWidgets().begin();
		itor != panel_.getWidgets().end();
		itor++)
	{
		GLWVisibleWidget *vw =
			(GLWVisibleWidget *) (*itor);
		vw->setY(vw->getY() - widget->getH() * movement);
	}
}

void GLWScrollPanel::clear()
{
	scrollW_.setCurrent(0);
	GLWVisiblePanel::clear();
}
