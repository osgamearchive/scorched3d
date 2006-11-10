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
#include <common/OptionsDisplay.h> // Hmm library code pollution
#include <common/DefinesString.h>
#include <common/Logger.h>
#include <SDL/SDL.h>
#include <string.h>
#include <string>

bool GLStateExtension::hasHardwareShadows_ = false;
bool GLStateExtension::hasCubeMap_ = false;
bool GLStateExtension::hasHardwareMipmaps_ = false;
bool GLStateExtension::envCombine_ = false;
bool GLStateExtension::noTexSubImage_ = false;
int GLStateExtension::textureUnits_ = 0;
PFNGLACTIVETEXTUREARBPROC GLStateExtension::glActiveTextureARB_ =  0;
PFNGLMULTITEXCOORD2FARBPROC GLStateExtension::glMultiTextCoord2fARB_ = 0;
PFNGLCLIENTACTIVETEXTUREARBPROC GLStateExtension::glClientActiveTextureARB_ = 0;

PFNGLGENBUFFERSARBPROC GLStateExtension::glGenBuffersARB_ = 0;
PFNGLBINDBUFFERARBPROC GLStateExtension::glBindBufferARB_ = 0;
PFNGLBUFFERDATAARBPROC GLStateExtension::glBufferDataARB_ = 0;
PFNGLDELETEBUFFERSARBPROC GLStateExtension::glDeleteBuffersARB_ = 0;
PFNGLMAPBUFFERARBPROC GLStateExtension::glMapBufferARB_ = 0;
PFNGLUNMAPBUFFERARBPROC GLStateExtension::glUnmapBufferARB_ = 0;

PFNGLGENFRAMEBUFFERSEXTPROC GLStateExtension::glGenFramebuffersEXT_ = 0;
PFNGLBINDFRAMEBUFFEREXTPROC GLStateExtension::glBindFramebufferEXT_ = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC GLStateExtension::glCheckFramebufferStatusEXT_ = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC GLStateExtension::glFramebufferTexture2DEXT_ = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC GLStateExtension::glDeleteFramebuffersEXT_ = 0;

bool GLStateExtension::hasExtension(char *name)
{
	bool result = false;

	if (!OptionsDisplay::instance()->getNoGLExt())
	{
		const char *ext = (char *) glGetString(GL_EXTENSIONS);
		std::string extCopy(ext);
		char *token = strtok((char *) extCopy.c_str(), " ");
		while(token != 0)
		{
			if (0 == strcmp(token, name)) result = true;
			token = strtok(NULL, " ");
		}
	}

	Logger::log(
		formatString("GL extension \"%s\" = %s",
		name,
		(result?"on":"off")));

	return result;
}

void GLStateExtension::setup()
{
	if (!OptionsDisplay::instance()->getNoVBO())
	{
		if (hasExtension("GL_ARB_vertex_buffer_object"))
		{
			glGenBuffersARB_ = (PFNGLGENBUFFERSARBPROC) 
				SDL_GL_GetProcAddress("glGenBuffersARB");
			glBindBufferARB_ = (PFNGLBINDBUFFERARBPROC) 
				SDL_GL_GetProcAddress("glBindBufferARB");
			glBufferDataARB_ = (PFNGLBUFFERDATAARBPROC) 
				SDL_GL_GetProcAddress("glBufferDataARB");
			glDeleteBuffersARB_ = (PFNGLDELETEBUFFERSARBPROC) 
				SDL_GL_GetProcAddress("glDeleteBuffersARB");
			glMapBufferARB_ = (PFNGLMAPBUFFERARBPROC) 
				SDL_GL_GetProcAddress("glMapBufferARB");
			glUnmapBufferARB_ = (PFNGLUNMAPBUFFERARBPROC) 
				SDL_GL_GetProcAddress("glUnmapBufferARB");
		}
	}

	if (!OptionsDisplay::instance()->getNoGLMultiTex())
	{
		if (hasExtension("GL_ARB_multitexture"))
		{
			GLint textureUnits;
			glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &textureUnits);
			textureUnits_ = textureUnits;

			glActiveTextureARB_ = (PFNGLACTIVETEXTUREARBPROC) 
				SDL_GL_GetProcAddress("glActiveTextureARB");
			glMultiTextCoord2fARB_ = (PFNGLMULTITEXCOORD2FARBPROC) 
				SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
			glClientActiveTextureARB_ = (PFNGLCLIENTACTIVETEXTUREARBPROC) 
				SDL_GL_GetProcAddress("glClientActiveTextureARB");
		}
	}

	if (!OptionsDisplay::instance()->getNoGLEnvCombine())
	{
		envCombine_ = hasExtension("GL_ARB_texture_env_combine");
	}

	if (!OptionsDisplay::instance()->getNoGLCubeMap())
	{
		hasCubeMap_ = hasExtension("GL_EXT_texture_cube_map") ||
			hasExtension("GL_ARB_texture_cube_map");
	}

	if (!OptionsDisplay::instance()->getNoGLHardwareMipmaps())
	{
		hasHardwareMipmaps_ = hasExtension("GL_SGIS_generate_mipmap");
	}

	{
		if (hasExtension("GL_EXT_framebuffer_object") &&
			hasExtension("GL_ARB_shadow") &&
			hasExtension("GL_ARB_depth_texture") &&
			hasExtension("GL_ARB_multitexture"))
		{
			hasHardwareShadows_ = false; //true;

			glGenFramebuffersEXT_ = (PFNGLGENFRAMEBUFFERSEXTPROC)
				SDL_GL_GetProcAddress("glGenFramebuffersEXT");
			glBindFramebufferEXT_ = (PFNGLBINDFRAMEBUFFEREXTPROC)
				SDL_GL_GetProcAddress("glBindFramebufferEXT");
			glCheckFramebufferStatusEXT_ = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)
				SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
			glFramebufferTexture2DEXT_ = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
				SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
			glDeleteFramebuffersEXT_ = (PFNGLDELETEFRAMEBUFFERSEXTPROC)
				SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
		}
	}

	noTexSubImage_ = OptionsDisplay::instance()->getNoGLTexSubImage();

	Logger::log("GL_VENDOR:");
	Logger::log((const char *) glGetString(GL_VENDOR));
	Logger::log("GL_RENDERER:");
	Logger::log((const char *) glGetString(GL_RENDERER));
	Logger::log("GL_VERSION:");
	Logger::log((const char *) glGetString(GL_VERSION));
	Logger::log("GL_EXTENSIONS:");
	Logger::log((const char *) glGetString(GL_EXTENSIONS));
	Logger::log("TEXTURE_UNITS:");
	Logger::log(formatString("%s (%i units)", ((glActiveTextureARB_==0)?"Off":"On"),textureUnits_));
	Logger::log("VBO:");
	Logger::log(formatString("%s", ((glGenBuffersARB_==0)?"Off":"On")));
	Logger::log("ENV COMBINE:");
	Logger::log(formatString("%s", (envCombine_?"On":"Off")));
	Logger::log("CUBE MAP:");
	Logger::log(formatString("%s", (hasCubeMap_?"On":"Off")));
	Logger::log("HW MIP MAPS:");
	Logger::log(formatString("%s", (hasHardwareMipmaps_?"On":"Off")));
	Logger::log("HW SHADOWS:");
	Logger::log(formatString("%s", (hasHardwareShadows_?"On":"Off")));
}
