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

unsigned int ScorchedPort = 27270;
char *ScorchedVersion = "38";
char *ScorchedProtocolVersion = "ba";

extern bool wxWindowInit;

void dialogMessage(const char *header, const char *fmt, ...)
{
	char text[2048];
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
	char buffer[1024];
	sprintf(buffer, "%s\n%i:%s", lineText, line, file);
	dialogMessage("Program Assert", buffer);
	exit(1);
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
	sprintf(buffer, S3D_DATADIR "/%s", filename);
	::wxDos2UnixFilename(buffer);
	return buffer;
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

	const char *homeDirStr = getDataFile("");
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

