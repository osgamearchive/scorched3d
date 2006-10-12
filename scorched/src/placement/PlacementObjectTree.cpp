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
#include <landscape/LandscapeObjectsEntryTree.h>
#include <landscape/LandscapeMaps.h>
#include <common/RandomGenerator.h>
#include <common/OptionsDisplay.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <tankgraph/TargetRendererImplTargetTree.h>
#include <target/TargetContainer.h>
#include <target/Target.h>
#include <XML/XMLParser.h>

PlacementObjectTree::PlacementObjectTree() : border_(0.0f)
{
}

PlacementObjectTree::~PlacementObjectTree()
{
}

bool PlacementObjectTree::readXML(XMLNode *node)
{
	static LandscapeObjectsEntryTree treeType;

	if (!node->getNamedChild("tree", tree)) return false;
	if (!node->getNamedChild("snow", snow)) return false;

	if (!treeType.setup(tree.c_str(), false))
	{
		return node->returnError("Unknown tree type");
	}

	node->getNamedChild("border", border_, false);
	node->getNamedChild("removeaction", removeaction, false);
	node->getNamedChild("burnaction", burnaction, false);
	if (!shadow_.readXML(node, ".")) return false;
	if (!group_.readXML(node)) return false;

	return PlacementObject::readXML(node);
}

void PlacementObjectTree::createObject(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	PlacementType::Position &position)
{
	bool useSnow = (position.position[2] > snow + (generator.getRandFloat() * 10.0f) - 5.0f);
	float color = generator.getRandFloat() * 0.5f + 0.5f;
	float size = generator.getRandFloat() * 2.0f + 1.0f;

	if (false)
	{
		Target *target = new Target(playerId++, "", context);
		target->getLife().setBoundingSphere(true);
		if (!context.serverMode)
		{
			target->setRenderer(
				new TargetRendererImplTargetTree(
					target, tree.c_str(), size, color, useSnow));
		}

		Vector vSize = Vector(1.0f, 1.0f, 2.0f) * size;
		target->getLife().setMaxLife(1.0f);
		target->getLife().setSize(vSize);
		target->getLife().setDriveOverToDestroy(true);
		target->getLife().setRotation(generator.getRandFloat() * 360.0f);
		target->setBorder(border_);
		target->newGame();

		if (removeaction.c_str()[0] && 0 != strcmp(removeaction.c_str(), "none"))
		{
			Accessory *action = context.accessoryStore->
				findByPrimaryAccessoryName(removeaction.c_str());		
			if (!action || action->getType() != AccessoryPart::AccessoryWeapon)
			{
				dialogExit("Scorched3D",
					formatString("Failed to find death action \"%s\"",
					removeaction.c_str()));
			}

			target->setDeathAction((Weapon *) action->getAction());
		}

		target->setTargetPosition(position.position);
		context.targetContainer->addTarget(target);
		group_.addToGroups(context, &target->getGroup(), true);
	}
	else
	{
		LandscapeObjectsEntryTree *treeEntry = new LandscapeObjectsEntryTree();
		treeEntry->setup(tree.c_str(), useSnow);
		treeEntry->color = color;
		treeEntry->modelscale = size;
		treeEntry->position = position.position;
		treeEntry->rotation = generator.getRandFloat() * 360.0f;
		treeEntry->removeaction = removeaction;
		treeEntry->burnaction = burnaction;
		treeEntry->border = border_;
		treeEntry->boundingsize = Vector(1.0f, 1.0f, 2.0f) * treeEntry->modelscale;

		context.landscapeMaps->getGroundMaps().getObjects().addObject(
			(unsigned int) position.position[0],
			(unsigned int) position.position[1],
			treeEntry);

		if (context.serverMode ||
			!OptionsDisplay::instance()->getNoTrees())
		{
			context.landscapeMaps->getGroundMaps().getObjects().getShadows().push_back(
				PlacementShadowDefinition::Entry(&shadow_, 
				position.position,
				treeEntry->boundingsize));
		}

		if (!context.serverMode)
		{
			group_.addToGroups(context, treeEntry, true);
		}
	}
}
