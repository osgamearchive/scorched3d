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

#include <landscape/Surround.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/Hemisphere.h>
#include <GLEXT/GLState.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/FileList.h>

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
	float currentSpeed = ScorchedClient::instance()->getOptionsTransient().getWindSpeed();
	float wantedAngle = 180.0f-ScorchedClient::instance()->getOptionsTransient().getWindAngle();
	float wantedSpeed = (((5.0f - currentSpeed) / 5.0f) * (slowSpeed - fastSpeed)) + fastSpeed;

	// Move the cloud layer
	cloudSpeed_ = wantedSpeed;
	cloudDirection_ = wantedAngle;
	xy_ += frameTime / cloudSpeed_;
}

void Surround::draw()
{
	Landscape::instance()->getCloudTexture().draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	float slowXY = xy_ / 1.5f;
	glPushMatrix();
		// Rotate the scene so clouds blow the correct way
		glTranslatef(128.0f, 128.0f, -15.0f);
		glRotatef(cloudDirection_, 0.0f, 0.0f, 1.0f);

		GLState mainState2(GLState::TEXTURE_ON | GLState::BLEND_OFF);
		Vector sunDir = 
			-Landscape::instance()->getSun().getPosition().Normalize();
		Hemisphere::drawColored(1800, 180, 10, 10, 
			slowXY, slowXY + 0.4f,
			Landscape::instance()->getSkyColorsMap(),
			sunDir,
			ScorchedClient::instance()->getLandscapeMaps().getLandDfn().getTex()->skytimeofday);

		if (!OptionsDisplay::instance()->getNoSkyLayers())
		{
			GLState currentState(GLState::BLEND_ON);
			glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
			Hemisphere::draw(2100, 120, 10, 20, 0, 0, 
				false, true, 0.0f, xy_ + 0.3f);
		}
	glPopMatrix();
}
