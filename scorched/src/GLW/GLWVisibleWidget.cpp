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


// GLWVisibleWidget.cpp: implementation of the GLWVisibleWidget class.
//
//////////////////////////////////////////////////////////////////////

#include <GLW/GLWVisibleWidget.h>
#include <common/Vector.h>
#include <math.h>

static const float roundSize = 20.0f;
static const float smallRoundSize = 10.0f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLWVisibleWidget::GLWVisibleWidget(float x, float y, float w, float h) :
	x_(x), y_(y), w_(w), h_(h)
{

}

GLWVisibleWidget::~GLWVisibleWidget()
{

}

void GLWVisibleWidget::drawCircle(int startA, int endA, float posX, float posY, bool size)
{
	static Vector smallPositions[16];
	static Vector largePositions[16];
	static bool init = false;
	if (!init)
	{
		init = true;
		for (int i=0; i<16; i++)
		{
			const float angDeg = 22.5f;
			float ang = i * angDeg;
			largePositions[i][0] = sinf(ang / 180.0f * PI) * roundSize;
			largePositions[i][1] = cosf(ang / 180.0f * PI) * roundSize;
			smallPositions[i][0] = sinf(ang / 180.0f * PI) * smallRoundSize;
			smallPositions[i][1] = cosf(ang / 180.0f * PI) * smallRoundSize;
		}
	}

	if (startA < endA)
	{
		for (int b=startA; b<=endA; b++)
		{
			int a=b; if (a>15) a=a-16;
			if (size) glVertex2f(posX + largePositions[a][0], posY + largePositions[a][1]);
			else glVertex2f(posX + smallPositions[a][0], posY + smallPositions[a][1]);
		}
	}
	else
	{
		for (int b=startA; b>=endA; b--)
		{
			int a=b; if (a<0) a=16+a;
			if (size) glVertex2f(posX + largePositions[a][0], posY + largePositions[a][1]);
			else glVertex2f(posX + smallPositions[a][0], posY + smallPositions[a][1]);
		}
	}
}

void GLWVisibleWidget::drawRoundBox(float x, float y, float w, float h, bool large)
{
	float size = roundSize;
	if (!large) size = smallRoundSize;

	drawCircle(8, 4, x + w - size, y + size, large);
	drawCircle(4, 0, x + w - size, y + h - size, large);
	drawCircle(0, -4, x + size, y + h - size, large);
	drawCircle(-4, -8, x + size, y + size, large);
}

void GLWVisibleWidget::drawBox(float x, float y, float w, float h, bool depressed)
{
	if (depressed) glColor3f(0.4f, 0.4f, 0.6f);
	else glColor3f(1.0f, 1.0f, 1.0f);

	glVertex2f(x , y);
	glVertex2f(x + w, y);

	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);

	if (depressed) glColor3f(1.0f, 1.0f, 1.0f);
	else glColor3f(0.4f, 0.4f, 0.6f);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);

	glVertex2f(x, y + h);
	glVertex2f(x, y);
}

bool GLWVisibleWidget::inBox(float posX, float posY, float x, float y, float w, float h)
{
	if ((posX >= x) && (posX <= x + w) &&
		(posY >= y) && (posY <= y + h))
	{
		return true;
	}

	return false;
}
