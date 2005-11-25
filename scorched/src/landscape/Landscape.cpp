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
#include <landscape/LandscapePoints.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeSoundManager.h>
#include <landscape/LandscapeDefinition.h>
#include <landscape/LandscapeDefinitions.h>
#include <landscape/Smoke.h>
#include <landscape/Wall.h>
#include <landscape/ShadowMap.h>
#include <landscape/Sky.h>
#include <landscape/Water.h>
#include <landscape/Surround.h>
#include <landscape/ShadowMap.h>
#include <landscape/InfoMap.h>
#include <landscape/HeightMapRenderer.h>
#include <landscape/PatchGrid.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLConsoleRuleMethodIAdapter.h>
#include <boids/ScorchedBoids.h>
#include <common/OptionsTransient.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <sound/Sound.h>
#include <client/ScorchedClient.h>
#include <client/MainCamera.h>
#include <dialogs/CameraDialog.h>
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
	resetLandscape_(false), resetLandscapeTimer_(0.0f), 
	textureType_(eDefault),
	changeCount_(1)
{
	soundManager_ = new LandscapeSoundManager();
	patchGrid_ = new PatchGrid(
		&ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(), 16), 
	water_ = new Water();
	points_ = new LandscapePoints();
	surround_ = new Surround(
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap());
	sky_ = new Sky();
	boids_ = new ScorchedBoids();
	smoke_ = new Smoke();
	wall_ = new Wall();
	shadowMap_ = new ShadowMap();

	new GLConsoleRuleMethodIAdapter<Landscape>(
		this, &Landscape::savePlan, "SavePlan");
}

Landscape::~Landscape()
{
}

void Landscape::simulate(const unsigned state, float frameTime)
{
	if (resetLandscape_)
	{
		resetLandscapeTimer_ -= frameTime;
		if (resetLandscapeTimer_ < 0.0f)
		{
			// Update the plan texture
			updatePlanATexture();
			updatePlanTexture();

			// Re-calculate the landsacpe on the wind indicator
			changeCount_++;
			resetLandscape_ = false;
		}
	}

	float speedMult = ScorchedClient::instance()->
		getActionController().getFast();
	water_->simulate(frameTime * speedMult);
	patchGrid_->simulate(frameTime);
	sky_->simulate(frameTime * speedMult);
	wall_->simulate(frameTime * speedMult);
	boids_->simulate(frameTime * speedMult);
	soundManager_->simulate(frameTime * speedMult);

	Sound::instance()->simulate(frameTime);
}

void Landscape::recalculate(int posX, int posY, int dist)
{
	if (!resetLandscape_)
	{
		resetLandscape_ = true;
		resetLandscapeTimer_ = 1.0f; // Recalculate the water in x seconds
	}

	// Recalculate the level of detail for the terrain
	patchGrid_->recalculate(posX, posY, dist);
	water_->recalculate();
}

void Landscape::reset(ProgressCounter *counter)
{
	changeCount_++;

	// Recalculate all landscape objects
	// Ensure all objects use any new landscape
	patchGrid_->reset(counter);
	water_->reset();
	ScorchedClient::instance()->getParticleEngine().killAll();
	MainCamera::instance()->getTarget().
		getPrecipitationEngine().killAll();
	CameraDialog::instance()->getCamera().
		getPrecipitationEngine().killAll();
}

void Landscape::draw(const unsigned state)
{
	if (OptionsDisplay::instance()->getDrawLines()) glPolygonMode(GL_FRONT, GL_LINE);

	// NOTE: The following code is drawn with fog on
	// Be carefull as this we "dull" bilboard textures
	if (!OptionsDisplay::instance()->getNoFog())
	{
		glEnable(GL_FOG); // NOTE: Fog on
	}

	sky_->draw();
	drawLand();
	points_->draw();
	surround_->draw();
	water_->draw();
	boids_->draw();
	ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getObjects().draw();
	wall_->draw();

	glDisable(GL_FOG); // NOTE: Fog off

	if (OptionsDisplay::instance()->getDrawLines()) glPolygonMode(GL_FRONT, GL_FILL);
}

int Landscape::getPlanTexSize()
{
	switch (OptionsDisplay::instance()->getTexSize())
	{
	case 0:
		return 64;
		break;
	case 2:
		return 256;
		break;
	default:
		return 128;
		break;
	}
	return 128;
}

int Landscape::getMapTexSize()
{
	switch (OptionsDisplay::instance()->getTexSize())
	{
	case 0:
		return 256;
		break;
	case 2:
		return 2048;
		break;
	default:
		return 1024;
		break;
	}
	return 1024;
}

void Landscape::generate(ProgressCounter *counter)
{
	textureType_ = eDefault;
	InfoMap::instance();

	// Choose the correct sizes for the current LOD
	int mapTexSize = getMapTexSize();
	int planTexSize = getPlanTexSize();

	// Generate the texture used to map onto the landscape
	if (!mainMap_.getBits())
	{
		mainMap_.createBlank(mapTexSize, mapTexSize);
		bitmapPlanAlpha_.createBlank(planTexSize, planTexSize, true);
		bitmapPlan_.createBlank(planTexSize, planTexSize);
		bitmapPlanAlphaAlpha_.loadFromFile(
			getDataFile("data/windows/planaa.bmp"), false);
		bitmapPlanAlphaAlpha_.resize(planTexSize, planTexSize);
	}

	// Load the texture bitmaps from resources 
	LandscapeTex *tex = 
			ScorchedClient::instance()->getLandscapeMaps().getDefinitions().getTex();
	if (tex->texture->getType() == LandscapeTexType::eTextureGenerate)
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
		GLBitmap bitmapRoof(getDataFile(generate->roof.c_str()));
		GLBitmap bitmapSurround(getDataFile(generate->surround.c_str()));
		GLBitmap *bitmaps[5];
		bitmaps[0] = &texture0;
		bitmaps[1] = &texture1;
		bitmaps[2] = &texture2;
		bitmaps[3] = &texture3;
		bitmaps[4] = &texture4;

		// Generate the new landscape
		GLBitmapModifier::addHeightToBitmap(
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(),
			mainMap_,
			bitmapRock, bitmapShore, bitmaps, 5, 1024, counter);

		// Set the general surround and roof texture
		groundTexture_.replace(texture0, GL_RGB, false);
		surroundTexture_.replace(bitmapSurround, GL_RGB, false);
		roofTexture_.replace(bitmapRoof, GL_RGB, true);
	}
	else
	{
		dialogExit("Landscape",
			"Failed to find heightmap type %i",
			tex->texture->getType());
	}

	// Create the water (if any)
	patchGrid_->generate();
	water_->generate(counter);
	points_->generate();

	// Add any boids
	boids_->generate();

	// Add lighting to the landscape texture
	sky_->getSun().setPosition(tex->skysunxy, tex->skysunyz);
	GLBitmapModifier::addLightMapToBitmap(mainMap_,
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(),
		sky_->getSun().getPosition(), 
		tex->skyambience, tex->skydiffuse, counter);

	// Add shadows to the mainmap
	{
		float shadowMultWidth = (float) getMainMap().getWidth() / 
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapWidth();
		float shadowMultHeight = (float) getMainMap().getHeight() / 
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getMapHeight();
		std::multimap<unsigned int, LandscapeObjectsEntry*> &objects =
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().
				getObjects().getEntries();
		std::multimap<unsigned int, LandscapeObjectsEntry*>::iterator itor;
		for (itor = objects.begin();
			itor != objects.end();
			itor++)
		{
			LandscapeObjectsEntry *entry = (*itor).second;
			GLBitmapModifier::addCircle(getMainMap(),
				entry->posX * shadowMultWidth, 
				entry->posY * shadowMultHeight, 
				entry->modelsize * entry->size * shadowMultWidth, 1.0f);
		}
	}

	// Create the main landscape texture
	DIALOG_ASSERT(texture_.replace(mainMap_, GL_RGB, false));

    // Create the landscape texture used for the small plan window
	gluScaleImage(
		GL_RGB,
		mainMap_.getWidth(), mainMap_.getHeight(),
		GL_UNSIGNED_BYTE, mainMap_.getBits(),
		bitmapPlan_.getWidth(), bitmapPlan_.getHeight(),
		GL_UNSIGNED_BYTE, bitmapPlan_.getBits());

	// Generate the scorch map for the landscape
	std::string sprayMaskFile = getDataFile("data/textures/smoke01.bmp");
	GLBitmap sprayMaskBitmap(sprayMaskFile.c_str(), sprayMaskFile.c_str(), false);
	scorchMap_.loadFromFile(getDataFile(tex->scorch.c_str()));
	GLBitmap scorchMap(getDataFile(tex->scorch.c_str()));
	scorchMap.resize(sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight());
	GLBitmap texture1New(sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight(), true);
	GLBitmapModifier::makeBitmapTransparent(texture1New, scorchMap, sprayMaskBitmap);
	landTex1_.replace(texture1New, GL_RGBA);

	// Magma
	GLBitmap bitmapMagma(getDataFile(tex->magmasmall.c_str()));
	DIALOG_ASSERT(magTexture_.replace(bitmapMagma));

	// Sky
	std::string ctex(getDataFile(tex->skytexture.c_str()));
	std::string ctexm(getDataFile(tex->skytexturemask.c_str()));
	GLBitmap bitmapCloud(ctex.c_str(), ctexm.c_str(), false);
	DIALOG_ASSERT(cloudTexture_.replace(bitmapCloud, GL_RGBA));
	skyColorsMap_.loadFromFile(getDataFile(tex->skycolormap.c_str()));

	// Detail
	GLBitmap bitmapDetail(getDataFile(tex->detail.c_str()));
	DIALOG_ASSERT(detailTexture_.replace(bitmapDetail, GL_RGB, true));

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
	GLfloat fogDensity = tex->fogdensity;
	glFogf(GL_FOG_DENSITY, fogDensity);	
	
	// Load the sky
	sky_->generate();
	surround_->generate();

	// Add any ambientsounds
	std::list<LandscapeSound *> sounds;
	std::vector<std::string>::iterator soundItor;
	for (soundItor = tex->sounds.begin();
		soundItor != tex->sounds.end();
		soundItor++)
	{
		const char *soundName = (*soundItor).c_str();
		LandscapeSound *sound = 
			ScorchedClient::instance()->getLandscapes().getSound(soundName);
		if (!sound) 
		{
			dialogExit("Landscape",
				"Failed to find sound type %s",
				soundName);
		}
		sounds.push_back(sound);
	}
	soundManager_->initialize(sounds);
}

void Landscape::updatePlanTexture()
{
	if (water_->getWaterOn())
	{
		GLBitmapModifier::addWaterToBitmap(
			ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(), 
			bitmapPlan_, water_->getWaterBitmap(), water_->getWaterHeight());
	}
	DIALOG_ASSERT(planTexture_.replace(bitmapPlan_, GL_RGB, false));
}

void Landscape::drawLand()
{
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
			shadowMap_->setTexture();

			GLStateExtension::glActiveTextureARB()(GL_TEXTURE0_ARB);
		}

		texture_.draw(true);
	}
	else
	{
		textureState = new GLState(GLState::TEXTURE_OFF);
	}
	
	if (OptionsDisplay::instance()->getDrawLandscape())
	{
		if (OptionsDisplay::instance()->getNoROAM())
		{
			HeightMapRenderer::drawHeightMap(
				ScorchedClient::instance()->getLandscapeMaps().
					getGroundMaps().getHeightMap());
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			patchGrid_->draw(PatchSide::typeTop);

			if (OptionsDisplay::instance()->getDrawNormals())
			{
				GLState currentState(GLState::TEXTURE_OFF);
				patchGrid_->draw(PatchSide::typeNormals);
			}
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
}

void Landscape::updatePlanATexture()
{
	GLBitmapModifier::removeWaterFromBitmap(
		ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(), 
		bitmapPlan_, bitmapPlanAlpha_, bitmapPlanAlphaAlpha_, 
		(water_->getWaterOn()?water_->getWaterHeight():-50.0f));
	DIALOG_ASSERT(planAlphaTexture_.replace(bitmapPlanAlpha_, GL_RGBA, false));
	planAlphaTexture_.draw();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
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

