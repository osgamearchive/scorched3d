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

#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ScorchedServer.h>
#include <scorched/ServerDialog.h>
#include <coms/ComsGameStoppedMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/OptionsGame.h>
#include <common/Logger.h>

ServerTooFewPlayersStimulus::ServerTooFewPlayersStimulus()
{

}

ServerTooFewPlayersStimulus::~ServerTooFewPlayersStimulus()
{

}

bool ServerTooFewPlayersStimulus::acceptStateChange(const unsigned state, 
													const unsigned nextState,
													float frameTime)
{
	if (ScorchedServer::instance()->getTankContainer().getNoOfTanks() <
		ScorchedServer::instance()->getOptionsGame().getNoMinPlayers())
	{
		// Move all dead + alive tanks to pending
		ScorchedServer::instance()->getTankContainer().resetTanks();

		// Tell any clients why the game has been abandonded
		ComsGameStoppedMessage gameStopped;
		ComsMessageSender::sendToAllConnectedClients(gameStopped);

		Logger::log(0, "Too few players, stopping play");
		sendString(0, "Too few players, stopping play");
		sendString(0,
			"--------------------------------------\n"
			"This server currently has %i players out\n"
			"of a maximum of %i players.\n"
			"This server will only start a game when\n"
			"there are %i or more players.\n"
			"%i more player(s) are required before a \n"
			"game will start.\n"
			"Waiting for more players to join...\n"
			"--------------------------------------\n",
			ScorchedServer::instance()->getTankContainer().getNoOfTanks(),
			ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers(),
			ScorchedServer::instance()->getOptionsGame().getNoMinPlayers(),
			ScorchedServer::instance()->getOptionsGame().getNoMinPlayers() - 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks());

		return true;
	}
	return false;
}
