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
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefinition.h>
#include <landscape/Hemisphere.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLCamera.h>
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLDynamicVertexArray.h>
#include <client/ScorchedClient.h>
#include <graph/OptionsDisplay.h>
#include <common/OptionsTransient.h>
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
	LandscapeTex *tex = ScorchedClient::instance()->
		getLandscapeMaps().getDefinitions().getTex();	

	// Sky
	std::string ctex(getDataFile(tex->skytexture.c_str()));
	std::string ctexm(getDataFile(tex->skytexturemask.c_str()));
	GLImageHandle bitmapCloud = GLImageFactory::loadImageHandle(ctex.c_str(), ctexm.c_str(), false);
	DIALOG_ASSERT(cloudTexture_.replace(bitmapCloud, GL_RGBA));
	skyColorsMap_ = GLImageFactory::loadImageHandle(getDataFile(tex->skycolormap.c_str()));

	// Stars
	useStarTexture_ = false;
	if (!tex->skytexturestatic.empty())
	{
		useStarTexture_ = true;
		std::string stex(getDataFile(tex->skytexturestatic.c_str()));
		GLImageHandle bitmapStars = GLImageFactory::loadImageHandle(stex.c_str(), stex.c_str(), false);
		DIALOG_ASSERT(starTexture_.replace(bitmapStars, GL_RGBA));
	}
	noSunFog_ = tex->nosunfog;

	// Clear so new sky color is picked up
	layer1_.clear();
	layer2_.clear();
	layer3_.clear();
	layer4_.clear();
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
	Vector &pos = GLCamera::getCurrentCamera()->getCurrentPos();
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
				skyColorsMap_,
				sunDir,
				tex.skytimeofday);
		}
		// Layer 4
		if (layer4_.empty())
		{
			Hemisphere::createXY(layer4_,
				2000, 225, 10, 10, 0, 0, 20.0f, 20.0f);
		}

		// Translate scene so it is in the middle of the camera
		glTranslatef(pos[0], pos[1], -15.0f);

		// Draw Layer 1
		GLState mainState2(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
		drawLayer(layer1_, 1800, 225, 0.0f, 0.0f, true);

		// Draw Layer 4
		if (useStarTexture_)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			starTexture_.draw();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			drawLayer(layer4_, 2000, 225, 0.0f, 0.0f, false, 9.0f, 9.0f);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	glPopMatrix();

	// Draw Sun Glow
	if (noSunFog_) glDisable(GL_FOG); 
	Landscape::instance()->getSky().getSun().draw();
	if (!OptionsDisplay::instance()->getNoFog())
	{
		glEnable(GL_FOG);
	}

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
			cloudTexture_.draw();

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
	bool useColor, float tx, float ty)
{
	std::list<Hemisphere::HemispherePoint>::iterator itor;
	for (itor = layer.begin();
		itor != layer.end();)
	{
		Hemisphere::HemispherePoint &pointA = *(itor++);
		Hemisphere::HemispherePoint &pointB = *(itor++);

		pointA.tx = tx * (pointA.x + radius) / (2 * radius) + xvalue;
		pointA.ty = ty * (pointA.y + radius) / (2 * radius) + yvalue;
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

		pointB.tx = tx * (pointB.x + radius) / (2 * radius) + xvalue;
		pointB.ty = ty * (pointB.y + radius) / (2 * radius) + yvalue;
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
