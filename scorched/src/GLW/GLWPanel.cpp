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


// GLWPanel.cpp: implementation of the GLWPanel class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <GLW/GLWPanel.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWPanel::GLWPanel() : GLWidget()
{

}

GLWPanel::~GLWPanel()
{
	clear();
}

GLWidget *GLWPanel::addWidget(GLWidget *widget)
{
	widgets_.push_back(widget);
	widget->setParent(this);
	return widget;
}

void GLWPanel::clear()
{
	std::list<GLWidget *>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		delete (*itor);
	}
	widgets_.clear();
}

void GLWPanel::simulate(float frameTime)
{
	std::list<GLWidget *>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		glPushMatrix();
		(*itor)->simulate(frameTime);
		glPopMatrix();
	}
}

void GLWPanel::draw()
{
	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF);

	std::list<GLWidget *>::iterator itor;
	for (itor = widgets_.begin();
		itor != widgets_.end();
		itor++)
	{
		glPushMatrix();
		(*itor)->draw();
		glPopMatrix();
	}
}

void GLWPanel::mouseDown(float x, float y, bool &skipRest)
{
	std::list<GLWidget *>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		(*itor)->mouseDown(x, y, skipRest);
		if (skipRest) break;
	}
}

void GLWPanel::mouseUp(float x, float y, bool &skipRest)
{
	std::list<GLWidget *>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		(*itor)->mouseUp(x, y, skipRest);
		if (skipRest) break;
	}
}

void GLWPanel::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	std::list<GLWidget *>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		(*itor)->mouseDrag(mx, my, x, y, skipRest);
		if (skipRest) break;
	}
}

void GLWPanel::keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	std::list<GLWidget *>::reverse_iterator itor;
	for (itor = widgets_.rbegin();
		itor != widgets_.rend();
		itor++)
	{
		(*itor)->keyDown(buffer, bufferCount, history, hisCount, skipRest);
		if (skipRest) break;
	}
}
