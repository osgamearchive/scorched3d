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

#include <server/ServerBuyingState.h>
#include <server/ServerShotHolder.h>
#include <server/ScorchedServer.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>

ServerBuyingState::ServerBuyingState() : time_(0.0f)
{
}

ServerBuyingState::~ServerBuyingState()
{
}

void ServerBuyingState::enterState(const unsigned state)
{
	// Set the wait timer to the current time
	time_ = 0.0f;
}

bool ServerBuyingState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// Check if the time to make the shots has expired
	time_ += frameTime;
	if (ScorchedServer::instance()->getOptionsGame().getBuyingTime() > 0)
	{
		if (time_ > ScorchedServer::instance()->getOptionsGame().getBuyingTime())
		{
			return true;
		}
	}

	// Or we already have all shots
	if (ServerShotHolder::instance()->haveAllTurnShots())
	{
		return true;
	}

	return false;
}
