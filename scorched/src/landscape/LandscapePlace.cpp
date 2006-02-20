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
#include <common/DefinesString.h>
#include <XML/XMLNode.h>

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
}

bool LandscapePlace::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *placementsNode, *placementNode;
		if (!node->getNamedChild("placements", placementsNode)) return false;
		while (placementsNode->getNamedChild("placement", placementNode, false))
		{
			std::string placementtype;
			PlacementType *placement = 0;
			if (!placementNode->getNamedParameter("type", placementtype)) return false;
			if (!(placement = PlacementType::create(placementtype.c_str()))) return false;
			if (!placement->readXML(placementNode)) return false;
			objects.push_back(placement);
		}
		if (!placementsNode->failChildren()) return false;
	}
	return node->failChildren();
}
