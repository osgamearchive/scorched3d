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

Tank::Tank(ScorchedContext &context, 
		   unsigned int playerId, 
		   unsigned int destinationId,
		   const char *name, 
		   Vector &color, TankModelId &modelId)
	: playerId_(playerId), destinationId_(destinationId),
	  color_(color), 
	  physics_(context, playerId), model_(modelId), tankAI_(0),
	  score_(context), state_(context)
{
	setTankName(name);
}

Tank::~Tank()
{
	TankColorGenerator::instance()->returnColor(color_);
	delete tankAI_;
}

void Tank::reset()
{
	accessories_.reset();
	score_.reset();
	state_.reset();
}

void Tank::newGame()
{
	accessories_.newGame();
	physics_.newGame();
	state_.newGame();
	if (tankAI_) tankAI_->newGame();
}

void Tank::nextShot()
{
	physics_.nextShot();
	state_.nextShot();
	if (tankAI_) tankAI_->nextShot();
}

void Tank::setUnqiueId(const char *id)
{
	uniqueId_ = id;
}

void Tank::setTankName(const char *name)
{
	name_ = name;
}

bool Tank::writeMessage(NetBuffer &buffer)
{
	if (!physics_.writeMessage(buffer)) return false;
	if (!state_.writeMessage(buffer)) return false;
	if (!accessories_.writeMessage(buffer)) return false;
	if (!score_.writeMessage(buffer)) return false;

	return true;
}

bool Tank::readMessage(NetBufferReader &reader)
{
	if (!physics_.readMessage(reader)) return false;
	if (!state_.readMessage(reader)) return false;
	if (!accessories_.readMessage(reader)) return false;
	if (!score_.readMessage(reader)) return false;
	return true;
}
