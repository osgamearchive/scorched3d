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


// GLCameraFrustum.cpp: implementation of the GLCameraFrustum class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCameraFrustum.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLCameraFrustum *GLCameraFrustum::instance_ = 0;

GLCameraFrustum *GLCameraFrustum::instance()
{
	if (!instance_)
	{
		instance_ = new GLCameraFrustum;
	}

	return instance_;
}

GLCameraFrustum::GLCameraFrustum()
{

}

GLCameraFrustum::~GLCameraFrustum()
{

}

void GLCameraFrustum::normalize(float vector[4])
{
   float fT = (float) sqrt(
	   vector[0] * vector[0] + 
	   vector[1] * vector[1] + 
	   vector[2] * vector[2]);

   vector[0] /= fT;
   vector[1] /= fT;
   vector[2] /= fT;
   vector[3] /= fT;
}

Vector &GLCameraFrustum::getBilboardVectorX()
{
	static Vector bil;
	bil[0] = fClip[0];
	bil[1] = fClip[4];
	bil[2] = fClip[8];

	return bil;
}

Vector &GLCameraFrustum::getBilboardVectorY()
{
	static Vector bil;
	bil[0] = fClip[1];
	bil[1] = fClip[5];
	bil[2] = fClip[9];

	return bil;
}

void GLCameraFrustum::draw(const unsigned state)
{
	// Get the current projection matrix from OpenGL
	// Get the current modelview matrix from OpenGL
	glGetFloatv(GL_PROJECTION_MATRIX, fProj);
	glGetFloatv(GL_MODELVIEW_MATRIX, fView);

	// Concenate the two matrices
	fClip[ 0] = fView[ 0] * fProj[ 0] + fView[ 1] * fProj[ 4] + fView[ 2] * fProj[ 8] + fView[ 3] * fProj[12];
	fClip[ 1] = fView[ 0] * fProj[ 1] + fView[ 1] * fProj[ 5] + fView[ 2] * fProj[ 9] + fView[ 3] * fProj[13];
	fClip[ 2] = fView[ 0] * fProj[ 2] + fView[ 1] * fProj[ 6] + fView[ 2] * fProj[10] + fView[ 3] * fProj[14];
	fClip[ 3] = fView[ 0] * fProj[ 3] + fView[ 1] * fProj[ 7] + fView[ 2] * fProj[11] + fView[ 3] * fProj[15];

	fClip[ 4] = fView[ 4] * fProj[ 0] + fView[ 5] * fProj[ 4] + fView[ 6] * fProj[ 8] + fView[ 7] * fProj[12];
	fClip[ 5] = fView[ 4] * fProj[ 1] + fView[ 5] * fProj[ 5] + fView[ 6] * fProj[ 9] + fView[ 7] * fProj[13];
	fClip[ 6] = fView[ 4] * fProj[ 2] + fView[ 5] * fProj[ 6] + fView[ 6] * fProj[10] + fView[ 7] * fProj[14];
	fClip[ 7] = fView[ 4] * fProj[ 3] + fView[ 5] * fProj[ 7] + fView[ 6] * fProj[11] + fView[ 7] * fProj[15];

	fClip[ 8] = fView[ 8] * fProj[ 0] + fView[ 9] * fProj[ 4] + fView[10] * fProj[ 8] + fView[11] * fProj[12];
	fClip[ 9] = fView[ 8] * fProj[ 1] + fView[ 9] * fProj[ 5] + fView[10] * fProj[ 9] + fView[11] * fProj[13];
	fClip[10] = fView[ 8] * fProj[ 2] + fView[ 9] * fProj[ 6] + fView[10] * fProj[10] + fView[11] * fProj[14];
	fClip[11] = fView[ 8] * fProj[ 3] + fView[ 9] * fProj[ 7] + fView[10] * fProj[11] + fView[11] * fProj[15];

	fClip[12] = fView[12] * fProj[ 0] + fView[13] * fProj[ 4] + fView[14] * fProj[ 8] + fView[15] * fProj[12];
	fClip[13] = fView[12] * fProj[ 1] + fView[13] * fProj[ 5] + fView[14] * fProj[ 9] + fView[15] * fProj[13];
	fClip[14] = fView[12] * fProj[ 2] + fView[13] * fProj[ 6] + fView[14] * fProj[10] + fView[15] * fProj[14];
	fClip[15] = fView[12] * fProj[ 3] + fView[13] * fProj[ 7] + fView[14] * fProj[11] + fView[15] * fProj[15];

	// Extract the right plane
	frustum_[0][0] = fClip[ 3] - fClip[ 0];
	frustum_[0][1] = fClip[ 7] - fClip[ 4];
	frustum_[0][2] = fClip[11] - fClip[ 8];
	frustum_[0][3] = fClip[15] - fClip[12];
	normalize(frustum_[0]);

	// Extract the left plane
	frustum_[1][0] = fClip[ 3] + fClip[ 0];
	frustum_[1][1] = fClip[ 7] + fClip[ 4];
	frustum_[1][2] = fClip[11] + fClip[ 8];
	frustum_[1][3] = fClip[15] + fClip[12];
	normalize(frustum_[1]);

	// Extract the bottom plane
	frustum_[2][0] = fClip[ 3] + fClip[ 1];
	frustum_[2][1] = fClip[ 7] + fClip[ 5];
	frustum_[2][2] = fClip[11] + fClip[ 9];
	frustum_[2][3] = fClip[15] + fClip[13];
	normalize(frustum_[2]);

	// Extract the top plane
	frustum_[3][0] = fClip[ 3] - fClip[ 1];
	frustum_[3][1] = fClip[ 7] - fClip[ 5];
	frustum_[3][2] = fClip[11] - fClip[ 9];
	frustum_[3][3] = fClip[15] - fClip[13];
	normalize(frustum_[3]);

	// Extract the far plane
	frustum_[4][0] = fClip[ 3] - fClip[ 2];
	frustum_[4][1] = fClip[ 7] - fClip[ 6];
	frustum_[4][2] = fClip[11] - fClip[10];
	frustum_[4][3] = fClip[15] - fClip[14];
	normalize(frustum_[4]);

	// Extract the near plane
	frustum_[5][0] = fClip[ 3] + fClip[ 2];
	frustum_[5][1] = fClip[ 7] + fClip[ 6];
	frustum_[5][2] = fClip[11] + fClip[10];
	frustum_[5][3] = fClip[15] + fClip[14];
	normalize(frustum_[5]);
}

bool GLCameraFrustum::pointInFrustum(Vector &point)
{
	for (int iCurPlane = 0; iCurPlane<6; iCurPlane++)
	{
		if (frustum_[iCurPlane][0] * point[0] + 
			frustum_[iCurPlane][1] * point[1] + 
			frustum_[iCurPlane][2] * point[2] + 
			frustum_[iCurPlane][3] <= 0)
		{
			return false;
		}
	}

	return true;
}

bool GLCameraFrustum::sphereInFrustum(Vector &point, float fRadius)
{
	for (int iCurPlane = 0; iCurPlane<6; iCurPlane++)
	{
		if (frustum_[iCurPlane][0] * point[0] + 
			frustum_[iCurPlane][1] * point[1] + 
			frustum_[iCurPlane][2] * point[2] + 
			frustum_[iCurPlane][3] <= -fRadius)
		{
			return false;
		}
	}

	return true;
}
