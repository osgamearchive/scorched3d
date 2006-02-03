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
#include <engine/ScorchedContext.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
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
	if (!model.initFromNode(".", modelnode)) return false;
	if (!node->getNamedChild("modelburnt", burntmodelnode)) return false;
	if (!modelburnt.initFromNode(".", burntmodelnode)) return false;
	node->getNamedChild("name", name, false);
	return PlacementObject::readXML(node);
}

void PlacementObjectModel::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int playerId,
	PlacementType::Position &position)
{
	TargetModelId targetModelId(model);
	Target *target = new Target(
		playerId, 
		targetModelId, 
		name.c_str(), context);
	target->newGame();
	target->setTargetPosition(position.position);

	/*if (addTarget_->getShield())
	{
		target->getShield().setCurrentShield(
			addTarget_->getShield()->getParent());
	}
	if (addTarget_->getParachute())
	{
		target->getParachute().setParachutesEnabled(true);
	}
	if (addTarget_->getDeathAction())
	{
		target->setDeathAction(addTarget_->getDeathAction());
	}*/

	context.targetContainer->addTarget(target);
}
