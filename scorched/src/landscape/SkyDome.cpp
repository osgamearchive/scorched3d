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

#include <landscape/SkyDome.h>
#include <landscape/Sky.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/Hemisphere.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLDynamicVertexArray.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsTransient.h>
#include <common/FileList.h>

SkyDome::SkyDome() : 
	xy_(0.0f), 
	cloudSpeed_(500.0f), 
	cloudDirection_(0.0f),
	flashTime_(0.0f)
{
}

SkyDome::~SkyDome()
{
}

void SkyDome::flash()
{
	flashTime_ = 0.25f;
}

void SkyDome::generate()
{
	// Clear so new sky color is picked up
	layer1_.clear();
	layer2_.clear();
}

void SkyDome::simulate(float frameTime)
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

	// The sky flash
	flashTime_ -= frameTime;
}

void SkyDome::draw()
{
	Landscape::instance()->getCloudTexture().draw();

	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	float slowXY = xy_ / 1.5f;
	glPushMatrix();
		// Rotate the scene so clouds blow the correct way
		glTranslatef(128.0f, 128.0f, -15.0f);
		glRotatef(cloudDirection_, 0.0f, 0.0f, 1.0f);

		// Layer 1
		if (layer1_.empty())
		{
			Vector sunDir = 
				-Landscape::instance()->getSky().getSun().getPosition().Normalize();

			Hemisphere::createColored(layer1_, 
				1800, 180, 10, 10,
				Landscape::instance()->getSkyColorsMap(),
				sunDir,
				ScorchedClient::instance()->getLandscapeMaps().
				getLandDfn()->getTex()->skytimeofday);
		}
		GLState mainState2(GLState::TEXTURE_ON | GLState::BLEND_OFF);
		drawLayer(layer1_, 1800, 180, slowXY, slowXY + 0.4f, true);

		// Layer 2
		if (!OptionsDisplay::instance()->getNoSkyLayers())
		{
			if (layer2_.empty())
			{
				Hemisphere::createXY(layer2_,
					2100, 120, 10, 20, 0, 0);
			}

			GLState currentState(GLState::BLEND_ON);
			glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
			drawLayer(layer2_, 2100, 120, 0.0f, xy_ + 0.3f, false);
		}
	glPopMatrix();
}

void SkyDome::drawLayer(
	std::list<Hemisphere::HemispherePoint> &layer,
	float radius, float radius2, float xvalue, float yvalue,
	bool useColor)
{
	std::list<Hemisphere::HemispherePoint>::iterator itor;
	for (itor = layer.begin();
		itor != layer.end();
		itor++)
	{
		Hemisphere::HemispherePoint &point = (*itor);

		point.tx = (point.x + radius) / (2 * radius) + xvalue;
		point.ty = (point.y + radius) / (2 * radius) + yvalue;

		GLDynamicVertexArray::instance()->addFloat(point.x);
		GLDynamicVertexArray::instance()->addFloat(point.y);
		GLDynamicVertexArray::instance()->addFloat(point.z);
		GLDynamicVertexArray::instance()->addFloat(point.tx);
		GLDynamicVertexArray::instance()->addFloat(point.ty);

		float r = point.r;
		float g = point.g;
		float b = point.b;
		if (flashTime_ > 0.0f)
		{
			r = MIN(r + flashTime_ * 2.0f, 1.0f);
			g = MIN(g + flashTime_ * 2.0f, 1.0f);
			b = MIN(b + flashTime_ * 2.0f, 1.0f);
		}

		if (useColor)
		{
			GLDynamicVertexArray::instance()->addFloat(r);
			GLDynamicVertexArray::instance()->addFloat(g);
			GLDynamicVertexArray::instance()->addFloat(b);
		}

		if (GLDynamicVertexArray::instance()->getSpace() < 10)
		{
			GLDynamicVertexArray::instance()->drawQuadStrip(useColor);
			GLDynamicVertexArray::instance()->addFloat(point.x);
			GLDynamicVertexArray::instance()->addFloat(point.y);
			GLDynamicVertexArray::instance()->addFloat(point.z);
			GLDynamicVertexArray::instance()->addFloat(point.tx);
			GLDynamicVertexArray::instance()->addFloat(point.ty);
			if (useColor)
			{
				GLDynamicVertexArray::instance()->addFloat(r);
				GLDynamicVertexArray::instance()->addFloat(g);
				GLDynamicVertexArray::instance()->addFloat(b);
			}
		}
	}
	GLDynamicVertexArray::instance()->drawQuadStrip(useColor);
}
