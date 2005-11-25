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

#include <string>
#include <stdlib.h>
#include <common/DefinesAssert.h>
#include <common/DefinesString.h>

char *s3d_stristr(const char *x, const char *y)
{
	std::string newX(x);
	std::string newY(y);
	_strlwr((char *) newX.c_str());
	_strlwr((char *) newY.c_str());

	char *result = strstr(newX.c_str(), newY.c_str());
	if (!result) return 0;

	return (char *)(x + (result - newX.c_str()));
}

const char *formatStringList(const char *format, va_list ap)
{
	static char buffer[20048];
	if (vsnprintf(buffer, 20048, format, ap) > 20000)
	{
		dialogAssert("buffer > 20000", __LINE__, __FILE__);
	}
	return buffer;
}

const char *formatString(const char *file, ...)
{
	va_list ap; 
	va_start(ap, file); 
	const char *result = formatStringList(file, ap);
	va_end(ap); 

	return result;
}
