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


// GLTextureCubeMap.h: interface for the GLTextureCubeMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_)
#define AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_


#include <GLEXT/GLTexture.h>

class GLTextureCubeMap : public GLTexture
{
public:
	GLTextureCubeMap();
	virtual ~GLTextureCubeMap();

	virtual bool create(GLImage &bitmap, 
			GLenum format = GL_RGB, 
			bool mipMap = true);
	virtual bool create(const void * data, 
			GLint width, 
			GLint height, 
			GLint components, 
			GLint alignment,
			GLenum format, 
			bool mipMap);

	virtual void draw(bool force = false);

protected:
	virtual bool createTexture(const void * data, 
						GLint width, 
						GLint height, 
						GLint components, 
						GLint alignment,
						GLenum format, 
						bool mipMap);

};

#endif // !defined(AFX_GLTEXTURECUBEMAP_H__490E9736_961A_4873_A10F_ED47E6E33527__INCLUDED_)
