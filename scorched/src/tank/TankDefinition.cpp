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
#include <tank/TankModelStore.h>
#include <tank/TankAvatar.h>
#include <tank/TankState.h>
#include <target/TargetLife.h>
#include <target/TargetParachute.h>
#include <target/TargetShield.h>
#include <tankai/TankAIStore.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <common/RandomGenerator.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <weapons/AccessoryStore.h>
#include <engine/ScorchedContext.h>
#include <server/ScorchedServer.h>

TankDefinition::TankDefinition() :
	team_(0),
	life_(100.0f), boundingsphere_(true),
	size_(2.0f, 2.0f, 2.0f),
	driveovertodestroy_(false)
{
	shadow_.setDrawShadow(false);
}

TankDefinition::~TankDefinition()
{
}

bool TankDefinition::readXML(XMLNode *node, const char *base)
{
	node->getNamedChild("name", name_, false);
	node->getNamedChild("life", life_, false);
	node->getNamedChild("shield", shield_, false);
	node->getNamedChild("parachute", parachute_, false);
	node->getNamedChild("boundingsphere", boundingsphere_, false);

	if (!node->getNamedChild("ai", ai_)) return false;
	if (!node->getNamedChild("model", tankmodel_)) return false;
	node->getNamedChild("team", team_, false);

	node->getNamedChild("driveovertodestroy", driveovertodestroy_, false);
	node->getNamedChild("removeaction", removeaction_, false);
	node->getNamedChild("burnaction", burnaction_, false);

	if (!shadow_.readXML(node, base)) return false;
	if (!groups_.readXML(node)) return false;

	return node->failChildren();
}

Tank *TankDefinition::createTank(unsigned int playerId,
	Vector &position,
	ScorchedContext &context,
	RandomGenerator &generator)
{
	Vector color = TankColorGenerator::getTeamColor(team_);
	if (team_ == 0) color = Vector(0.7f, 0.7f, 0.7f);

	TankModel *model =
		context.tankModelStore->getModelByName(
			tankmodel_.c_str(), 
			team_,
			true);
	if (!model || strcmp("Random", model->getName()) == 0)
	{
		dialogExit("TankDefinition",
			formatString("Cannot find a tank model named \"%s\"",
			tankmodel_.c_str()));
	}

	Tank *tank = new Tank(context, playerId, 0, 
		name_.c_str(), color, model->getName(), model->getTypeName());
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
		if (ai->isHuman())
		{
			dialogExit("TankDefinition",
				formatString("Cannot create a human tank"));
		}

		tank->setTankAI(ai->getCopy(tank));
		tank->setUniqueId(formatString("Tank - %u", playerId));
	}
	else
	{
#ifndef S3D_SERVER
		tank->setRenderer(new TargetRendererImplTank(tank));
#endif
	}

	if (context.optionsGame->getTeams() > 1)
	{
		tank->setTeam(team_);
	}
	tank->getAvatar().loadFromFile("computer.png");
	tank->getLife().setMaxLife(life_);
	tank->getLife().setSize(size_);
	tank->getLife().setDriveOverToDestroy(driveovertodestroy_);
	tank->getState().setState(TankState::sInitializing);
	tank->getState().setState(TankState::sPending);
	tank->newMatch();
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

		tank->getParachute().setCurrentParachute(parachute);
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
	if (burnaction_.c_str()[0] && 0 != strcmp(burnaction_.c_str(), "none"))
	{
		Accessory *action = context.accessoryStore->
			findByPrimaryAccessoryName(burnaction_.c_str());		
		if (!action || action->getType() != AccessoryPart::AccessoryWeapon)
		{
			dialogExit("Scorched3D",
				formatString("Failed to find burn action \"%s\"",
				burnaction_.c_str()));
		}

		tank->setBurnAction((Weapon *) action->getAction());
	}

	tank->getLife().setTargetPosition(position);

	groups_.addToGroups(context, &tank->getGroup(), false);

	return tank;
}
