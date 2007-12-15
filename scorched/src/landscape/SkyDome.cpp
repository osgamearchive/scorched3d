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
	DIALOG_ASSERT(cloudTexture_.replace(bitmapCloud));
	skyColorsMap_ = GLImageFactory::loadImageHandle(getDataFile(tex->skycolormap.c_str()));

	// Stars
	useStarTexture_ = false;
	if (!tex->skytexturestatic.empty())
	{
		useStarTexture_ = true;
		std::string stex(getDataFile(tex->skytexturestatic.c_str()));
		GLImageHandle bitmapStars = GLImageFactory::loadImageHandle(stex.c_str(), stex.c_str(), false);
		DIALOG_ASSERT(starTexture_.replace(bitmapStars));
	}
	noSunFog_ = tex->nosunfog;

	// Force refresh of colors
	colors_.clear();
}

void SkyDome::simulate(float frameTime)
{
	if (OptionsDisplay::instance()->getNoSkyMovement()) return;

	float fastSpeed = 100;
	float slowSpeed = 500;
	float currentSpeed = ScorchedClient::instance()->getOptionsTransient().getWindSpeed().asFloat();
	float wantedAngle = 180.0f-ScorchedClient::instance()->getOptionsTransient().getWindAngle().asFloat();
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
	Vector sunDir = 
		Landscape::instance()->getSky().getSun().getPosition().Normalize();
	LandscapeTex &tex =
		*ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getTex();

	// Cannot use a display list for heimisphere as we change texture
	// coordinates all the time
	glPushMatrix();
		// Translate scene so it is in the middle of the camera
		glTranslatef(pos[0], pos[1], -15.0f);

		// Draw sky color
		GLState mainState2(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
		colors_.drawColored(2000, 225, skyColorsMap_, sunDir, tex.skytimeofday);
		//Hemisphere::drawColored(2000, 225, 10, 10, 0, 0, false, 
		//	skyColorsMap_, sunDir, tex.skytimeofday);

		// Draw stars
		if (useStarTexture_)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
			GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
			starTexture_.draw();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(9.0f, 9.0f, 0.0f);
			glMatrixMode(GL_MODELVIEW);

			stars_.draw(1990, 215, Hemisphere::eWidthTexture);
			//Hemisphere::draw(2000, 225, 10, 10, 0, 0, false, Hemisphere::eWidthTexture);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
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
	
			// Cloud Layer 1
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glTranslatef(0.0f, xy_ + 0.3f, 0.0f);
			glMatrixMode(GL_MODELVIEW);

			clouds1_.draw(1850, 210, Hemisphere::eWidthTexture);
			//Hemisphere::draw(1850, 210, 10, 10, 0, 0, false, Hemisphere::eWidthTexture);

			// Cloud Layer 2
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glTranslatef(slowXY, slowXY + 0.4f, 0.0f);
			glMatrixMode(GL_MODELVIEW);

			clouds2_.draw(1900, 170, Hemisphere::eWidthTexture);
			//Hemisphere::draw(1900, 170, 10, 10, 0, 0, false, Hemisphere::eWidthTexture);

			// Reset tex matrix
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);

		glPopMatrix();
	}
}
