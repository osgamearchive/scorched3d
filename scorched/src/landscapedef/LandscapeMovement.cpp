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

#include <landscapedef/LandscapeMovement.h>
#include <XML/XMLNode.h>
#include <common/Defines.h>

LandscapeMovementType *LandscapeMovementType::create(const char *type)
{
	if (0 == strcmp(type, "boids")) return new LandscapeMovementTypeBoids;
	if (0 == strcmp(type, "ships")) return new LandscapeMovementTypeShips;
	dialogMessage("LandscapeMovementType", formatString("Unknown movement type %s", type));
	return 0;
}

bool LandscapeMovementType::readXML(XMLNode *node)
{
	if (!node->getNamedChild("groupname", groupname)) return false;

	return node->failChildren();
}

bool LandscapeMovementTypeShips::readXML(XMLNode *node)
{
	if (!node->getNamedChild("speed", speed)) return false;
	if (!node->getNamedChild("controlpoints", controlpoints)) return false;
	if (!node->getNamedChild("controlpointswidth", controlpointswidth)) return false;
	if (!node->getNamedChild("controlpointsheight", controlpointsheight)) return false;
	if (!node->getNamedChild("controlpointsrand", controlpointsrand)) return false;
	if (!node->getNamedChild("starttime", starttime)) return false;

	return LandscapeMovementType::readXML(node);
}

bool LandscapeMovementTypeBoids::readXML(XMLNode *node)
{
	if (!node->getNamedChild("minbounds", minbounds)) return false;
	if (!node->getNamedChild("maxbounds", maxbounds)) return false;
	if (!node->getNamedChild("maxvelocity", maxvelocity)) return false;
	if (!node->getNamedChild("cruisedistance", cruisedistance)) return false;
	if (!node->getNamedChild("maxacceleration", maxacceleration)) return false;

	if (maxbounds[0] - minbounds[0] < 25.0f ||
		maxbounds[1] - minbounds[1] < 25.0f ||
		maxbounds[2] - minbounds[2] < 10.0f)
	{
		return node->returnError(
			"Boid bounding box is too small, it must be at least 25x10 units");
	}

	return LandscapeMovementType::readXML(node);
}
