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

#include <weapons/AccessoryStore.h>
#include <tankgraph/TargetRendererImplTarget.h>
#include <target/TargetDefinition.h>
#include <target/Target.h>
#include <common/Defines.h>
#include <XML/XMLNode.h>

TargetDefinition::TargetDefinition() : 
	life_(1.0f), size_(2.0f), scale_(1.0f), rotation_(0.0f)
{
}

TargetDefinition::~TargetDefinition()
{
}

bool TargetDefinition::readXML(XMLNode *node)
{
	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("name", name_)) return false;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!modelId_.initFromNode("data/accessories", modelnode)) return false;
	if (node->getNamedChild("modelburnt", burntmodelnode, false))
	{
		if (!modelburntId_.initFromNode(".", burntmodelnode)) return false;
	}
	
	node->getNamedChild("life", life_, false);
	node->getNamedChild("size", size_, false);
	node->getNamedChild("scale", scale_, false);
	node->getNamedChild("rotation", rotation_, false);
	node->getNamedChild("shield", shield_, false);
	node->getNamedChild("parachute", parachute_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);
	return true;
}

Target *TargetDefinition::createTarget(unsigned int playerId,
	ScorchedContext &context)
{
	Target *target = new Target(playerId, 
		name_.c_str(), context);
	if (!context.serverMode)
	{
		target->setRenderer(
			new TargetRendererImplTarget(
				target, modelId_, 
				scale_, rotation_));
	}

	target->getLife().setMaxLife(life_);
	target->getLife().setSize(size_);
	target->newGame();

	if (shield_.c_str()[0] && 0 != strcmp(shield_.c_str(), "none"))
	{
		Accessory *shield = context.accessoryStore->
			findByPrimaryAccessoryName(shield_.c_str());
		if (!shield)
		{
			dialogExit("Scorched3D",
				formatString("Failed to find shield named \"%s\"",
				shield_.c_str()));
		}

		target->getShield().setCurrentShield(shield);
	}

	if (parachute_.c_str()[0] && 0 != strcmp(parachute_.c_str(), "none"))
	{
		Accessory *parachute = context.accessoryStore->
			findByPrimaryAccessoryName(parachute_.c_str());
		if (!parachute)
		{
			dialogExit("Scorched3D",
				formatString("Failed to find parachute named \"%s\"",
				parachute_.c_str()));
		}

		target->getParachute().setParachutesEnabled(true);
	}

	// TODO burnaction, burnmodel etc...
	if (removeaction_.c_str()[0] && 0 != strcmp(removeaction_.c_str(), "none"))
	{
		Accessory *action = context.accessoryStore->
			findByPrimaryAccessoryName(removeaction_.c_str());		
		if (!action || action->getType() != AccessoryPart::AccessoryWeapon)
		{
			dialogExit("Scorched3D",
				formatString("Failed to find death action \"%s\"",
				removeaction_.c_str()));
		}

		target->setDeathAction((Weapon *) action->getAction());
	}

	return target;
}
