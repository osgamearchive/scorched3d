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

#include <server/ServerNextTurnState.h>
#include <server/ServerState.h>
#include <server/ScorchedServer.h>
#include <server/TurnController.h>
#include <scorched/ServerDialog.h>
#include <tankai/TankAIComputer.h>
#include <coms/ComsStartGameMessage.h>
#include <coms/ComsMessageSender.h>
#include <common/Logger.h>

ServerNextTurnState::ServerNextTurnState()
{
}

ServerNextTurnState::~ServerNextTurnState()
{
}

void ServerNextTurnState::enterState(const unsigned state)
{
	serverLog(0, "NextTurn");

	TurnController::instance()->nextTurn();
	if (TurnController::instance()->getPlayersThisTurn().empty())
	{
		// No more turns left, play the shot
		ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusShot);
	}
	else
	{
		bool weaponBuy = 
			(ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo() == 0);

		// Tell the players to play the turn
		std::list<unsigned int>::iterator itor;
		for (itor = TurnController::instance()->getPlayersThisTurn().begin();
			itor != TurnController::instance()->getPlayersThisTurn().end();
			itor++)
		{
			unsigned int playerId = (*itor);

			Tank *tank =
				ScorchedServer::instance()->getTankContainer().getTankById(playerId);
			DIALOG_ASSERT(tank);
			DIALOG_ASSERT(tank->getState().getState() == TankState::sNormal);

			if (tank->getDestinationId() == 0) // This is a local tank
			{
				if (weaponBuy)
				{
					// Tell the computer ai to buy weapons
					((TankAIComputer *) tank->getTankAI())->buyAccessories();
					((TankAIComputer *) tank->getTankAI())->autoDefense();
				}
				else
				{
					// Tell the computer ai to move
					tank->getTankAI()->playMove(state, 0.0f, 0, 0);
				}
			}
			else
			{
				sendStringMessage(tank->getDestinationId(), "%s's Move", tank->getName());

				// Tell the clients to start the game
				ComsStartGameMessage startMessage(tank->getPlayerId(), weaponBuy);
				ComsMessageSender::sendToSingleClient(startMessage, tank->getDestinationId());
			}
		}

		if (weaponBuy)
		{
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusBuying);
		}
		else
		{
			ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusPlaying);
		}
	}
}
