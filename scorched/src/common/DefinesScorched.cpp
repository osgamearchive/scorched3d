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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <common/DefinesScorched.h>
#include <common/DefinesFile.h>
#include <common/DefinesString.h>
#include <common/DefinesAssert.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning(disable : 4996)

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

unsigned int ScorchedPort = 27270;
char *ScorchedVersion = "41.1";
char *ScorchedProtocolVersion = "dj";
#ifdef __DATE__
char *ScorchedBuildTime = __DATE__;
#else
char *ScorchedBuildTime = "Unknown";
#endif
static char exeName[1024];
static char *dataModFile = 0;
static std::string settingsDir = ".scorched3d";

void showURL(const char *url)
{
#ifdef _WIN32
	WinExec(formatString("explorer %s", url) ,SW_SHOWDEFAULT);
#else
#ifdef __DARWIN__
	system(formatString("open %s", url));
#else
	system(formatString("firefox %s", url));
	dialogMessage("Web site location", formatString("%s", url));
#endif // __DARWIN__
#endif // _WIN32
}

void setExeName(const char *name)
{
	snprintf(exeName, sizeof(exeName), "%s", name);
}

const char *getExeName()
{
	return exeName;
}

const char *getStartTime()
{
	static std::string startTime;
	if (startTime.empty())
	{
		time_t theTime = time(0);
		startTime = ctime(&theTime);
	}
	return startTime.c_str();
}

void setSettingsDir(const char *dir)
{
	settingsDir = dir;
}

void setDataFileMod(const char *mod)
{
	delete [] dataModFile;
	dataModFile = new char[strlen(mod) + 1];
	strcpy(dataModFile, mod);
}

const char *getDataFileMod()
{
	return (dataModFile?dataModFile:"none");
}

#ifndef S3D_DATADIR
#define S3D_DATADIR "."
#endif
#ifndef S3D_DOCDIR
#define S3D_DOCDIR "./documentation"
#endif
#ifndef S3D_BINDIR
#define S3D_BINDIR "."
#endif

static const char *GET_DIR(char *dir)
{
	if (dir[0] == '.')
	{
		static char path[1024];
#ifdef _WIN32
			GetCurrentDirectory(sizeof(path), path);
#else
			getcwd(path, sizeof(path));
#endif // _WIN32
		if (strlen(path) + strlen(dir) + 1 < sizeof(path))
		{
			strcat(path, "/");
			strcat(path, dir);
		}
		dir = path;

		/*dir = (char *) formatString("%s", getExeName());
		s3d_fileDos2Unix(dir);
		char *ch = strrchr(dir, '/');
		if (ch) *ch = '\0';
		else dir = ".";*/
	}
	return dir;
}

const char *getDataFile(const char *filename)
{
	static char buffer[1024];

	snprintf(buffer, 1024, getModFile(formatString("%s/%s", getDataFileMod(), filename)));
	s3d_fileDos2Unix(buffer);
	if (s3d_fileExists(buffer)) return buffer;

	snprintf(buffer, 1024, getGlobalModFile(formatString("%s/%s", getDataFileMod(), filename)));
	s3d_fileDos2Unix(buffer);
	if (s3d_fileExists(buffer)) return buffer;

	snprintf(buffer, 1024, "%s/%s", GET_DIR(S3D_DATADIR), filename);
	s3d_fileDos2Unix(buffer);

	return buffer;
}

extern bool checkDataFile(const char *filename)
{
	const char *dataFileName = getDataFile(filename);
	if (!s3d_fileExists(dataFileName))
	{
		if (0 == strstr(filename, "none"))
		{
			dialogMessage("Scorched3D", formatString(
				"The file \"%s\" does not exist",
				dataFileName));
			return false;
		}
	}
	return true;
}

const char *getDocFile(const char *filename)
{
	static char buffer[1024];
	snprintf(buffer, 1024, "%s/%s", GET_DIR(S3D_DOCDIR), filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getHomeFile(const char *filename)
{
	static std::string homeDir;
	if (!homeDir.c_str()[0])
	{
		homeDir = GET_DIR(S3D_DATADIR);
		if (s3d_dirExists(s3d_getHomeDir()))
		{
			homeDir = s3d_getHomeDir();
		}
	}

	static char buffer[1024];
	snprintf(buffer, 1024, "%s/%s", homeDir.c_str(), filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getSettingsFile(const char *filename)
{
	static std::string homeDir;
	if (!homeDir.c_str()[0])
	{
		const char *homeDirStr = getHomeFile(formatString("/%s", 
			settingsDir.c_str()));
		if (!s3d_dirExists(homeDirStr))
		{
			if (!s3d_dirMake(homeDirStr))
			{
				homeDirStr = getHomeFile("");
			}
		}
		homeDir = homeDirStr;
	}

	static char buffer[1024];
	snprintf(buffer, 1024, "%s/%s", homeDir.c_str(), filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getLogFile(const char *filename)
{
	static char buffer[1024];

	const char *homeDirStr = getSettingsFile("");
	std::string newDir(std::string(homeDirStr) + std::string("/logs"));
	if (s3d_dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
	else if (s3d_dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

	snprintf(buffer, 1024, "%s/%s", homeDirStr, filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getSaveFile(const char *filename)
{
	static char buffer[1024];
	const char *homeDirStr = getSettingsFile("");
	std::string newDir(std::string(homeDirStr) + std::string("/saves"));
	if (s3d_dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
	else if (s3d_dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

	snprintf(buffer, 1024, "%s/%s", homeDirStr, filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getModFile(const char *filename)
{
	static std::string modDir;
	if (!modDir.c_str()[0])
	{
		const char *homeDirStr = getSettingsFile("");
		std::string newDir(std::string(homeDirStr) + std::string("/mods"));
		if (s3d_dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
		else if (s3d_dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

		modDir = homeDirStr;
	}
	         
	static char buffer[1024];
	snprintf(buffer, 1024, "%s/%s", modDir.c_str(), filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

const char *getGlobalModFile(const char *filename)
{
	static char buffer[1024];
	snprintf(buffer, 1024, "%s/data/globalmods/%s", GET_DIR(S3D_DATADIR), filename);
	s3d_fileDos2Unix(buffer);
	return buffer;
}

