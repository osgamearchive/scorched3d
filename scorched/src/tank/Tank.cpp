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


// Tank.cpp: implementation of the Tank class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <common/Defines.h>
#include <tank/Tank.h>
#include <tank/TankColorGenerator.h>

static void removeBadChars(char *string)
{
	for (;*string; string++)
	{
		if (*string == '\"') *string = ' ';
		else if (*string == ']') *string = ' ';
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tank::Tank(unsigned int playerId, const char *name, 
		   Vector &color, TankModelId &modelId)
	: playerId_(playerId),
	  color_(color), 
	  physics_(playerId), model_(modelId), tankAI_(0)
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

void Tank::nextRound()
{
	physics_.nextRound();
	state_.nextRound();
	if (tankAI_) tankAI_->nextRound();
}

void Tank::setUnqiueId(const char *id)
{
	const unsigned int MaxUniqueIdLen = 16;
	if (strlen(id) < MaxUniqueIdLen)
	{
		 uniqueId_ = id;
	}
	else
	{
		uniqueId_.append(id, 16);
	}
	removeBadChars((char *) uniqueId_.c_str());
}

void Tank::setTankName(const char *name)
{
	const unsigned int MaxNameLen = 16;
	if (strlen(name) < MaxNameLen)
	{
		 name_ = name;
	}
	else
	{
		name_.append(name, 16);
	}
	removeBadChars((char *) name_.c_str());
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
