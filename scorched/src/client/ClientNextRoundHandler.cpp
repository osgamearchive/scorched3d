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

#include <client/ScorchedClient.h>
#include <client/ClientNextRoundHandler.h>
#include <client/ClientState.h>
#include <landscape/Landscape.h>
#include <coms/ComsNextRoundMessage.h>

ClientNextRoundHandler *ClientNextRoundHandler::instance_ = 0;

ClientNextRoundHandler *ClientNextRoundHandler::instance()
{
	if (!instance_)
	{
		instance_ = new ClientNextRoundHandler;
	}
	return instance_;
}

ClientNextRoundHandler::ClientNextRoundHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsNextRoundMessage",
		this);
}

ClientNextRoundHandler::~ClientNextRoundHandler()
{
}

bool ClientNextRoundHandler::processMessage(unsigned int id,
	const char *messageType,
	NetBufferReader &reader)
{
	// Decode the connect message
	ComsNextRoundMessage message;
	if (!message.readMessage(reader)) return false;

	// Remove trees around tanks
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedClient::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		 itor != tanks.end();
		 itor++)
	{
		Tank *tank = (*itor).second;
		if (tank->getState().getState() == TankState::sNormal)
		{
			Vector &tankPos = tank->getPhysics().getTankPosition();
			for (int x=int(tankPos[0]) - 3; x<=int(tankPos[0])+3; x++)
			{
				for (int y=int(tankPos[1]) - 3; y<=int(tankPos[1])+3; y++)
				{
					Landscape::instance()->getObjects().removeTrees(x, y);
				}
			}
		}
	}

	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
	ScorchedClient::instance()->getGameState().checkStimulate();
	ScorchedClient::instance()->getGameState().stimulate(ClientState::StimReady);
    
	return true;
}
