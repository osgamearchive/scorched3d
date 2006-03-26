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
#include <tankai/TankAIAdder.h>
#include <target/Target.h>
#include <target/TargetRenderer.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>

Target::Target(unsigned int playerId, 
	const char *name,
	ScorchedContext &context) :
	playerId_(playerId),
	name_(name),
	context_(context),
	life_(context, playerId), 
	shield_(context, playerId),
	deathAction_(0),
	renderer_(0)
{
	life_.setTarget(this);
}

Target::~Target()
{
	delete renderer_;
	renderer_ = 0;
}

void Target::newGame()
{
	life_.newGame();
	shield_.newGame();
	parachute_.newGame();
}

bool Target::isTemp()
{
	return (getPlayerId() >= TankAIAdder::MIN_TARGET_ID);
}

bool Target::getAlive()
{
	return (life_.getLife() > 0.0f);
}

void Target::setTargetPosition(Vector &pos)
{
	targetPosition_ = pos;
	life_.setPosition(pos);
	shield_.setPosition(pos);
}

bool Target::writeMessage(NetBuffer &buffer, bool writeAccessories)
{
	buffer.addToBuffer(name_);
	if (!shield_.writeMessage(buffer)) return false;
	if (!life_.writeMessage(buffer)) return false;
	if (!parachute_.writeMessage(buffer, writeAccessories)) return false;

	// Do after shield and life so their position is set
	buffer.addToBuffer(targetPosition_);
	return true;
}

bool Target::readMessage(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(name_)) return false;
	if (!shield_.readMessage(reader)) return false;
	if (!life_.readMessage(reader)) return false;
	if (!parachute_.readMessage(reader)) return false;

	// Do after shield and life so their position is set
	Vector pos;
	if (!reader.getFromBuffer(pos)) return false;
	setTargetPosition(pos);
	return true;
}
