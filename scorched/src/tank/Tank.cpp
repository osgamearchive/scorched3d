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

#include <math.h>
#include <tank/Tank.h>
#include <tank/TankType.h>
#include <tank/TankColorGenerator.h>
#include <tankgraph/TankModelStore.h>
#include <tankai/TankAIStore.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>

Tank::Tank(ScorchedContext &context, 
		unsigned int playerId, 
		unsigned int destinationId,
		const char *name, 
		Vector &color, 
		const char *modelName,
		const char *typeName) :
	context_(context),
	Target(playerId, name, context), 
	destinationId_(destinationId),
	color_(color), 
	position_(context),
	tankAI_(0),
	score_(context), 
	state_(context, playerId), 
	modelContainer_(modelName, typeName),
	accessories_(context),
	team_(0), 
	ipAddress_(0), 
	keepAlive_(0)
{
	position_.setTank(this);
	state_.setTank(this);
	accessories_.setTank(this);
	modelContainer_.setTank(this);
	state_.setState(TankState::sPending);

	if (context.serverMode) accessories_.newMatch();
}

Tank::~Tank()
{
	delete tankAI_;
}

void Tank::setTankAI(TankAI *ai)
{
	if (tankAI_) delete tankAI_;
	tankAI_ = ai;
}

void Tank::newMatch()
{
	accessories_.newMatch();
	score_.newMatch();
	state_.newMatch();
	if (tankAI_) tankAI_->newMatch();
}

void Tank::newGame()
{
	TankType *tankType = 
		context_.tankModelStore->getTypeByName(
			getModelContainer().getTankTypeName());
	getLife().setMaxLife(tankType->getLife());

	Target::newGame();

	state_.newGame();
	score_.newGame();
	position_.newGame();
	if (tankAI_) tankAI_->newGame();
}

void Tank::rezTank()
{
	if (tankAI_) tankAI_->newGame();
	getState().setState(TankState::sNormal);
	getLife().setLife(getLife().getMaxLife());
}

void Tank::clientNewGame()
{
	position_.clientNewGame();
	state_.clientNewGame();
	score_.clientNewGame();
}

bool Tank::getAlive()
{
	return (getState().getState() == TankState::sNormal &&
		getState().getSpectator() == false);
}

Weapon *Tank::getDeathAction()
{
	setDeathAction(context_.accessoryStore->getDeathAnimation());
	return Target::getDeathAction();
}

Vector &Tank::getColor()
{
	if (team_ > 0) return TankColorGenerator::getTeamColor(team_);
	return color_;
}

bool Tank::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	if (!Target::writeMessage(buffer, writeAccessories)) return false;  // Base class 1st
	buffer.addToBuffer(destinationId_);
	buffer.addToBuffer(team_);
	buffer.addToBuffer(color_);
	if (!state_.writeMessage(buffer)) return false;
	if (!accessories_.writeMessage(buffer, writeAccessories)) return false;
	if (!score_.writeMessage(buffer)) return false;
	if (!position_.writeMessage(buffer)) return false;
	if (!modelContainer_.writeMessage(buffer)) return false;
	return true;
}

bool Tank::readMessage(NetBufferReader &reader)
{
	if (!Target::readMessage(reader)) return false; // Base class 1st
	if (!reader.getFromBuffer(destinationId_)) return false;
	if (!reader.getFromBuffer(team_)) return false;
	if (!reader.getFromBuffer(color_)) return false;
	if (!state_.readMessage(reader)) return false;
	if (!accessories_.readMessage(reader)) return false;
	if (!score_.readMessage(reader)) return false;
	if (!position_.readMessage(reader)) return false;
	if (!modelContainer_.readMessage(reader)) return false;

	if (!context_.serverMode)
	{
		// If any humans turn into computers remove the HumanAI
		if (destinationId_ == 0) setTankAI(0);
	}
	return true;
}
