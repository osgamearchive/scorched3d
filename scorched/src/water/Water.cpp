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

#include <water/Water.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeDefinition.h>
#include <graph/OptionsDisplay.h>
#include <common/Defines.h>
#include <sound/SoundUtils.h>
#include <client/ScorchedClient.h>
#include <graph/ParticleEmitter.h>
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLImageModifier.h>
#include <GLEXT/GLBitmap.h>

Water::Water() :
	height_(25.0f), waterOn_(false)
{
}

Water::~Water()
{
}

void Water::draw()
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return;

	wTex_.draw(wMap_, wMapPoints_);
}

void Water::simulate(float frameTime)
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return;

	wTex_.simulate(frameTime);
}

void Water::generate(ProgressCounter *counter)
{
	LandscapeTex &tex = *ScorchedClient::instance()->getLandscapeMaps().
		getDefinitions().getTex();
	LandscapeDefn &defn = *ScorchedClient::instance()->getLandscapeMaps().
		getDefinitions().getDefn();
	if (tex.border->getType() == LandscapeTexType::eNone)
	{
		waterOn_ = false;
		return;
	}

	if (tex.border->getType() != LandscapeTexType::eWater)
	{
		dialogExit("Landscape", formatString(
			"Failed to find border type %i",
			tex.border->getType()));
	}

	LandscapeTexBorderWater *water = 
		(LandscapeTexBorderWater *) tex.border;

	height_ = water->height;
	waterOn_ = true;

	wMap_.generate(water, counter);
	wTex_.generate(water, counter);
	wMapPoints_.generate(defn.landscapewidth, defn.landscapewidth);

	// Generate the water texture for the spray sprite
	std::string sprayMaskFile = getDataFile("data/textures/smoke01.bmp");
	GLImageHandle sprayMaskBitmap = 
		GLImageFactory::loadImageHandle(
			sprayMaskFile.c_str(), sprayMaskFile.c_str(), false);
	GLImageHandle loadedBitmapWater = 
		GLImageFactory::loadImageHandle(getDataFile(water->reflection.c_str()));
	bitmapWater_ = GLImageFactory::loadImageHandle(getDataFile(water->reflection.c_str()));

	{
		GLImageHandle bitmapWater = loadedBitmapWater.createResize(
			sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight());
		GLBitmap textureWaterNew(
			sprayMaskBitmap.getWidth(), sprayMaskBitmap.getHeight(), true);
		GLImageModifier::makeBitmapTransparent(textureWaterNew, 
			bitmapWater, sprayMaskBitmap);
		landTexWater_.replace(textureWaterNew);
	}
}

bool Water::explosion(Vector position, float size)
{
	if (!waterOn_ ||
		!OptionsDisplay::instance()->getDrawWater()) return false;

	if (position[2] < height_)
	{
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
