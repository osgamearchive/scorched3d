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

#include <engine/ModDirs.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/FileList.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <stdio.h>

ModDirs::ModDirs()
{
}

ModDirs::~ModDirs()
{
}

bool ModDirs::loadModDirs()
{
	loadModFile("none", true);
	if (!loadModDir(getModFile(""), false)) return false;
	if (!loadModDir(getGlobalModFile(""), true)) return false;
	return true;
}
	
bool ModDirs::loadModDir(const char *dirName, bool global)
{
	wxDir dir(wxString(dirName, wxConvUTF8));
	if (dir.IsOpened())
	{
		wxString filename;
		bool cont = dir.GetFirst(&filename, wxT(""), wxDIR_DIRS);
		while (cont)
		{
			if (!loadModFile(filename.mb_str(wxConvUTF8), global)) return false;
			cont = dir.GetNext(&filename);
		}
	}
	return true;
}

bool ModDirs::loadModFile(const char *fileName, bool global)
{
	if (strstr(fileName, "CVS")) return true;

	std::string oldFileName(fileName);
	_strlwr((char *) fileName);

	if (strcmp(oldFileName.c_str(), fileName) != 0)
	{
		dialogMessage("ModDirs",
			"ERROR: All mod directories must have lower case filenames.\n"
			"Directory \"%s\" has upper case charaters in it",
			fileName);
		return false;
	}
	
	// Check if this mod has already been loaded
	std::list<ModInfo>::iterator itor;
	for (itor = dirs_.begin();
		itor != dirs_.end();
		itor++)
	{
		ModInfo &info = (*itor);
		if (0 == strcmp(info.getName(), fileName)) return true;
	}

	// Check if there is a modinfo file for this mod
	std::string noneGamesFile = getDataFile("data/modinfo.xml");
	setDataFileMod(fileName);
	std::string modGamesFile = getDataFile("data/modinfo.xml");
	setDataFileMod("none");

	bool defaultInfoFile = (0 == strcmp(noneGamesFile.c_str(), modGamesFile.c_str()));
	if (0 != strcmp(fileName, "none") && defaultInfoFile)
	{
		// This mod does not have a mod info, no one can play it
		// or even see it
		return true;
	}

	ModInfo newInfo(fileName);

	setDataFileMod(fileName);
	if (!newInfo.parse(modGamesFile.c_str()))
	{
		dialogExit("Scorched3D",
			"Failed to parse mod info file \"%s\"",
		modGamesFile.c_str());
	}
	setDataFileMod("none");

	if (0 != strcmp(newInfo.getProtocolVersion(), ScorchedProtocolVersion))
	{
		// Check if this is a global mod (one bundled with s3d)
		// If its not we can't play it as it is out of date
		// If it is we remove the custom version in the home dir
		if (!global)
		{
			return true;
		}

		// The version has changed move the current mod directories
		if (!s3d_dirExists(getSettingsFile("/oldmods")))
		{
			s3d_dirMake(getSettingsFile("/oldmods"));
		}

		// The gloabl mod is out of date, move it out the way
		std::string src = getModFile(fileName);
		std::string dest = getSettingsFile("/oldmods/%s-%u", fileName, time(0));
		if (s3d_dirExists(src.c_str()))
		{
			if (::wxRenameFile(wxString(src.c_str(), wxConvUTF8), 
				wxString(dest.c_str(), wxConvUTF8)))
			{
				dialogMessage("Scorched3D",
					"Mod directory\n"
					"\"%s\"\n"
					"was moved to\n"
					"\"%s\"\n"
					"as it may be incompatable with this version of Scorched3D",
					src.c_str(), dest.c_str());
			}
		}
	}

	dirs_.push_back(newInfo);
	return true;
}
