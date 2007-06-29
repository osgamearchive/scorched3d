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
#include <GLEXT/GLJpg.h>
#include <GLEXT/GLPng.h>
#ifndef S3D_SERVER
	#include <GLEXT/GLState.h>
#endif
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
		GLPng *png = new GLPng();
		png->loadFromFile(filename, true);
		return png;
	}
	else if (strstr(extension.c_str(), ".jpg"))
	{
		GLJpg *jpg = new GLJpg();
		jpg->loadFromFile(filename, true);
		return jpg;
	}

	// Failsafe !!
	GLBitmap *bitmap = new GLBitmap();
	bitmap->loadFromFile(filename, true);
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
			GLPng *png = new GLPng();
			png->loadFromFile(filename, alphafilename, invert);
			return png;
		}
		GLPng *png = new GLPng();
		png->loadFromFile(filename);
		return png;
	} else if (strstr(extension.c_str(), ".jpg"))
	{
		if (alphafilename)
		{
			GLJpg *jpg = new GLJpg();
			jpg->loadFromFile(filename, alphafilename, invert);
			return jpg;
		}
		GLJpg *jpg = new GLJpg();
		jpg->loadFromFile(filename);
		return jpg;
	} 

	// Failsafe !!
	if (alphafilename)
	{
		GLBitmap *bitmap = new GLBitmap();
		bitmap->loadFromFile(filename, alphafilename, invert);
		return bitmap;
	}
	GLBitmap *bitmap = new GLBitmap();
	bitmap->loadFromFile(filename);
	return bitmap;
}

GLImageHandle GLImageFactory::loadImageHandle(
	const char *filename, 
	const char *alphafilename, 
	bool invert)
{
	GLImage *image = loadImage(filename, alphafilename, invert);
	GL_ASSERT();

	GLImageHandle handle(*image);
	delete image;
	return handle;
}

GLImageHandle GLImageFactory::loadAlphaImageHandle(
	const char *filename)
{
	GLImage *image = loadAlphaImage(filename);
	GL_ASSERT();

	GLImageHandle handle(*image);
	delete image;
	return handle;
}

GLImageHandle GLImageFactory::createBlank(int width, int height, bool alpha, unsigned char fill)
{
	GLBitmap result(width, height, alpha, fill);
	return GLImageHandle(result);
}

#ifndef S3D_SERVER

#include <GLEXT/GLState.h>
#include <common/Defines.h>

GLImageHandle GLImageFactory::grabScreen()
{
	GLint		viewport[4];		/* Current viewport */
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLBitmap map(viewport[2], viewport[3], false);

	glFinish();				/* Finish all OpenGL commands */
	glPixelStorei(GL_PACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	glReadPixels(0, 0, map.getWidth(), map.getHeight(), 
		GL_RGB, GL_UNSIGNED_BYTE, map.getBits());

	return GLImageHandle(map);
}
#endif
