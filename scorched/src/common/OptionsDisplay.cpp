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
#include <common/OptionsParam.h>
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
		"Use detail textures on the landscape."
		"Adds the texture seen when very close to the landscape."
		"Requires multi-texturing", RWAccess, true),
	depricatedUniqueUserId_(options_, "UniqueUserId",
		"The unique string given by this client to any servers to uniquely identify itself."
		"Used for server stats and logging (confidentially)", RAccess | OptionEntry::DataDepricated | NoRestore, ""),
	hostDescription_(options_, "HostDescription",
		"The description of this host given to any servers for stats.", RAccess | NoRestore, ""),
	onlineUserName_(options_, "OnlineUserName",
		"The player name that will be used for all online games.", RAccess | NoRestore, "Player"),
	onlineTankModel_(options_, "OnlineTankModel",
		"The tank model that will be used for all online games.", RAccess | NoRestore, ""),
	explosionParts_(options_, "ExplosionParts",
		"How many explosion clouds are drawn", RAccess, 8, 0, 10),
	explosionSubParts_(options_, "ExplosionSubParts",
		"How many explosion sub clouds are drawn", RAccess, 4, 0, 5),
	dialogSize_(options_, "DialogSize",
		"How large the on screen dialogs and menus are.", RAccess, 3, 0, 4),
	tankModelSize_(options_, "TankModelSize",
		"The percentage size of the tank models", RWAccess, 100),
	maxModelTriPercentage_(options_, "MaxModelTriPercentage",
		"The maximum percentage of triangles each model will use.", RAccess, 100, 50, 100),
	explosionParticleMult_(options_, "ExplosionParticleMult",
		"The number of particles that each explosion will create (relative to explosion size)", RAccess, 20, 0, 100),
	depricatedDayTime_(options_, "DayTime",
		"The Day Time", RWAccess | OptionEntry::DataDepricated, 2),
	depricatedSunXYAng_(options_, "SunXYAng",
		"The XY Rotation of the sun (degrees)", RAccess | OptionEntry::DataDepricated, 110),
	depricatedSunYZAng_(options_, "SunYZAng",
		"The YZ Rotation of the sun (degrees)", RAccess | OptionEntry::DataDepricated, 25),
	brightness_(options_, "Brightness", 
		"The game screen brightness (gamma).", RAccess, 10, 3, 40),
	fullScreen_(options_, "FullScreen", 
		"Run the game in a full screen mode.  The default is a windowed mode.", RAccess, false),
	moreRes_(options_, "MoreRes",
		"Show more screen resolutions in the drop down.  By default only hardware supported modes are shown.", RAccess, false),
	fullClear_(options_, "FullClear", 
		"Completely clear the screen before drawing each frame", RWAccess, false),
	invertElevation_(options_, "InvertUpDownKeys",
		"Invert/reverse the tanks up/down elevation directions.", RWAccess, false),
	invertMouse_(options_, "InvertMouse",
		"Invert/reverse the mouses y-axis when rotating camera.", RWAccess, false),
	swapYAxis_(options_, "SwapYAxis",
		"Remaps mouse pointer from top of window to the bottom and vice-versa (MAC/OSX)", RWAccess, false),
	sideScroll_(options_, "SideScroll",
		"Allows the user to scroll the viewport using the mouse", RWAccess, true),
	drawPlayerNames_(options_, "DrawPlayerNames",
		"Draw the names above the tanks", RWAccess, true),
	drawPlayerSight_(options_, "DrawPlayerSight",
		"Draw the aiming sight infront of the tanks", RWAccess, true),
	drawPlayerColor_(options_, "DrawPlayerColor",
		"Draw the player color triangle over the tank", RWAccess, true),
	drawPlayerHealth_(options_, "DrawPlayerHealth",
		"Draw the health bars above the tank", RWAccess, true),
	noGLTexSubImage_(options_, "NoGLTexSubImage",
		"Do not use texture area replacing.  Required to be able to scorch the ground.", RAccess, false),
	noGLExt_(options_, "NoGLExt", 
		"Do not use any OpenGL extensions.  Turn off to disable any extra OpenGL features.", RAccess, false),
	noGLMultiTex_(options_, "NoGLMultiTex", 
		"Only use one texture for all models and the landscape.", RAccess, false),
	noGLCompiledArrays_(options_, "NoGLCompiledArrays",
		"Do not compile vertex arrays.", RAccess, false),
	noGLEnvCombine_(options_, "NoGLEnvCombine",
		"Only use one texture on the landscape.", RAccess, false),
	noGLCubeMap_(options_, "NoGLCubeMap",
		"Draw the water without using cubemap relfections.", RAccess, false),
	noGLSphereMap_(options_, "NoGLSphereMap",
		"Draw the water without using sphere map relfections.", RAccess, false),
	noGLHardwareMipmaps_(options_, "NoGLHardwareMipmaps",
		"Generate texture LOD in software only.", RAccess, false),
	soundChannels_(options_, "SoundChannels", 
		"Number of sound channels to mix.", RAccess, 8),
	noSound_(options_, "NoSound", 
		"Do not use any sound.", RWAccess, false),
	noTrees_(options_, "NoTrees",
		"Do not draw any trees", RWAccess, false),
	lowTreeDetail_(options_, "LowTreeDetail",
		"Only use low detail trees.  Faster.", RWAccess, false),
	noWaves_(options_, "NoWaves",
		"Do not draw the moving shore waves.", RWAccess, false),
	noDepthSorting_(options_, "NoDepthSorting",
		"Do not depth sort sprites.", RWAccess, false),
	drawLines_(options_, "DrawLines", 
		"Do not fill in the landscape", RWAccess, false),
	drawNormals_(options_, "DrawNormals", 
		"Show landscape normals on the landscape", RWAccess, false),
	frameTimer_(options_, "FrameTimer",
		"Show the current number of frames per second (FPS)", RWAccess, false),
	noSkins_(options_,"NoTankSkins", 
		"Do not texture the tank models.", RAccess, false),
	drawWater_(options_,"DrawWater", 
		"Draw the water", RWAccess, true),
	drawLandscape_(options_, "DrawLandscape",
		"Draw the landscape", RWAccess, true),
	drawSurround_(options_, "DrawSurround",
		"Draw the surround", RWAccess, true),
	noWaterMovement_(options_, "NoWaterMovement",
		"Draw very simple flat water with no shading", RWAccess, false),
	noSkyLayers_(options_, "NoSkyLayers",
		"Only draw one sky layer.", RWAccess, false),
	noSkyMovement_(options_, "NoSkyMovement",
		"Do not animate the sky", RWAccess, false),
	noROAM_(options_,"NoROAM", 
		"Do not use ROAM algorithm", RWAccess, false),
	noPrecipitation_(options_, "NoPrecipitation",
		"Do not draw precipitation", RWAccess, false),
	noTessalation_(options_,"NoTessalation", 
		"Do not use ROAM tessalation algorithm", RWAccess, false),
	noVBO_(options_,"NoVBO", 
		"Do not use Vertex Buffer Objects (if avaialable)", RWAccess, true),
	useLandscapeTexture_(options_, "LandscapeTexture",
		"Texture the landscape", RWAccess, true),
	noLenseFlare_(options_,"NoLenseFlare", 
		"Do not show the full lense flare effect", RWAccess, true),
	softwareMouse_(options_,"SoftwareMouse", 
		"Use a software mouse pointer.  Useful if mouse clicks are not aligned.", RWAccess, false),
	useHex_(options_,"UseHexidecimal", 
		"Show the tank angles and amounts in hex", RWAccess, false),
	soundVolume_(options_, "SoundVolume",
		"The sound effect volume. Max = 128, Min = 0", RAccess, 128, 0, 128),
	texSize_(options_, "TexureSize", 
		"The texture details setting.  Lower is faster.", RAccess, 1, 0, 2),
	bannerRows_(options_, "BannerRows",
		"The number of log rows shown at the bottom of the screen", RAccess, 5),
	tankDetail_(options_, "TankDetail", 
		"The tank details setting.  Lower is faster but shows less tank models.", RAccess, 2, 0, 2),
	effectsDetail_(options_, "EffectsDetail",
		"The number of effects allowed at once.",  RAccess, 1, 0, 2),
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
		"Show in game help bubbles (tooltips) for items that have it", RWAccess, true),
	sortAccessories_(options_, "SortAccessories",
		"Sort accessories alphabetically by name before displaying", RWAccess, false)
{
}

OptionsDisplay::~OptionsDisplay()
{	
}

bool OptionsDisplay::writeOptionsToFile()
{
	if (OptionsParam::instance()->getScreenSaverMode()) return true;

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
	if (OptionsParam::instance()->getScreenSaverMode())
	{
		path = getDataFile("data/ssdisplay.xml");
	}

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
		if (!(entry->getData() & OptionEntry::DataDepricated))
		{
			GLConsoleRuleAccessType access = GLConsoleRuleAccessTypeRead;
			if (entry->getData() & RWAccess) access = GLConsoleRuleAccessTypeReadWrite;

			adapters_.push_back(new GLConsoleRuleFnIOptionsAdapter(
				*entry,
				access));
		}
	}
}

