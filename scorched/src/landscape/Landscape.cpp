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

#include <landscape/Landscape.h>
#include <landscape/ShadowMap.h>
#include <landscape/InfoMap.h>
#include <landscape/WaterMapModifier.h>
#include <landscape/HeightMapRenderer.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLStateExtension.h>
#include <common/OptionsTransient.h>
#include <common/Resources.h>
#include <common/OptionsDisplay.h>
#include <dialogs/WindDialog.h>
#include <client/ScorchedClient.h>

Landscape *Landscape::instance_ = 0;

Landscape *Landscape::instance()
{
	if (!instance_)
	{
		instance_ = new Landscape;
	}
	return instance_;
}

Landscape::Landscape() : 
	wMap_(64, 8, 512), patchGrid_(&ScorchedClient::instance()->getLandscapeMaps().getHMap(), 16), 
	wMapPoints_(wMap_, 256, 4),
	surroundDefs_(ScorchedClient::instance()->getLandscapeMaps().getHMap(), 1524, 256), 
	surround_(surroundDefs_),
	hMapSurround_(surroundDefs_),
	resetWater_(false), resetWaterTimer_(0.0f), waterTexture_(0),
	textureType_(eDefault)
{

}

Landscape::~Landscape()
{
}

void Landscape::simulate(const unsigned state, float frameTime)
{
	if (OptionsDisplay::instance()->getDrawWater())
	{
		wMap_.simulate(frameTime);
		if (resetWater_)
		{
			resetWaterTimer_ -= frameTime;
			if (resetWaterTimer_ < 0.0f)
			{
				// Re-calculate the water (what water is visible)
				WaterMapModifier::addWaterVisibility(
					ScorchedClient::instance()->getLandscapeMaps().getHMap(), wMap_);

				// Re-calculate the landsacpe on the wind indicator
				WindDialog::instance()->buildMap();
				resetWater_ = false;
			}
		}
	}
	patchGrid_.simulate(frameTime);
	smoke_.simulate(frameTime);
	surround_.simulate(frameTime);
	wall_.simulate(frameTime);
}

void Landscape::recalculate(int posX, int posY, int dist)
{
	if (!resetWater_)
	{
		resetWater_ = true;
		resetWaterTimer_ = 1.0f; // Recalculate the water in x seconds
	}

	// Recalculate the level of detail for the terrain
	patchGrid_.recalculate(posX, posY, dist);
}

void Landscape::draw(const unsigned state)
{
	if (OptionsDisplay::instance()->getDrawLines()) glPolygonMode(GL_FRONT, GL_LINE);
	else glPolygonMode(GL_FRONT, GL_FILL);

	GLState *textureState = 0;
	if (OptionsDisplay::instance()->getUseLandscapeTexture())
	{
		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);
			shadowMap_.setTexture();

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

		texture_.draw(true);
	}
	else
	{
		textureState = new GLState(GLState::TEXTURE_OFF);
	}
	
	if (OptionsDisplay::instance()->getNoROAM())
	{
		HeightMapRenderer::drawHeightMap(ScorchedClient::instance()->getLandscapeMaps().getHMap());
	}
	else
	{
		patchGrid_.draw(PatchSide::typeTop);

		if (OptionsDisplay::instance()->getDrawNormals())
		{
			GLState currentState(GLState::TEXTURE_OFF);
			patchGrid_.draw(PatchSide::typeNormals);
		}
	}

	if (OptionsDisplay::instance()->getUseLandscapeTexture())
	{
		if (GLStateExtension::glActiveTextureARB())
		{
			GLStateExtension::glActiveTextureARB()(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}
	}
	else
	{
		delete textureState;
	}

	//static CloudSim sim;
	//sim.simulate();

	// NOTE: The following code is drawn with fog on
	// Be carefull as this we "dull" bilboard textures
	glEnable(GL_FOG); // NOTE: Fog on
	surroundTexture_.draw(true);
	hMapSurround_.draw();
	cloudTexture_.draw();
	surround_.draw();
	if (OptionsDisplay::instance()->getDrawWater())
	{
		glColor3fv(ScorchedClient::instance()->getOptionsTransient().getWallColor());
		wMapPoints_.draw();
		wMap_.draw();
	}
	glDisable(GL_FOG); // NOTE: Fog off

	wall_.draw();
	sun_.draw();
	smoke_.draw();

	if (OptionsDisplay::instance()->getDrawLines()) glPolygonMode(GL_FRONT, GL_FILL);
}

void Landscape::generate(ProgressCounter *counter)
{
	textureType_ = eDefault;
	InfoMap::instance();

	// Set a random resource value
	Resources::instance()->main.setRandomModule();

	// Choose the correct sizes for the current LOD
	int mapTexSize = 1024;
	int planTexSize = 128;
	switch (OptionsDisplay::instance()->getTexSize())
	{
	case 0:
		mapTexSize = 256;
		planTexSize = 64;
		break;
	case 2:
		mapTexSize = 2048;
		planTexSize = 256;
		break;
	default:
		mapTexSize = 1024;
		planTexSize = 128;
		break;
	}

	// Generate the texture used to map onto the landscape
	// Load the textures used for the levels of the landscape
	if (!mainMap_.getBits())
	{
		mainMap_.createBlank(mapTexSize, mapTexSize);
		scorchMap_.createBlank(mapTexSize, mapTexSize);
	}
	GLBitmap texture0(Resources::stringResource("bitmap-texture0"));
	surroundTexture_.create(texture0);
	GLBitmap texture1(Resources::stringResource("bitmap-texture1"));
	GLBitmap texture2(Resources::stringResource("bitmap-texture2"));
	GLBitmap texture3(Resources::stringResource("bitmap-texture3"));
	GLBitmap texture4(Resources::stringResource("bitmap-texture4"));

	GLBitmap scorchMap(Resources::stringResource("bitmap-scorch"));
	GLBitmapModifier::tileBitmap(scorchMap, scorchMap_);

	GLBitmap bitmapShore(Resources::stringResource("bitmap-shore"));
	GLBitmap bitmapRock(Resources::stringResource("bitmap-rockside"));
	GLBitmap *bitmaps[5];
	bitmaps[0] = &texture0;
	bitmaps[1] = &texture1;
	bitmaps[2] = &texture2;
	bitmaps[3] = &texture3;
	bitmaps[4] = &texture4;

	// Generate landscape texture
	GLBitmapModifier::addHeightToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		mainMap_, 
		bitmapRock, bitmapShore, bitmaps, 5, 1024, counter);

	// Add lighting to the landscape texture
	GLBitmapModifier::addLightMapToBitmap(mainMap_, 
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		sun_.getPosition(), counter);

	// Create the landscape texture used for the small plan window
	GLBitmap bitmapPlan(planTexSize, planTexSize);
	gluScaleImage(
		GL_RGB, 
		mainMap_.getWidth(), mainMap_.getHeight(), 
		GL_UNSIGNED_BYTE, mainMap_.getBits(),
		bitmapPlan.getWidth(), bitmapPlan.getHeight(), 
		GL_UNSIGNED_BYTE, bitmapPlan.getBits());

	GLBitmap bitmapPlanAlpha(planTexSize, planTexSize, true);
	GLBitmap bitmapWater(Resources::stringResource("bitmap-cloudreflection"));
	GLBitmapModifier::addWaterToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		bitmapPlan, bitmapWater, wMap_.getHeight());
	GLBitmapModifier::removeWaterFromBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		bitmapPlan, bitmapPlanAlpha, wMap_.getHeight());

	// Load the bitmaps into the textures
	GLBitmap bitmapMagma(Resources::stringResource("bitmap-magmasmall"));
	magTexture_.create(bitmapMagma);
	if (!texture_.textureValid())
	{
		texture_.create(mainMap_, GL_RGB, false);
	}
	else
	{
		texture_.draw(true);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, mainMap_.getWidth());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 
			0, 0, 
			mainMap_.getWidth(), mainMap_.getHeight(), 
			GL_RGB, GL_UNSIGNED_BYTE, 
			mainMap_.getBits());
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
	planTexture_.create(bitmapPlan, GL_RGB, false);
	planAlphaTexture_.create(bitmapPlanAlpha, GL_RGBA, true);

	GLBitmapModifier::addWavesToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		wMap_.getBitmap(), wMap_.getHeight(), 0.3f);
	wMap_.refreshTexture();

	// Load the cloud layer bitmap
	GLBitmap map(Resources::stringResource("bitmap-cloud"));
	DIALOG_ASSERT(cloudTexture_.create(map));

	// Load the water reflection bitmap
	// Create water cubemap
	delete waterTexture_;
	if (GLStateExtension::hasCubeMap())
	{
		waterTexture_ = new GLTextureCubeMap;
	}
	else
	{
		waterTexture_ = new GLTexture;
	}
	GLBitmap waterMap(Resources::stringResource("bitmap-cloudreflection"));
	waterMap.resize(256, 256);
	DIALOG_ASSERT(waterMap.getBits());
	waterTexture_->create(waterMap, GL_RGB, false);

	// Ensure that all components use new landscape
	reset();
}

void Landscape::reset()
{
	// Recalculate all landscape objects
	// Ensure all objects use any new landscape
	recalculate(0, 0, 1000);
	patchGrid_.forceCalculate(256);
	WindDialog::instance()->buildMap();
	WaterMapModifier::addWaterVisibility(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		wMap_);
	smoke_.removeAllSmokes();
	wMap_.reset();
}

void Landscape::restoreLandscapeTexture()
{
	if (textureType_ == eDefault) return;

	Landscape::instance()->getMainTexture().draw(true);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, mainMap_.getWidth());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 
					0, 0, 
					mainMap_.getWidth(), mainMap_.getHeight(), 
					GL_RGB, GL_UNSIGNED_BYTE, 
					mainMap_.getBits());
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	textureType_ = eDefault;
}
