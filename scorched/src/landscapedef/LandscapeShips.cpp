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

#include <landscapedef/LandscapeShips.h>
#include <XML/XMLNode.h>
#include <common/Defines.h>

bool LandscapeShip::readXML(XMLNode *node)
{
	XMLNode *modelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!model.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("scale", scale)) return false;
	return node->failChildren();
}

bool LandscapeShipGroup::readXML(XMLNode *node)
{
	if (!node->getNamedChild("speed", speed)) return false;
	if (!node->getNamedChild("controlpoints", controlpoints)) return false;
	if (!node->getNamedChild("controlpointswidth", controlpointswidth)) return false;
	if (!node->getNamedChild("controlpointsheight", controlpointsheight)) return false;
	if (!node->getNamedChild("controlpointsrand", controlpointsrand)) return false;
	if (!node->getNamedChild("starttime", starttime)) return false;

	{
		XMLNode *shipNode;
		while (node->getNamedChild("ship", shipNode, false))
		{
			LandscapeShip *ship = new LandscapeShip;
			if (!ship->readXML(shipNode)) return false;
			ships.push_back(ship);
		}
	}

	return node->failChildren();
}

LandscapeShips::LandscapeShips()
{
}

LandscapeShips::~LandscapeShips()
{
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

bool LandscapeShips::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *shipgroupNode;
		while (node->getNamedChild("shipgroup", shipgroupNode, false))
		{
			LandscapeShipGroup *shipgroup = new LandscapeShipGroup;
			if (!shipgroup->readXML(shipgroupNode)) return false;
			objects.push_back(shipgroup);
		}
	}
	return node->failChildren();
}
