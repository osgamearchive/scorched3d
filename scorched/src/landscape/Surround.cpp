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


// Surround.cpp: implementation of the Surround class.
//
//////////////////////////////////////////////////////////////////////

#include <landscape/Surround.h>
#include <landscape/Hemisphere.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLState.h>
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/FileList.h>
#include <common/Resources.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Surround::Surround(SurroundDefs &defs) : xy_(0.0f), 
	cloudSpeed_(500.0f), cloudDirection_(0.0f)
{

}

Surround::~Surround()
{

}

void Surround::simulate(float frameTime)
{
	if (OptionsDisplay::instance()->getNoSkyMovement()) return;

	float fastSpeed = 100;
	float slowSpeed = 500;
	float currentSpeed = OptionsTransient::instance()->getWindSpeed();
	float wantedAngle = 180.0f-OptionsTransient::instance()->getWindAngle();
	float wantedSpeed = (((5.0f - currentSpeed) / 5.0f) * (slowSpeed - fastSpeed)) + fastSpeed;

	// Move the cloud layer
	cloudSpeed_ = wantedSpeed;
	cloudDirection_ = wantedAngle;
	xy_ += frameTime / cloudSpeed_;
}

void Surround::draw()
{
	GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	float slowXY = xy_ / 1.5f;
	glPushMatrix();
		// Rotate the scene so clouds blow the correct way
		glRotatef(cloudDirection_, 0.0f, 0.0f, 1.0f);

		glTranslatef(128.0f, 128.0f, -15.0f);

		// Should change this to use a compiled GLVertexArray
		// this would be faster
		Hemisphere h;
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		//h.draw(1900, 300);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		h.draw(2000, 180, 5, 10, 0, 0, false, true, slowXY, slowXY + 0.4f);

		if (!OptionsDisplay::instance()->getNoSkyLayers())
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
			h.draw(1800, 120, 10, 20, 0, 0, false, true, 0.0f, xy_ + 0.3f);
		}
	glPopMatrix();
}
