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

#ifndef __DEFINESSCORCHED__
#define __DEFINESSCORCHED__

extern void setSettingsDir(const char *dir);
extern void setDataFileMod(const char *mod);
extern const char *getDataFileMod();

extern bool checkDataFile(const char *file, ...);
extern const char *getDataFile(const char *file, ...);
extern const char *getDocFile(const char *file, ...);
extern const char *getLogFile(const char *file, ...);
extern const char *getSettingsFile(const char *file, ...);
extern const char *getHomeFile(const char *file, ...);
extern const char *getSaveFile(const char *file, ...);
extern const char *getModFile(const char *file, ...);
extern const char *getGlobalModFile(const char *file, ...);

extern unsigned int ScorchedPort;
extern char *ScorchedVersion;
extern char *ScorchedProtocolVersion;

#endif // __DEFINESSCORCHED__
