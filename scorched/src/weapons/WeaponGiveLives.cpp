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

#include <weapons/WeaponGiveLives.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveLives);

WeaponGiveLives::WeaponGiveLives()
{

}

WeaponGiveLives::~WeaponGiveLives()
{

}

bool WeaponGiveLives::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("lives", lives_)) return false;

	return true;
}

void WeaponGiveLives::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	context.actionController->addAction(
		new CallbackWeapon(this, 0.0f, 0, 
			playerId, position, velocity, data));
}

void WeaponGiveLives::weaponCallback(
	ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data,
	unsigned int userData)
{
	Tank *tank = context.tankContainer->getTankById(playerId);
	if (!tank) return;

	if (tank->getState().getMaxLives() > 0)
	{
		tank->getState().setLives(
			tank->getState().getLives() + 1);

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("\"%s\" has received %i extra live(s)", 
				tank->getName(), lives_));
			info.setPlayerId(playerId);
			Logger::log(info);
		}
	}
}

