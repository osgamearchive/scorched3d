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

#ifndef __DEFINESASSERT__
#define __DEFINESASSERT__

// This is defined as a fn, so I can
// A) Change it without recompiling everything
// B) Put break points in the fn
// C) Don't put windows headers in everything
extern void dialogAssert(const char *lineText, const int line, const char *file);
#define DIALOG_ASSERT(x) if(!(x)) dialogAssert(#x, __LINE__, __FILE__);

extern void dialogMessage(const char *header, const char *text);
extern void dialogExit(const char *header, const char *text);

#endif // __DEFINESASSERT__
