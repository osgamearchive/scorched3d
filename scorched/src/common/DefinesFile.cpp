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
#include <common/DefinesFile.h>
#include <sys/types.h>
#include <sys/stat.h>

#pragma warning(disable : 4996)

#ifdef WIN32
#include <direct.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

char *s3d_getHomeDir()
{
#ifdef _WIN32
	if (getenv("USERPROFILE")) return getenv("USERPROFILE");
	if (getenv("HOMEPATH") && getenv("HOMEDRIVE"))
	{
		if (0 != strcmp(getenv("HOMEPATH"), getenv("HOMEDRIVE")))
		{
			// Path not set the root
			return getenv("HOMEPATH");
		}
	}
#endif
	if (getenv("HOME")) return getenv("HOME");

	return ".";
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
#ifdef WIN32
	_mkdir(file);
#else
	mkdir(file, 0755);
#endif
	return true;
}

bool s3d_fileExists(const char *file)
{
	return (s3d_fileModTime(file) != 0);
}

bool s3d_dirExists(const char *file)
{
	return (s3d_fileModTime(file) != 0);
}

time_t s3d_fileModTime(const char *file)
{
	struct stat buf;
	memset(&buf, 0, sizeof(buf));
	int result = stat(file, &buf );

	return buf.st_mtime;
}

char *s3d_getOSDesc()
{
#ifdef _WIN32
	return "Windows";
#else
#ifdef __DARWIN__
	return "OSX";
#else
	return "Unix";
#endif
#endif
}
