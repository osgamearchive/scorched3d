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
	wMap_(64, 8, 256), patchGrid_(&ScorchedClient::instance()->getLandscapeMaps().getHMap(), 16), 
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

				// Update the plan texture
				updatePlanATexture();
				updatePlanTexture();

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
			if (GLStateExtension::getTextureUnits() > 2 &&
				OptionsDisplay::instance()->getDetailTexture() &&
				GLStateExtension::hasEnvCombine())
			{
				GLStateExtension::glActiveTextureARB()(GL_TEXTURE2_ARB);
				glEnable(GL_TEXTURE_2D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

				detailTexture_.draw(true);
			}

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
		HeightMapRenderer::drawHeightMap(
			ScorchedClient::instance()->getLandscapeMaps().getHMap());
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f);
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
			if (GLStateExtension::getTextureUnits() > 2 &&
				OptionsDisplay::instance()->getDetailTexture() &&
				GLStateExtension::hasEnvCombine())
			{
				GLStateExtension::glActiveTextureARB()(GL_TEXTURE2_ARB);
				glDisable(GL_TEXTURE_2D);
			}

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
	if (!mainMap_.getBits())
	{
		mainMap_.createBlank(mapTexSize, mapTexSize);
		scorchMap_.createBlank(mapTexSize, mapTexSize);
		bitmapPlanAlpha_.createBlank(planTexSize, planTexSize, true);
		bitmapPlan_.createBlank(planTexSize, planTexSize);
	}

	// Load the texture bitmaps from resources 
	GLBitmap texture0(Resources::stringResource("bitmap-texture0"));
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
	GLBitmap bitmapWater(Resources::stringResource("bitmap-cloudreflection"));
	bitmapWater_.loadFromFile(Resources::stringResource("bitmap-cloudreflection"), false);
	GLBitmap bitmapMagma(Resources::stringResource("bitmap-magmasmall"));
	GLBitmap bitmapCloud(Resources::stringResource("bitmap-cloud"));
	GLBitmap bitmapDetail(Resources::stringResource("bitmap-detail"));
	GLBitmap bitmapWaterDetail(Resources::stringResource("bitmap-water"));

	// Generate landscape texture
	surroundTexture_.replace(texture0);
	GLBitmapModifier::addHeightToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		mainMap_, 
		bitmapRock, bitmapShore, bitmaps, 5, 1024, counter);

	// Add lighting to the landscape texture
	GLBitmapModifier::addLightMapToBitmap(mainMap_, 
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		sun_.getPosition(), counter);

	// Create the landscape texture used for the small plan window
	gluScaleImage(
		GL_RGB, 
		mainMap_.getWidth(), mainMap_.getHeight(), 
		GL_UNSIGNED_BYTE, mainMap_.getBits(),
		bitmapPlan_.getWidth(), bitmapPlan_.getHeight(), 
		GL_UNSIGNED_BYTE, bitmapPlan_.getBits());

	// Create the magma texture
	DIALOG_ASSERT(magTexture_.replace(bitmapMagma));

	// Create the main landscape texture
	DIALOG_ASSERT(texture_.replace(mainMap_, GL_RGB, false));

	// Create the detail textures
	DIALOG_ASSERT(detailTexture_.replace(bitmapDetail, GL_RGB, true));
	DIALOG_ASSERT(waterDetail_.replace(bitmapWaterDetail, GL_RGB, true));
	
	// Create the plan textures (for the plan and wind dialogs)
	updatePlanTexture();
	updatePlanATexture();

	// Create the cloud texture
	DIALOG_ASSERT(cloudTexture_.create(bitmapCloud));

	// Set the fog color
	Vector *fogColor = Resources::vectorResource("color-fog");
	GLfloat fogColorF[4];
	fogColorF[0] = (*fogColor)[0];
	fogColorF[1] = (*fogColor)[1];
	fogColorF[2] = (*fogColor)[2];
	fogColorF[3] = 1.0f;
	glFogfv(GL_FOG_COLOR, fogColorF);

	// Water waves texture
	Vector *wavesColor = Resources::vectorResource("color-waves");
	GLBitmapModifier::addWavesToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		wMap_.getBitmap(), wMap_.getHeight(), 0.6f, *wavesColor, 
		counter);
	wMap_.refreshTexture();

	// Load the water reflection bitmap
	// Create water cubemap texture
	delete waterTexture_;
	if (GLStateExtension::hasCubeMap()) waterTexture_ = new GLTextureCubeMap;
	else waterTexture_ = new GLTexture;
	bitmapWater.resize(256, 256);
	DIALOG_ASSERT(bitmapWater.getBits());
	waterTexture_->create(bitmapWater, GL_RGB, false);

	// Ensure that all components use new landscape
	reset();
}

void Landscape::updatePlanTexture()
{
	GLBitmapModifier::addWaterToBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		bitmapPlan_, bitmapWater_, wMap_.getHeight());
	DIALOG_ASSERT(planTexture_.replace(bitmapPlan_, GL_RGB, false));
}

void Landscape::updatePlanATexture()
{
	GLBitmapModifier::removeWaterFromBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		bitmapPlan_, bitmapPlanAlpha_, wMap_.getHeight());
	DIALOG_ASSERT(planAlphaTexture_.replace(bitmapPlanAlpha_, GL_RGBA, false));
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

	DIALOG_ASSERT(texture_.replace(mainMap_, GL_RGB, false));
	textureType_ = eDefault;
}
