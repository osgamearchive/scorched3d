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

#ifndef __DEFINESFILE__
#define __DEFINESFILE__

#include <time.h>

extern void s3d_fileDos2Unix(char *file);
extern bool s3d_fileExists(const char *file);
extern bool s3d_dirExists(const char *file);
extern bool s3d_dirMake(const char *file);
extern time_t s3d_fileModTime(const char *file);
extern char *s3d_getHomeDir();

#endif // __DEFINESFILE__
