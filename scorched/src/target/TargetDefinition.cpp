#include <weapons/AccessoryStore.h>
#include <target/TargetDefinition.h>
#include <target/Target.h>
#include <common/Defines.h>
#include <XML/XMLNode.h>

TargetDefinition::TargetDefinition() : life_(1.0f)
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
	node->getNamedChild("shield", shield_, false);
	node->getNamedChild("parachute", parachute_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);
	return true;
}

Target *TargetDefinition::createTarget(unsigned int playerId,
	ScorchedContext &context)
{
	TargetModelId targetModelId(modelId_);
	Target *target = new Target(playerId, 
		targetModelId, 
		name_.c_str(), context);
	target->getLife().setMaxLife(life_);
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

	// TODO burnaction, burnmodel, rotation, size etc...
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
