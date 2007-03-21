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

#include <GLW/GLWIconList.h>
#include <tank/TankModelStore.h>
#include <tankgraph/TankMeshStore.h>
#include <3dsparse/ModelStore.h>
#include <client/ScorchedClient.h>
#include <graph/OptionsDisplay.h>
#include <common/DefinesString.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWTranslate.h>

static const float TankSquareSize = 60.0f;
static const float TankHalfSquareSize = TankSquareSize / 2.0f;
static const float TankPadding = 20.0f;
static const float TankInfo = 165.0f;

GLWIconList::GLWIconList(
	float x, float y, float w, float h,
	int squares) :
	GLWidget(x, y, w, h),
	scrollBar_(w_ - 12.0f, y + 2.0f, h_ - 4.0f, 0, 0, squares)
{
}

GLWIconList::~GLWIconList()
{
}

void GLWIconList::simulate(float frameTime)
{
	scrollBar_.simulate(frameTime);
}

void GLWIconList::draw()
{
	GLWidget::draw();
	scrollBar_.draw();

	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 10.0f, false);
	glEnd();
}

void GLWIconList::mouseDown(int button, float x, float y, bool &skipRest)
{
	scrollBar_.mouseDown(button, x, y, skipRest);
	if (!skipRest)
	{

	}
}

void GLWIconList::mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest)
{
	scrollBar_.mouseDrag(button, mx, my, x, y, skipRest);
}

void GLWIconList::mouseUp(int button, float x, float y, bool &skipRest)
{
	scrollBar_.mouseUp(button, x, y, skipRest);
}

void GLWIconList::mouseWheel(float x, float y, float z, bool &skipRest)
{
	if (inBox(x, y, x_, y_, w_, h_))
	{
		skipRest = true;

		if (z < 0.0f) scrollBar_.setCurrent(scrollBar_.getCurrent() + 1);
		else scrollBar_.setCurrent(scrollBar_.getCurrent() - 1);
	}
}
