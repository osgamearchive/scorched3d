////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <weapons/WeaponGiveLife.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/Tank.h>
#include <target/TargetContainer.h>
#include <target/TargetLife.h>
#include <common/Defines.h>
#include <common/ChannelManager.h>
#include <lang/LangResource.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveLife);

WeaponGiveLife::WeaponGiveLife() :
	WeaponCallback("WeaponGiveLife", "Gives a specified amount of life (health) to the player. It can also be used to reduce the player's health."),
	life_("WeaponGiveLife::life", "Amount of life to give to the player"),
	exceedMax_("If true, player can exceed their maximum health.  "
		"When exceedmax is  true, the value used must be over 100 to give a maximum health over 100")
{
	addChildXMLEntry("life", &life_);
	addChildXMLEntry("exceedmax", &exceedMax_);
}

WeaponGiveLife::~WeaponGiveLife()
{

}

void WeaponGiveLife::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity)
{
	context.getActionController().addAction(
		new CallbackWeapon("WeaponGiveLife", this, 0, 0, 
			weaponContext, position, velocity));
}

void WeaponGiveLife::weaponCallback(
	ScorchedContext &context,
	WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity,
	unsigned int userData)
{
	Tank *tank = context.getTargetContainer().getTankById(weaponContext.getPlayerId());
	if (!tank) return;

	fixed life = life_.getValue(context);
	if (life > 0)
	{
		if (life > tank->getLife().getMaxLife() &&
			exceedMax_.getValue())
		{
			tank->getLife().setMaxLife(life);
		}
		tank->getLife().setLife(
			tank->getLife().getLife() + life);

		{
			ChannelText text("combat", 
				LANG_RESOURCE_2("TANK_GET_LIFE",
				"[p:{0}] received {1} life", 
				tank->getTargetName(), 
				S3D::formatStringBuffer("%.0f", life.asFloat())));
			ChannelManager::showText(context, text);
		}
	}
	else
	{
		if (tank->getLife().getLife() + life <= 0)
		{
			tank->getLife().setLife(
				S3D_MIN(fixed(1), tank->getLife().getLife()));
		}
		else
		{
			tank->getLife().setLife(
				tank->getLife().getLife() + life);
		}

		{
			ChannelText text("combat", 
				LANG_RESOURCE_2("TANK_LOST_LIFE",
				"[p:{0}] lost {1} life", 
				tank->getTargetName(), 
				S3D::formatStringBuffer("%.0f", -life.asFloat())));
			ChannelManager::showText(context, text);
		}
	}
}

