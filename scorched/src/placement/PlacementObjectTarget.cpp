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

#include <placement/PlacementObjectTarget.h>
#include <landscape/LandscapeObjectsEntryModel.h>
#include <landscape/LandscapeMaps.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
#include <XML/XMLParser.h>

PlacementObjectTarget::PlacementObjectTarget()
{
}

PlacementObjectTarget::~PlacementObjectTarget()
{
}

bool PlacementObjectTarget::readXML(XMLNode *node)
{
	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!modelId_.initFromNode("data/accessories", modelnode)) return false;
	//if (!node->getNamedChild("modelburnt", burntmodelnode)) return false;
	//if (!modelburntId_.initFromNode(".", burntmodelnode)) return false;
	if (!node->getNamedChild("name", name_)) return false;
	if (!node->getNamedChild("shield", shield_)) return false;
	if (!node->getNamedChild("parachute", parachute_)) return false;

	return PlacementObject::readXML(node);
}

void PlacementObjectTarget::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int playerId,
	PlacementType::Information &information,
	PlacementType::Position &position)
{
	TargetModelId targetModelId(modelId_);
	Target *target = new Target(++playerId, 
		targetModelId, 
		name_.c_str(), context);
	target->newGame();
	target->setTargetPosition(position.position);

	if (0 != strcmp(shield_.c_str(), "none"))
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

	if (0 != strcmp(parachute_.c_str(), "none"))
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

	// TODO burnaction, burnmodel, rotation, size etc...
	if (0 != strcmp(information.removeaction.c_str(), "none"))
	{
		Accessory *action = context.accessoryStore->
			findByPrimaryAccessoryName(information.removeaction.c_str());		
		if (!action || action->getType() != AccessoryPart::AccessoryWeapon)
		{
			dialogExit("Scorched3D",
				formatString("Failed to find death action \"%s\"",
				information.removeaction.c_str()));
		}

		target->setDeathAction((Weapon *) action->getAction());
	}

	context.targetContainer->addTarget(target);
}
