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
typedef void (* PFNGLUNLOCKARRAYSEXTPROC) ();
#endif

class GLStateExtension
{
public:

	static void setup(); // Setup and check for each extension

	// Use multi textureing?
	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB() { return glActiveTextureARB_; }
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB() { return glMultiTextCoord2fARB_; }
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB() { return glClientActiveTextureARB_; }
	// Use locked arrays?
	static PFNGLLOCKARRAYSEXTPROC glLockArraysEXT() { return glLockArraysEXT_; }
	static PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT() { return glUnlockArraysEXT_; }
	static bool getNoTexSubImage(); // Use tex sub image extension?
	static bool hasCubeMap(); // Use cube map extension?
	static bool hasHardwareMipmaps(); // Use hardware mipmap extension?
	static bool hasEnvCombine(); // Use env combine
	static int getTextureUnits(); // How many texture units

protected:
	static bool hasExtension(char *name);

	static bool envCombine_;
	static bool multiTexDisabled_;

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB_;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB_;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB_;

	static PFNGLLOCKARRAYSEXTPROC glLockArraysEXT_;
	static PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT_;

	static int textureUnits_;
	static bool hasCubeMap_;
	static bool hasHardwareMipmaps_;

private:
	GLStateExtension();
	~GLStateExtension();
};


#endif
