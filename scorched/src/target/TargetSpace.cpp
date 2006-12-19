////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <target/TargetSpace.h>
#include <target/TargetLife.h>

TargetSpace::TargetSpace() :
	spaceX_(-128), spaceY_(-128),
	spaceW_(1024), spaceH_(1024),
	spaceSq_(4)
{
	spaceWSq_ = (spaceW_ / spaceSq_);
	spaceHSq_ = (spaceH_ / spaceSq_);
	noSquares_ = spaceWSq_ * spaceHSq_;
	squares_ = new Square[noSquares_];
}

TargetSpace::~TargetSpace()
{
	delete squares_;
}

void TargetSpace::addTarget(Target *target)
{
	int x = (int) target->getTargetPosition()[0];
	int y = (int) target->getTargetPosition()[1];
	int w = (int) target->getLife().getSize()[0];
	int h = (int) target->getLife().getSize()[1];

	// Find the bounding box coords
	int halfW = w / 2;
	int halfH = h / 2;
	int minX = x - halfW - 1;
	int minY = y - halfH - 1;
	int maxX = x + halfW + 1;
	int maxY = y + halfH + 1;

	// Find the square positions
	normalizeCoords(minX, minY);
	normalizeCoords(maxX, maxY);

	// Find the square numbers
	for (int b=minY; b<=maxY; b++)
	{
		for (int a=minX; a<=maxX; a++)
		{
			int num = spaceHSq_ * b + a;
			DIALOG_ASSERT(num >= 0 && num < noSquares_);

			Square *square = &squares_[num];
			square->targets[target->getPlayerId()] = target;
		}
	}
}

void TargetSpace::removeTarget(Target *target)
{

}

#ifndef S3D_SERVER
#include <GLEXT/GLState.h>
#include <client/ScorchedClient.h>
#include <landscapemap/LandscapeMaps.h>

static void drawBox(Vector &position, Vector &size)
{
	float wid = size[0];
	float hgt = size[1];
	float dep = size[2];
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glBegin(GL_LINE_LOOP);
			// Top
			glNormal3f(0,1,0);
			glVertex3f(-wid/2,hgt/2,dep/2);
			glVertex3f(wid/2,hgt/2,dep/2);
			glVertex3f(wid/2,hgt/2,-dep/2);
			glVertex3f(-wid/2,hgt/2,-dep/2);
		glEnd();
		glBegin(GL_LINE_LOOP);
			// Back
			glNormal3f(0,0,1);
			glVertex3f(-wid/2,hgt/2,dep/2);
			glVertex3f(-wid/2,-hgt/2,dep/2);
			glVertex3f(wid/2,-hgt/2,dep/2);
			glVertex3f(wid/2,hgt/2,dep/2);
		glEnd();
		glBegin(GL_LINE_LOOP);
			// Front
			glNormal3f(0,0,-1);
			glVertex3f(-wid/2,hgt/2,-dep/2);
			glVertex3f(wid/2,hgt/2,-dep/2);
			glVertex3f(wid/2,-hgt/2,-dep/2);
			glVertex3f(-wid/2,-hgt/2,-dep/2);
		glEnd();
		glBegin(GL_LINE_LOOP);
			// Left
			glNormal3f(1,0,0);
			glVertex3f(wid/2,hgt/2,-dep/2);
			glVertex3f(wid/2,hgt/2,dep/2);
			glVertex3f(wid/2,-hgt/2,dep/2);
			glVertex3f(wid/2,-hgt/2,-dep/2);
		glEnd();
		glBegin(GL_LINE_LOOP);
			// Right
			glNormal3f(-1,0,0);
			glVertex3f(-wid/2,hgt/2,dep/2);
			glVertex3f(-wid/2,hgt/2,-dep/2);
			glVertex3f(-wid/2,-hgt/2,-dep/2);
			glVertex3f(-wid/2,-hgt/2,dep/2);
		glEnd();
		glBegin(GL_LINE_LOOP);
			// Bottom
			glNormal3f(0,-1,0);
			glVertex3f(-wid/2,-hgt/2,dep/2);
			glVertex3f(-wid/2,-hgt/2,-dep/2);
			glVertex3f(wid/2,-hgt/2,-dep/2);
			glVertex3f(wid/2,-hgt/2,dep/2);
		glEnd();
	glPopMatrix();
}

void TargetSpace::draw()
{
	GLState glState(GLState::TEXTURE_OFF | GLState::LIGHTING_OFF);

	for (int b=0; b<spaceHSq_; b++)
	{
		for (int a=0; a<spaceWSq_; a++)
		{
			int num = spaceHSq_ * b + a;
			DIALOG_ASSERT(num >= 0 && num < noSquares_);

			Vector position(a * spaceSq_ + spaceX_ + spaceSq_ / 2,
				b * spaceSq_ + spaceY_ + spaceSq_ / 2, 10);
			Vector size(spaceSq_, spaceSq_, 20);

			if (position[0] > ScorchedClient::instance()->getLandscapeMaps().
				getGroundMaps().getMapWidth() ||
				position[1] > ScorchedClient::instance()->getLandscapeMaps().
				getGroundMaps().getMapHeight())
			{
				continue;
			}

			Square *square = &squares_[num];
			if (!square->targets.empty())
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else
			{
				glColor3f(1.0f, 1.0f, 0.0f);
			}

			drawBox(position,size);

			glBegin(GL_LINES);
			std::map<unsigned int, Target *>::iterator itor;
			for (itor = square->targets.begin();
				itor != square->targets.end();
				itor++)
			{
				Target *target = (*itor).second;
				glVertex3fv(target->getTargetPosition());
				glVertex3fv(position);
			}
			glEnd();
		}
	}
}
#endif
