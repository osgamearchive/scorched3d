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

#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <server/ScorchedServer.h>
#include <server/TurnController.h>
#include <scorched/ServerDialog.h>
#include <common/OptionsGame.h>
#include <common/Timer.h>
#include <common/Logger.h>
#include <coms/ComsNewGameMessage.h>
#include <coms/ComsMessageSender.h>

extern Timer serverTimer;

ServerNewGameState::ServerNewGameState()
{
}

ServerNewGameState::~ServerNewGameState()
{
}

void ServerNewGameState::enterState(const unsigned state)
{
	// Tell clients a new game is starting
	serverLog(0, "Starting new game");
	sendString(0, "Starting a new game.");

	// Setup landscape and tank start pos
	serverLog(0, "Generating landscape");

	// Set all options (wind etc..)
	ScorchedServer::instance()->getContext().optionsTransient.newGame();

	// Move all pending and dead tanks into the fray
	// Set them all to not ready
	ScorchedServer::instance()->getContext().tankContainer.newGame();

	// Generate the new level
	unsigned long seed = rand();
	ScorchedServer::instance()->getContext().landscapeMaps.generateHMap(
		ScorchedServer::instance()->getContext(), seed);

	// Set the start positions for the tanks
	// Must be generated after the level as it alters the
	// level
	calculateStartPosition(ScorchedServer::instance()->getContext());

	// Add pending tanks (all tanks should be pending) into the game
	addTanksToGame(state);

	// Create the player order for this game
	TurnController::instance()->newGame();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	serverTimer.getTimeDifference();

	// Move into the state that waits for players to become ready
	ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNextRound);
}

void ServerNewGameState::addTanksToGame(const unsigned state)
{
	// Check if there are any pending tanks
	// An optimization that is only for the
	// next round state case
	bool pending = false;
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		// Check to see if any tanks are pending being added
		if (tank->getState().getState() == TankState::sPending)
		{
			pending = true;
		}
	}
	if (!pending) return;

	// Generate the level message
	ComsNewGameMessage newGameMessage;
	if (!ScorchedServer::instance()->getLandscapeMaps().generateHMapDiff(
		newGameMessage.getLevelMessage()))
	{
		Logger::log(0, "ERROR: Failed to generate diff");
	}
	serverLog(0, "Finished generating landscape (%i bytes)", 
		newGameMessage.getLevelMessage().getLevelLen());

	// Check if the generated landscape is too large to send to the clients
	if (newGameMessage.getLevelMessage().getLevelLen() >
		(unsigned int) ScorchedServer::instance()->getOptionsGame().getMaxLandscapeSize())
	{
		serverLog(0, "Landscape too large to send to waiting clients.");
		sendString(0, "Landscape too large to send to waiting clients (%i bytes).", 
			newGameMessage.getLevelMessage().getLevelLen());
		return;
	}

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	std::set<unsigned int>::iterator findItor;

	// Tell any pending tanks to join the game
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		// Check to see if any tanks are pending being added
		if (tank->getState().getState() == TankState::sPending)
		{
			if (state == ServerState::ServerStateNewGame)
			{
				// This tank is now playing
				tank->getState().setState(TankState::sNormal);
			}
			else
			{
				// This tank is now playing (but dead)
				tank->getState().setState(TankState::sDead);
			}

			unsigned int destination = tank->getDestinationId();
			findItor = destinations.find(destination);
			if (findItor == destinations.end())
			{
				destinations.insert(destination);
				// Send new game message to clients
				ComsMessageSender::sendToSingleClient(newGameMessage,
					destination);
			}
		}
	}
}

void ServerNewGameState::flattenArea(ScorchedContext &context, Vector &tankPos)
{
	// Flatten a small area around the tank,
	// before the game starts
	for (int x=-2; x<=2; x++)
	{
		for (int y=-2; y<=2; y++)
		{
			int ix = (int) tankPos[0] + x;
			int iy = (int) tankPos[1] + y;
			if (ix >= 0 && iy >= 0 &&
				ix < context.landscapeMaps.getHMap().getWidth() &&
				iy < context.landscapeMaps.getHMap().getWidth())
			{
				context.landscapeMaps.getHMap().getHeight(ix, iy) = tankPos[2];
			}
		}
	}
}

void ServerNewGameState::calculateStartPosition(ScorchedContext &context)
{
	std::map<unsigned int, Tank *> &tanks = 
		context.tankContainer.getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	const int tankBorder = 10;
	for (mainitor = tanks.begin();
		 mainitor != tanks.end();
		 mainitor++)
	{
		Vector tankPos;
		bool tooClose = true;
		float closeness = (float) 
			context.optionsGame.getTankStartCloseness();
		while (tooClose)
		{
			// Find a new position for the tank
			tooClose = false;
			float posX = float (context.landscapeMaps.getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float posY = float (context.landscapeMaps.getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float height = context.landscapeMaps.getHMap().
				getHeight((int) posX, (int) posY);
			tankPos = Vector(posX, posY, height);

			// Make sure not lower than water line
			// And that the tank is not too close to other tanks
			if (tankPos[2] < 5.5f) 
			{
				tooClose = true;
			}
			else
			{
				std::map<unsigned int, Tank *>::iterator itor;
				for (itor = tanks.begin();
					itor != mainitor;
					itor++)
				{
					if ((tankPos - (*itor).second->getPhysics().getTankPosition()).Magnitude() < 
						closeness) 
					{
						tooClose = true;
						break;
					}
				}
			}

			closeness -= 1.0f;
		}

		// Get the height of the tank
		tankPos[2] = context.landscapeMaps.getHMap().getInterpHeight(
			tankPos[0], tankPos[1]);
	
		// Set the starting position of the tank
		flattenArea(context, tankPos);
		(*mainitor).second->getPhysics().setTankPosition(tankPos);
	}
}
