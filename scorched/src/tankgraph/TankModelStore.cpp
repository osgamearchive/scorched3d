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
#include <tankgraph/TankModel.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>
#include <3dsparse/ModelsFile.h>
#include <XML/XMLFile.h>

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
	modelCatagories_.insert("All");
}

TankModelStore::~TankModelStore()
{

}

bool TankModelStore::loadTankMeshes(ProgressCounter *counter)
{
	// Load tank definition file
	if (counter) counter->setNewOp("Loading Tank Models");
	XMLFile file;
	if (!file.readFile(getDataFile("data/tanks.xml")))
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
	int count = 0;
    std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> &children = file.getRootNode()->getChildren();
    for (childrenItor = children.begin();
        childrenItor != children.end();
        childrenItor++, count++)
    {
		if (counter) counter->
			setNewPercentage(float(count) / float(children.size()) * 100.0f);
		// Parse the tank entry
        XMLNode *currentNode = (*childrenItor);
		if (stricmp(currentNode->getName(), "tank"))
		{
			dialogMessage("Scorched Models",
						  "Failed to find tank node");
			return false;
		}

		// Get the name of tank
		XMLNode *nameNode;
		if (!currentNode->getNamedChild("name", nameNode)) return false;
		const char *modelName = nameNode->getContent();
		TankModelId id(modelName);

		// Get the model node
		XMLNode *modelNode;
		if (!currentNode->getNamedChild("model", modelNode)) return false;

		// Parse the modelId which tells us which files and
		// 3d type the model actuall is
		// The model files are not parsed until later
		ModelID modelId;
		if (!modelId.initFromNode("data/tanks", modelNode))
		{
			dialogMessage("Scorched Models",
						"Failed to load mesh for tank \"%s\"",
						modelName);
			return false;
		}

		// Create the tank model
		TankModel *model = new TankModel(id, modelId);

		// Get the model file to determine if the file is too large
		ModelsFile *mFile = model->getTankModelID().getModelsFile();
		if (!mFile || !mFile->getSuccess()) return false;		
		if (strcmp(model->getId().getModelName(), "Random") != 0)
		{
			// Check if the model uses too many triangles
			int triangles = mFile->getNumberFaces();
			if (OptionsDisplay::instance()->getTankDetail() == 0)
			{
				if (triangles > 250) continue;
			}
			else if (OptionsDisplay::instance()->getTankDetail() == 1)
			{
				if (triangles > 500) continue;
			}
		}

		// Get the projectile model node (if any)
		XMLNode *projectileModelNode;
		if (currentNode->getNamedChild("projectilemodel", projectileModelNode, false))
		{
			ModelID projModelId;
			if (!projModelId.initFromNode("data/accessories", projectileModelNode))
			{
				dialogMessage("Scorched Models",
							"Failed to load projectile mesh for tank \"%s\"",
							modelName);
				return false;
			}
			model->getProjectileModelID() = projModelId;
		}

		// Read all of the tank display catagories
		std::list<XMLNode *>::iterator catItor;
		for (catItor = currentNode->getChildren().begin();
			catItor != currentNode->getChildren().end();
			catItor++)
		{
			XMLNode *catNode = *catItor;
			if (strcmp(catNode->getName(), "catagory") == 0)
			{
				model->getCatagories().push_back(catNode->getContent());
				modelCatagories_.insert(catNode->getContent());
			}
		}

		// Add this model to the store
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
