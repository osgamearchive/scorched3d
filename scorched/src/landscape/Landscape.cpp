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
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/ShadowMap.h>
#include <landscape/InfoMap.h>
#include <landscape/WaterMapModifier.h>
#include <landscape/HeightMapRenderer.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLConsoleRuleMethodIAdapter.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <client/ScorchedClient.h>
#include <engine/ActionController.h>
#include <time.h>

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
	textureType_(eDefault),
	changeCount_(1)
{
	new GLConsoleRuleMethodIAdapter<Landscape>(
		this, &Landscape::savePlan, "SavePlan");
}

Landscape::~Landscape()
{
}

void Landscape::simulate(const unsigned state, float frameTime)
{
	float speedMult = ScorchedClient::instance()->getActionController().getFast();
	if (OptionsDisplay::instance()->getDrawWater())
	{
		wMap_.simulate(frameTime * speedMult);
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
				changeCount_++;
				resetWater_ = false;
			}
		}
	}
	patchGrid_.simulate(frameTime);
	smoke_.simulate(frameTime * speedMult);
	surround_.simulate(frameTime * speedMult);
	wall_.simulate(frameTime * speedMult);
	wWaves_.simulate(frameTime * speedMult);
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
	surround_.draw();
	if (OptionsDisplay::instance()->getDrawWater())
	{
		wWaves_.draw();
		glColor3fv(ScorchedClient::instance()->getOptionsTransient().getWallColor());
		wMapPoints_.draw();
		wMap_.draw();
	}
	glDisable(GL_FOG); // NOTE: Fog off

	objects_.draw();
	wall_.draw();
	sun_.draw();
	smoke_.draw();

	if (OptionsDisplay::instance()->getDrawLines()) glPolygonMode(GL_FRONT, GL_FILL);
}

void Landscape::generate(ProgressCounter *counter)
{
	textureType_ = eDefault;
	InfoMap::instance();

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
		bitmapPlanAlphaAlpha_.loadFromFile(
			getDataFile("data/windows/planaa.bmp"), false);
		bitmapPlanAlphaAlpha_.resize(planTexSize, planTexSize);
	}

	// Load the texture bitmaps from resources 
	LandscapeTex *tex = 
			ScorchedClient::instance()->getLandscapeMaps().getLandDfn().getTex();

	if (0 == strcmp(tex->texturetype.c_str(), "generate"))
	{
		LandscapeTexTextureGenerate *generate = 
			(LandscapeTexTextureGenerate *) tex->texture;

		GLBitmap texture0(getDataFile(generate->texture0.c_str()));
		GLBitmap texture1(getDataFile(generate->texture1.c_str()));
		GLBitmap texture2(getDataFile(generate->texture2.c_str()));
		GLBitmap texture3(getDataFile(generate->texture3.c_str()));
		GLBitmap texture4(getDataFile(generate->texture4.c_str()));
		GLBitmap bitmapShore(getDataFile(generate->shore.c_str()));
		GLBitmap bitmapRock(getDataFile(generate->rockside.c_str()));
		GLBitmap *bitmaps[5];
		bitmaps[0] = &texture0;
		bitmaps[1] = &texture1;
		bitmaps[2] = &texture2;
		bitmaps[3] = &texture3;
		bitmaps[4] = &texture4;

		// Generate the new landscape
		GLBitmapModifier::addHeightToBitmap(
			ScorchedClient::instance()->getLandscapeMaps().getHMap(),
			mainMap_,
			bitmapRock, bitmapShore, bitmaps, 5, 1024, counter);

		// Set the general surround texture
		surroundTexture_.replace(texture0);
	}
	else
	{
		dialogExit("Landscape",
			"Failed to find heightmap type %s",
			tex->texturetype.c_str());
	}

	// Create the main landscape texture
	DIALOG_ASSERT(texture_.replace(mainMap_, GL_RGB, false));

	// Load a mask
	std::string sprayMaskFile = getDataFile("data/textures/smoke01.bmp");
	GLBitmap sprayMaskBitmap(sprayMaskFile.c_str(), sprayMaskFile.c_str(), false);

	if (0 == strcmp(tex->bordertype.c_str(), "water"))
	{
		LandscapeTexBorderWater *water = 
			(LandscapeTexBorderWater *) tex->border;

		const char *wave1 = getDataFile(
			water->wavetexture1.c_str());
		GLBitmap waves1Map(wave1, wave1, false);
		const char *wave2 = getDataFile(
			water->wavetexture2.c_str());
		GLBitmap waves2Map(wave2, wave2, false);
		waves1Texture_.replace(waves1Map, GL_RGBA);
		waves2Texture_.replace(waves2Map, GL_RGBA);
		bitmapWater_.loadFromFile(getDataFile(water->reflection.c_str()), false);
		GLBitmap bitmapWaterDetail(getDataFile(water->texture.c_str()));
		waterDetail_.replace(bitmapWaterDetail, GL_RGB, true);

		// Generate the water texture for the spray sprite
		GLBitmap bitmapWater(getDataFile(water->reflection.c_str()));
		bitmapWater.resize(
			sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight());
		GLBitmap textureWaterNew(
			sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight(), true);
		GLBitmapModifier::makeBitmapTransparent(textureWaterNew, 
			bitmapWater, sprayMaskBitmap);
		landTexWater_.replace(textureWaterNew, GL_RGBA);

		// Load the water reflection bitmap
		// Create water cubemap texture
		bitmapWater.resize(256, 256);
		DIALOG_ASSERT(bitmapWater.getBits());
		delete waterTexture_;
		if (GLStateExtension::hasCubeMap() &&
			!OptionsDisplay::instance()->getNoGLSphereMap())
		{
			GLTextureCubeMap *waterCubeMap = new GLTextureCubeMap();
			waterCubeMap->create(bitmapWater, GL_RGB, false);
			waterTexture_ = waterCubeMap;
		}
		else 
		{
			GLTexture *waterNormalMap = new GLTexture();
			waterNormalMap->create(bitmapWater, GL_RGB, false);
			waterTexture_ = waterNormalMap;
		}

		// Find the waves around this island
		wWaves_.generateWaves(counter);
	}
	else
	{
		dialogExit("Landscape",
			"Failed to find border type %s",
			tex->bordertype.c_str());
	}

	// Add lighting to the landscape texture
	sun_.setPosition(tex->skysunxy, tex->skysunyz);
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

	// Generate the scorch map for the landscape
	GLBitmap scorchMap(getDataFile(tex->scorch.c_str()));
	GLBitmapModifier::tileBitmap(scorchMap, scorchMap_);
	scorchMap.resize(sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight());
	GLBitmap texture1New(sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight(), true);
	GLBitmapModifier::makeBitmapTransparent(texture1New, scorchMap, sprayMaskBitmap);
	landTex1_.replace(texture1New, GL_RGBA);

	// Magma
	GLBitmap bitmapMagma(getDataFile(tex->magmasmall.c_str()));
	DIALOG_ASSERT(magTexture_.replace(bitmapMagma));

	// Sky
	GLBitmap bitmapCloud(getDataFile(tex->skytexture.c_str()));
	DIALOG_ASSERT(cloudTexture_.replace(bitmapCloud));
	skyColorsMap_.loadFromFile(getDataFile(tex->skycolormap.c_str()));

	// Detail
	GLBitmap bitmapDetail(getDataFile(tex->detail.c_str()));
	DIALOG_ASSERT(detailTexture_.replace(bitmapDetail, GL_RGB, true));

	// Add objects to the landscape (if any)
	objects_.removeAllObjects();
	RandomGenerator objectsGenerator;
	objectsGenerator.seed(
		ScorchedClient::instance()->getLandscapeMaps().getLandDfn().getSeed());
	objects_.generate(objectsGenerator, *tex, counter);

	// Create the plan textures (for the plan and wind dialogs)
	updatePlanTexture();
	updatePlanATexture();

	// Set the fog color
	GLfloat fogColorF[4];
	fogColorF[0] = tex->fog[0];
	fogColorF[1] = tex->fog[1];
	fogColorF[2] = tex->fog[2];
	fogColorF[3] = 1.0f;
	glFogfv(GL_FOG_COLOR, fogColorF);
	
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
		bitmapPlan_, bitmapPlanAlpha_, bitmapPlanAlphaAlpha_, 
		wMap_.getHeight());
	DIALOG_ASSERT(planAlphaTexture_.replace(bitmapPlanAlpha_, GL_RGBA, false));
	planAlphaTexture_.draw();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}

void Landscape::reset()
{
	// Recalculate all landscape objects
	// Ensure all objects use any new landscape
	recalculate(0, 0, 1000);
	patchGrid_.forceCalculate(256);
	changeCount_++;
	WaterMapModifier::addWaterVisibility(
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		wMap_);
	smoke_.removeAllSmokes();
	wMap_.reset();
	surround_.clear();
}

void Landscape::restoreLandscapeTexture()
{
	if (textureType_ == eDefault) return;

	DIALOG_ASSERT(texture_.replace(mainMap_, GL_RGB, false));
	textureType_ = eDefault;
}

void Landscape::savePlan()
{
	static unsigned counter = 0;
	time_t currentTime = time(0);
	bitmapPlan_.writeToFile(getHomeFile("PlanShot-%i-%i.bmp", currentTime, counter++));
}

