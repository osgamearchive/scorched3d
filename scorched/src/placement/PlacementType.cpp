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

#include <placement/PlacementType.h>
#include <placement/PlacementObject.h>
#include <placement/PlacementTypeMask.h>
#include <placement/PlacementTypeDirect.h>
#include <placement/PlacementTypeTree.h>
#include <placement/PlacementTypeTankStart.h>
#include <landscape/LandscapeObjectsGroupEntry.h>
#include <landscape/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/DefinesString.h>
#include <XML/XMLParser.h>

PlacementType *PlacementType::create(const char *type)
{
	if (0 == strcmp(type, "trees")) return new PlacementTypeTree;
	if (0 == strcmp(type, "mask")) return new PlacementTypeMask;
	if (0 == strcmp(type, "direct")) return new PlacementTypeDirect;
	if (0 == strcmp(type, "tankstart")) return new PlacementTypeTankStart;
	dialogMessage("PlacementType", formatString("Unknown placement type %s", type));
	return 0;
}

PlacementType::PlacementType()
{
}

PlacementType::~PlacementType()
{
}

bool PlacementType::readXML(XMLNode *node)
{
	std::string objecttype;
	XMLNode *objectNode;
	if (!node->getNamedChild("object", objectNode)) return false;
	if (!objectNode->getNamedParameter("type", objecttype)) return false;
	if (!(placementobject = PlacementObject::create(objecttype.c_str()))) return false;
	if (!placementobject->readXML(objectNode)) return false;
	return node->failChildren();
}

void PlacementType::createObjects(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	ProgressCounter *counter)
{
	std::list<Position> returnPositions;
	getPositions(context, generator, returnPositions, counter);

	std::list<Position>::iterator itor;
	for (itor = returnPositions.begin();
		itor != returnPositions.end();
		itor++)
	{
		Position &position = *itor;
		placementobject->createObject(
			context, generator, playerId, position);
	}
}
