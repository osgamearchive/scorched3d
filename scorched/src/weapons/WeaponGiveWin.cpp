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
#include <tank/TankScore.h>
#include <common/Defines.h>
#include <common/LoggerI.h>
#include <common/Logger.h>
#include <common/OptionsScorched.h>

REGISTER_ACCESSORY_SOURCE(WeaponGiveWin);

WeaponGiveWin::WeaponGiveWin() :
	winningTeam_(0)
{

}

WeaponGiveWin::~WeaponGiveWin()
{

}

bool WeaponGiveWin::parseXML(AccessoryCreateContext &context, XMLNode *accessoryNode)
{
	if (!Weapon::parseXML(context, accessoryNode)) return false;

	if (!accessoryNode->getNamedChild("objective", objective_)) return false;

	accessoryNode->getNamedChild("winningteam", winningTeam_);

	return true;
}

void WeaponGiveWin::fireWeapon(ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity)
{
	context.actionController->addAction(
		new CallbackWeapon(this, 0.0f, 0, 
			weaponContext, position, velocity));
}

void WeaponGiveWin::weaponCallback(
	ScorchedContext &context,
	WeaponFireContext &weaponContext, Vector &position, Vector &velocity,
	unsigned int data)
{
	if (context.optionsGame->getTeams() > 1)
	{
		int team = winningTeam_;
		if (team == 0) 
		{
			Tank *tank = context.tankContainer->getTankById(weaponContext.getPlayerId());
			if (!tank) return;

			team = tank->getTeam();
		}
		context.tankTeamScore->setWonGame(team);

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("%s team %s and won the game", 
				TankColorGenerator::getTeamName(team), 
				objective_.c_str()));
			Logger::log(info);
		}
	}
	else
	{
		Tank *tank = context.tankContainer->getTankById(weaponContext.getPlayerId());
		if (!tank) return;

		tank->getScore().setWonGame();

		if (!context.serverMode)
		{
			LoggerInfo info(LoggerInfo::TypeDeath,
				formatString("\"%s\" %s and won the game", 
				tank->getName(), objective_.c_str()));
			info.setPlayerId(weaponContext.getPlayerId());
			Logger::log(info);
		}
	}
}

