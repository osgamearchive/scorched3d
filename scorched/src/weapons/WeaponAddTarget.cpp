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

#include <weapons/WeaponAddTarget.h>
#include <weapons/AccessoryStore.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <actions/AddTarget.h>
#include <engine/ActionController.h>
#include <engine/ScorchedContext.h>
#include <tankai/TankAIAdder.h>
#include <landscape/LandscapeMaps.h>

REGISTER_ACCESSORY_SOURCE(WeaponAddTarget);

WeaponAddTarget::WeaponAddTarget() : 
	shield_(0), parachute_(0), deathAction_(0)
{
}

WeaponAddTarget::~WeaponAddTarget()
{
}

bool WeaponAddTarget::parseXML(OptionsGame &context,
	AccessoryStore *store, XMLNode *accessoryNode)
{
	// Get target model
	XMLNode *modelNode = 0;
	if (accessoryNode->getNamedChild("model", modelNode))
	{
		if (!targetModelId_.initFromNode("data/accessories", modelNode)) return false;
	}

	// Does target have a shield
	std::string shield;
	if (!accessoryNode->getNamedChild("shield", shield)) return false;
	if (0 != strcmp(shield.c_str(), "none"))
	{
		shield_ = (Shield *) store->
			findByPrimaryAccessoryName(shield.c_str())->getAction();
	}

	// Does target have parachutes
	std::string parachute;
	if (!accessoryNode->getNamedChild("parachute", parachute)) return false;
	if (0 != strcmp(parachute.c_str(), "none"))
	{
		parachute_ = (Parachute *) store->
			findByPrimaryAccessoryName(parachute.c_str())->getAction();
	}

	// Get the next weapon
	std::string deathaction;
	if (!accessoryNode->getNamedChild("deathaction", deathaction)) return false;
	if (0 != strcmp(deathaction.c_str(), "none"))
	{
		deathAction_ = (Weapon *) store->
			findByPrimaryAccessoryName(deathaction.c_str())->getAction();
	}

	return accessoryNode->failChildren();
}

void WeaponAddTarget::fireWeapon(ScorchedContext &context, 
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Action *action = new AddTarget(TankAIAdder::getNextTankId(context), position, this);
	context.actionController->addAction(action);
}
