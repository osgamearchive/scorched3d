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

#include <GLEXT/GLBitmap.h>
#include <3dsparse/ASEStore.h>
#include <3dsparse/ASEFile.h>
#include <common/OptionsDisplay.h>

ASEStore *ASEStore::instance_ = 0;

ASEStore *ASEStore::instance()
{
	if (!instance_)
	{
		instance_ = new ASEStore;
	}
	return instance_;
}

ASEStore::ASEStore()
{
}

ASEStore::~ASEStore()
{
}

GLTexture *ASEStore::loadTexture(const char *name, const char *aname)
{
	std::string wholeName;
	wholeName += name;
	wholeName += "::";
	wholeName += aname;

	// Try to find the texture in the cache first
	std::map<std::string, GLTexture *>::iterator itor =
		skins_.find(wholeName);
	if (itor != skins_.end())
	{
		return (*itor).second;
	}

	// Load tank skin as bitmap
	GLBitmap *map = 0;
	if (aname[0])
	{
		map = new GLBitmap((char *) name, (char *) aname);
	}
	else
	{
		map = new GLBitmap((char *) name);
	}
	if (!map->getBits()) return 0;

	// HACK for skin creator
#ifdef dDOUBLE
	// Use smaller tank skins for texture size 0
	// Resize the bitmap
	if (OptionsDisplay::instance()->getTexSize() == 0)
	{
		map->resize(map->getWidth() / 2, 
				   map->getHeight() / 2);
	}
#endif

	// Create skin texture from bitmap
	GLenum format = GL_RGB;
	if (aname[0]) format = GL_RGBA;
	GLTexture *texture = new GLTexture;
	if (!texture->create(*map, format)) return 0;
	delete map;

	skins_[wholeName] = texture;
	return texture;
}

GLVertexArray *ASEStore::loadOrGetArray(const char *fileName)
{
	std::map<const char *, GLVertexArray *>::iterator findItor =
		fileMap_.find(fileName);
	if (findItor == fileMap_.end())
	{
		ASEFile file(fileName, "NOTEXTURE");
		if (file.getSuccess())
		{
			file.centre();
			GLVertexArray *array = file.getModels().front()->getArray(false);
			fileMap_[fileName] = array;
			return array;
		}
	}
	else
	{
		return (*findItor).second;
	}
	return 0;
}
