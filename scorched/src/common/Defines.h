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

#ifndef __DEFINES__
#define __DEFINES__

#include <stdarg.h>

#ifndef _WIN32
#include "porting.h"
#endif

#define PI 3.14159f
#define TWOPI 6.28318f
#define HALFPI 1.570795f

// This is defined as a fn, so I can
// A) Change it without recompiling everything
// B) Put break points in the fn
// C) Don't put windows headers in everything
extern void dialogAssert(const char *lineText, const int line, const char *file);
#define DIALOG_ASSERT(x) if(!(x)) dialogAssert(#x, __LINE__, __FILE__);

extern void dialogMessage(const char *header, const char *fmt, ...);
extern void dialogExit(const char *header, const char *fmt, ...);

extern const char *formatString(const char *format, ...);
extern const char *formatStringList(const char *format, va_list ap); 

extern void setDataFileMod(const char *mod);
extern const char *getDataFileMod();
extern bool checkDataFile(const char *file, ...);
extern bool fileExists(const char *file);
extern bool dirExists(const char *file);

extern const char *getDataFile(const char *file, ...);
extern const char *getDocFile(const char *file, ...);
extern const char *getLogFile(const char *file, ...);
extern const char *getSettingsFile(const char *file, ...);
extern const char *getHomeFile(const char *file, ...);
extern const char *getSaveFile(const char *file, ...);
extern const char *getModFile(const char *file, ...);
extern const char *getGlobalModFile(const char *file, ...);

#define RAND ((float) rand() / (float) RAND_MAX)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

#ifdef NO_FLOAT_MATH

#define sinf(x) ((float)sin(x))
#define cosf(x) ((float)cos(x))
#ifndef __DARWIN__
#define fabsf(x) ((float)fabs(x))
#endif
#define sqrtf(x) ((float)sqrt(x))
#define acosf(x) ((float)acos(x))
#define atan2f(x,y) ((float)atan2(x,y))
#define powf(x,y) ((float)pow(x,y))
#define floorf(x) ((float)floor(x))

#endif

extern unsigned int ScorchedPort;
extern char *ScorchedVersion;
extern char *ScorchedProtocolVersion;

#endif // __DEFINES__
