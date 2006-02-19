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

#include <placement/PlacementTypeDirect.h>
#include <landscape/LandscapeMaps.h>
#include <engine/ScorchedContext.h>
#include <common/ProgressCounter.h>
#include <XML/XMLParser.h>

PlacementTypeDirect::PlacementTypeDirect()
{
}

PlacementTypeDirect::~PlacementTypeDirect()
{
}

bool PlacementTypeDirect::readXML(XMLNode *node)
{
	XMLNode *positionNode;
	while (node->getNamedChild("position", positionNode, false))
	{
		Position position;
		if (!positionNode->getNamedChild("position", position.position)) return false;
		if (!positionNode->getNamedChild("rotation", position.rotation)) return false;
		if (!positionNode->getNamedChild("size", position.size)) return false;
		positions.push_back(position);
	}
	return PlacementType::readXML(node);
}

void PlacementTypeDirect::getPositions(ScorchedContext &context,
	RandomGenerator &generator,
	std::list<Position> &returnPositions,
	ProgressCounter *counter)
{
	std::list<Position>::iterator itor;
	int i = 0;
	for (itor = positions.begin();
		itor != positions.end();
		itor++, i++)
	{
		if (i % 10 == 0) if (counter) 
			counter->setNewPercentage(float(i)/float(positions.size())*100.0f);

		Position position = (*itor);
		float height = 
			context.landscapeMaps->
				getGroundMaps().getInterpHeight(position.position[0], position.position[1]);
		if (position.position[2] == 0.0f) position.position[2] = height;

		returnPositions.push_back(position);
	}
}