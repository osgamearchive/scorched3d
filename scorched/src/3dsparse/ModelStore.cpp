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
#include <GLEXT/GLTexture.h>
#include <3dsparse/ASEModelFactory.h>
#include <3dsparse/MSModelFactory.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/Model.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>

ModelStore *ModelStore::instance_ = 0;

ModelStore *ModelStore::instance()
{
	if (!instance_)
	{
		instance_ = new ModelStore;
	}
	return instance_;
}

ModelStore::ModelStore()
{
}

ModelStore::~ModelStore()
{
}

GLTexture *ModelStore::loadTexture(const char *name, 
	const char *aname, bool invert)
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
		map = new GLBitmap((char *) name, (char *) aname, invert);
		if (!map->getBits())
		{
			dialogMessage("Scorched3D load model texture", formatString(
						  "Failed to load texture file \"%s\",\n"
						  "alpha file \"%s\"",
						  name,
						  aname));
			return 0;
		}
	}
	else
	{
		map = new GLBitmap((char *) name);
		if (!map->getBits())
		{
			dialogMessage("Scorched3D load model texture", formatString(
						  "Failed to load texture file \"%s\"",
						  name));
			return 0;
		}
	}

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

Model *ModelStore::loadModel(ModelID &modelId)
{
	std::map<std::string, Model *>::iterator findItor =
		fileMap_.find(modelId.getStringHash());
	if (findItor == fileMap_.end())
	{
		Model *model = getModel(modelId);
		fileMap_[modelId.getStringHash()] = model;
		return model;
	}
	else
	{
		return (*findItor).second;
	}
	return 0;
}

Model *ModelStore::getModel(ModelID &id)
{
	Model *model = 0;
	if (0 == strcmp(id.getType(), "ase"))
	{
		// Load the ASEFile containing the tank definitions
		std::string meshName(getDataFile(id.getMeshName()));

		bool noSkin = 
			(0 == strcmp("none", id.getSkinName()));
		ASEModelFactory factory;
		model = factory.createModel(meshName.c_str(), 
			(noSkin?"":getDataFile(id.getSkinName())));
	}
	else
	{
		// Load the Milkshape containing the tank definitions
		std::string meshName(getDataFile(id.getMeshName()));
		MSModelFactory factory;
		model = factory.createModel(meshName.c_str());		
	}

	return model;
}
