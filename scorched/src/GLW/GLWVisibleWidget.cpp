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

#include <GLW/GLWVisibleWidget.h>
#include <GLW/GLWToolTip.h>
#include <GLW/GLWTranslate.h>
#include <common/Vector.h>
#include <common/Defines.h>
#include <math.h>

static GLuint list = 0;

GLWVisibleWidget::GLWVisibleWidget(float x, float y, float w, float h) :
	x_(x), y_(y), w_(w), h_(h), tooltip_(0)
{

}

GLWVisibleWidget::~GLWVisibleWidget()
{

}

void GLWVisibleWidget::draw()
{
	if (list == 0)
	{
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
			for (float a=360.0f; a>0.0f; a-=360.0f / 36.0f)
			{
				glVertex2f(sinf(a/180.0f * PI), 
					cosf(a/180.0f * PI));
			}
		glEndList();
	}

	if (tooltip_)
	{
		GLWToolTip::instance()->addToolTip(tooltip_, 
			GLWTranslate::getPosX() + x_, 
			GLWTranslate::getPosY() + y_, 
			w_, h_);
	}
}

void GLWVisibleWidget::drawWholeCircle(bool cap)
{
	glCallList(list);
	if (cap) glVertex2f(0.0f, 1.0f);
}

void GLWVisibleWidget::drawCircle(int startA, int endA, float posX, float posY, float size)
{
	static Vector positions[16];
	static bool init = false;
	if (!init)
	{
		init = true;
		for (int i=0; i<16; i++)
		{
			const float angDeg = 22.5f;
			float ang = i * angDeg;
			positions[i][0] = sinf(ang / 180.0f * PI);
			positions[i][1] = cosf(ang / 180.0f * PI);
		}
	}

	if (startA < endA)
	{
		for (int b=startA; b<=endA; b++)
		{
			int a=b; if (a>15) a=a-16;
			glVertex2f(posX + positions[a][0] * size, posY + positions[a][1] * size);
		}
	}
	else
	{
		for (int b=startA; b>=endA; b--)
		{
			int a=b; if (a<0) a=16+a;
			glVertex2f(posX + positions[a][0] * size, posY + positions[a][1] * size);
		}
	}
}

void GLWVisibleWidget::drawRoundBox(float x, float y, float w, float h, float size)
{
	drawCircle(8, 4, x + w - size, y + size, size);
	drawCircle(4, 0, x + w - size, y + h - size, size);
	drawCircle(0, -4, x + size, y + h - size, size);
	drawCircle(-4, -8, x + size, y + size, size);
}

void GLWVisibleWidget::drawShadedRoundBox(float x, float y, float w, float h, float size, bool depressed)
{
	if (depressed) glColor3f(0.4f, 0.4f, 0.6f);
	else glColor3f(1.0f, 1.0f, 1.0f);

	drawCircle(-6, -8, x + size, y + size, size);
	drawCircle(8, 4, x + w - size, y + size, size);
	drawCircle(4, 2, x + w - size, y + h - size, size);

	if (depressed) glColor3f(1.0f, 1.0f, 1.0f); 
	else glColor3f(0.4f, 0.4f, 0.6f);

	drawCircle(2, 0, x + w - size, y + h - size, size);
	drawCircle(0, -4, x + size, y + h - size, size);
	drawCircle(-4, -6, x + size, y + size, size);
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

bool GLWVisibleWidget::initFromXML(XMLNode *node)
{
	if ((x_ = node->getNamedFloatChild("x", true)) == 
		XMLNode::ErrorFloat) return false;
	if ((y_ = node->getNamedFloatChild("y", true)) == 
		XMLNode::ErrorFloat) return false;
	if ((w_ = node->getNamedFloatChild("w", true)) == 
		XMLNode::ErrorFloat) return false;
	if ((h_ = node->getNamedFloatChild("h", true)) == 
		XMLNode::ErrorFloat) return false;

	return true;
}
