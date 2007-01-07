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
#include <common/RandomGenerator.h>
#include <tankgraph/TargetRendererImplTargetModel.h>
#include <target/TargetDefinition.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <target/TargetShield.h>
#include <target/TargetParachute.h>
#include <target/TargetState.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/Model.h>
#include <common/Defines.h>
#include <XML/XMLNode.h>

TargetDefinition::TargetDefinition() : 
	life_(1.0f), boundingsphere_(true),
	size_(2.0f, 2.0f, 2.0f), 
	modelscale_(0.05f), modelrotation_(0.0f), modelrotationsnap_(-1.0f),
	driveovertodestroy_(false), border_(0.0f), 
	displaydamage_(true), displayshadow_(true), 
	nodamageburn_(false), nocollision_(false)
{
	shadow_.setDrawShadow(false);
}

TargetDefinition::~TargetDefinition()
{
}

bool TargetDefinition::readXML(XMLNode *node, const char *base)
{
	node->getNamedChild("name", name_, false);
	node->getNamedChild("life", life_, false);
	node->getNamedChild("shield", shield_, false);
	node->getNamedChild("parachute", parachute_, false);
	node->getNamedChild("boundingsphere", boundingsphere_, false);
	node->getNamedChild("nocollision", nocollision_, false);
	node->getNamedChild("nodamageburn", nodamageburn_, false);
	node->getNamedChild("displaydamage", displaydamage_, false);
	node->getNamedChild("displayshadow", displayshadow_, false);

	node->getNamedChild("modelscale", modelscale_, false);
	node->getNamedChild("modelrotation", modelrotation_, false);
	node->getNamedChild("modelrotationsnap", modelrotationsnap_, false);
	node->getNamedChild("border", border_, false);

	XMLNode *modelnode, *burntmodelnode;
	if (!node->getNamedChild("model", modelnode)) return false;
	if (!modelId_.initFromNode(base, modelnode)) return false;
	if (node->getNamedChild("modelburnt", burntmodelnode, false))
	{
		if (!modelburntId_.initFromNode(base, burntmodelnode)) return false;
	}
	else
	{
		modelnode->resurrectRemovedChildren();
		if (!modelburntId_.initFromNode(base, modelnode)) return false;
	}
	if (!node->getNamedChild("size", size_, false))
	{
		Model *model = ModelStore::instance()->loadModel(modelId_);
		size_ = model->getMax() - model->getMin();
		size_ *= modelscale_;
	}

	node->getNamedChild("driveovertodestroy", driveovertodestroy_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);

	if (!shadow_.readXML(node, base)) return false;
	if (!groups_.readXML(node)) return false;

	return node->failChildren();
}

Target *TargetDefinition::createTarget(unsigned int playerId,
	Vector &position,
	Vector &velocity,
	ScorchedContext &context,
	RandomGenerator &generator)
{
	Target *target = new Target(playerId, 
		name_.c_str(), context);
	target->getLife().setBoundingSphere(boundingsphere_);

	float rotation = modelrotation_;
	if (modelrotationsnap_ > 0.0f)
	{
		rotation = float(int(generator.getRandFloat() * 360.0f) / 
			int(modelrotationsnap_)) * modelrotationsnap_;
	}
#ifndef S3D_SERVER
	if (!context.serverMode)
	{
		target->setRenderer(
			new TargetRendererImplTargetModel(
				target, modelId_, modelburntId_,
				modelscale_));
	}
#endif // #ifndef S3D_SERVER

	target->getTargetState().setNoCollision(nocollision_);
	target->getTargetState().setDisplayDamage(displaydamage_);
	target->getTargetState().setDisplayShadow(displayshadow_);
	target->getTargetState().setNoDamageBurn(nodamageburn_);
	target->getLife().setMaxLife(life_);
	target->getLife().setSize(size_);
	target->getLife().setVelocity(velocity);
	target->getLife().setDriveOverToDestroy(driveovertodestroy_);
	target->getLife().setRotation(rotation);
	target->setBorder(border_);
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

		target->getParachute().setCurrentParachute(parachute);
	}

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

	target->getLife().setTargetPosition(position);

	groups_.addToGroups(context, &target->getGroup(), false);

	return target;
}
