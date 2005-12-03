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

#include <common/SplinePath.h>
#include <common/SplineCurve.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <GLEXT/GLState.h>
#include <math.h>

SplinePath::SplinePath() : 
	pathTime_(0.0f), pointsPerSecond_(0.0f)
{
}

SplinePath::~SplinePath()
{
}

void SplinePath::generate(
	std::vector<Vector> &inPoints, 
	int resolution,
	int polynomials,
	float pointsPerSecond)
{
	pointsPerSecond_ = pointsPerSecond;
	controlPoints_ = inPoints;
	SplineCurve::generate(controlPoints_, pathPoints_, 
		resolution, polynomials);

	DIALOG_ASSERT(pointsPerSecond_ > 0.0f);
	DIALOG_ASSERT(!controlPoints_.empty());
	DIALOG_ASSERT(!pathPoints_.empty());
}

void SplinePath::simulate(float frameTime)
{
	pathTime_ += frameTime;
}

Vector &SplinePath::getPathPosition()
{
	float currentPointTime = pathTime_ * pointsPerSecond_;
	return getPosition(currentPointTime);
}

Vector &SplinePath::getPathDirection()
{
	static Vector direction;

	float currentPointTime = pathTime_ * pointsPerSecond_;
	Vector pos1 = getPosition(currentPointTime);
	Vector pos2 = getPosition(currentPointTime + 0.5f);

	direction = pos2 - pos1;
	direction.StoreNormalize();

	return direction;
}

Vector &SplinePath::getPosition(float currentPointTime)
{
	static Vector position;

	int noPoints = (int) pathPoints_.size();
	int currentPointId = (int) floorf(currentPointTime);
	float currentPointDiff = currentPointTime - float(currentPointId);
	currentPointId = currentPointId % noPoints;
	int nextPointId = currentPointId + 1;
	noPoints = noPoints % noPoints;

	Vector &currentPoint = pathPoints_[currentPointId];
	Vector &nextPoint = pathPoints_[nextPointId];

	Vector diff = nextPoint - currentPoint;
	diff *= currentPointDiff;

	Logger::log("%.2f %.2f %i %i", currentPointTime, currentPointDiff, currentPointId, nextPointId);

	position = currentPoint;
	position += diff;

	return position;
}

void SplinePath::draw()
{
	std::vector<Vector>::iterator itor;
	GLState state(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		for (itor = pathPoints_.begin();
			itor != pathPoints_.end();
			itor++)
		{
			Vector &pt = (*itor);
			glVertex3fv(pt);
		}
	glEnd();

	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
		for (itor = controlPoints_.begin();
			itor != controlPoints_.end();
			itor++)
		{
			Vector &pt = (*itor);
			glVertex3fv(pt);
		}
		glColor3f(0.5f, 0.5f, 1.0f);
		glVertex3fv(getPathPosition());
	glEnd();

	glPointSize(1.0f);
}