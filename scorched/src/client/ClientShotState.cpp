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

#include <client/ClientShotState.h>
#include <client/ScorchedClient.h>
#include <engine/ActionController.h>
#include <common/OptionsParam.h>
#include <landscape/Landscape.h>
#include <tank/TankStart.h>

ClientShotState *ClientShotState::instance_ = 0;

ClientShotState *ClientShotState::instance()
{
	if (!instance_)
	{
		instance_ = new ClientShotState;
	}
	return instance_;
}

ClientShotState::ClientShotState()
{
}

ClientShotState::~ClientShotState()
{
}

void ClientShotState::enterState(const unsigned state)
{
	endOfState_ = true;
	Landscape::instance()->restoreLandscapeTexture();
}

bool ClientShotState::acceptStateChange(const unsigned state, 
		const unsigned nextState,
		float frameTime)
{
	// All the shots have finished, move to finished
	if (ScorchedClient::instance()->getActionController().noReferencedActions())
	{
		if (endOfState_ && 
			!OptionsParam::instance()->getConnectedToServer())
		{
			endOfState_ = false;
			if (ScorchedClient::instance()->getTankContainer().aliveCount() < 2)
			{
				// Finished with the round 
				// Next game
				TankStart::scoreWinners(ScorchedClient::instance()->getContext());
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}
