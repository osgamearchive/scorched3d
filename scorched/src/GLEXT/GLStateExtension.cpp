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

#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLConsole.h>
#include <SDL/SDL.h>
#include <string.h>
#include <string>

bool GLStateExtension::noExtensions_ = false;
bool GLStateExtension::multiTexDisabled_ = false;
bool GLStateExtension::hasCubeMap_ = false;
bool GLStateExtension::hasHardwareMipmaps_ = false;
PFNGLLOCKARRAYSEXTPROC GLStateExtension::glLockArraysEXT_ = 0;
PFNGLACTIVETEXTUREARBPROC GLStateExtension::glActiveTextureARB_ =  0;
PFNGLMULTITEXCOORD2FARBPROC GLStateExtension::glMultiTextCoord2fARB_ = 0;
PFNGLCLIENTACTIVETEXTUREARBPROC GLStateExtension::glClientActiveTextureARB_ = 0;

bool GLStateExtension::hasExtension(char *name)
{
	bool result = false;

	if (!noExtensions_)
	{
		if (multiTexDisabled_ && (strcmp(name, "GL_ARB_multitexture") == 0))
		{
		}
		else
		{
			const char *ext = (char *) glGetString(GL_EXTENSIONS);
			std::string extCopy(ext);
			char *token = strtok((char *) extCopy.c_str(), " ");
			while(token != 0)
			{
				if (0 == strcmp(token, name)) result = true;
				token = strtok(NULL, "\n");
			}
		}
	}

	GLConsole::instance()->addLine(false, "GL extension \"%s\" = %s",
								   name,
								   (result?"on":"off"));

	return result;
}

void GLStateExtension::setup()
{
	GLConsole::instance()->addLine(false, "GL_VENDOR:");
	GLConsole::instance()->addLine(false, (const char *) glGetString(GL_VENDOR));
	GLConsole::instance()->addLine(false, "GL_RENDERER:");
	GLConsole::instance()->addLine(false, (const char *) glGetString(GL_RENDERER));
	GLConsole::instance()->addLine(false, "GL_VERSION:");
	GLConsole::instance()->addLine(false, (const char *) glGetString(GL_VERSION));
	GLConsole::instance()->addLine(false, "GL_EXTENSIONS:");
	GLConsole::instance()->addLine(false, (const char *) glGetString(GL_EXTENSIONS));

	if (hasExtension("GL_ARB_multitexture"))
	{
		GLint num_tus = 0;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&num_tus);
		GLConsole::instance()->addLine(false, "%i texture units", num_tus);

		glActiveTextureARB_ = 
			(PFNGLACTIVETEXTUREARBPROC)	SDL_GL_GetProcAddress("glActiveTextureARB");
		glMultiTextCoord2fARB_ = 
			(PFNGLMULTITEXCOORD2FARBPROC) SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
		glClientActiveTextureARB_ = 
			(PFNGLCLIENTACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glClientActiveTextureARB");
	}

	if (hasExtension("EXT_compiled_vertex_array"))
	{
		glLockArraysEXT_ = (PFNGLLOCKARRAYSEXTPROC)
			SDL_GL_GetProcAddress("glLockArraysEXT");
	}

	hasCubeMap_ = hasExtension("GL_EXT_texture_cube_map");
	hasHardwareMipmaps_ = hasExtension("GL_SGIS_generate_mipmap");
}
