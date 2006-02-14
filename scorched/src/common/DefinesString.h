////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#ifndef __SNPRINTF_SAMBA__
#define __SNPRINTF_SAMBA__

#include <stdio.h> // For snprintf on linux

#if !defined(HAVE_SNPRINTF)

#include <stdarg.h> // For va_list

#define snprintf smb_snprintf
#define vsnprintf smb_vsnprintf

#ifdef __cplusplus
extern "C" {
#endif

extern int snprintf(char *str, size_t size, const char *format, ...);
extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // HAVE_SNPRINTF

/*
#define _STD_USING
#ifdef sprintf
#undef sprintf
#endif 
#define sprintf __ERROR__XX__NEVER_USE_SPRINTF__;

#ifdef vsprintf
#undef vsprintf
#endif 
#define vsprintf __ERROR__XX__NEVER_USE_VSPRINTF__;
*/

extern const char *formatString(const char *format, ...);
extern char *s3d_stristr(const char *x, const char *y);

#endif // __SNPRINTF_SAMBA__
