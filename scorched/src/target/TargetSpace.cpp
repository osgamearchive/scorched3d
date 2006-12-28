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
#include <target/TargetShield.h>
#include <weapons/Accessory.h>
#include <weapons/ShieldRound.h>
#include <weapons/ShieldSquare.h>

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
	// Set the bounding constaints
	int x = (int) target->getTargetPosition()[0];
	int y = (int) target->getTargetPosition()[1];
	int w = (int) target->getLife().getAabbSize()[0];
	int h = (int) target->getLife().getAabbSize()[1];

	// Update bounding constraints for shield
	Accessory *shieldAcc = target->getShield().getCurrentShield();
	if (shieldAcc)
	{
		Shield *shield = (Shield *) shieldAcc->getAction();
		if (shield->getRound())
		{
			ShieldRound *round = (ShieldRound *) shield;

			w = MAX(w, int(round->getActualRadius() * 2.0f));
			h = MAX(w, int(round->getActualRadius() * 2.0f));
		}
		else
		{
			ShieldSquare *square = (ShieldSquare *) shield;

			w = MAX(w, int(square->getSize()[0] * 2.0f));
			h = MAX(w, int(square->getSize()[1] * 2.0f));
		}
	}

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
			target->getLife().getSpaceContainment().squares.insert(num);
		}
	}
}

void TargetSpace::removeTarget(Target *target)
{
	std::set<int> &squares = target->getLife().getSpaceContainment().squares;
	std::set<int>::iterator itor;
	for (itor = squares.begin();
		itor != squares.end();
		itor++)
	{
		int num = (*itor);
		Square *square = &squares_[num];
		square->targets.erase(target->getPlayerId());
	}
	squares.clear();
}

Target *TargetSpace::getCollision(Vector &position)
{
	int x = (int) position[0];
	int y = (int) position[1];

	// Find the square positions
	normalizeCoords(x, y);

	int num = spaceHSq_ * y + x;
	DIALOG_ASSERT(num >= 0 && num < noSquares_);

	Square *square = &squares_[num];

	std::map<unsigned int, Target *> &potentialTargets = square->targets;
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = potentialTargets.begin();
		itor != potentialTargets.end();
		itor++)
	{
		Target *target = (*itor).second;
		Accessory *shieldAcc = target->getShield().getCurrentShield();
		if (shieldAcc)
		{
			Shield *shield = (Shield *) shieldAcc->getAction();
			Vector direction = position - target->getTargetPosition();
			if (shield->inShield(direction)) return target;
		}

		if (target->getLife().collision(position)) return target;
	}
	return 0;
}

void TargetSpace::getCollisionSet(Vector &position, float radius, 
	std::map<unsigned int, Target *> &collisionTargets, 
	bool ignoreHeight)
{
	int x = (int) position[0];
	int y = (int) position[1];
	int w = (int) radius;
	int h = (int) radius;

	// Find the bounding box coords
	int halfW = w; // Not need 1/2 as its the radius thats passed in
	int halfH = h;
	int minX = x - halfW - 1;
	int minY = y - halfH - 1;
	int maxX = x + halfW + 1;
	int maxY = y + halfH + 1;

	// Find the square positions
	normalizeCoords(minX, minY);
	normalizeCoords(maxX, maxY);

	// Find the square numbers
	std::map<unsigned int, Target *>::iterator itor;
	for (int b=minY; b<=maxY; b++)
	{
		for (int a=minX; a<=maxX; a++)
		{
			int num = spaceHSq_ * b + a;
			DIALOG_ASSERT(num >= 0 && num < noSquares_);

			Square *square = &squares_[num];

			std::map<unsigned int, Target *> &potentialTargets =
				square->targets;
			for (itor = potentialTargets.begin();
				itor != potentialTargets.end();
				itor++)
			{
				Target *target = (*itor).second;
				Vector checkPos = position;
				if (ignoreHeight)
				{
					Vector centerPos = target->getCenterPosition();
					checkPos[2] = centerPos[2];
				}
				float distance = target->getLife().collisionDistance(checkPos);
				if (distance < radius)
				{
					collisionTargets[target->getPlayerId()] = target;
				}
			}
		}
	}
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

			Square *square = &squares_[num];
			if (square->targets.empty())
			{
				continue;
			}

			Vector position(a * spaceSq_ + spaceX_ + spaceSq_ / 2,
				b * spaceSq_ + spaceY_ + spaceSq_ / 2, 0);
			position[2] = 
				ScorchedClient::instance()->getLandscapeMaps().
					getGroundMaps().getHeight((int) position[0], (int) position[1]) - 7.0f;
			Vector size(spaceSq_, spaceSq_, 20);

			if (position[0] > ScorchedClient::instance()->getLandscapeMaps().
				getGroundMaps().getMapWidth() ||
				position[1] > ScorchedClient::instance()->getLandscapeMaps().
				getGroundMaps().getMapHeight())
			{
				continue;
			}

			glColor3f(1.0f, 0.0f, 0.0f);
			drawBox(position, size);

			glBegin(GL_LINES);
			std::map<unsigned int, Target *>::iterator itor;
			for (itor = square->targets.begin();
				itor != square->targets.end();
				itor++)
			{
				Target *target = (*itor).second;
				glVertex3fv(target->getTargetPosition());
				glVertex3f(position[0], position[1], position[2] + 10.0f);
			}
			glEnd();
		}
	}
}
#endif
