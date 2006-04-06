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

#include <placement/PlacementModelDefinition.h>
#include <landscape/LandscapeObjectsEntryModel.h>
#include <common/RandomGenerator.h>
#include <common/Defines.h>
#include <3dsparse/ModelRenderer.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/Model.h>
#include <XML/XMLNode.h>

PlacementModelDefinition::PlacementModelDefinition() :
	size_(2.0f), modelscale_(0.05f), modelrotation_(0.0f),
	modelrotationsnap_(-1.0f), usemodel_(true)
{
}

PlacementModelDefinition::~PlacementModelDefinition()
{
}

LandscapeObjectsEntryModel *PlacementModelDefinition::createModel(
	Vector &position,
	ScorchedContext &context, RandomGenerator &generator)
{
	Model *model = ModelStore::instance()->loadModel(modelId_);
	Model *modelburnt = model;
	if (modelburntId_.modelValid())
	{
		modelburnt = ModelStore::instance()->loadModel(modelburntId_);
	}

	LandscapeObjectsEntryModel *modelEntry = new LandscapeObjectsEntryModel();
	modelEntry->model = new ModelRenderer(model);
	modelEntry->modelburnt = new ModelRenderer(modelburnt);
	modelEntry->color = 1.0f;
	modelEntry->size = modelscale_;
	modelEntry->rotation = modelrotation_;
	modelEntry->removeaction = removeaction_;
	modelEntry->burnaction = burnaction_;
	modelEntry->modelsize = size_;
	if (modelrotationsnap_ > 0.0f)
	{
		modelEntry->rotation = float(int(generator.getRandFloat() * 360.0f) / 
			int(modelrotationsnap_)) * modelrotationsnap_;
	}

	modelEntry->posX = position[0];
	modelEntry->posY = position[1];
	modelEntry->posZ = position[2];

	return modelEntry;
}

bool PlacementModelDefinition::readXML(XMLNode *node, const char *base)
{
	node->getNamedChild("modelscale", modelscale_, false);

	if (usemodel_)
	{
		XMLNode *modelnode, *burntmodelnode;
		if (!node->getNamedChild("model", modelnode)) return false;
		if (!modelId_.initFromNode(base, modelnode)) return false;
		if (node->getNamedChild("modelburnt", burntmodelnode, false))
		{
			if (!modelburntId_.initFromNode(base, burntmodelnode)) return false;
		}
		if (!node->getNamedChild("size", size_, false))
		{
			Model *model = ModelStore::instance()->loadModel(modelId_);
			size_ = MAX(model->getMax()[0] - model->getMin()[0],
				model->getMax()[1] - model->getMin()[1]);
			size_ /= 2.0f;
			size_ *= modelscale_;
		}
	}

	node->getNamedChild("modelrotation", modelrotation_, false);
	node->getNamedChild("modelrotationsnap", modelrotationsnap_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);
	if (!shadow_.readXML(node, base)) return false;

	return node->failChildren();
}
