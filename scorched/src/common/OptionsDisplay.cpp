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


#include <common/OptionsDisplay.h>
#include <GLEXT/GLConsoleRuleFnIAdapter.h>

OptionsDisplay *OptionsDisplay::instance_ = 0;

OptionsDisplay *OptionsDisplay::instance()
{
	if (!instance_)
	{
		instance_ = new OptionsDisplay;
	}

	return instance_;
}

OptionsDisplay::OptionsDisplay() :
	uniqueUserId_(options_, "UniqueUserId",
		"The unique string given to any servers to uniquely identify the client.", RAccess, ""),
	explosionParts_(options_, "ExplosionParts",
		"How many explosion clouds are drawn", RAccess, 8, 0, 10),
	explosionSubParts_(options_, "ExplosionSubParts",
		"How many explosion sub clouds are drawn", RAccess, 4, 0, 5),
	maxModelTriPercentage_(options_, "MaxModelTriPercentage",
		"The maximum percentage of triangles each model will use", RAccess, 100, 50, 100),
	explosionParticleMult_(options_, "ExplosionParticleMult",
		"The number of particles that each explosion will create (relative to explosion size)", RAccess, 20, 0, 100),
	waterHeight_(options_, "WaterHeight",
		"The height of the sea water level", RAccess, 5),
	sunXYAng_(options_, "SunXYAng",
		"The XY Rotation of the sun (degrees)", RAccess, 110),
	sunYZAng_(options_, "SunYZAng",
		"The YZ Rotation of the sun (degrees)", RAccess, 75),
	brightness_(options_, "Brightness", 
		"The screen brightness", RAccess, 10, 3, 40),
	fullScreen_(options_, "FullScreen", 
		"Whether to use full screen mode or not", RAccess, false),
	fullClear_(options_, "FullClear", 
		"Completely clear the screen on each frame", RWAccess, false),
	invertUpDownKey_(options_, "InvertUpDownKeys",
		"Invert the up and down cursor key directions", RWAccess, false),
	drawPlayerNames_(options_, "DrawPlayerNames",
		"Draw the names above the tanks", RWAccess, true),
	drawPlayerSight_(options_, "DrawPlayerSight",
		"Draw the aiming sight infront of the tanks", RWAccess, true),
	drawPlayerColor_(options_, "DrawPlayerColor",
		"Draw the player color triangle over the tank", RWAccess, true),
	drawPlayerHealth_(options_, "DrawPlayerHealth",
		"Draw the health bars above the tank", RWAccess, true),
	noExt_(options_, "NoGLExt", 
		"Do not use any OpenGL extensions", RWAccess, false),
	noSound_(options_, "NoSound", 
		"Do not use the sound.", RWAccess, false),
	noMultiTex_(options_, "NoMultiTex", 
		"Do not use more than one texture per face", RWAccess, false),
	drawLines_(options_, "DrawLines", 
		"Do not fill in the landscape", RWAccess, false),
	drawNormals_(options_, "DrawNormals", 
		"Show landscape normals on the landscape", RWAccess, false),
	frameTimer_(options_, "FrameTimer",
		"Show the current number of frames per second (FPS)", RWAccess, false),
	noSkins_(options_,"NoTankSkins", 
		"Do not texture tanks", RWAccess, false),
	drawWater_(options_,"DrawWater", 
		"Do not draw water", RWAccess, true),
	noWaterMovement_(options_, "NoWaterMovement",
		"Do not animate the water", RWAccess, false),
	noSkyLayers_(options_, "NoSkyLayers",
		"Do not use multiple sky layers (just 1)", RWAccess, false),
	noSkyMovement_(options_, "NoSkyMovement",
		"Do not animate the sky", RWAccess, false),
	noROAM_(options_,"NoROAM", 
		"Do not use ROAM algorithm", RWAccess, false),
	useLandscapeTexture_(options_, "LandscapeTexture",
		"Texture the landscape", RWAccess, true),
	noLenseFlare_(options_,"NoLenseFlare", 
		"Do not show the full lense flare effect", RWAccess, true),
	useHex_(options_,"UseHexidecimal", 
		"Show the tank angles and amounts in hex", RWAccess, false),
	texSize_(options_, "TexureSize", 
		"The texture details setting", RAccess, 1, 0, 2),
	screenWidth_(options_, "ScreenWidth", 
		"The window width to use (in pixels)", RAccess, 640),
	screenHeight_(options_, "ScreenHeight",
		"The window height to use (in pixels)", RAccess, 480),
	numberSprites_(options_, "NumberSprites",
		"The number of sprites to allow on screen", RWAccess, 40),
	numberBilboards_(options_, "NumberBilboards",
		"The number of bilboards to allow on screen", RWAccess, 2000),
	depthBufferBits_(options_, "DepthBufferBits",
		"The number of bits requested for the depth buffer", RAccess, 24),
	showContextHelp_(options_, "ShowContextHelp",
		"Show help bubbles for items that have it", RWAccess, true)
{

}

OptionsDisplay::~OptionsDisplay()
{
	
}

bool OptionsDisplay::writeOptionsToFile(char *filePath)
{
	if (!OptionEntryHelper::writeToFile(options_, filePath)) return false;
	return true;
}

bool OptionsDisplay::readOptionsFromFile(char *filePath)
{
	if (!OptionEntryHelper::readFromFile(options_, filePath)) return false;
	return true;
}

void OptionsDisplay::addToConsole()
{
	static std::list<GLConsoleRuleFnIOptionsAdapter *> adapters_;

	std::list<OptionEntry *>::iterator itor;
	for (itor = options_.begin();
		itor != options_.end();
		itor++)
	{
		OptionEntry *entry = (*itor);

		GLConsoleRuleAccessType access = GLConsoleRuleAccessTypeReadWrite;
		if (entry->getData() == RAccess) access = GLConsoleRuleAccessTypeRead;

		adapters_.push_back(new GLConsoleRuleFnIOptionsAdapter(
			*entry,
			access));
	}
}
