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
#include <server/ServerMessageHandler.h>
#include <server/TurnController.h>
#include <server/ServerCommon.h>
#include <client/ClientSave.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/Clock.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <tankai/TankAIAdder.h>
#include <tank/TankContainer.h>
#include <weapons/EconomyStore.h>
#include <coms/ComsNextRoundMessage.h>
#include <coms/ComsGameStateMessage.h>
#include <coms/ComsNewGameMessage.h>
#include <coms/ComsMessageSender.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/LandscapeDefn.h>
#include <landscape/LandscapeDefinitions.h>
#include <landscape/DeformLandscape.h>
#include <GLEXT/GLBitmap.h>
#include <set>

extern Clock serverTimer;

ServerNewGameState::ServerNewGameState()
{
}

ServerNewGameState::~ServerNewGameState()
{
}

void ServerNewGameState::enterState(const unsigned state)
{
	std::list<Tank *> currentTanks;
	std::map<unsigned int, Tank *> &playingTanks =
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator playingTanksItor;
	for (playingTanksItor = playingTanks.begin();
		playingTanksItor != playingTanks.end();
		playingTanksItor++)
	{
		Tank *tank = (*playingTanksItor).second;
		currentTanks.push_back(tank);
	}
	StatsLogger::instance()->gameStart(currentTanks);

	// Tell clients a new game is starting
	ServerCommon::sendString(0, "Next Round");

	// Make any enconomic changes
	EconomyStore::instance()->getEconomy()->calculatePrices();
	EconomyStore::instance()->getEconomy()->savePrices();

	// Make sure the most uptodate options have been used
	bool optionsChanged = 
		ScorchedServer::instance()->getOptionsGame().commitChanges();

	// Set all options (wind etc..)
	ScorchedServer::instance()->getContext().optionsTransient->newGame();

	// Check if we can load/save a game
	if (OptionsParam::instance()->getConnectedToServer() == false)
	{
		if (ScorchedServer::instance()->getTankContainer().getNoOfTanks() == 0 ||
			ScorchedServer::instance()->getTankContainer().getNoOfTanks() -
			ScorchedServer::instance()->getTankContainer().getNoOfNonSpectatorTanks() > 1)
		{
			// We have not loaded players yet
			// this is the very first landscape
		}
		else
		{
			// Not the first landscape
			if (ClientSave::stateRestored())
			{
				ClientSave::restoreClient(false, true);
				ClientSave::setStateNotRestored();
			}
			else
			{
				ClientSave::storeClient();
			}
		}
	}

	// Setup landscape and tank start pos
	ServerCommon::serverLog(0, "Generating landscape");
	// Move all pending and dead tanks into the fray
	// Set them all to not ready
	ScorchedServer::instance()->getContext().tankContainer->newGame();

	// Check teams are even
	checkTeams();

	// Check that we dont have too many bots
	checkBots();

	// Generate the new level
	LandscapeDefinition *defn = ScorchedServer::instance()->getLandscapes().getRandomLandscapeDefn(
		*ScorchedServer::instance()->getContext().optionsGame);
	if (!defn)
	{
		dialogExit("ServerNewGameState", 
			"No available landscapes are configured for the server.");
	}
	ScorchedServer::instance()->getContext().landscapeMaps->generateHMap(defn);

	ServerCommon::serverLog(0, "Finished generating landscape (%s, %s)", 
		ScorchedServer::instance()->getLandscapeMaps().getLandDfn()->getDefn()->name.c_str(),
		ScorchedServer::instance()->getLandscapeMaps().getLandDfn()->getTex()->name.c_str());

	// Set the start positions for the tanks
	// Must be generated after the level as it alters the
	// level
	calculateStartPosition(defn->getDefn()->tankstart, 
		defn->getDefn()->tankstarttype.c_str(), 
		ScorchedServer::instance()->getContext());

	// Add pending tanks (all tanks should be pending) into the game
	addTanksToGame(state, optionsChanged);

	// Create the player order for this game
	TurnController::instance()->newGame();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	serverTimer.getTimeDifference();

	// Move into the state that waits for players to become ready
	ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNextRound);
}

int ServerNewGameState::addTanksToGame(const unsigned state,
									   bool addState)
{
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;

	// Check if there are any pending tanks
	// An optimization that is only for the
	// next round state case
	if (state != ServerState::ServerStateNewGame)
	{
		bool pending = false;
		for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
		{
			Tank *tank = (*itor).second;
			// Check to see if any tanks are pending being added
			if (tank->getState().getState() == TankState::sPending &&
				!tank->getState().getLoading())
			{
				pending = true;
			}
		}
		if (!pending) return 0;
	}

	// Generate the level message
	ComsNewGameMessage newGameMessage;
	if (addState)
	{
		// Tell client(s) of game settings changes
		ServerCommon::serverLog(0, "Sending a new game state");
		ServerCommon::sendString(0, "Game options have been changed!");
		newGameMessage.addGameState(); 
	}
	if (!ScorchedServer::instance()->getLandscapeMaps().generateHMapDiff(
		newGameMessage.getLevelMessage()))
	{
		Logger::log(0, "ERROR: Failed to generate diff");
	}

	// Check if the generated landscape is too large to send to the clients
	if (newGameMessage.getLevelMessage().getLevelLen() >
		(unsigned int) ScorchedServer::instance()->getOptionsGame().getMaxLandscapeSize())
	{
		ServerCommon::serverLog(0, "Landscape too large to send to waiting clients.");
		ServerCommon::sendString(0, "Landscape too large to send to waiting clients (%i bytes).", 
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
		if (!tank->getState().getLoading() &&
			(tank->getState().getState() == TankState::sPending ||
			state == ServerState::ServerStateNewGame))
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

void ServerNewGameState::calculateStartPosition(
	LandscapeDefnType *defn,
	const char *type,
	ScorchedContext &context)
{
	Vector tankPos;
	float minHeight = -1000.0f;
	float maxHeight = 1000.0f;
	float tankCloseness = 0.0f;
	GLBitmap tankMask(256, 256, false);

	if (0 == strcmp(type, "height"))
	{
		LandscapeDefnStartHeight *height = 
			(LandscapeDefnStartHeight *) defn;
		minHeight = height->heightmin;
		maxHeight = height->heightmax;
		tankCloseness = height->startcloseness;
		if (!height->startmask.empty())
		{
			tankMask.loadFromFile(getDataFile(height->startmask.c_str()));
			DIALOG_ASSERT(tankMask.getBits());
		}
	}
	else
	{
		dialogExit("ServerNewGameState",
			"Failed to find tank start type \"%s\"",
			type);
	}

	std::map<unsigned int, Tank *> &tanks = 
		context.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	const int tankBorder = 10;
	for (mainitor = tanks.begin();
		 mainitor != tanks.end();
		 mainitor++)
	{
		Tank *tank = (*mainitor).second;

		bool tooClose = true;
		float closeness = tankCloseness;
		while (tooClose)
		{
			tooClose = false;

			// Find a new position for the tank
			float posX = float (context.landscapeMaps->getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float posY = float (context.landscapeMaps->getHMap().getWidth() - tankBorder * 2) * 
				RAND + float(tankBorder);
			float height = context.landscapeMaps->getHMap().
				getHeight((int) posX, (int) posY);
			tankPos = Vector(posX, posY, height);

			// Make sure not lower than water line
			if (tankPos[2] < minHeight ||
				tankPos[2] > maxHeight) 
			{
				tooClose = true;
				closeness -= 0.1f;
			}
			
			// Make sure the mask allows the tank
			if (!tooClose)
			{
				// Find the mask position
				int maskX = int(posX * float(tankMask.getWidth())) / 
					context.landscapeMaps->getHMap().getWidth();
				int maskY = int(posY * float(tankMask.getHeight())) / 
					context.landscapeMaps->getHMap().getWidth();
				unsigned char *maskPos = tankMask.getBits() +
					maskY * 3 + maskX * tankMask.getWidth() * 3;

				switch(tank->getTeam())
				{
				case 0:
					if (maskPos[0] == 0 && maskPos[2] == 0)
					{
						tooClose = true;
						closeness -= 0.1f;
					}
					break;
				case 1:
					if (maskPos[0] == 0)
					{
						tooClose = true;
						closeness -= 0.1f;
					}
					break;
				case 2:
					if (maskPos[2] == 0)
					{
						tooClose = true;
						closeness -= 0.1f;
					}
					break;
				}
			}

			// Make sure the tank is not too close to other tanks
			if (!tooClose)
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
		tankPos[2] = context.landscapeMaps->getHMap().getInterpHeight(
			tankPos[0], tankPos[1]);
	
		// Set the starting position of the tank
		DeformLandscape::flattenArea(context, tankPos);
		tank->getPhysics().setTankPosition(tankPos);
	}
}

void ServerNewGameState::checkTeams()
{
	// Make sure everyone is in a team if they should be
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
			if (ScorchedServer::instance()->getOptionsGame().getTeams() > 1 &&
				current->getTeam() == 0) current->setTeam(1); 
			if (ScorchedServer::instance()->getOptionsGame().getTeams() == 1 &&
				current->getTeam() > 0) current->setTeam(0); 
		}
	}

	// Do we check teams
	if (ScorchedServer::instance()->getOptionsGame().getTeams() == 1) return;

	// Do we auto ballance
	if (!ScorchedServer::instance()->getOptionsGame().getAutoBallanceTeams()) return;

	// Count players in each team
	std::list<Tank *> team1;
	std::list<Tank *> team2;
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

	// Check if teams are ballanced
	int offSet = int(team1.size()) - int(team2.size());
	if (abs(offSet) < 2) return;

	ServerCommon::sendString(0, "Auto ballancing teams");
	ServerCommon::serverLog(0, "Auto ballancing teams");

	// Ballance the teams
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

void ServerNewGameState::checkBots()
{
	int requiredPlayers =
		ScorchedServer::instance()->getOptionsGame().
			getRemoveBotsAtPlayers();
	if (requiredPlayers == 0)
	{
		// Check if this feature is turned off
		return;
	}

	// Add up the number of tanks that are either
	// human and not spectators
	// or an ai
	int noPlayers = 0;
	std::map<unsigned int, Tank *> &playingTanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = playingTanks.begin();
		mainitor != playingTanks.end();
		mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (!current->getState().getSpectator() ||
			current->getDestinationId() == 0)
		{
			noPlayers++;
		}
	}

	if (noPlayers > requiredPlayers)
	{
		std::multimap<unsigned int, unsigned int> ais_;

		// Get this list of computer players and sort them
		// by the time they have been playing for
		for (mainitor = playingTanks.begin();
			mainitor != playingTanks.end();
			mainitor++)
		{
			Tank *current = (*mainitor).second;
			if (current->getDestinationId() == 0)
			{
				unsigned int startTime = (unsigned int)
					current->getScore().getStartTime();
				ais_.insert(std::pair<unsigned int, unsigned int>
					(startTime, current->getPlayerId()));
			}
		}

		// Kick the ais that have been on the server the longest
		std::multimap<unsigned int, unsigned int>::reverse_iterator
			aiItor = ais_.rbegin();
		while (noPlayers > requiredPlayers)
		{
			if (aiItor != ais_.rend())
			{
				std::pair<unsigned int, unsigned int> item = *aiItor;
				ServerMessageHandler::instance()->destroyPlayer(item.second);
				aiItor++;
			}
			noPlayers--;
		}
	}
	else if (noPlayers < requiredPlayers)
	{
		std::multimap<std::string, unsigned int> ais_;

		// Get this list of computer players and sort them
		// by ai name
		for (mainitor = playingTanks.begin();
			mainitor != playingTanks.end();
			mainitor++)
		{
			Tank *current = (*mainitor).second;
			if (current->getDestinationId() == 0)
			{
				ais_.insert(std::pair<std::string, unsigned int>
					(current->getTankAI()->getName(), 
					current->getPlayerId()));
			}
		}

		// Add any computer players that should be playing 
		// and that are not in the list of currently playing
		int maxComputerAIs = 
			ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers();
		for (int i=0; i<maxComputerAIs; i++)
		{
			const char *playerType = 
				ScorchedServer::instance()->getOptionsGame().getPlayerType(i);
			if (0 != stricmp(playerType, "Human"))
			{
				std::multimap<std::string, unsigned int>::iterator findItor =
					ais_.find(playerType);
				if (findItor == ais_.end())
				{
					if (noPlayers < requiredPlayers)
					{
						// This player does not exist add them
						TankAIAdder::addTankAI(*ScorchedServer::instance(),
							playerType, "Random", "", true);
						noPlayers++;
					}
				}
				else
				{
					// This player does exist dont add them
					ais_.erase(findItor);
				}
			}
		}
	}
}
