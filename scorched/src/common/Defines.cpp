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
#include <windows.h>

unsigned int ScorchedPort = 27270;
char *ScorchedVersion = "37";
char *ScorchedProtocolVersion = "ag";

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

const char *getHomeFile(const char *file, ...)
{
        static char filename[1024];
        static char buffer[1024];
        va_list ap;
        va_start(ap, file);
        vsprintf(filename, file, ap);
        va_end(ap);

        wxString homeDir = ::wxGetHomeDir();
        const char *homeDirStr = homeDir.c_str();
        if (!::wxDirExists(homeDir)) homeDirStr = getDataFile("");

        sprintf(buffer, "%s/%s", homeDirStr, filename);
	::wxDos2UnixFilename(buffer);
        return buffer;
}

