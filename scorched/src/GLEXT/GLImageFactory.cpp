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

#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLPng.h>
#include <string>

GLImageFactory::GLImageFactory()
{
}

GLImage *GLImageFactory::loadAlphaImage(
	const char *filename)
{
	std::string extension(filename);
	_strlwr((char *) extension.c_str());

	if (strstr(extension.c_str(), ".png"))
	{
		GLPng *png = new GLPng(filename, true);
		return png;
	}

	// Failsafe !!
	GLBitmap *bitmap = new GLBitmap(filename, true);
	return bitmap;
}
GLImage *GLImageFactory::loadImage(
	const char *filename, 
	const char *alphafilename, 
	bool invert)
{
	std::string extension(filename);
	_strlwr((char *) extension.c_str());

	if (strstr(extension.c_str(), ".png"))
	{
		if (alphafilename)
		{
			GLPng *png = new GLPng(filename, alphafilename, invert);
			return png;
		}
		GLPng *png = new GLPng(filename);
		return png;
	}

	// Failsafe !!
	if (alphafilename)
	{
		GLBitmap *bitmap = new GLBitmap(filename, alphafilename, invert);
		return bitmap;
	}
	GLBitmap *bitmap = new GLBitmap(filename);
	return bitmap;
}

GLImageHandle GLImageFactory::loadImageHandle(
	const char *filename, 
	const char *alphafilename, 
	bool invert)
{
	GLImage *image = loadImage(filename, alphafilename, invert);
	GLImageHandle handle(*image);
	delete image;
	return handle;
}

GLImageHandle GLImageFactory::loadAlphaImageHandle(
	const char *filename)
{
	GLImage *image = loadAlphaImage(filename);
	GLImageHandle handle(*image);
	delete image;
	return handle;
}
