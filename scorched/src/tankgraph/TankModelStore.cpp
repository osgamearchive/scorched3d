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


#include <tankgraph/TankModelStore.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <3dsparse/ASEFile.h>
#include <XML/XMLFile.h>
#include <GLEXT/GLBitmap.h>
#include <wx/utils.h>

TankModelStore *TankModelStore::instance_ = 0;

TankModelStore *TankModelStore::instance()
{
	if (!instance_)
	{
	  instance_ = new TankModelStore();
	}

	return instance_;
}

TankModelStore::TankModelStore()
{

}

TankModelStore::~TankModelStore()
{

}

GLTexture *TankModelStore::loadTexture(const char *name)
{
	// Try to find the texture in the cache first
	std::map<std::string, GLTexture *>::iterator itor =
		skins_.find(name);
	if (itor != skins_.end())
	{
		return (*itor).second;
	}

	// Load tank skin as bitmap
	GLBitmap map((char *) name);
	if (!map.getBits()) return 0;

	// Use smaller tank skins for texture size 0
	// Resize the bitmap
	if (OptionsDisplay::instance()->getTexSize() == 0)
	{
		map.resize(map.getWidth() / 2, 
				   map.getHeight() / 2);
	}

	// Create skin texture from bitmap
	GLTexture *texture = new GLTexture;
	if (!texture->create(map)) return 0;
	skins_[name] = texture;

	return texture;
}

TankMesh *TankModelStore::loadMesh(const char *name)
{
	// Try to find the mesh in the cache first
	std::map<std::string, TankMesh *>::iterator itor =
		meshes_.find(name);
	if (itor != meshes_.end())
	{
		return (*itor).second;
	}

    // Load the ASEFile containing the tank definitions
    ASEFile newFile((char *) name);
    if (!newFile.getSuccess()) return 0;

    // Make sure the tank is not too large
    const float maxSize = 3.0f;
    float size = (newFile.getMax() - newFile.getMin()).Magnitude();
    if (size > maxSize)
    {
        const float sfactor = 2.2f / size;
        newFile.scale(sfactor);
    }

	// Get the model detail
	float detail = 
		float(OptionsDisplay::instance()->getMaxModelTriPercentage()) / 100.0f;

	// Create tank mesh
	TankMesh *tankMesh = new TankMesh(
		newFile, 
		!OptionsDisplay::instance()->getNoSkins(),
		detail);
	meshes_[name] = tankMesh;
	return tankMesh;
}

bool TankModelStore::loadTankMeshes()
{
	// Load tank definition file
	XMLFile file;
    if (!file.readFile(PKGDIR "data/tanks.xml"))
	{
		dialogMessage("Scorched Models", 
					  "Failed to parse tanks.xml\n%s", 
					  file.getParserError());
		return false;
	}

	// Check file exists
	if (!file.getRootNode())
	{
		dialogMessage("Scorched Models",
					"Failed to find tank definition file \"data/tanks.xml\"");
		return false;		
	}

	// Itterate all of the tanks in the file
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++)
    {
		// Parse the tank entry
        XMLNode *currentNode = (*childrenItor);
		if (stricmp(currentNode->getName(), "tank"))
		{
			dialogMessage("Scorched Models",
						  "Failed to find tank node");
			return false;
		}

		XMLNode *nameNode = currentNode->getNamedChild("name");
		if (!nameNode)
		{
			dialogMessage("Scorched Models",
						  "Failed to find name node");
			return false;
		}
		const char *modelName = nameNode->getContent();

		XMLNode *modelNode = currentNode->getNamedChild("model");
		if (!modelNode)
		{
			dialogMessage("Scorched Models",
						  "Failed to find model node");
			return false;
		}

		XMLNode *meshNode = modelNode->getNamedChild("mesh");
		XMLNode *skinNode = modelNode->getNamedChild("skin");
		if (!meshNode || !skinNode)
		{
			dialogMessage("Scorched Models",
						  "Failed to find mesh or skin node for tank \"%s\"",
						  modelName);
			return false;
		}

		const char *skinNameContent = skinNode->getContent();
		static char skinName[1024];
		sprintf(skinName, PKGDIR "data/tanks/%s", skinNameContent);

		const char *meshNameContent = meshNode->getContent();
		static char meshName[1024];
		sprintf(meshName, PKGDIR "data/tanks/%s", meshNameContent);

		if (!::wxFileExists(skinName))
		{
			dialogMessage("Scorched Models",
						"Skin file \"%s\" does not exist for tank \"%s\"",
						skinName, modelName);
			return false;
		}
		if (!::wxFileExists(meshName))
		{
			dialogMessage("Scorched Models",
						  "Mesh file \"%s\"does not exist  for tank \"%s\"",
						  meshName, modelName);
			return false;
		}


		// Create the new model and store in vector
		TankModelId id(modelName);
		TankModel *model = new TankModel(
			id,
			meshName,
			OptionsDisplay::instance()->getNoSkins()?0:skinName);
		models_.push_back(model);
	}
	return true;
}

TankModel *TankModelStore::getRandomModel()
{
	DIALOG_ASSERT(models_.size());

	int pos = (int) (RAND * (float)models_.size());
	TankModel *model = models_[pos % models_.size()];

	if (strcmp(model->getId().getModelName(), "Random") == 0) return getRandomModel();
	return model;
}

TankModel *TankModelStore::getModelByName(const char *name)
{
	DIALOG_ASSERT(models_.size());

	// A hack to allow the random model
	if (strcmp(name, "Random") == 0) return getRandomModel();

	std::vector<TankModel *>::iterator itor;
	for (itor = models_.begin();
		 itor != models_.end();
		 itor++)
	{
		TankModel *current = (*itor);
		if (0 == strcmp(current->getId().getModelName(), name))
		{
			return current;
		}
	}

	return getRandomModel();
}
