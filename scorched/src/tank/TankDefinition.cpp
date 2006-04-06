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

#include <tank/TankDefinition.h>
#include <tank/Tank.h>
#include <tank/TankColorGenerator.h>
#include <tankai/TankAIStore.h>
#include <tankgraph/TankModelStore.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <common/RandomGenerator.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <engine/ScorchedContext.h>
#include <server/ScorchedServer.h>

TankDefinition::TankDefinition() :
	team_(0)
{
	usemodel_ = false;
}

TankDefinition::~TankDefinition()
{
}

bool TankDefinition::readXML(XMLNode *node, const char *base)
{
	if (!node->getNamedChild("ai", ai_)) return false;
	if (!node->getNamedChild("model", tankmodel_)) return false;
	node->getNamedChild("team", team_, false);

	return TargetDefinition::readXML(node, base);
}

Tank *TankDefinition::createTank(unsigned int playerId,
	Vector &position,
	ScorchedContext &context,
	RandomGenerator &generator)
{
	Vector color = TankColorGenerator::getTeamColor(team_);
	if (team_ == 0) color = Vector(0.7f, 0.7f, 0.7f);

	TankModel *model =
		context.tankModelStore->getModelByName(tankmodel_.c_str(), team_);
	if (!model || strcmp("Random", model->getName()) == 0)
	{
		dialogExit("TankDefinition",
			formatString("Cannot find a tank model named \"%s\"",
			tankmodel_.c_str()));
	}

	Tank *tank = new Tank(context, playerId, 0, 
		name_.c_str(), color, tankmodel_.c_str());
	tank->getLife().setBoundingSphere(boundingsphere_);

	if (context.serverMode)
	{
		TankAI *ai = ScorchedServer::instance()->getTankAIs().getAIByName(ai_.c_str());
		if (!ai)
		{
			dialogExit("TankDefinition",
				formatString("Cannot find a tank ai named \"%s\"",
				ai_.c_str()));
		}

		tank->setTankAI(ai->getCopy(tank));
		tank->setUniqueId(formatString("Tank - %u", playerId));
	}
	else
	{
		tank->setRenderer(new TargetRendererImplTank(tank));
	}

	if (context.optionsGame->getTeams() > 1)
	{
		tank->setTeam(team_);
	}
	tank->getAvatar().loadFromFile("computer.gif");
	tank->getLife().setMaxLife(life_);
	tank->getLife().setSize(size_);
	tank->getLife().setDriveOverToDestroy(driveovertodestroy_);
	tank->newGame();

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

		tank->getShield().setCurrentShield(shield);
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

		tank->getParachute().setParachutesEnabled(true);
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

		tank->setDeathAction((Weapon *) action->getAction());
	}

	tank->setTargetPosition(position);
	return tank;
}
