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

#include <landscape/LandscapePlace.h>

static LandscapePlaceType *fetchPlacementPlaceType(const char *type)
{
	if (0 == strcmp(type, "trees")) return new LandscapePlaceObjectsPlacementTree;
	if (0 == strcmp(type, "mask")) return new LandscapePlaceObjectsPlacementMask;
	dialogMessage("LandscapePlaceType", "Unknown placement type %s", type);
	return 0;
}

static LandscapePlaceType *fetchObjectPlaceType(const char *type)
{
	if (0 == strcmp(type, "tree")) return new LandscapePlaceObjectsTree;
	if (0 == strcmp(type, "model")) return new LandscapePlaceObjectsModel;
	dialogMessage("LandscapePlaceType", "Unknown object type %s", type);
	return 0;
}

// LandscapePlaceObjectsModel
bool LandscapePlaceObjectsModel::readXML(XMLNode *node)
{
	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!model.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("modelburnt", burntmodelnode)) return false;
	if (!modelburnt.initFromNode(".", burntmodelnode)) return false;
	return node->failChildren();
}

// LandscapePlaceObjectsTree
bool LandscapePlaceObjectsTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("tree", tree)) return false;
	if (!node->getNamedChild("snow", snow)) return false;
	return node->failChildren();
}

// LandscapePlaceObjectsPlacementTree
LandscapePlaceObjectsPlacement::~LandscapePlaceObjectsPlacement()
{
	delete object;
}

bool LandscapePlaceObjectsPlacement::readXML(XMLNode *node)
{
	XMLNode *objectNode;
	if (!node->getNamedChild("object", objectNode)) return false;
	if (!objectNode->getNamedParameter("type", objecttype)) return false;
	if (!(object = fetchObjectPlaceType(objecttype.c_str()))) return false;
	if (!object->readXML(objectNode)) return false;
	return node->failChildren();
}

// LandscapePlaceObjectsPlacementMask
bool LandscapePlaceObjectsPlacementMask::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("mask", mask)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	if (!node->getNamedChild("mincloseness", mincloseness)) return false;
	if (!node->getNamedChild("minslope", minslope)) return false;
	if (!node->getNamedChild("xsnap", xsnap)) return false;
	if (!node->getNamedChild("ysnap", ysnap)) return false;
	if (!node->getNamedChild("angsnap", angsnap)) return false;
	return LandscapePlaceObjectsPlacement::readXML(node);
}

// LandscapePlaceObjectsPlacementTree
bool LandscapePlaceObjectsPlacementTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("numobjects", numobjects)) return false;
	if (!node->getNamedChild("numclusters", numclusters)) return false;
	if (!node->getNamedChild("minheight", minheight)) return false;
	if (!node->getNamedChild("maxheight", maxheight)) return false;
	return LandscapePlaceObjectsPlacement::readXML(node);
}

LandscapePlace::LandscapePlace()
{
}

LandscapePlace::~LandscapePlace()
{
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
	objectstype.clear();
}

bool LandscapePlace::readXML(XMLNode *node)
{
	if (!node->getNamedChild("name", name)) return false;
	{
		XMLNode *placementsNode, *placementNode;
		if (!node->getNamedChild("placements", placementsNode)) return false;
		while (placementsNode->getNamedChild("placement", placementNode, false))
		{
			std::string placementtype;
			LandscapePlaceType *placement = 0;
			if (!placementNode->getNamedParameter("type", placementtype)) return false;
			if (!(placement = fetchPlacementPlaceType(placementtype.c_str()))) return false;
			if (!placement->readXML(placementNode)) return false;
			objects.push_back(placement);
			objectstype.push_back(placementtype);
		}
		if (!placementsNode->failChildren()) return false;
	}
	return node->failChildren();
}
