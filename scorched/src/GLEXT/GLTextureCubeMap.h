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

#if !defined(AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_)
#define AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_

#include <GLEXT/GLTextureBase.h>
#include <GLEXT/GLImage.h>

class GLTextureCubeMap : public GLTextureBase
{
public:
	GLTextureCubeMap();
	virtual ~GLTextureCubeMap();

	virtual void draw(bool force = false);

	bool create(GLImage &bitmap, 
			GLenum format = GL_RGB, 
			bool mipMap = true);
	bool create(const void * data, 
			GLint width, 
			GLint height, 
			GLint components, 
			GLint alignment,
			GLenum format, 
			bool mipMap);

	bool textureValid();

protected:
	GLuint cubeTexNum_[6];

	bool createTexture(const void * data, 
						GLint width, 
						GLint height, 
						GLint components, 
						GLint alignment,
						GLenum format, 
						bool mipMap);

};

#endif // !defined(AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_)
