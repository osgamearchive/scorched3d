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

#include <landscapedef/LandscapeEvents.h>
#include <engine/ScorchedContext.h>
#include <weapons/AccessoryStore.h>
#include <XML/XMLNode.h>
#include <common/Defines.h>
#include <float.h>

// LandscapeEvent
LandscapeEvent::~LandscapeEvent()
{
	delete condition;
	delete action;
}

bool LandscapeEvent::readXML(XMLNode *node)
{
	{
		XMLNode *conditionNode;
		std::string conditiontype;
		if (!node->getNamedChild("condition", conditionNode)) return false;
		if (!conditionNode->getNamedParameter("type", conditiontype)) return false;
		if (!(condition = LandscapeCondition::create(conditiontype.c_str()))) return false;
		if (!condition->readXML(conditionNode)) return false;
	}
	{
		XMLNode *actionNode;
		std::string actiontype;
		if (!node->getNamedChild("action", actionNode)) return false;
		if (!actionNode->getNamedParameter("type", actiontype)) return false;
		if (!(action = LandscapeAction::create(actiontype.c_str()))) return false;
		if (!action->readXML(actionNode)) return false;
	}	
	
	return node->failChildren();
}

LandscapeCondition *LandscapeCondition::create(const char *type)
{
	if (0 == strcmp(type, "time")) return new LandscapeConditionTime;
	if (0 == strcmp(type, "random")) return new LandscapeConditionRandom;
	dialogMessage("LandscapeCondition", formatString("Unknown condition type %s", type));
	return 0;
}

// LandscapeConditionTime
float LandscapeConditionTime::getNextEventTime(int eventNumber)
{
	if (eventNumber > 1 &&
		singletimeonly)
	{
		return FLT_MAX;
	}

	return RAND * (maxtime - mintime) + mintime;
}

bool LandscapeConditionTime::readXML(XMLNode *node)
{
	if (!node->getNamedChild("mintime", mintime)) return false;
	if (!node->getNamedChild("maxtime", maxtime)) return false;
	if (!node->getNamedChild("singletimeonly", singletimeonly)) return false;
	return node->failChildren();
}

// LandscapeConditionRandom
float LandscapeConditionRandom::getNextEventTime(int eventNumber)
{
	if (eventNumber > 1)
	{
		return FLT_MAX;
	}

	if (RAND < randomchance)
	{
		return randomdelay;
	}
	return FLT_MAX;
}

bool LandscapeConditionRandom::readXML(XMLNode *node)
{
	if (!node->getNamedChild("randomchance", randomchance)) return false;
	if (!node->getNamedChild("randomdelay", randomdelay)) return false;
	return node->failChildren();
}

LandscapeAction *LandscapeAction::create(const char *type)
{
	if (0 == strcmp(type, "fireweapon")) return new LandscapeActionFireWeapon;
	dialogMessage("LandscapeAction", formatString("Unknown action type %s", type));
	return 0;
}

// LandscapeActionFireWeapon
void LandscapeActionFireWeapon::fireAction(ScorchedContext &context)
{
	Accessory *accessory = 
		context.accessoryStore->findByPrimaryAccessoryName(
			weapon.c_str());
	if (!accessory) dialogExit("LandscapeActionFireWeapon",
		formatString("Failed to find weapon named \"%s\"", weapon.c_str()));
	if (accessory->getType() != AccessoryPart::AccessoryWeapon) 
		dialogExit("LandscapeActionFireWeapon",
			formatString("Accessory named \"%s\" is not a weapon", weapon.c_str()));
	Weapon *weapon = (Weapon *) accessory->getAction();

	Vector pos, vel;
	weapon->fireWeapon(context, 0, pos, vel, 
		Weapon::eDataDeathAnimation);
}

bool LandscapeActionFireWeapon::readXML(XMLNode *node)
{
	if (!node->getNamedChild("weapon", weapon)) return false;
	return node->failChildren();
}

LandscapeEvents::LandscapeEvents()
{
}

LandscapeEvents::~LandscapeEvents()
{
	for (unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

bool LandscapeEvents::readXML(LandscapeDefinitions *definitions, XMLNode *node)
{
	{
		XMLNode *eventNode;
		while (node->getNamedChild("event", eventNode, false))
		{
			LandscapeEvent *event = new LandscapeEvent;
			if (!event->readXML(eventNode)) return false;
			objects.push_back(event);
		}
	}
	return node->failChildren();
}
