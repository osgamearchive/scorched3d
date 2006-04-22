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

#include <weapons/WeaponGiveWin.h>
#include <weapons/AccessoryStore.h>
#include <engine/ActionController.h>
#include <tank/TankContainer.h>
#include <tank/TankTeamScore.h>
#include <tank/TankColorGenerator.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>
#include <common/OptionsGame.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveWin);

WeaponGiveWin::WeaponGiveWin()
{

}

WeaponGiveWin::~WeaponGiveWin()
{

}

bool WeaponGiveWin::parseXML(OptionsGame &context, 
	AccessoryStore *store, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, store, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("objective", objective_)) return false;

	return true;
}

void WeaponGiveWin::fireWeapon(ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data)
{
	context.actionController->addAction(
		new CallbackWeapon(this, 0.0f, 0, 
			playerId, position, velocity, data));
}

void WeaponGiveWin::weaponCallback(
	ScorchedContext &context,
	unsigned int playerId, Vector &position, Vector &velocity,
	unsigned int data,
	unsigned int userData)
{
	Tank *tank = context.tankContainer->getTankById(playerId);
	if (!tank) return;

	if (context.optionsGame->getTeams() > 1)
	{
		context.tankTeamScore->setWonGame(tank->getTeam());

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("%s team %s and won the game", 
				TankColorGenerator::getTeamName(tank->getTeam()), 
				objective_.c_str()));
			Logger::log(info);
		}
	}
	else
	{
		tank->getScore().setWonGame();

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("\"%s\" %s and won the game", 
				tank->getName(), objective_.c_str()));
			info.setPlayerId(playerId);
			Logger::log(info);
		}
	}
}

