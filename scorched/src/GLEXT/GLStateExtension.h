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


#if !defined(__INCLUDE_GLStateExtensionh_INCLUDE__)
#define __INCLUDE_GLStateExtensionh_INCLUDE__

#include <GLEXT/GLState.h>

#ifndef GL_COMBINE_ARB
#define GL_COMBINE_ARB						0x8570
#endif

#ifndef GL_RGB_SCALE_ARB
#define GL_RGB_SCALE_ARB					0x8573
#endif

#ifndef _WIN32
typedef void (* PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (* PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (* PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (* PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
#endif

class GLStateExtension
{
public:
	static bool &getNoExtensions() { return noExtensions_; }
	static bool &getNoMultiTex() { return multiTexDisabled_; }
	static bool &getNoTexSubImage() { return noTexSubImage_; }

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB() { return glActiveTextureARB_; }
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB() { return glMultiTextCoord2fARB_; }
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB() { return glClientActiveTextureARB_; }

	static PFNGLLOCKARRAYSEXTPROC glLockArraysEXT() { return glLockArraysEXT_; }

	static bool hasCubeMap() { return hasCubeMap_; }
	static bool hasHardwareMipmaps() { return hasHardwareMipmaps_; }
	static bool hasEnvCombine() { return envCombine_; }
	static int getTextureUnits() { return textureUnits_; }

	static void setup();

protected:
	static bool hasExtension(char *name);

	static bool noTexSubImage_;
	static bool envCombine_;
	static bool noExtensions_;
	static bool multiTexDisabled_;

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB_;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB_;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB_;

	static PFNGLLOCKARRAYSEXTPROC glLockArraysEXT_;

	static int textureUnits_;
	static bool hasCubeMap_;
	static bool hasHardwareMipmaps_;

private:
	GLStateExtension();
	~GLStateExtension();
};


#endif
