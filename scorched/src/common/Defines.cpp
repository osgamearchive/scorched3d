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

unsigned int ScorchedPort = 27270;
char *ScorchedVersion = "38";
char *ScorchedProtocolVersion = "bb";
static char *dataModFile = 0;

extern bool wxWindowInit;

bool fileExists(const char *file)
{
	return ::wxFileExists(file);
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
		wxString wxText(text);
		wxString wxHeader(header);
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

void setDataFileMod(const char *mod)
{
	delete [] dataModFile;
	dataModFile = new char[strlen(mod) + 1];
	strcpy(dataModFile, mod);
}

const char *getDataFileMod()
{
	return (dataModFile?dataModFile:"");
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

	if (dataModFile && dataModFile[0])
	{
		sprintf(buffer, getModFile("%s/%s", dataModFile, filename));
		::wxDos2UnixFilename(buffer);
		if (::wxFileExists(buffer)) return buffer;

		sprintf(buffer, getGlobalModFile("%s/%s", dataModFile, filename));
		::wxDos2UnixFilename(buffer);
		if (::wxFileExists(buffer)) return buffer;
	}

	sprintf(buffer, S3D_DATADIR "/%s", filename);
	::wxDos2UnixFilename(buffer);

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
	if (!::fileExists(dataFileName))
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
	::wxDos2UnixFilename(buffer);
	return buffer;
}

const char *getHomeFile(const char *file, ...)
{
	static char filename[1024];
	static char buffer[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	const char *homeDirStr = S3D_DATADIR;
	wxString homeDir = ::wxGetHomeDir();
	if (::wxDirExists(homeDir)) homeDirStr = homeDir.c_str();

	sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
	return buffer;
}

const char *getSettingsFile(const char *file ...)
{
	static char filename[1024];
	static char buffer[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);
	                                                                                                    
	const char *homeDirStr = getHomeFile("");
	wxString newDir(wxString(homeDirStr) + wxString("/.scorched3d"));
	if (::wxDirExists(newDir)) homeDirStr = newDir.c_str();
	else if (::wxMkdir(newDir, 0755)) homeDirStr = newDir.c_str();

	sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
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
	wxString newDir(wxString(homeDirStr) + wxString("/logs"));
	if (::wxDirExists(newDir)) homeDirStr = newDir.c_str();
	else if (::wxMkdir(newDir, 0755)) homeDirStr = newDir.c_str();
	                                                                                                    
	sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
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
	wxString newDir(wxString(homeDirStr) + wxString("/saves"));
	if (::wxDirExists(newDir)) homeDirStr = newDir.c_str();
	else if (::wxMkdir(newDir, 0755)) homeDirStr = newDir.c_str();
	                                                                                                    
	sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
	return buffer;
}

const char *getModFile(const char *file ...)
{
	static char filename[1024];
	va_list ap;
	va_start(ap, file);
	vsprintf(filename, file, ap);
	va_end(ap);

	static char buffer[1024];
	const char *homeDirStr = getSettingsFile("");
	wxString newDir(wxString(homeDirStr) + wxString("/mods"));
	if (::wxDirExists(newDir)) homeDirStr = newDir.c_str();
	else if (::wxMkdir(newDir, 0755)) homeDirStr = newDir.c_str();
	                                                                                                    
	sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
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
	::wxDos2UnixFilename(buffer);
	return buffer;
}
