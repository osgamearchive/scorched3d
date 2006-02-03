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
	return PlacementObject::readXML(node);
}

void PlacementObjectTree::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int playerId,
	PlacementType::Position &position)
{
	std::string name = tree;
	if (position.position[2] < snow)
	{
		name += "snow";
	}

	TargetModelId targetModelId(name.c_str(), true);
	Target *target = new Target(
		playerId, 
		targetModelId, 
		"", 
		context);
	target->newGame();
	target->setTargetPosition(position.position);

	context.targetContainer->addTarget(target);
}
