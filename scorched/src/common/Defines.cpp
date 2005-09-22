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

#include <stdio.h>
#include <stdlib.h>
#include <wx/wx.h>
#include <wx/utils.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <common/Defines.h>
#include <map>
#include <string>

unsigned int ScorchedPort = 27270;
char *ScorchedVersion = "39.1";
char *ScorchedProtocolVersion = "bf";
static char *dataModFile = 0;
static std::string settingsDir = ".scorched3d";

extern bool wxWindowInit;

void DefinesUtil::setSettingsDir(const char *dir)
{
	settingsDir = dir;
}

void DefinesUtil::fileDos2Unix(char *file)
{
    for (char *f=file; *f; f++)
    {
        if (*f == '\\') *f = '/';
    }
}

bool DefinesUtil::dirMake(const char *file)
{
    ::wxMkDir(file, 0755);
}

bool DefinesUtil::fileExists(const char *file)
{
    return ::wxFileExists(wxString(file, wxConvUTF8));
}

bool DefinesUtil::dirExists(const char *file)
{
    bool result = ::wxDirExists(wxString(file, wxConvUTF8));
    return result;
}

void dialogMessage(const char *header, const char *fmt, ...)
{
	char text[20048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

#ifdef _WIN32
	MessageBox(NULL, text, header, MB_OK);
#else
	if (wxWindowInit)
	{
		wxString wxText(text, wxConvUTF8);
		wxString wxHeader(header, wxConvUTF8);
		::wxMessageBox(wxText, wxHeader, wxOK | wxCENTRE);
	}
	else
	{
		printf("%s : %s\n", header, text);
	}
#endif
}

void dialogAssert(const char *lineText, const int line, const char *file)
{
	// Dont use formatString here as this method is called by formatString.
	char buffer[20048];
	sprintf(buffer, "%s\n%i:%s", lineText, line, file);
	dialogMessage("Program Assert", buffer);
	exit(64);
}

const char *formatStringList(const char *format, va_list ap)
{
	static char buffer[20048];
	buffer[sizeof(buffer) - 1] = '\0';
	vsprintf(buffer, format, ap);
	if (buffer[sizeof(buffer) - 1] != 0) 
		dialogAssert("buffer[sizeof(buffer) - 1] == 0", __LINE__, __FILE__);

	return buffer;
}

const char *formatString(const char *file, ...)
{
	va_list ap; 
	va_start(ap, file); 
	const char *result = formatStringList(file, ap);
	va_end(ap); 

	return result;
}

void dialogExit(const char *header, const char *file, ...)
{
	va_list ap; 
	va_start(ap, file); 
	const char *result = formatStringList(file, ap);
	va_end(ap); 

	dialogMessage(header, result);
	exit(64);
}

void DefinesUtil::setDataFileMod(const char *mod)
{
	delete [] dataModFile;
	dataModFile = new char[strlen(mod) + 1];
	strcpy(dataModFile, mod);
}

const char *DefinesUtil::getDataFileMod()
{
	return (dataModFile?dataModFile:"none");
}

#ifndef S3D_DATADIR
#define S3D_DATADIR "."
#endif
#ifndef S3D_DOCDIR
#define S3D_DOCDIR "."
#endif
#ifndef S3D_BINDIR
#define S3D_BINDIR "."
#endif

const char *getDataFile(const char *file, ...)
{
	static char filename[1024];
	static char buffer[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	sprintf(buffer, getModFile("%s/%s", DefinesUtil::getDataFileMod(), filename));
	DefinesUtil::fileDos2Unix(buffer);
	if (DefinesUtil::fileExists(buffer)) return buffer;

	sprintf(buffer, getGlobalModFile("%s/%s", DefinesUtil::getDataFileMod(), filename));
	DefinesUtil::fileDos2Unix(buffer);
	if (DefinesUtil::fileExists(buffer)) return buffer;

	sprintf(buffer, S3D_DATADIR "/%s", filename);
	DefinesUtil::fileDos2Unix(buffer);

	return buffer;
}

extern bool checkDataFile(const char *file, ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	const char *dataFileName = getDataFile(filename);
	if (!DefinesUtil::fileExists(dataFileName))
	{
		if (0 == strstr(filename, "none"))
		{
			dialogMessage("Scorched3D",
				"The file \"%s\" does not exist",
				dataFileName);
			return false;
		}
	}
	return true;
}

const char *getDocFile(const char *file, ...)
{
	static char filename[1024];
	static char buffer[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);
	sprintf(buffer, S3D_DOCDIR "/%s", filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getHomeFile(const char *file, ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static std::string homeDir;
	if (!homeDir.c_str()[0])
	{
		homeDir = S3D_DATADIR;
		if (::wxDirExists(::wxGetHomeDir()))
		{
			homeDir = (const char *) wxString(::wxGetHomeDir()).mb_str(wxConvUTF8);
		}
	}

	static char buffer[1024];
	sprintf(buffer, "%s/%s", homeDir.c_str(), filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getSettingsFile(const char *file ...)
{
	static char filename[1024];
	
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static std::string homeDir;
	if (!homeDir.c_str()[0])
	{
		const char *homeDirStr = getHomeFile("/%s", 
			settingsDir.c_str());
		if (!DefinesUtil::dirExists(homeDirStr))
		{
			if (!DefinesUtil::dirMake(homeDirStr))
			{
				homeDirStr = getHomeFile("");
			}
		}
		homeDir = homeDirStr;
	}

	static char buffer[1024];
	sprintf(buffer, "%s/%s", homeDir.c_str(), filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getLogFile(const char *file ...)
{
	static char filename[1024];
	static char buffer[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	const char *homeDirStr = getSettingsFile("");
	std::string newDir(std::string(homeDirStr) + std::string("/logs"));
	if (DefinesUtil::dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
	else if (DefinesUtil::dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

	sprintf(buffer, "%s/%s", homeDirStr, filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getSaveFile(const char *file ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static char buffer[1024];
	const char *homeDirStr = getSettingsFile("");
	std::string newDir(std::string(homeDirStr) + std::string("/saves"));
	if (DefinesUtil::dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
	else if (DefinesUtil::dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

	sprintf(buffer, "%s/%s", homeDirStr, filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getModFile(const char *file ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static std::string modDir;
	if (!modDir.c_str()[0])
	{
		const char *homeDirStr = getSettingsFile("");
		std::string newDir(std::string(homeDirStr) + std::string("/mods"));
		if (DefinesUtil::dirExists(newDir.c_str())) homeDirStr = newDir.c_str();
		else if (DefinesUtil::dirMake(newDir.c_str())) homeDirStr = newDir.c_str();

		modDir = homeDirStr;
	}
	         
	static char buffer[1024];
	sprintf(buffer, "%s/%s", modDir.c_str(), filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

const char *getGlobalModFile(const char *file, ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static char buffer[1024];
	sprintf(buffer, S3D_DATADIR "/data/globalmods/%s", filename);
	DefinesUtil::fileDos2Unix(buffer);
	return buffer;
}

#include <math.h>
static float fastSin[628];
static float fastCos[628];
static bool calculatedFast = false;

static void calculateFast()
{
	calculatedFast = true;
	for (int i=0; i<628; i++)
	{
		float a = float(i) / 100.0f;
		fastSin[i] = (float) sin(a);
		fastCos[i] = (float) cos(a);
	}
}

float DefinesUtil::getFastSin(float angle)
{
	if (!calculatedFast) calculateFast();
	if (angle < 0.0f)
	{
		return -fastSin[(int(angle * -100)) % 628];
	}
	return fastSin[(int(angle * 100)) % 628];
}

float DefinesUtil::getFastCos(float angle)
{
	if (!calculatedFast) calculateFast();
	if (angle < 0.0f)
	{
		return fastCos[(int(angle * -100)) % 628];
	}
	return fastCos[(int(angle * 100)) % 628];
}

char *DefinesUtil::my_stristr(const char *x, const char *y)
{
	std::string newX(x);
	std::string newY(y);
	_strlwr((char *) newX.c_str());
	_strlwr((char *) newY.c_str());

	char *result = strstr(newX.c_str(), newY.c_str());
	if (!result) return 0;

	return (char *)(x + (result - newX.c_str()));
}

