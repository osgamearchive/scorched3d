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
#endif

#ifndef GL_ARRAY_BUFFER_ARB
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
#endif

class GLStateExtension
{
public:

	static void setup(); // Setup and check for each extension

	// Use VBO
	inline static PFNGLGENBUFFERSARBPROC glGenBuffersARB() { return glGenBuffersARB_; }
	inline static PFNGLBINDBUFFERARBPROC glBindBufferARB() { return glBindBufferARB_; }
	inline static PFNGLBUFFERDATAARBPROC glBufferDataARB() { return glBufferDataARB_; }
	inline static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB() { return glDeleteBuffersARB_; }
	// Use multi textureing?
	inline static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB() { return glActiveTextureARB_; }
	inline static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB() { return glMultiTextCoord2fARB_; }
	inline static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB() { return glClientActiveTextureARB_; }

	// Use tex sub image extension?
	inline static bool getNoTexSubImage() { return noTexSubImage_; }
	// Use cube map extension?
	inline static bool hasCubeMap() { return hasCubeMap_; }
	// Use hardware mipmap extension?
	inline static bool hasHardwareMipmaps() { return hasHardwareMipmaps_; }
	// Use env combine
	inline static bool hasEnvCombine() { return envCombine_; }
	 // How many texture units
	inline static int getTextureUnits() { return textureUnits_; }

protected:
	static bool hasExtension(char *name);

	static bool envCombine_;
	static bool multiTexDisabled_;

	static PFNGLGENBUFFERSARBPROC glGenBuffersARB_;
	static PFNGLBINDBUFFERARBPROC glBindBufferARB_;
	static PFNGLBUFFERDATAARBPROC glBufferDataARB_;
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB_;

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB_;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB_;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB_;

	static int textureUnits_;
	static bool hasCubeMap_;
	static bool hasHardwareMipmaps_;
	static bool noTexSubImage_;

private:
	GLStateExtension();
	~GLStateExtension();
};


#endif
