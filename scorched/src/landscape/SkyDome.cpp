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
#include <common/Defines.h>

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
	layer3_.clear();
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
	Vector &pos = MainCamera::instance()->getCamera().getCurrentPos();
	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	glPushMatrix();
		// Layer 1
		if (layer1_.empty())
		{
			Vector sunDir = 
				Landscape::instance()->getSky().getSun().getPosition().Normalize();

			LandscapeTex &tex =
				*ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getTex();
			Hemisphere::createColored(layer1_, 
				1800, 250, 30, 30,
				Landscape::instance()->getSkyColorsMap(),
				sunDir,
				tex.skytimeofday);
		}

		// Translate scene so it is in the middle of the camera
		glTranslatef(pos[0], pos[1], -15.0f);
		GLState mainState2(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
		drawLayer(layer1_, 1800, 250, 0.0f, 0.0f, true);
	glPopMatrix();

	// Draw Sun Glow
	Landscape::instance()->getSky().getSun().draw();

	// Layer 2
	if (!OptionsDisplay::instance()->getNoSkyLayers())
	{
		glPushMatrix();
			// Translate scene so it is in the middle of the land
			glTranslatef(pos[0], pos[1], -15.0f);

			// Rotate the scene so clouds blow the correct way
			float slowXY = xy_ / 1.5f;
			glRotatef(cloudDirection_, 0.0f, 0.0f, 1.0f);
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

			// Cloud texture
			Landscape::instance()->getCloudTexture().draw();

			// Draw the other layers
			if (layer3_.empty())
			{
				Hemisphere::createXY(layer3_,
					1850, 210, 10, 10, 0, 0);
			}
			drawLayer(layer3_, 1850, 210, 0.0f, xy_ + 0.3f, false);

			if (layer2_.empty())
			{
				Hemisphere::createXY(layer2_,
					1900, 170, 10, 10, 0, 0);
			}
			drawLayer(layer2_, 1900, 170, slowXY, slowXY + 0.4f, false);
		glPopMatrix();
	}
}

void SkyDome::drawLayer(
	std::list<Hemisphere::HemispherePoint> &layer,
	float radius, float radius2, float xvalue, float yvalue,
	bool useColor)
{
	std::list<Hemisphere::HemispherePoint>::iterator itor;
	for (itor = layer.begin();
		itor != layer.end();)
	{
		Hemisphere::HemispherePoint &pointA = *(itor++);
		Hemisphere::HemispherePoint &pointB = *(itor++);

		pointA.tx = (pointA.x + radius) / (2 * radius) + xvalue;
		pointA.ty = (pointA.y + radius) / (2 * radius) + yvalue;
		GLDynamicVertexArray::instance()->addFloat(pointA.x);
		GLDynamicVertexArray::instance()->addFloat(pointA.y);
		GLDynamicVertexArray::instance()->addFloat(pointA.z);
		GLDynamicVertexArray::instance()->addFloat(pointA.tx);
		GLDynamicVertexArray::instance()->addFloat(pointA.ty);

		float ra = pointA.r;
		float ga = pointA.g;
		float ba = pointA.b;
		if (useColor)
		{
			if (flashTime_ > 0.0f)
			{
				ra = MIN(pointA.r + flashTime_ * 2.0f, 1.0f);
				ga = MIN(pointA.g + flashTime_ * 2.0f, 1.0f);
				ba = MIN(pointA.b + flashTime_ * 2.0f, 1.0f);
			}
			GLDynamicVertexArray::instance()->addFloat(ra);
			GLDynamicVertexArray::instance()->addFloat(ga);
			GLDynamicVertexArray::instance()->addFloat(ba);
		}

		pointB.tx = (pointB.x + radius) / (2 * radius) + xvalue;
		pointB.ty = (pointB.y + radius) / (2 * radius) + yvalue;
		GLDynamicVertexArray::instance()->addFloat(pointB.x);
		GLDynamicVertexArray::instance()->addFloat(pointB.y);
		GLDynamicVertexArray::instance()->addFloat(pointB.z);
		GLDynamicVertexArray::instance()->addFloat(pointB.tx);
		GLDynamicVertexArray::instance()->addFloat(pointB.ty);

		float rb = pointB.r;
		float gb = pointB.g;
		float bb = pointB.b;
		if (useColor)
		{
			if (flashTime_ > 0.0f)
			{
				rb = MIN(pointB.r + flashTime_ * 2.0f, 1.0f);
				gb = MIN(pointB.g + flashTime_ * 2.0f, 1.0f);
				bb = MIN(pointB.b + flashTime_ * 2.0f, 1.0f);
			}
			GLDynamicVertexArray::instance()->addFloat(rb);
			GLDynamicVertexArray::instance()->addFloat(gb);
			GLDynamicVertexArray::instance()->addFloat(bb);
		}

		if (GLDynamicVertexArray::instance()->getSpace() < 10)
		{
			GLDynamicVertexArray::instance()->drawQuadStrip(useColor);

			GLDynamicVertexArray::instance()->addFloat(pointA.x);
			GLDynamicVertexArray::instance()->addFloat(pointA.y);
			GLDynamicVertexArray::instance()->addFloat(pointA.z);
			GLDynamicVertexArray::instance()->addFloat(pointA.tx);
			GLDynamicVertexArray::instance()->addFloat(pointA.ty);
			if (useColor)
			{
				GLDynamicVertexArray::instance()->addFloat(ra);
				GLDynamicVertexArray::instance()->addFloat(ga);
				GLDynamicVertexArray::instance()->addFloat(ba);
			}

			GLDynamicVertexArray::instance()->addFloat(pointB.x);
			GLDynamicVertexArray::instance()->addFloat(pointB.y);
			GLDynamicVertexArray::instance()->addFloat(pointB.z);
			GLDynamicVertexArray::instance()->addFloat(pointB.tx);
			GLDynamicVertexArray::instance()->addFloat(pointB.ty);
			if (useColor)
			{
				GLDynamicVertexArray::instance()->addFloat(rb);
				GLDynamicVertexArray::instance()->addFloat(gb);
				GLDynamicVertexArray::instance()->addFloat(bb);
			}
		}
	}
	GLDynamicVertexArray::instance()->drawQuadStrip(useColor);
}
