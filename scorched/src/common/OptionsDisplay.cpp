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

#include <wx/wx.h>
#include <wx/utils.h>
#include <common/OptionsDisplay.h>
#include <GLEXT/GLConsoleRuleFnIAdapter.h>
#include <stdio.h>

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
	detailTexture_(options_, "DetailTexture",
		"Use the detail texture on the landscape", RWAccess, true),
	uniqueUserId_(options_, "UniqueUserId",
		"The unique string given to any servers to uniquely identify the client.", RAccess | NoRestore, ""),
	hostDescription_(options_, "HostDescription",
		"The description of this host given to any servers for stats.", RAccess | NoRestore, ""),
	onlineUserName_(options_, "OnlineUserName",
		"The player name that will be used for all online games.", RAccess | NoRestore, "Player"),
	explosionParts_(options_, "ExplosionParts",
		"How many explosion clouds are drawn", RAccess, 8, 0, 10),
	explosionSubParts_(options_, "ExplosionSubParts",
		"How many explosion sub clouds are drawn", RAccess, 4, 0, 5),
	dialogSize_(options_, "DialogSize",
		"How large the on screen dialogs and menus are", RAccess, 3, 0, 4),
	maxModelTriPercentage_(options_, "MaxModelTriPercentage",
		"The maximum percentage of triangles each model will use", RAccess, 100, 50, 100),
	explosionParticleMult_(options_, "ExplosionParticleMult",
		"The number of particles that each explosion will create (relative to explosion size)", RAccess, 20, 0, 100),
	sunXYAng_(options_, "SunXYAng",
		"The XY Rotation of the sun (degrees)", RAccess, 110),
	sunYZAng_(options_, "SunYZAng",
		"The YZ Rotation of the sun (degrees)", RAccess, 75),
	brightness_(options_, "Brightness", 
		"The screen brightness", RAccess, 10, 3, 40),
	fullScreen_(options_, "FullScreen", 
		"Whether to use full screen mode or not", RAccess, false),
	moreRes_(options_, "MoreRes",
		"Show more screen resolutions in the drop down", RAccess, false),
	fullClear_(options_, "FullClear", 
		"Completely clear the screen on each frame", RWAccess, false),
	invertElevation_(options_, "InvertUpDownKeys",
		"Invert the elevation directions", RWAccess, false),
	invertMouse_(options_, "InvertMouse",
		"Invert the mouse vertical directions", RWAccess, false),
	drawPlayerNames_(options_, "DrawPlayerNames",
		"Draw the names above the tanks", RWAccess, true),
	drawPlayerSight_(options_, "DrawPlayerSight",
		"Draw the aiming sight infront of the tanks", RWAccess, true),
	drawPlayerColor_(options_, "DrawPlayerColor",
		"Draw the player color triangle over the tank", RWAccess, true),
	drawPlayerHealth_(options_, "DrawPlayerHealth",
		"Draw the health bars above the tank", RWAccess, true),
	noGLTexSubImage_(options_, "NoGLTexSubImage",
		"Do not use texture area replacing", RAccess, false),
	noGLExt_(options_, "NoGLExt", 
		"Do not use any OpenGL extensions", RAccess, false),
	noGLMultiTex_(options_, "NoGLMultiTex", 
		"Do not use more than one texture per face", RAccess, false),
	noGLCompiledArrays_(options_, "NoGLCompiledArrays",
		"Do not compile vertex arrays", RAccess, false),
	noGLEnvCombine_(options_, "NoGLEnvCombine",
		"Do not use combined textures", RAccess, false),
	noGLCubeMap_(options_, "NoGLCubeMap",
		"Do not use cube maps", RAccess, false),
	noGLHardwareMipmaps_(options_, "NoGLHardwareMipmaps",
		"Do not use hardware mipmaps", RAccess, false),
	noSound_(options_, "NoSound", 
		"Do not use the sound.", RWAccess, false),
	noTrees_(options_, "NoTrees",
		"Do not draw the trees", RWAccess, false),
	lowTreeDetail_(options_, "LowTreeDetail",
		"Only use low detail trees", RWAccess, false),
	noWaves_(options_, "NoWaves",
		"Do not draw the shore waves", RWAccess, false),
	noDepthSorting_(options_, "NoDepthSorting",
		"Do not sort sprites, can cause graphic artifacts", RWAccess, false),
	drawLines_(options_, "DrawLines", 
		"Do not fill in the landscape", RWAccess, false),
	drawNormals_(options_, "DrawNormals", 
		"Show landscape normals on the landscape", RWAccess, false),
	frameTimer_(options_, "FrameTimer",
		"Show the current number of frames per second (FPS)", RWAccess, false),
	noSkins_(options_,"NoTankSkins", 
		"Do not texture tanks", RAccess, false),
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
	bannerRows_(options_, "BannerRows",
		"The number of log rows shown at the bottom of the screen", RAccess, 5),
	tankDetail_(options_, "TankDetail", 
		"The tank details setting", RAccess, 2, 0, 2),
	effectsDetail_(options_, "EffectsDetail",
		"The number of allowed on screen effects",  RAccess, 1, 0, 2),
	screenWidth_(options_, "ScreenWidth", 
		"The window width to use (in pixels)", RAccess, 640),
	screenHeight_(options_, "ScreenHeight",
		"The window height to use (in pixels)", RAccess, 480),
	depthBufferBits_(options_, "DepthBufferBits",
		"The number of bits requested for the depth buffer", RAccess, 24),
	doubleBuffer_(options_, "DoubleBuffer",
		"Use double buffering", RAccess, true),
	colorComponentSize_(options_, "ColorComponentSize",
		"The number of bits to use for each of the RGBA components (0 = use default)", RAccess, 0),
	bitsPerPixel_(options_, "BitsPerPixel",
		"The number of bits per pixel to use for the display (0 = current display bbp)", RAccess, 0),
	showContextHelp_(options_, "ShowContextHelp",
		"Show help bubbles for items that have it", RWAccess, true)
{

}

OptionsDisplay::~OptionsDisplay()
{
	
}

bool OptionsDisplay::writeOptionsToFile()
{
	const char *path = getSettingsFile("display.xml");

	// Check the options files are writeable
	FILE *checkfile = fopen(path, "a");
	if (!checkfile)
	{
		dialogMessage(
			"Scorched3D",
			"Warning: Your display settings file (%s) cannot be\n"
			"written to.  Your settings will not be saved from one game to the next.\n\n"
			"To fix this problem correct the permissions for this file.",
			path);
	}
	else fclose(checkfile);

	if (!OptionEntryHelper::writeToFile(options_, (char *) path)) return false;
	return true;
}

bool OptionsDisplay::readOptionsFromFile()
{
	const char *path = getSettingsFile("display.xml");

	if (!OptionEntryHelper::readFromFile(options_, (char *) path))
	{
		dialogMessage(
			"Scorched3D",
			"Warning: Your display settings file (%s) cannot be\n"
			"read.  This may be because it was create by an out of date version of Scorched3D.\n"
			"If this is the case it can be safely deleted.",
			path);
		return false;
	}

	return true;
}

void OptionsDisplay::loadDefaultValues()
{
        std::list<OptionEntry *>::iterator itor;
        for (itor = options_.begin();
                itor != options_.end();
                itor++)
	{
                OptionEntry *entry = (*itor);
		if (!(entry->getData() & NoRestore))
		{
			entry->setValueFromString(entry->getDefaultValueAsString());
		}
	}
}

void OptionsDisplay::loadSafeValues()
{
	const char *path = getDataFile("data/safedisplay.xml");
	OptionEntryHelper::readFromFile(options_, (char *) path);
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

		GLConsoleRuleAccessType access = GLConsoleRuleAccessTypeRead;
		if (entry->getData() & RWAccess) access = GLConsoleRuleAccessTypeReadWrite;

		adapters_.push_back(new GLConsoleRuleFnIOptionsAdapter(
			*entry,
			access));
	}
}

