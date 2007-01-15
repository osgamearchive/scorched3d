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

#include <weapons/WeaponGiveLife.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <target/TargetLife.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveLife);

WeaponGiveLife::WeaponGiveLife()
{

}

WeaponGiveLife::~WeaponGiveLife()
{

}

bool WeaponGiveLife::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("life", life_)) return false;
	if (!accessoryNode->getNamedChild("exceedmax", exceedMax_)) return false;

	return true;
}

void WeaponGiveLife::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity)
{
	context.actionController->addAction(
		new CallbackWeapon(this, 0.0f, 0, 
			weaponContext, position, velocity));
}

void WeaponGiveLife::weaponCallback(
	ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity,
	unsigned int userData)
{
	Tank *tank = context.tankContainer->getTankById(weaponContext.getPlayerId());
	if (!tank) return;

	if (life_ > 0.0f)
	{
		if (life_ > tank->getLife().getMaxLife() &&
			exceedMax_)
		{
			tank->getLife().setMaxLife(life_);
		}
		tank->getLife().setLife(
			tank->getLife().getLife() + life_);

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("\"%s\" received %.0f life", 
				tank->getName(), life_));
			info.setPlayerId(weaponContext.getPlayerId());
			Logger::log(info);
		}
	}
	else
	{
		if (tank->getLife().getLife() + life_ <= 0.0f)
		{
			tank->getLife().setLife(
				MIN(1.0f, tank->getLife().getLife()));
		}
		else
		{
			tank->getLife().setLife(
				tank->getLife().getLife() + life_);
		}

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("\"%s\" lost %.0f life", 
				tank->getName(), -life_));
			info.setPlayerId(weaponContext.getPlayerId());
			Logger::log(info);
		}
	}
}

