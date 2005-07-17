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
#include <common/Defines.h>
#include <tank/Tank.h>
#include <tank/TankColorGenerator.h>
#include <tankai/TankAIStore.h>
#include <engine/ScorchedContext.h>

Tank::Tank(ScorchedContext &context, 
		   unsigned int playerId, 
		   unsigned int destinationId,
		   const char *name, 
		   Vector &color, TankModelId &modelId)
	: playerId_(playerId), destinationId_(destinationId),
	  color_(color), context_(context),
	  physics_(context, playerId), model_(modelId), tankAI_(0),
	  score_(context), state_(context, playerId), accessories_(context),
	  name_(name), team_(0), ipAddress_(0), keepAlive_(0)
{
	physics_.setTank(this);
	state_.setTank(this);
	state_.setState(TankState::sPending);
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

void Tank::reset()
{
	accessories_.reset();
	score_.reset();
	state_.reset();
	if (tankAI_) tankAI_->reset();
}

void Tank::newGame()
{
	accessories_.newGame();
	state_.newGame();
	if (tankAI_) tankAI_->newGame();
}

void Tank::clientNewGame()
{
	physics_.clientNewGame();
	state_.clientNewGame();
}

Vector &Tank::getColor()
{
	static Vector red(1.0f, 0.0f, 0.0f);
	static Vector blue(0.0f, 0.3f, 1.0f);
	static Vector green(0.0f, 1.0f, 0.0f);
	static Vector yellow(1.0f, 1.0f, 0.0f);
	if (team_ == 1) return red;
	else if (team_ == 2) return blue;
	else if (team_ == 3) return green;
	else if (team_ == 4) return yellow;
	return color_;
}

bool Tank::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer(name_);
	buffer.addToBuffer(destinationId_);
	buffer.addToBuffer(team_);
	buffer.addToBuffer(color_[0]);
	buffer.addToBuffer(color_[1]);
	buffer.addToBuffer(color_[2]);
	if (!model_.writeMessage(buffer)) return false;
	if (!physics_.writeMessage(buffer)) return false;
	if (!state_.writeMessage(buffer)) return false;
	if (!accessories_.writeMessage(buffer)) return false;
	if (!score_.writeMessage(buffer)) return false;
	return true;
}

bool Tank::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name_)) return false;
	if (!reader.getFromBuffer(destinationId_)) return false;
	if (!reader.getFromBuffer(team_)) return false;
	if (!reader.getFromBuffer(color_[0])) return false;
	if (!reader.getFromBuffer(color_[1])) return false;
	if (!reader.getFromBuffer(color_[2])) return false;
	if (!model_.readMessage(reader)) return false;
	if (!physics_.readMessage(reader)) return false;
	if (!state_.readMessage(reader)) return false;
	if (!accessories_.readMessage(reader)) return false;
	if (!score_.readMessage(reader)) return false;

	if (!context_.serverMode)
	{
		// If any humans turn into computers remove the HumanAI
		if (destinationId_ == 0) setTankAI(0);
	}
	return true;
}
