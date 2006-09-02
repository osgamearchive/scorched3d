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
#include <GLEXT/GLMissingExt.h>

class GLStateExtension
{
public:

	static void setup(); // Setup and check for each extension

	// Use VBO
	inline static PFNGLGENBUFFERSARBPROC glGenBuffersARB() { return glGenBuffersARB_; }
	inline static PFNGLBINDBUFFERARBPROC glBindBufferARB() { return glBindBufferARB_; }
	inline static PFNGLBUFFERDATAARBPROC glBufferDataARB() { return glBufferDataARB_; }
	inline static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB() { return glDeleteBuffersARB_; }
	inline static PFNGLMAPBUFFERARBPROC glMapBufferARB() { return glMapBufferARB_; }
	inline static PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB() { return glUnmapBufferARB_; }
	// Use multi textureing?
	inline static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB() { return glActiveTextureARB_; }
	inline static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB() { return glMultiTextCoord2fARB_; }
	inline static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB() { return glClientActiveTextureARB_; }
	// Use frame buffers
	inline static PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT() { return glGenFramebuffersEXT_; }
	inline static PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT() { return glBindFramebufferEXT_; }
	inline static PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT() { return glDeleteFramebuffersEXT_; }
	inline static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT() { return glCheckFramebufferStatusEXT_; }
	inline static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT() { return glFramebufferTexture2DEXT_; }

	// Use tex sub image extension?
	static bool getNoTexSubImage() { return noTexSubImage_; }
	// Use cube map extension?
	static bool hasCubeMap() { return hasCubeMap_; }
	// Use hardware mipmap extension?
	static bool hasHardwareMipmaps() { return hasHardwareMipmaps_; }
	// Use env combine
	static bool hasEnvCombine() { return envCombine_; }
	// How many texture units
	static int getTextureUnits() { return textureUnits_; }
	// Use FrameBuffer extension
	static bool hasHardwareShadows() { return hasHardwareShadows_; }

protected:
	static bool hasExtension(char *name);

	static bool envCombine_;
	static bool multiTexDisabled_;

	static PFNGLGENBUFFERSARBPROC glGenBuffersARB_;
	static PFNGLBINDBUFFERARBPROC glBindBufferARB_;
	static PFNGLBUFFERDATAARBPROC glBufferDataARB_;
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB_;
	static PFNGLMAPBUFFERARBPROC glMapBufferARB_;
	static PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB_;

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB_;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTextCoord2fARB_;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB_;

	static PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT_;
	static PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT_;
	static PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT_;
	static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT_;
	static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT_;

	static int textureUnits_;
	static bool hasCubeMap_;
	static bool hasHardwareMipmaps_;
	static bool noTexSubImage_;
	static bool hasHardwareShadows_;

private:
	GLStateExtension();
	~GLStateExtension();
};


#endif
