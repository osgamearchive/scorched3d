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
#include <tankgraph/TargetRendererImplTarget.h>
#include <target/TargetDefinition.h>
#include <target/Target.h>
#include <common/Defines.h>
#include <XML/XMLNode.h>

TargetDefinition::TargetDefinition() : 
	life_(1.0f), boundingsphere_(true)
{
	shadow_.setDrawShadow(false);
	driveovertodestroy_ = false;
}

TargetDefinition::~TargetDefinition()
{
}

bool TargetDefinition::readXML(XMLNode *node, const char *base)
{
	if (!node->getNamedChild("name", name_)) return false;

	node->getNamedChild("life", life_, false);
	node->getNamedChild("shield", shield_, false);
	node->getNamedChild("parachute", parachute_, false);
	node->getNamedChild("boundingsphere", boundingsphere_, false);

	return PlacementModelDefinition::readXML(node, base);
}

Target *TargetDefinition::createTarget(unsigned int playerId,
	Vector &position,
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
	if (!context.serverMode)
	{
		target->setRenderer(
			new TargetRendererImplTarget(
				target, modelId_, modelburntId_,
				modelscale_));
	}

	target->getLife().setMaxLife(life_);
	target->getLife().setSize(size_);
	target->getLife().setDriveOverToDestroy(driveovertodestroy_);
	target->getLife().setRotation(rotation);
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

	target->setTargetPosition(position);
	return target;
}
