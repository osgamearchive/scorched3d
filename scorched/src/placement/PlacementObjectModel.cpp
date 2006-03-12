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

#include <placement/PlacementObjectModel.h>
#include <landscape/LandscapeObjectsEntryModel.h>
#include <landscape/LandscapeMaps.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <engine/ScorchedContext.h>
#include <XML/XMLParser.h>

PlacementObjectModel::PlacementObjectModel()
{
}

PlacementObjectModel::~PlacementObjectModel()
{
}

bool PlacementObjectModel::readXML(XMLNode *node)
{
	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!modelId.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("modelburnt", burntmodelnode)) return false;
	if (!modelburntId.initFromNode(".", burntmodelnode)) return false;

	node->getNamedChild("removeaction", removeaction, false);
	node->getNamedChild("burnaction", burnaction, false);

	model = ModelStore::instance()->loadModel(modelId);
	modelburnt = ModelStore::instance()->loadModel(modelburntId);
	if (!model || !modelburnt)
	{
		dialogExit("PlacementObjectModel",
			"Failed to find models");
	}

	Vector sizev = model->getMax() - model->getMin();
	sizev[2] = 0.0f;
	modelsize = sizev.Magnitude();

	return PlacementObject::readXML(node);
}

void PlacementObjectModel::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	PlacementType::Information &information,
	PlacementType::Position &position)
{
	LandscapeObjectsEntryModel *modelEntry = new LandscapeObjectsEntryModel();
	modelEntry->model = new ModelRenderer(model);
	modelEntry->modelburnt = new ModelRenderer(modelburnt);
	modelEntry->color = 1.0f;
	modelEntry->size = position.size;
	modelEntry->posX = position.position[0];
	modelEntry->posY = position.position[1];
	modelEntry->posZ = position.position[2];
	modelEntry->rotation = position.rotation;
	modelEntry->removeaction = removeaction;
	modelEntry->burnaction = burnaction;
	modelEntry->modelsize = modelsize;

	context.landscapeMaps->getGroundMaps().getObjects().addObject(
		(unsigned int) position.position[0],
		(unsigned int) position.position[1],
		modelEntry);
}
