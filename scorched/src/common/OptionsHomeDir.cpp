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

#include <common/OptionsHomeDir.h>

OptionsHomeDir *OptionsHomeDir::instance_ = 0;

OptionsHomeDir *OptionsHomeDir::instance()
{
	if (!instance_) instance_ = new OptionsHomeDir;
	return instance_;
}

OptionsHomeDir::OptionsHomeDir() :
	directoryVersion_(options_, "DirectoryVersion",
		"The Scorched Protocol version of the settings directory", 0, "")
{
}

OptionsHomeDir::~OptionsHomeDir()
{
}

bool OptionsHomeDir::writeOptionsToFile()
{
	const char *path = getSettingsFile("homedir.xml");
	if (!OptionEntryHelper::writeToFile(options_, (char *) path)) return false;
	return true;
}

bool OptionsHomeDir::readOptionsFromFile()
{
	const char *path = getSettingsFile("homedir.xml");
	if (!OptionEntryHelper::readFromFile(options_, (char *) path)) return false;
	return true;
}
