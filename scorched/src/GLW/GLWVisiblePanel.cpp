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
#include <GLW/GLWVisiblePanel.h>
#include <GLW/GLWTranslate.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWVisiblePanel::GLWVisiblePanel(float x, float y, float w, float h, bool depressed) : 
	GLWVisibleWidget(x, y, w, h), depressed_(depressed), drawPanel_(true)
{

}

GLWVisiblePanel::~GLWVisiblePanel()
{

}

void GLWVisiblePanel::simulate(float frameTime)
{
	panel_.simulate(frameTime);
}

void GLWVisiblePanel::draw()
{
	GLWVisibleWidget::draw();

	glPushMatrix();
		if (drawPanel_)
		{
			glColor3f(0.8f, 0.8f, 1.0f);
			glBegin(GL_QUADS);
				glVertex2f(x_, y_);
				glVertex2f(x_ + w_, y_);
				glVertex2f(x_ + w_, y_ + h_);
				glVertex2f(x_, y_ + h_);
			glEnd();

			glBegin(GL_LINE_LOOP);
				drawShadedRoundBox(x_, y_, w_, h_, 10.0f, !depressed_);
			glEnd();
		}

		{
			GLWTranslate trans(x_, y_);
			glTranslatef(x_, y_, 0.0f);
			panel_.draw();
		}
	glPopMatrix();
}

void GLWVisiblePanel::mouseDown(float x, float y, bool &skipRest)
{
	x -= x_;
	y -= y_;

	panel_.mouseDown(x, y, skipRest);
}

void GLWVisiblePanel::mouseUp(float x, float y, bool &skipRest)
{
	x -= x_;
	y -= y_;

	panel_.mouseUp(x, y, skipRest);
}

void GLWVisiblePanel::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	panel_.keyDown(buffer, keyState, history, hisCount, skipRest);
}

void GLWVisiblePanel::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	mx -= x_;
	my -= y_;

	panel_.mouseDrag(mx, my, x, y, skipRest);
}
