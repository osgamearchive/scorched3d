////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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

#include <weapons/WeaponGiveAccessory.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveAccessory);

WeaponGiveAccessory::WeaponGiveAccessory()
{

}

WeaponGiveAccessory::~WeaponGiveAccessory()
{

}

bool WeaponGiveAccessory::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	std::string giveaccessory;
	while (accessoryNode->getNamedChild("giveaccessory", giveaccessory, false))
	{
		Accessory *accessory = store->findByPrimaryAccessoryName(giveaccessory.c_str());
		if (!accessory)
		{
			return accessoryNode->returnError(
				formatString("Failed to find accessory named %s",
					giveaccessory.c_str()));
		}
		giveAccessories_.push_back(accessory);
	}

	if (!accessoryNode->getNamedChild("number", number_)) return false;

	return true;
}

void WeaponGiveAccessory::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	context.actionController->addAction(new PowerUp(playerId, position, velocity, data, this));
}

void WeaponGiveAccessory::invokePowerUp(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	Tank *tank = context.tankContainer->getTankById(playerId);
	if (!tank) return;

	std::vector<Accessory *>::iterator itor;
	for (itor = giveAccessories_.begin();
		itor != giveAccessories_.end();
		itor++)
	{
		Accessory *accessory = (*itor);
		tank->getAccessories().add(accessory, number_);

		LoggerInfo info(LoggerInfo::TypeDeath,
			formatString("\"%s\" received %i %s", 
			number_, accessory->getName()));
		info.setPlayerId(playerId);
		Logger::log(info);
	}
}