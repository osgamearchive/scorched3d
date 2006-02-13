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
#include <3dsparse/Model.h>
#include <3dsparse/ModelStore.h>
#include <XML/XMLFile.h>

TankModelStore::TankModelStore()
{
	modelCatagories_.insert("All");
}

TankModelStore::~TankModelStore()
{

}

bool TankModelStore::loadTankMeshes(ScorchedContext &context, ProgressCounter *counter)
{
	// Load the tank types
	if (!types_.loadTankTypes(context)) return false;

	// Load tank definition file
	if (counter) counter->setNewOp("Loading tanks");
	XMLFile file;
	if (!file.readFile(getDataFile("data/tanks.xml")))
	{
		dialogMessage("Scorched Models", formatString(
					  "Failed to parse tanks.xml\n%s", 
					  file.getParserError()));
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
			return currentNode->returnError("Failed to tank node");
		}

		// Get the name of tank
		XMLNode *nameNode;
		if (!currentNode->getNamedChild("name", nameNode)) return false;
		const char *modelName = nameNode->getContent();
		TargetModelId id(modelName);

		// Get the tank type (if any)
		std::string tankType = "none";
		currentNode->getNamedChild("type", tankType, false);
		TankType *type = types_.getType(tankType.c_str());
		if (!type)
		{
			return currentNode->returnError(
				formatString(
				"Failed to find tank type \"%s\" for tank \"%s\"",
				tankType.c_str(),
				modelName));
		}

		// Get the model node
		XMLNode *modelNode;
		if (!currentNode->getNamedChild("model", modelNode)) return false;

		// Parse the modelId which tells us which files and
		// 3d type the model actuall is
		// The model files are not parsed until later
		ModelID modelId;
		if (!modelId.initFromNode("data/tanks", modelNode))
		{
			return modelNode->returnError(
				formatString("Failed to load mesh for tank \"%s\"",
				modelName));
		}

		// Create the tank model
		TankModel *model = new TankModel(id, modelId, type);

		// Get the projectile model node (if any)
		XMLNode *projectileModelNode;
		if (currentNode->getNamedChild("projectilemodel", projectileModelNode, false))
		{
			ModelID projModelId;
			if (!projModelId.initFromNode("data/accessories", projectileModelNode))
			{
				return projectileModelNode->returnError(
					formatString("Failed to load projectile mesh for tank \"%s\"",
					modelName));
			}
			model->getProjectileModelID() = projModelId;
		}

		// Read all of the tank display catagories
		std::string catagory;
		while (currentNode->getNamedChild("catagory", catagory, false))
		{
			model->addCatagory(catagory.c_str());
			modelCatagories_.insert(catagory.c_str());
		}

		// Read all of the tank team catagories
		int team;
		while (currentNode->getNamedChild("team", team, false))
		{
			model->addTeam(team);
		}

		// Check there are no more nodes in this node
		if (!currentNode->failChildren()) return false;

		// Add this model to the store
		models_.push_back(model);
	}
	return true;
}

TankModel *TankModelStore::getRandomModel(int team)
{
	std::vector<TankModel *> models;
	std::vector<TankModel *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		TankModel *model = (*itor);

		if (strcmp(model->getId().getTankModelName(), "Random") != 0)
		{
			if (model->isOfTeam(team)) models.push_back(model);
		}
	}

	DIALOG_ASSERT(models.size());
	TankModel *model = models_[rand() % models.size()];
	return model;
}

TankModel *TankModelStore::getModelByName(const char *name, int team)
{
	DIALOG_ASSERT(models_.size());

	// A hack to allow the random model
	if (strcmp(name, "Random") == 0) return getRandomModel(team);

	std::vector<TankModel *>::iterator itor;
	for (itor = models_.begin();
		 itor != models_.end();
		 itor++)
	{
		TankModel *current = (*itor);
		if (0 == strcmp(current->getId().getTankModelName(), name) &&
			current->isOfTeam(team))
		{
			return current;
		}
	}

	return getRandomModel(team);
}
