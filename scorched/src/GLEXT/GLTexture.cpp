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


// GLTexture.cpp: implementation of the GLTexture class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLTexture.h>
#include <GLEXT/GLStateExtension.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLTexture *GLTexture::lastBind_ = 0;

GLTexture::GLTexture() : texNum_(0)
{

}

GLTexture::~GLTexture()
{
	if (texNum_)
	{
		glDeleteTextures(1, &texNum_);
		texNum_ = 0;
	}
}

void GLTexture::draw(bool force)
{
	if ((this != lastBind_) || force)
	{
		glBindTexture(texType_, texNum_);
		lastBind_ = this;
	}
}

bool GLTexture::replace(GLImage &bitmap,
						GLenum format, 
						bool mipMap)
{
	if (textureValid())
	{
		glBindTexture(texType_, texNum_);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.getWidth());
		glTexSubImage2D(texType_, 0, 
			0, 0, 
			bitmap.getWidth(), bitmap.getHeight(), 
			format, GL_UNSIGNED_BYTE, 
			bitmap.getBits());
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
	else
	{
		return create(bitmap, format, mipMap);
	}
	return true;
}

bool GLTexture::create(GLImage &bitmap, 
					   GLenum format, 
					   bool mipMap)
{
	bool success = create(bitmap.getBits(),
			bitmap.getWidth(), 
			bitmap.getHeight(), 
			bitmap.getComponents(), 
			bitmap.getAlignment(),
			format,
			mipMap);

	return success;
}

bool GLTexture::create(const void * data, 
					   GLint width, 
					   GLint height, 
					   GLint components, 
					   GLint alignment,
					   GLenum format, 
					   bool mipMap)
{
	bool success = false;
	if (data)
	{
		if (height == 1 || width == 1) texType_ = GL_TEXTURE_1D;
		else texType_ = GL_TEXTURE_2D;

		if (!texNum_)
		{
			GLfloat priority = 1.0f;
			glGenTextures(1, &texNum_);
			glPrioritizeTextures(1, &texNum_, &priority);
		}
		glBindTexture(texType_, texNum_);

		success = createTexture(data, width, height, components, alignment, format, mipMap);
	}

	return success;
}

bool GLTexture::createTexture(const void * data, 
							  GLint width, 
							  GLint height, 
							  GLint components, 
							  GLint alignment,
							  GLenum format, 
							  bool mipMap)
{
	texFormat_ = format;
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	if (height == 1)
	{
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild1DMipmaps(GL_TEXTURE_1D, components, width, 
				format, GL_UNSIGNED_BYTE, data);
	}
	else if (width == 1)
	{
		if (mipMap)
		{
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			gluBuild1DMipmaps(GL_TEXTURE_1D, components, height, 
					format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage1D(GL_TEXTURE_1D, 0, components, height, 
				0, format, GL_UNSIGNED_BYTE, data);
		}
	}
	else
	{
		if (mipMap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

			if (GLStateExtension::hasHardwareMipmaps()) // Use hardware mipmaps
			{
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

				glTexImage2D(GL_TEXTURE_2D, 0, components, width, 
					height, 0, format, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, 
					height, format, GL_UNSIGNED_BYTE, data);
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

			glTexImage2D(GL_TEXTURE_2D, 0, components, width, 
				height, 0, format, GL_UNSIGNED_BYTE, data);
		}
	};

	return true;
}
