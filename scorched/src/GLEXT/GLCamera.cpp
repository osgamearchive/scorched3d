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

#include <math.h>
#include <common/Defines.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCamera.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLCamera::GLCamera(GLsizei windowWidth, GLsizei windowHeight) :
	rotationXY_(0.0f), rotationYZ_(PI / 4), zoom_(150.0f),
	useHeightFunc_(false), heightFunc_(0), totalTime_(0.0f)
{
	setWindowOffset(0, 0);
	setWindowSize(windowWidth, windowHeight);
	calculateWantedOffset();
}

GLCamera::~GLCamera()
{

}

void GLCamera::setUseHeightFunc(bool toggle)
{
	useHeightFunc_ = toggle;
}

void GLCamera::setHeightFunc(HeightFunc func)
{
	heightFunc_ = func;
	if (func)
	{
		useHeightFunc_ = true;
	}
}

void GLCamera::setLookAt(Vector &lookAt, bool instant)
{
	wantedLookAt_ = lookAt;
	if (instant)
	{
		lookAt_ = lookAt;
	}
}

void GLCamera::setWindowSize(GLsizei windowWidth, GLsizei windowHeight)
{
	windowW_ = windowWidth;
	windowH_ = windowHeight;
	windowAspect_ = float(windowWidth) / float(windowHeight?windowHeight:0.0001);
}

void GLCamera::setWindowOffset(GLsizei windowLeft, GLsizei windowTop)
{
	windowL_ = windowLeft;
	windowT_ = windowTop;
}

void GLCamera::calculateWantedOffset()
{
	wantedOffset_[0] =  zoom_ * float(sin(rotationXY_) * sin(rotationYZ_));
	wantedOffset_[1] =  zoom_ * float(cos(rotationXY_) * sin(rotationYZ_));
	wantedOffset_[2] =  zoom_ * float(cos(rotationYZ_));
}

void GLCamera::moveViewport(Vector &lookFrom, Vector &lookAt)
{
	GLfloat lz(lookFrom[2]);
	if (heightFunc_ && useHeightFunc_)
	{
		lz = MAX(lz, (*heightFunc_)((int) lookFrom[0], (int) lookFrom[1]));
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(windowL_, windowT_, windowW_, windowH_);
	gluPerspective(60.0f, windowAspect_, 1.0f, 2500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lookFrom[0], lookFrom[1], lz, lookAt[0], lookAt[1], lookAt[2] ,0.0f, 0.0f, 1.0f);
}

void GLCamera::simulate(float frameTime)
{
	const float SecondsToReachTarget = 0.3f;

	// Make some constant changes, regardless of framerate
	totalTime_ += frameTime;
	while (totalTime_ > 0.1f)
	{
		totalTime_ -= 0.1f;

		// Calculate any camera shake
		shake_ -= 0.1f;
		if (shake_ > 0.0f)
		{
			shakeV_[0] = RAND * shake_;
			shakeV_[1] = RAND * shake_;
			shakeV_[2] = RAND * shake_;
		}
		else 
		{
			shake_ = 0.0f;
			shakeV_.zero();
		}

		// Calculate the new look at value
		Vector directionLookAt = wantedLookAt_ - lookAt_;
		directionLookAt	*= SecondsToReachTarget;
		lookAt_ += directionLookAt;

		// Calculate the new look from value
		Vector wantedPosition = wantedOffset_ + lookAt_;
		Vector directionPosition = wantedPosition - currentPosition_;
		directionPosition *= SecondsToReachTarget;
		currentPosition_ += directionPosition;
	}
}

void GLCamera::draw()
{
	static Vector look;
	look = lookAt_;
	look += shakeV_;
	moveViewport(currentPosition_, look);
}

void GLCamera::addShake(float shake)
{
	shake_ += shake;
}

void GLCamera::movePosition(float XY, float YZ, float Z)
{
	rotationXY_ = XY;
	rotationYZ_ = YZ;
	zoom_ = Z;

	calculateWantedOffset();
}

void GLCamera::movePositionDelta(float XY, float YZ, float Z)
{
	XY += rotationXY_;
	YZ += rotationYZ_;
	Z += zoom_;

	if (YZ < 0.17f) YZ = 0.17f;
	if (YZ > 1.91f) YZ = 1.91f;
	if (Z < 5.0) Z = 5.0f;
	if (Z > 200.0f) Z = 200.0f;

	movePosition(XY, YZ, Z);
}

bool GLCamera::getDirectionFromPt(GLfloat winX, GLfloat winY, Line &direction)
{
	static GLdouble modelMatrix[16];
	static GLdouble projMatrix[16];
	static GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble x,y,z;
	gluUnProject(
		winX,
		viewport[3] - winY,
		0.0f,
		modelMatrix,
		projMatrix,
		viewport,
		&x,
		&y,
		&z);
	Vector pos1((float) x, (float) y, (float) z);

	gluUnProject(
		winX,
		viewport[3] - winY,
		100.0f,
		modelMatrix,
		projMatrix,
		viewport,
		&x,
		&y,
		&z);

	Vector pos2((float) x, (float) y, (float) z);

	Vector dir = pos2 - pos1;

	float dist = dir[2];
	if (dist == 0.0f)
	{
		dist = 0.000001f;
	}
	float number = (float) fabs(pos2[2] / dist);

	Vector groundPos = pos2 - dir * number;
	direction.setStart(groundPos);
	direction.setEnd(pos2);

	return true;
}
