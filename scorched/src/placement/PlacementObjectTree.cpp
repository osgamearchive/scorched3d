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
#include <landscapemap/LandscapeMaps.h>
#include <common/RandomGenerator.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <tankgraph/TargetRendererImplTargetTree.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <target/TargetState.h>
#include <XML/XMLParser.h>

PlacementObjectTree::PlacementObjectTree() : 
	border_(0.0f),
	minsize_(1.0f), maxsize_(3.0f)
{
}

PlacementObjectTree::~PlacementObjectTree()
{
}

bool PlacementObjectTree::getTypes(const char *type, bool snow, 
	TreeType &normalType, TreeType &burntType)
{
	if (0 == strcmp(type, "pine"))
	{
		normalType = (snow?ePineSnow:ePineNormal);
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine2"))
	{
		normalType = (snow?ePine2Snow:ePine2);
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine3"))
	{
		normalType = (snow?ePine3Snow:ePine3);;
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "pine4"))
	{
		normalType = (snow?ePine4Snow:ePine4);;
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "burntpine"))
	{
		normalType = ePineBurnt;
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "yellowpine"))
	{
		normalType = (snow?ePineSnow:ePineYellow);
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "lightpine"))
	{
		normalType = (snow?ePineSnow:ePineLight);
		burntType = ePineBurnt;
	}
	else if (0 == strcmp(type, "palm"))
	{
		normalType = ePalmNormal;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm2"))
	{
		normalType = ePalm2;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm3"))
	{
		normalType = ePalm3;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palm4"))
	{
		normalType = ePalm4;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb"))
	{
		normalType = ePalmB;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb2"))
	{
		normalType = ePalmB2;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb3"))
	{
		normalType = ePalmB3;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb4"))
	{
		normalType = ePalmB4;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb5"))
	{
		normalType = ePalmB5;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb6"))
	{
		normalType = ePalmB6;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "palmb7"))
	{
		normalType = ePalmB7;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "burntpalm"))
	{
		normalType = ePalmBurnt;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak"))
	{
		normalType = eOak;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak2"))
	{
		normalType = eOak2;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak3"))
	{
		normalType = eOak3;
		burntType = ePalmBurnt;
	}
	else if (0 == strcmp(type, "oak4"))
	{
		normalType = eOak4;
		burntType = ePalmBurnt;
	}
	else return false;
	return true;
}


bool PlacementObjectTree::readXML(XMLNode *node)
{
	if (!node->getNamedChild("tree", tree_)) return false;
	if (!node->getNamedChild("snow", snow_)) return false;

	TreeType tmp1, tmp2;
	if (!getTypes(tree_.c_str(), true, tmp1, tmp2))
	{
		return node->returnError("Unknown tree type");
	}

	node->getNamedChild("border", border_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);
	node->getNamedChild("minsize", minsize_, false);
	node->getNamedChild("maxsize", maxsize_, false);

	if (!shadow_.readXML(node, ".")) return false;
	if (!group_.readXML(node)) return false;

	return PlacementObject::readXML(node);
}

void PlacementObjectTree::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	PlacementType::Position &position)
{
	++playerId;

	bool useSnow = (position.position[2] > snow_ + (generator.getRandFloat() * 10.0f) - 5.0f);
	float color = generator.getRandFloat() * 0.5f + 0.5f;
	float size = generator.getRandFloat() * (maxsize_ - minsize_) + minsize_;
	float rotation = generator.getRandFloat() * 360.0f;

	{
		Target *target = new Target(playerId, "", context);
		target->getLife().setBoundingSphere(true);

		TreeType normalType, burntType;
		getTypes(tree_.c_str(), useSnow, normalType, burntType);

#ifndef S3D_SERVER
		if (!context.serverMode)
		{
			target->setRenderer(
				new TargetRendererImplTargetTree(
					target, size, color, rotation, normalType, burntType));
		}
#endif

		Vector vSize = Vector(1.0f, 1.0f, 1.0f) * size;
		target->getLife().setMaxLife(1.0f);
		target->getLife().setSize(vSize);
		target->getLife().setDriveOverToDestroy(true);
		target->getTargetState().setDisplayDamage(false);
		target->getTargetState().setNoDamageBurn(true);
		target->getLife().setRotation(generator.getRandFloat() * 360.0f);
		target->setBorder(border_);
		target->newGame();

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
		if (burnaction_.c_str()[0] && 0 != strcmp(burnaction_.c_str(), "none"))
		{
			Accessory *action = context.accessoryStore->
				findByPrimaryAccessoryName(burnaction_.c_str());		
			if (!action || action->getType() != AccessoryPart::AccessoryWeapon)
			{
				dialogExit("Scorched3D",
					formatString("Failed to find burn action \"%s\"",
					burnaction_.c_str()));
			}

			target->setBurnAction((Weapon *) action->getAction());
		}

		target->getLife().setTargetPosition(position.position);
		context.targetContainer->addTarget(target);
		group_.addToGroups(context, &target->getGroup(), true);

		context.landscapeMaps->getGroundMaps().getGroups().getShadows().push_back(
			PlacementShadowDefinition::Entry(
			&shadow_,
			position.position,
			vSize));
	}
}
