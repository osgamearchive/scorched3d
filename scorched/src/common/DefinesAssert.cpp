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

#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <stdarg.h>
#include <common/Defines.h>
#include <windows.h>
#include <string>

#pragma warning(disable : 4996)

extern bool wxWindowInit;

void dialogMessage(const char *header, const char *text)
{
	std::string newtext;
	int count = 0;
	for (const char *t=text; *t; t++)
	{
		if (*t == '\n') count = 0;
		else count++;
		if (count > 75)
		{
			newtext.append("...\n");
			count = 0;
		}

		newtext.append(formatString("%c", *t));
	}


#if defined(_WIN32) && !defined(S3D_SERVER)
	MessageBox(NULL, newtext.c_str(), header, MB_OK);
#else
	printf("%s : %s\n", header, newtext.c_str());
#endif
}

void dialogAssert(const char *lineText, const int line, const char *file)
{
	// Dont use formatString here as this method is called by formatString.
	char buffer[20048];
	snprintf(buffer, 20048, "%s\n%i:%s", lineText, line, file);
	dialogMessage("Program Assert", buffer);
	exit(64);
}

void dialogExit(const char *header, const char *text)
{
	dialogMessage(header, text);
	exit(64);
}
