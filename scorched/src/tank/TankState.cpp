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
#include <stdio.h>

TankState::TankState() : 
	state_(sPending), life_(100.0f), power_(1000.0f),
	readyState_(SNotReady), oldPower_(1000.0f)
{
}

TankState::~TankState()
{
}

void TankState::reset()
{
	state_ = sPending;
	readyState_ = SNotReady;
	life_ = 100.0f;
	power_ = 1000.0f;
}

void TankState::nextRound()
{
	readyState_ = SNotReady;
	oldPower_ = power_;
}

void TankState::newGame()
{
	readyState_ = SNotReady;
	state_ = sPending;
	life_ = 100.0f;
	oldPower_ = power_ = 1000.0f;
}

void TankState::setLife(float life)
{
	life_ = life;

	if (life_ >= 100) life_ = 100;
	if (life_ <= 0) life_ = 0;
	if (power_ > life_ * 10.0f) power_ = life_ * 10.0f;
}

float TankState::changePower(float power, bool diff)
{
	if (diff) power_ += power;
	else power_ = power;

	if (power_ < 0.0f) power_ = 0.0f;
	else if (power_ > life_ * 10.0f) power_ = life_ * 10.0f;

	return power_;
}

const char *TankState::getStateString()
{
	static char string[1024];
	sprintf(string, "%s - %s (%i hp)",
		((readyState_==sReady)?"Ready":"NotReady"),
		((state_==sDead)?"Dead":((state_==sNormal)?"Alive":"Pending")),
		(int) life_);
	return string;
}

bool TankState::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) state_);
	buffer.addToBuffer(life_);
	buffer.addToBuffer(power_);
	buffer.addToBuffer(oldPower_);
	return true;
}

bool TankState::readMessage(NetBufferReader &reader)
{
	int s;
	if (!reader.getFromBuffer(s)) return false;
	state_ = (TankState::State) s;
	if (!reader.getFromBuffer(life_)) return false;
	if (!reader.getFromBuffer(power_)) return false;
	if (!reader.getFromBuffer(oldPower_)) return false;
	return true;
}
