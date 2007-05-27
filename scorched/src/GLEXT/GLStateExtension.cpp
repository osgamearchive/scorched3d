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
#include <graph/OptionsDisplay.h>
#include <common/DefinesString.h>
#include <common/Logger.h>
#include <common/Defines.h>

bool GLStateExtension::hasHardwareShadows_ = false;
bool GLStateExtension::hasMultiTex_ = false;
bool GLStateExtension::hasCubeMap_ = false;
bool GLStateExtension::hasSphereMap_ = false;
bool GLStateExtension::hasVBO_ = false;
bool GLStateExtension::hasHardwareMipmaps_ = false;
bool GLStateExtension::envCombine_ = false;
bool GLStateExtension::noTexSubImage_ = false;
bool GLStateExtension::hasBlendColor_ = false;
bool GLStateExtension::hasShaders_ = false;
bool GLStateExtension::hasFBO_ = false;
int GLStateExtension::textureUnits_ = 0;

void GLStateExtension::setup()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		dialogExit("GLEW", (const char *) glewGetErrorString(err));
	}
	Logger::log(formatString("GLEW VERSION:%s", glewGetString(GLEW_VERSION)));

	if (!OptionsDisplay::instance()->getNoVBO())
	{
		if (GLEW_ARB_vertex_buffer_object &&
			GLEW_EXT_draw_range_elements)
		{
			hasVBO_ = true;
		}
	}

	if (!OptionsDisplay::instance()->getNoGLMultiTex())
	{
		if (GLEW_ARB_multitexture)
		{
			GLint textureUnits;
			glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &textureUnits);
			textureUnits_ = textureUnits;
			hasMultiTex_ = true;
		}
	}

	if (!OptionsDisplay::instance()->getNoGLEnvCombine())
	{
		envCombine_ = GLEW_ARB_texture_env_combine == GL_TRUE;
	}

	if (!OptionsDisplay::instance()->getNoGLCubeMap())
	{
		hasCubeMap_ = GLEW_EXT_texture_cube_map == GL_TRUE ||
			GLEW_ARB_texture_cube_map == GL_TRUE;
	}

	if (!OptionsDisplay::instance()->getNoGLSphereMap())
	{
		hasSphereMap_ = true;
	}

	if (!OptionsDisplay::instance()->getNoGLHardwareMipmaps())
	{
		hasHardwareMipmaps_ = GLEW_SGIS_generate_mipmap == GL_TRUE;
	}

	if (GLEW_EXT_blend_color)
	{
		hasBlendColor_ = true;
	}

	{
		if (GLEW_EXT_framebuffer_object)
		{
			hasFBO_ = true;
		}
	}
	{
		if (GLEW_EXT_framebuffer_object &&
			GLEW_ARB_shadow &&
			GLEW_ARB_depth_texture &&
			GLEW_ARB_multitexture)
		{
			hasHardwareShadows_ = false; //true;
		}
	}

	if (!OptionsDisplay::instance()->getNoGLShaders())
	{
		hasShaders_ = 
			GLEW_ARB_fragment_shader &&
			GLEW_ARB_shader_objects &&
			GLEW_ARB_vertex_shader;
	}

	noTexSubImage_ = OptionsDisplay::instance()->getNoGLTexSubImage();

	Logger::log(formatString("GL_VENDOR:%s", glGetString(GL_VENDOR)));
	Logger::log(formatString("GL_RENDERER:%s", glGetString(GL_RENDERER)));
	Logger::log(formatString("GL_VERSION:%s", glGetString(GL_VERSION)));
	Logger::log(formatString("GL_EXTENSIONS:%s", glGetString(GL_EXTENSIONS)));
	Logger::log(formatString("TEXTURE UNITS: %s (%i units)", 
		(hasMultiTex()?"On":"Off"),textureUnits_));
	Logger::log(formatString("VERTEX BUFFER OBJECT:%s", 
		(hasVBO()?"On":"Off")));
	Logger::log(formatString("FRAME BUFFER OBJECT:%s", 
		(hasFBO()?"On":"Off")));
	Logger::log(formatString("SHADERS:%s", 
		(!hasShaders_?"Off":"On")));
	Logger::log(formatString("ENV COMBINE:%s", 
		(envCombine_?"On":"Off")));
	Logger::log(formatString("CUBE MAP:%s", 
		(hasCubeMap_?"On":"Off")));
	Logger::log(formatString("HW MIP MAPS:%s", 
		(hasHardwareMipmaps_?"On":"Off")));
	Logger::log(formatString("HW SHADOWS:%s", 
		(hasHardwareShadows_?"On":"Off")));
	Logger::log(formatString("BLEND COLOR:%s", 
		(hasBlendColor_?"On":"Off")));
}
