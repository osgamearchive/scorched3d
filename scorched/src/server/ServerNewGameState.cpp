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
#include <common/Clock.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <coms/ComsNextRoundMessage.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsNewGameMessage.h>
#include <coms/ComsMessageSender.h>

extern Clock serverTimer;

ServerNewGameState::ServerNewGameState()
{
}

ServerNewGameState::~ServerNewGameState()
{
}

void ServerNewGameState::enterState(const unsigned state)
{
	StatsLogger::instance()->gameStart();

	// Tell clients a new game is starting
	sendString(0, "Next Round");

	// Setup landscape and tank start pos
	serverLog(0, "Generating landscape");

	// Set all options (wind etc..)
	ScorchedServer::instance()->getContext().optionsTransient.newGame();

	// Move all pending and dead tanks into the fray
	// Set them all to not ready
	ScorchedServer::instance()->getContext().tankContainer.newGame();

	// Check teams are even
	checkTeams();

	// Generate the new level
	LandscapeDefinition &defn = LandscapeDefinitions::instance()->getRandomLandscapeDefn();
	ScorchedServer::instance()->getContext().landscapeMaps.generateHMap(defn);

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

int ServerNewGameState::addTanksToGame(const unsigned state)
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
		if (tank->getState().getState() == TankState::sPending ||
			state == ServerState::ServerStateNewGame)
		{
			pending = true;
		}
	}
	if (!pending) return 0;

	// Generate the level message
	ComsNewGameMessage newGameMessage;
	if (!ScorchedServer::instance()->getLandscapeMaps().generateHMapDiff(
		newGameMessage.getLevelMessage()))
	{
		Logger::log(0, "ERROR: Failed to generate diff");
	}
	serverLog(0, "Finished generating landscape \"%s\" message (%i bytes)", 
		newGameMessage.getLevelMessage().getHmapDefn().name.c_str(),
		newGameMessage.getLevelMessage().getLevelLen());

	// Check if the generated landscape is too large to send to the clients
	if (newGameMessage.getLevelMessage().getLevelLen() >
		(unsigned int) ScorchedServer::instance()->getOptionsGame().getMaxLandscapeSize())
	{
		serverLog(0, "Landscape too large to send to waiting clients.");
		sendString(0, "Landscape too large to send to waiting clients (%i bytes).", 
			newGameMessage.getLevelMessage().getLevelLen());
		return 0;
	}

	// Used to ensure we only send messages to each
	// destination once
	std::set<unsigned int> destinations;
	std::set<unsigned int>::iterator findItor;

	// Tell any pending tanks to join the game
	int count = 0;
	for (itor = tanks.begin();
		itor != tanks.end();
		itor++)
	{
		Tank *tank = (*itor).second;
		// Check to see if any tanks are pending being added
		if (tank->getState().getState() == TankState::sPending ||
			state == ServerState::ServerStateNewGame)
		{
			count++;
			if (tank->getState().getSpectator())
			{
				// This tank is now playing (but dead)
				tank->getState().setState(TankState::sDead);
			}
			else if (state == ServerState::ServerStateNewGame)
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

				// Make sure all clients have the correct state
				ComsGameStateMessage stateMessage;
				ComsMessageSender::sendToSingleClient(stateMessage,
					destination);

				// Make sure client says it is ready
				if (state == ServerState::ServerStateReady)
				{
					ComsNextRoundMessage nrmessage;
					ComsMessageSender::sendToSingleClient(nrmessage, destination);
				}
			}
		}
	}

	return count;
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
		float closeness =
			ScorchedServer::instance()->getLandscapeMaps().getLandDfn().tankStartCloseness;
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
			if (tankPos[2] < ScorchedServer::instance()->getLandscapeMaps().getLandDfn().tankStartHeightMin ||
				tankPos[2] > ScorchedServer::instance()->getLandscapeMaps().getLandDfn().tankStartHeightMax) 
			{
				tooClose = true;
				closeness -= 0.1f;
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
						closeness -= 1.0f;
						break;
					}
				}
			}
			
			// Ensure we never go inifinite
			if (closeness < 1.0f) tooClose = false;
		}

		// Get the height of the tank
		tankPos[2] = context.landscapeMaps.getHMap().getInterpHeight(
			tankPos[0], tankPos[1]);
	
		// Set the starting position of the tank
		flattenArea(context, tankPos);
		(*mainitor).second->getPhysics().setTankPosition(tankPos);
	}
}

void ServerNewGameState::checkTeams()
{
	if (ScorchedServer::instance()->getOptionsGame().getTeams() == 1) return;
	if (!ScorchedServer::instance()->getOptionsGame().getAutoBallanceTeams()) return;

	std::list<Tank *> team1;
	std::list<Tank *> team2;
	std::map<unsigned int, Tank *> &playingTanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks.begin();
		 mainitor != playingTanks.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (!current->getState().getSpectator())
		{
			if (current->getTeam() == 1) team1.push_back(current);
			if (current->getTeam() == 2) team2.push_back(current);
		}
	}

	int offSet = int(team1.size()) - int(team2.size());
	if (abs(offSet) < 2) return;

	sendString(0, "Auto ballancing teams");
	serverLog(0, "Auto ballancing teams");

	offSet /= 2;
	if (offSet < 0)
	{
		for (int i=0; i<abs(offSet); i++)
		{
			Tank *tank = team2.front();
			team2.pop_front();
			tank->setTeam(1);
		}
	}
	else
	{
		for (int i=0; i<abs(offSet); i++)
		{
			Tank *tank = team1.front();
			team1.pop_front();
			tank->setTeam(2);
		}
	}
}
