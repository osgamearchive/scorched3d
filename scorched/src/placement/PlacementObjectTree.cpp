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

#include <placement/PlacementObjectTree.h>
#include <landscape/LandscapeObjectsEntryTree.h>
#include <landscape/LandscapeMaps.h>
#include <common/RandomGenerator.h>
#include <engine/ScorchedContext.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
#include <XML/XMLParser.h>

PlacementObjectTree::PlacementObjectTree()
{
}

PlacementObjectTree::~PlacementObjectTree()
{
}

bool PlacementObjectTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("tree", tree)) return false;
	if (!node->getNamedChild("snow", snow)) return false;

	node->getNamedChild("removeaction", removeaction, false);
	node->getNamedChild("burnaction", burnaction, false);
	return PlacementObject::readXML(node);
}

void PlacementObjectTree::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	PlacementType::Information &information,
	PlacementType::Position &position)
{
	bool pine = (0 == strcmp(tree.c_str(), "pine"));

	LandscapeObjectsEntryTree *treeEntry = new LandscapeObjectsEntryTree();
	treeEntry->snow = (pine && 
		(position.position[2] > snow + (generator.getRandFloat() * 10.0f) - 5.0f));
	treeEntry->pine = pine;
	treeEntry->color = generator.getRandFloat() * 0.5f + 0.5f;
	treeEntry->size = generator.getRandFloat() * 2.0f + 1.0f;
	treeEntry->posX = position.position[0];
	treeEntry->posY = position.position[1];
	treeEntry->posZ = position.position[2];
	treeEntry->rotation = position.rotation;
	treeEntry->removeaction = removeaction;
	treeEntry->burnaction = burnaction;
	treeEntry->modelsize = 1.0f;

	context.landscapeMaps->getGroundMaps().getObjects().addObject(
		(unsigned int) position.position[0],
		(unsigned int) position.position[1],
		treeEntry);
}
