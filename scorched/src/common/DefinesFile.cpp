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
#include <string>
#include <wx/wx.h>
#include <wx/utils.h>
#include <common/DefinesFile.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

char *s3d_getHomeDir()
{
	static std::string result;
	result = (const char *) wxString(::wxGetHomeDir()).mb_str(wxConvUTF8);
	return (char *) result.c_str();
}

void s3d_fileDos2Unix(char *file)
{
    for (char *f=file; *f; f++)
    {
        if (*f == '\\') *f = '/';
    }
}

bool s3d_dirMake(const char *file)
{
    ::wxMkdir(wxString(file, wxConvUTF8), 0755);
	return true;
}

bool s3d_fileExists(const char *file)
{
    return ::wxFileExists(wxString(file, wxConvUTF8));
}

bool s3d_dirExists(const char *file)
{
    bool result = ::wxDirExists(wxString(file, wxConvUTF8));
    return result;
}

time_t s3d_fileModTime(const char *file)
{
	struct stat buf;
	memset(&buf, 0, sizeof(buf));
	int result = stat(file, &buf );

	return buf.st_atime;
}
