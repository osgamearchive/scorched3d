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


// GLTextureCubeMap.cpp: implementation of the GLTextureCubeMap class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLState.h>
#include <GLEXT/GLTextureCubeMap.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLTextureCubeMap::GLTextureCubeMap()
{

}

GLTextureCubeMap::~GLTextureCubeMap()
{
	if (texNum_)
	{
		glDeleteTextures(6, &texNum_);
		texNum_ = 0;
	}
}

void GLTextureCubeMap::draw(bool force)
{
	if ((this != lastBind_) || force)
	{
		lastBind_ = this;
	}
}

bool GLTextureCubeMap::create(GLImage &bitmap, 
			GLenum format, 
			bool mipMap)
{
	return GLTexture::create(bitmap, format, mipMap);
}

bool GLTextureCubeMap::create(const void * data, 
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
		if (!texNum_)
		{
			GLuint texNum[6];
			glGenTextures(6, texNum);
			GLfloat priority = 1.0f;
			glPrioritizeTextures(6, texNum, &priority);

			texNum_ = texNum[0];
		}

		// Generate a texture GL_TEXTURE_CUBE_MAP_EXT and bind it
		glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, texNum_);

		success = createTexture(data, width, height, components, alignment, format, mipMap);
	}

	return success;
}

bool GLTextureCubeMap::createTexture(const void * data, 
						GLint width, 
						GLint height, 
						GLint components, 
						GLint alignment,
						GLenum format, 
						bool mipMap)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLenum texDefines[] = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT};

	// Load all six cube maps
	for (int i=0; i<6; i++)
	{
		// Build the texture
		if (mipMap)
		{
			// Texture parameters
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(texDefines[i], components, width, 
					height, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			// Texture parameters
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(texDefines[i], 0, components, width, 
				height, 0, format, GL_UNSIGNED_BYTE, data);
		}
	}

	return true;
}
