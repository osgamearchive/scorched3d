////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <GLEXT/GLShadowFrameBuffer.h>
#include <GLEXT/GLStateExtension.h>

GLShadowFrameBuffer::GLShadowFrameBuffer() : frameBufferObject_(0)
{
}

GLShadowFrameBuffer::~GLShadowFrameBuffer()
{
	destroy();
}

bool GLShadowFrameBuffer::create(GLTexture &texture, GLenum type)
{
	type_ = type;

	//Generate the frame buffer object
	glGenFramebuffersEXT(1, &frameBufferObject_);

	//Activate the frame buffer object
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject_);

	//Attach our depth texture to the frame buffer object
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,
		type, // e.g. GL_DEPTH_ATTACHMENT_EXT or GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, 
		texture.getTexName(),
		0);

	//You must set DrawBuffer and ReadBuffer to none since we're rendering to depth only
	if (type == GL_DEPTH_ATTACHMENT_EXT)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE); 
	}

	//Check the completeness of our frame buffer object, with depth textures, there is no need for stencil
	//and depth attachment for a FBO to be considered as complete
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);  
	switch(status) {                                          
		case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;                                                
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			return false;
		break;                                                
		default:
			return false;
		break;
	}

	//Turn off our frame buffer object
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return true;
}

void GLShadowFrameBuffer::destroy()
{
	if (!bufferValid()) return;

	//Activate the frame buffer objcet
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject_);

	//Detach our depth texture from the frame buffer object
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,
		type_,
		GL_TEXTURE_2D, 
		0,
		0);

	//DeActivate the frame buffer objcet
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//Delete the frame buffer object
	glDeleteFramebuffersEXT(1, &frameBufferObject_);

	frameBufferObject_ = 0;
}

void GLShadowFrameBuffer::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject_);
}

void GLShadowFrameBuffer::unBind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
