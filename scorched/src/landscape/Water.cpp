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

#include <landscape/Water.h>
#include <landscape/WaterMapModifier.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeTex.h>
#include <landscape/LandscapeDefinition.h>
#include <common/OptionsDisplay.h>
#include <sound/SoundUtils.h>
#include <client/ScorchedClient.h>
#include <engine/ParticleEmitter.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLStateExtension.h>

Water::Water() :
	height_(25.0f),
	wMap_(64, 8), 
	wMapPoints_(wMap_, 256, 4),
	resetWater_(false), resetWaterTimer_(0.0f), 
	waterOn_(false)
{
}

Water::~Water()
{
}

void Water::draw()
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return;

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, height_);
		wWaves_.draw();
		wMapPoints_.draw();
		wMap_.draw();
	glPopMatrix();
}

void Water::reset()
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return;

	WaterMapModifier::addWaterVisibility(
		height_,
		ScorchedClient::instance()->getLandscapeMaps().getHMap(), 
		wMap_);
	wMap_.reset();
}

void Water::recalculate()
{
	resetWater_ = true;
}

void Water::simulate(float frameTime)
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return;

	if (resetWater_)
	{
		resetWaterTimer_ -= frameTime;
		if (resetWaterTimer_ < 0.0f)
		{
			// Re-calculate the water (what water is visible)
			WaterMapModifier::addWaterVisibility(
				height_,
				ScorchedClient::instance()->getLandscapeMaps().getHMap(), wMap_);

			// Re-calculate the landsacpe on the wind indicator
			resetWater_ = false;
		}
	}
	
	wMap_.simulate(frameTime);
	wWaves_.simulate(frameTime);
}

void Water::generate(ProgressCounter *counter)
{
	LandscapeTex &tex = ScorchedClient::instance()->getLandscapeMaps().getTex(
		ScorchedClient::instance()->getContext());
	if (0 == strcmp(tex.bordertype.c_str(), "none"))
	{
		waterOn_ = false;
	}
	else if (0 == strcmp(tex.bordertype.c_str(), "water"))
	{
		LandscapeTexBorderWater *water = 
			(LandscapeTexBorderWater *) tex.border;

		height_ = water->height;

		waterOn_ = true;
		const char *wave1 = getDataFile(
			water->wavetexture1.c_str());
		GLBitmap waves1Map(wave1, wave1, false);
		const char *wave2 = getDataFile(
			water->wavetexture2.c_str());
		GLBitmap waves2Map(wave2, wave2, false);
		wWaves_.getWavesTexture1().replace(waves1Map, GL_RGBA);
		wWaves_.getWavesTexture2().replace(waves2Map, GL_RGBA);
		bitmapWater_.loadFromFile(getDataFile(water->reflection.c_str()), false);
		GLBitmap bitmapWaterDetail(getDataFile(water->texture.c_str()));
		wMap_.getWaterDetail().replace(bitmapWaterDetail, GL_RGB, true);
		wWaves_.getWavesColor() = water->wavecolor;

		// Generate the water texture for the spray sprite
		std::string sprayMaskFile = getDataFile("data/textures/smoke01.bmp");
		GLBitmap sprayMaskBitmap(sprayMaskFile.c_str(), sprayMaskFile.c_str(), false);
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
		delete wMap_.getWaterTexture();
		if (GLStateExtension::hasCubeMap() &&
			!OptionsDisplay::instance()->getNoGLSphereMap())
		{
			GLTextureCubeMap *waterCubeMap = new GLTextureCubeMap();
			waterCubeMap->create(bitmapWater, GL_RGB, false);
			wMap_.getWaterTexture() = waterCubeMap;
		}
		else 
		{
			GLTexture *waterNormalMap = new GLTexture();
			waterNormalMap->create(bitmapWater, GL_RGB, false);
			wMap_.getWaterTexture() = waterNormalMap;
		}

		// Find the waves around this island
		wWaves_.generateWaves(height_, wMap_, counter);
	}
	else
	{
		dialogExit("Landscape",
			"Failed to find border type %s",
			tex.bordertype.c_str());
	}
}

void Water::addWave(Vector position, float height)
{
	float mult = wMap_.getWidthMult();
	int posX = int((position[0] + 64.0f) / mult);
	int posY = int((position[1] + 64.0f) / mult);

	wMap_.addWave(posX, posY, height);
}

bool Water::explosion(Vector position, float size)
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return false;

	if (position[2] < height_)
	{
		addWave(position, 5.0f);

		CACHE_SOUND(sound, (char *) getDataFile("data/wav/misc/splash.wav"));
		SoundUtils::playAbsoluteSound(VirtualSoundPriority::eAction,
			sound, position);

		ParticleEmitter sprayemitter;
		sprayemitter.setAttributes(
			3.0f, 4.0f, // Life
			0.5f, 1.0f, // Mass
			0.01f, 0.02f, // Friction
			Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), // Velocity
			Vector(0.9f, 0.9f, 0.9f), 0.5f, // StartColor1
			Vector(1.0f, 1.0f, 1.0f), 0.7f, // StartColor2
			Vector(0.9f, 0.9f, 0.9f), 0.0f, // EndColor1
			Vector(1.0f, 1.0f, 1.0f), 0.1f, // EndColor2
			3.0f, 3.0f, 4.0f, 4.0f, // Start Size
			3.0f, 3.0f, 4.0f, 4.0f, // EndSize
			Vector(0.0f, 0.0f, -800.0f), // Gravity
			false,
			true);
		sprayemitter.emitSpray(position, 
			ScorchedClient::instance()->getParticleEngine(),
			size - 2.0f,
			&landTexWater_);
		return true;
	}

	return false;
}
