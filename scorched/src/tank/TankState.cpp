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


#include <tank/TankState.h>
#include <common/OptionsDisplay.h>
#include <engine/ScorchedContext.h>
#include <stdio.h>

TankState::TankState(ScorchedContext &context) : 
	state_(sPending), life_(100.0f), power_(1000.0f),
	readyState_(sReady), 
	context_(context), spectator_(false)
{
}

TankState::~TankState()
{
}

void TankState::reset()
{
	state_ = sDead;
	readyState_ = sReady;
	life_ = 100.0f;
	power_ = 1000.0f;
}

void TankState::newGame()
{
	state_ = sDead;
	life_ = 100.0f;
	power_ = 1000.0f;
}

void TankState::clientNewGame()
{
	oldPowers_.clear();
}

void TankState::clientNextShot()
{
	oldPowers_.push_back(power_);
}

void TankState::setLife(float life)
{
	life_ = life;

	if (life_ >= 100) life_ = 100;
	if (life_ <= 0) life_ = 0;
	if (power_ > life_ * 10.0f) power_ = life_ * 10.0f;
}

void TankState::revertPower(unsigned int index)
{
	if (index < oldPowers_.size())
	{
		changePower(oldPowers_[(oldPowers_.size() - 1) - index], false);
	}
}

float TankState::getOldPower()
{
	if (oldPowers_.empty()) return power_;
	return oldPowers_.back();
}

float TankState::changePower(float power, bool diff)
{
	if (diff) power_ += power;
	else power_ = power;

	if (power_ < 0.0f) power_ = 0.0f;
	if (context_.optionsGame.getLimitPowerByHealth())
	{
		if (power_ > life_ * 10.0f) power_ = life_ * 10.0f;
	}
	else
	{
		if (power_ > 1000.0f) power_ = 1000.0f;
	}

	return power_;
}

const char *TankState::getStateString()
{
	static char string[1024];
	sprintf(string, "%s - %s (%i hp)",
		((readyState_==sReady)?"Ready":"NotReady"),
		getSmallStateString(),
		(int) life_);
	return string;
}

const char *TankState::getSmallStateString()
{
	if (spectator_) return "Spectator";
	return ((state_==sDead)?"Dead":((state_==sNormal)?"Alive":"Pending"));
}

const char *TankState::getPowerString()
{
	static char messageBuffer[255];
	if (OptionsDisplay::instance()->getUseHexidecimal())
	{
		sprintf(messageBuffer, "0X%x (0X%x)", 		
				int(getPower()),
				int(getPower() - 
				getOldPower()));
	}
	else
	{
		sprintf(messageBuffer, "%.1f (%+.1f)", 		
				getPower(),
				getPower() - 
				getOldPower());
	}

	return messageBuffer;
}

bool TankState::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) state_);
	buffer.addToBuffer(life_);
	buffer.addToBuffer(power_);
	buffer.addToBuffer(spectator_);
	return true;
}

bool TankState::readMessage(NetBufferReader &reader)
{
	int s;
	if (!reader.getFromBuffer(s)) return false;
	state_ = (TankState::State) s;
	if (!reader.getFromBuffer(life_)) return false;
	if (!reader.getFromBuffer(power_)) return false;
	if (!reader.getFromBuffer(spectator_)) return false;
	return true;
}

