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


#include <client/ClientStartGameHandler.h>
#include <client/ClientState.h>
#include <client/ClientNewGameHandler.h>
#include <landscape/Landscape.h>
#include <tank/TankStart.h>
#include <tank/TankContainer.h>
#include <engine/GameState.h>
#include <engine/ActionController.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/Defines.h>
#include <common/SoundStore.h>
#include <coms/ComsStartGameMessage.h>

ClientStartGameHandler *ClientStartGameHandler::instance_ = 0;

ClientStartGameHandler *ClientStartGameHandler::instance()
{
	if (!instance_)
	{
	  instance_ = new ClientStartGameHandler();
	}

	return instance_;
}

ClientStartGameHandler::ClientStartGameHandler()
{
	ComsMessageHandler::instance()->addHandler(
		"ComsStartGameMessage",
		this);
}

ClientStartGameHandler::~ClientStartGameHandler()
{

}

bool ClientStartGameHandler::processMessage(NetPlayerID &id,
		const char *messageType,
		NetBufferReader &reader)
{
	ComsStartGameMessage message;
	if (!message.readMessage(reader)) return false;

	startGame();
	return true;
}

void ClientStartGameHandler::startClientGame()
{
	if (TankContainer::instance()->aliveCount() < 2)
	{
		// check for all rounds completely finished
		if (OptionsTransient::instance()->getNoRoundsLeft() <= 0)
		{
			// We have finished with all rounds show the score
			GameState::instance()->stimulate(ClientState::StimScore);
		}
		else
		{
			// We have finished with this round, go onto the next round
			ClientNewGameHandler::instance()->clientNewGame();
		}

		return;
	}

	TankStart::nextRound();

	// Find who the next player is
	if (TankContainer::instance()->getCurrentPlayerId() == 0)
	{
		// This is the beginning of a new round,
		// Use the first player in the map as the starting player
		std::map<unsigned, Tank *>::iterator itor =
			TankContainer::instance()->getPlayingTanks().begin();
		TankContainer::instance()->setCurrentPlayerId(
			(*itor).second->getPlayerId());
	}
	else
	{
		// This is not the beginning of a new round
		// So find the first alive player after the currently
		// playing player and make this the current player
		bool found = false;
		bool useNext = false;
		while (!found)
		{
			std::map<unsigned, Tank *>::iterator itor;
			for (itor = TankContainer::instance()->getPlayingTanks().begin();
				(itor != TankContainer::instance()->getPlayingTanks().end()) && !found;
				itor++)
			{
				Tank *current = (*itor).second;

				if (useNext && current->getState().getState() == 
					TankState::sNormal)
				{
					found = true;
					TankContainer::instance()->
						setCurrentPlayerId(current->getPlayerId());
				}
				if (current->getPlayerId() == 
					TankContainer::instance()->getCurrentPlayerId())
				{
					useNext = true;
				}
			}
		}
	}

	startGame();
}

void ClientStartGameHandler::startGame()
{
	// Ensure that the landscape is set to the "proper" texture
	Landscape::instance()->restoreLandscapeTexture();

	// Set the wind for the next shot
	Vector wind;
	if (OptionsTransient::instance()->getWindOn())
	{
		wind = OptionsTransient::instance()->getWindDirection();
		wind *= OptionsTransient::instance()->getWindSpeed() / 2.0f;
	}
	ActionController::instance()->getPhysics().setWind(wind);

	// make sound to tell client a new game is commencing
	Tank *current = TankContainer::instance()->getCurrentTank();
	if (current && current->getTankAI())
	{
		if (current->getTankAI()->isHuman())
		{
			CACHE_SOUND(playSound, PKGDIR "data/wav/misc/play.wav");
			playSound->play();
		}
	}

	// Stimulate into the new game state
	GameState::instance()->stimulate(ClientState::StimBuyWeapons);
}
