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
#include <client/ClientNewGameHandler.h>
#include <client/ClientState.h>
#include <server/ScorchedServer.h>
#include <common/OptionsParam.h>
#include <common/WindowManager.h>
#include <coms/ComsNewGameMessage.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/ProgressDialog.h>

ClientNewGameHandler *ClientNewGameHandler::instance_ = 0;

ClientNewGameHandler *ClientNewGameHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientNewGameHandler();
	}

	return instance_;
}

ClientNewGameHandler::ClientNewGameHandler()
{
	ScorchedClient::instance()->getComsMessageHandler().addHandler(
		"ComsNewGameMessage",
		this);
}

ClientNewGameHandler::~ClientNewGameHandler()
{

}

bool ClientNewGameHandler::processMessage(unsigned int id,
		const char *messageType,
		NetBufferReader &reader)
{
	ComsNewGameMessage message;
	if (!message.readMessage(reader)) return false;

	// A small hack
	// When playing a single player game and the player(s) have
	// not been choosen simply show the player choosing screen
	// rather than generating the landscape
	bool localPlayers = false;
	if (OptionsParam::instance()->getConnectedToServer() == false)
	{
		if (ScorchedServer::instance()->getTankContainer().getNoOfTanks() -
			ScorchedServer::instance()->getTankContainer().getNoOfNonSpectatorTanks() > 1)
		{
			localPlayers = true;
		}
	}

	if (localPlayers)
	{
		// Stimulate into the next round state
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
		ScorchedClient::instance()->getGameState().checkStimulate();
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimGetPlayers);
		ScorchedClient::instance()->getGameState().checkStimulate();

		// Show player dialog
		WindowManager::instance()->showWindow(PlayerDialog::instance()->getId());	
	}
	else
	{
		// Generate new landscape
		if (!ScorchedClient::instance()->getLandscapeMaps().generateHMapFromDiff(
			ScorchedClient::instance()->getContext(),
			message.getLevelMessage(),
			ProgressDialog::instance()))
		{
			dialogMessage("Scorched3D", "Failed to generate heightmap diff");
			return false;
		}

		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimWait);
		ScorchedClient::instance()->getGameState().checkStimulate();
		ScorchedClient::instance()->getGameState().stimulate(ClientState::StimNewGame);
		ScorchedClient::instance()->getGameState().checkStimulate();
	}
	return true;
}
