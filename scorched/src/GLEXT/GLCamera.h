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

#if !defined(AFX_GLCAMERA_H__A41E0133_3B1F_11D4_BCBB_00A0C9A4CA3E__INCLUDED_)
#define AFX_GLCAMERA_H__A41E0133_3B1F_11D4_BCBB_00A0C9A4CA3E__INCLUDED_

#include <GLEXT/GLState.h>
#include <common/Line.h>

class GLCamera
{
public:
	typedef float (*HeightFunc)(int, int, void *);

	GLCamera(GLsizei windowWidth, GLsizei windowHeight);
	virtual ~GLCamera();

	void setHeightFunc(HeightFunc func, void *heightData = 0);
	void setUseHeightFunc(bool toggle);
	void setLookAt(Vector &lookAt, bool instant = false);
	void setOffSet(Vector &offSet, bool instant = false);
	void setWindowSize(GLsizei windowWidth, GLsizei windowHeight);
	void setWindowOffset(GLsizei windowLeft, GLsizei windowTop);

	void draw();
	void simulate(float frameTime = 0.02f);
	void addShake(float shake);

	void movePosition(float XY, float YZ, float Z);
	void movePositionDelta(float XY, float YZ, float Z);

	bool getDirectionFromPt(GLfloat ptX, GLfloat ptY, Line &direction);

	GLsizei getWidth() { return windowW_; }
	GLsizei getHeight() { return windowH_; }

	Vector &getCurrentPos() { return currentPosition_; }
	Vector &getLookAt() { return lookAt_; }

	float getRotationXY() { return rotationXY_; }
	float getRotationYZ() { return rotationYZ_; }

protected:
	GLsizei windowW_, windowH_;
	GLsizei windowL_, windowT_;
	GLfloat windowAspect_;
	GLfloat rotationXY_, rotationYZ_, zoom_;
	float shake_;
	float totalTime_;
	Vector shakeV_;
	bool useHeightFunc_;
	Vector lookAt_;
	Vector wantedLookAt_;
	Vector wantedOffset_;
	Vector currentPosition_;
	HeightFunc heightFunc_;
	void *heightData_;

	virtual void calculateWantedOffset();
	virtual void moveViewport(Vector &lookFrom, Vector &lookAt);

};

#endif // !defined(AFX_GLCAMERA_H__A41E0133_3B1F_11D4_BCBB_00A0C9A4CA3E__INCLUDED_)
