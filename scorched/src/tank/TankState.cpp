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

#include <stdio.h>
#include <tank/Tank.h>
#include <tank/TankAdmin.h>
#include <engine/ScorchedContext.h>
#include <common/OptionsDisplay.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>

TankState::TankState(ScorchedContext &context, unsigned int playerId) : 
	state_(sPending), tank_(0),
	readyState_(sReady), admin_(0),
	context_(context), spectator_(false), 
	loading_(false), initializing_(false),
	muted_(false), adminTries_(0),
	skipshots_(false), 
	lives_(1), maxLives_(1)
{
}

TankState::~TankState()
{
}

void TankState::newMatch()
{
	setState(sDead);
	readyState_ = sReady;
}

void TankState::newGame()
{
	setState(sNormal);
	if (!tank_->isTemp())
	{
		maxLives_ = context_.optionsGame->getPlayerLives();
	}

	lives_ = maxLives_;
}

void TankState::clientNewGame()
{
	skipshots_ = false;
}

void TankState::setState(State s)
{
	if (loading_) return;

	state_ = s;

 	if (state_ != sNormal)
	{
		// Make sure the target and shield physics
		// are disabled
		tank_->getLife().setLife(0.0f);
		tank_->getShield().setCurrentShield(0);
	}
}

void TankState::setAdmin(TankAdmin *admin)
{ 
	delete admin_; 
	admin_ = admin; 
}

const char *TankState::getStateString()
{
	static char string[1024];
	snprintf(string, 1024, "%s - %s %s(%i hp)",
		((readyState_==sReady)?"Rdy":"Wait"),
		getSmallStateString(),
		(muted_?"muted ":""),
		(int) tank_->getLife().getLife());
	return string;
}

const char *TankState::getSmallStateString()
{
	if (loading_) return "Loading";
	if (initializing_) return "Initializing";
	if (spectator_) return "Spectator";
	return ((state_==sDead)?"Dead":((state_==sNormal)?"Alive":"Pending"));
}

bool TankState::writeMessage(NetBuffer &buffer)
{
	buffer.addToBuffer((int) state_);
	buffer.addToBuffer(spectator_);
	buffer.addToBuffer(lives_);
	buffer.addToBuffer(maxLives_);
	return true;
}

bool TankState::readMessage(NetBufferReader &reader)
{
	int s;
	if (!reader.getFromBuffer(s)) return false;
	setState((TankState::State) s);
	if (!reader.getFromBuffer(spectator_)) return false;
	if (!reader.getFromBuffer(lives_)) return false;
	if (!reader.getFromBuffer(maxLives_)) return false;
	return true;
}
