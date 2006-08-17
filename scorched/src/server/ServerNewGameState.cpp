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

#include <set>
#include <server/ServerNewGameState.h>
#include <server/ServerState.h>
#include <server/ScorchedServer.h>
#include <server/ServerMessageHandler.h>
#include <server/TurnController.h>
#include <server/ServerCommon.h>
#include <client/ClientSave.h>
#include <tankai/TankAIAdder.h>
#include <tank/TankContainer.h>
#include <tank/TankSort.h>
#include <tank/TankTeamScore.h>
#include <tank/TankDeadContainer.h>
#include <tankgraph/TankModelStore.h>
#include <weapons/EconomyStore.h>
#include <coms/ComsNewGameMessage.h>
#include <coms/ComsMessageSender.h>
#include <landscape/LandscapeMaps.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeDefinitions.h>
#include <landscape/DeformLandscape.h>
#include <landscape/HeightMapSender.h>
#include <GLEXT/GLBitmap.h>
#include <common/OptionsTransient.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <common/Clock.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <common/Defines.h>
#include <algorithm>

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
	ScorchedServer::instance()->getContext().tankTeamScore->newGame();

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
	ServerCommon::serverLog( "Generating landscape");

	// Remove any old targets
	removeTargets();

	// Check teams are even
	checkTeams();

	// Check that we dont have too many bots
	checkBots();

	// Generate the new level
	LandscapeDefinition defn = ScorchedServer::instance()->getLandscapes().getRandomLandscapeDefn(
		*ScorchedServer::instance()->getContext().optionsGame);
	ScorchedServer::instance()->getContext().landscapeMaps->generateMaps(
		ScorchedServer::instance()->getContext(), defn);

	// Set the start positions for the tanks
	// Must be generated after the level as it alters the
	// level
	calculateStartPosition(ScorchedServer::instance()->getContext());

	// Add pending tanks (all tanks should be pending) into the game
	addTanksToGame(state, optionsChanged);

	// Create the player order for this game
	TurnController::instance()->newGame();

	// As we have not returned to the main loop for ages the
	// timer will have a lot of time in it
	// Get rid of this time so we don't screw things up
	serverTimer.getTimeDifference();

	// Move into the state that waits for players to become ready
	ScorchedServer::instance()->getGameState().stimulate(ServerState::ServerStimulusNewGameReady);
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
				!tank->getState().getLoading() &&
				!tank->getState().getInitializing())
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
		ServerCommon::sendString(0, "Game options have been changed!");
		newGameMessage.addGameState(); 
	}

	// Add the height map info
	newGameMessage.getLevelMessage().createMessage(
		ScorchedServer::instance()->getLandscapeMaps().getDefinitions().getDefinition());
	if (!HeightMapSender::generateHMapDiff(
		ScorchedServer::instance()->getLandscapeMaps().getGroundMaps().getHeightMap(),
		newGameMessage.getLevelMessage().getHeightMap()))
	{
		Logger::log( "ERROR: Failed to generate diff");
	}
	LandscapeDefinitionCache &definitions =
		ScorchedServer::instance()->getLandscapeMaps().getDefinitions();
	ServerCommon::serverLog(
		formatString("Finished generating landscape (%u, %s, %s)", 
		definitions.getSeed(), 
		definitions.getDefinition().getDefn(), 
		definitions.getDefinition().getTex()));

	// Check if the generated landscape is too large to send to the clients
	if (newGameMessage.getLevelMessage().getHeightMap().getLevelLen() >
		(unsigned int) ScorchedServer::instance()->getOptionsGame().getMaxLandscapeSize())
	{
		ServerCommon::sendString(0, formatString("Landscape too large to send to waiting clients (%i bytes).", 
			newGameMessage.getLevelMessage().getHeightMap().getLevelLen()));
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
			!tank->getState().getInitializing() &&
			(tank->getState().getState() == TankState::sPending ||
			state == ServerState::ServerStateNewGame))
		{
			count++;

			// This is the very first time this tank
			// has played the game, load it with the starting
			// weapons etc...
			if (tank->getState().getState() == TankState::sPending)
			{
				tank->newMatch();

				// Check if this is a bot
				// if not update from any residual tank we have.
				// Residual tanks are only available until the next
				// whole game starts.
				if (tank->getDestinationId() != 0)
				{
					Tank *savedTank = ScorchedServer::instance()->
						getTankDeadContainer().getTank(tank->getUniqueId());
					if (savedTank)
					{
						Logger::log( "Found residual player info");
						NetBufferDefault::defaultBuffer.reset();
						if (savedTank->getAccessories().writeMessage(
								NetBufferDefault::defaultBuffer, true) &&
							savedTank->getScore().writeMessage(
								NetBufferDefault::defaultBuffer))
						{
							NetBufferReader reader(NetBufferDefault::defaultBuffer);
							if (!tank->getAccessories().readMessage(reader) ||
								!tank->getScore().readMessage(reader))
							{
								Logger::log( "Failed to update residual player info (read)");
							}
						}
						else 
						{
							Logger::log( "Failed to update residual player info (write)");
						}
						delete savedTank;
					}
				}
			}

			if (tank->getState().getSpectator())
			{
				// This tank is now playing (but dead)
				tank->getState().setState(TankState::sDead);
			}
			else if (state == ServerState::ServerStateNewGame)
			{
				// This tank is now playing
				tank->newGame();
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

	return count;
}

Vector ServerNewGameState::placeTank(unsigned int playerId, int team,
	ScorchedContext &context, RandomGenerator &generator)
{
	Vector tankPos;
	const int tankBorder = 10;

	float minHeight = -1000.0f;
	float maxHeight = 1000.0f;
	float tankCloseness = 0.0f;
	GLBitmap *tankMask = 0;

	LandscapeDefnType *defn =
		context.landscapeMaps->getDefinitions().getDefn()->tankstart;
	if (defn->getType() == LandscapeDefnType::eStartHeight)
	{
		LandscapeDefnStartHeight *height = 
			(LandscapeDefnStartHeight *) defn;
		minHeight = height->heightmin;
		maxHeight = height->heightmax;
		tankCloseness = height->startcloseness;
		if (!height->startmask.empty())
		{
			tankMask = new GLBitmap();
			tankMask->loadFromFile(getDataFile(height->startmask.c_str()));
			DIALOG_ASSERT(tankMask->getBits());
		}
	}
	else
	{
		dialogExit("ServerNewGameState",
			formatString("Failed to find tank start type \"%i\"",
			defn->getType()));
	}
    
	bool tooClose = true;
	float closeness = tankCloseness;
	while (tooClose)
	{
		tooClose = false;

		// Find a new position for the tank
		float posX = float (context.landscapeMaps->getDefinitions().
			getDefn()->landscapewidth - tankBorder * 2) * 
			generator.getRandFloat() + float(tankBorder);
		float posY = float (context.landscapeMaps->getDefinitions().
			getDefn()->landscapeheight - tankBorder * 2) * 
			generator.getRandFloat() + float(tankBorder);
		float height = context.landscapeMaps->getGroundMaps().
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
			if (tankMask)
			{
				// Find the mask position
				int maskX = int(posX * float(tankMask->getWidth())) / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapewidth;
				int maskY = int(posY * float(tankMask->getHeight())) / 
					context.landscapeMaps->getDefinitions().getDefn()->landscapeheight;
				unsigned char *maskPos = tankMask->getBits() +
					maskX * 3 + maskY * tankMask->getWidth() * 3;
					
				if (maskPos[0] == 0 && maskPos[1] == 0 && maskPos[2] == 0)
				{
					// No tank is allowed on the black parts ot the mask
					// regardless of the team
					tooClose = true;
					closeness -= 0.1f;				
				}
				else if (maskPos[0] == 255 && maskPos[1] == 255 && maskPos[2] == 255)
				{
					// All tanks are allowed on the white parts of the mask
					// regardless of the team
				}
				else if (team > 0)
				{
					// Check for team specific colors
					switch(team)
					{
					case 1:
						if (maskPos[0] != 255 || maskPos[1] != 0 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;
					case 2:
						if (maskPos[0] != 0 || maskPos[1] != 0 || maskPos[2] != 255)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;
					case 3:
						if (maskPos[0] != 0 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;
					case 4:
						if (maskPos[0] != 255 || maskPos[1] != 255 || maskPos[2] != 0)
						{
							tooClose = true;
							closeness -= 0.1f;
						}
						break;						
					}
				}
			}
		}

		// Make sure the tank is not too close to other tanks
		if (!tooClose)
		{
			std::map<unsigned int, Tank *> &alltanks = 
				context.tankContainer->getAllTanks();
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = alltanks.begin();
				itor != alltanks.end();
				itor++)
			{
				Tank *thisTank = (*itor).second;
				if (thisTank->getPlayerId() == playerId) break;

				if ((tankPos - thisTank->getPosition().getTankPosition()).Magnitude() < 
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

	delete tankMask;

	// Get the height of the tank
	tankPos[2] = context.landscapeMaps->getGroundMaps().getInterpHeight(
		tankPos[0], tankPos[1]);

	return tankPos;
}

void ServerNewGameState::calculateStartPosition(
	ScorchedContext &context)
{
	RandomGenerator generator;
	generator.seed(rand());

	std::map<unsigned int, Tank *> &tanks = 
		context.tankContainer->getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator mainitor;
	for (mainitor = tanks.begin();
		mainitor != tanks.end();
		mainitor++)
	{
		Tank *tank = (*mainitor).second;

		if (!tank->getState().getSpectator() &&
			!tank->getState().getLoading() &&
			!tank->getState().getInitializing())
		{
			Vector tankPos = placeTank(
				tank->getPlayerId(), tank->getTeam(), 
				context, generator);

			// Set the starting position of the tank
			DeformLandscape::flattenArea(context, tankPos, 0);
			tank->setTargetPosition(tankPos);
		}
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

	// Do we check teams ballance
	if (ScorchedServer::instance()->getOptionsGame().getTeams() != 1)
	{
		// Check for team ballance types
		switch (ScorchedServer::instance()->getOptionsGame().getTeamBallance())
		{
			case OptionsGame::TeamBallanceAuto:
			case OptionsGame::TeamBallanceAutoByScore:
			case OptionsGame::TeamBallanceAutoByBots:
				checkTeamsAuto();
				break;
			case OptionsGame::TeamBallanceBotsVs:
				checkTeamsBotsVs();
				break;
			default:
				break;
		}
	}

	// Make sure everyone is using a team model
	for (mainitor = playingTanks.begin();
		 mainitor != playingTanks.end();
		 mainitor++)
	{
		Tank *current = (*mainitor).second;
		if (!current->getState().getSpectator())
		{
			TankModel *model = 
				ScorchedServer::instance()->getTankModels().getModelByName(
					current->getModelContainer().getTankModelName(),
					current->getTeam(),
					(current->getDestinationId() == 0));
			if (0 != strcmp(model->getName(),
				current->getModelContainer().getTankModelName()))
			{
				// The model is not allowed for this team,
				// use the correct model
				current->getModelContainer().setTankModelName(
					model->getName(), 
					model->getName(),
					model->getTypeName());
			}
		}
	}
}

static inline bool lt_score(const Tank *o1, const Tank *o2) 
{ 
	return ((Tank*)o1)->getScore().getScore() > ((Tank *)o2)->getScore().getScore();
}

static inline bool lt_bots(const Tank *o1, const Tank *o2) 
{ 
	return ((Tank*)o1)->getTankAI() < ((Tank *)o2)->getTankAI();
}

void ServerNewGameState::checkTeamsAuto()
{
	// Count players in each team
	std::vector<Tank *> teamPlayers[4];
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
			if (current->getTeam() > 0)
			{
				teamPlayers[current->getTeam() - 1].push_back(current);
			}
		}
	}

	bool ballanced = false;
	bool check = true;
	while (check)
	{
		check = false;
	
		// Find the teams with the min and max players in them
		std::vector<Tank *> *minPlayers = &teamPlayers[0];
		std::vector<Tank *> *maxPlayers = &teamPlayers[0];
		for (int i=0; i<ScorchedServer::instance()->getOptionsGame().getTeams(); i++)
		{
			if (teamPlayers[i].size() < minPlayers->size()) minPlayers = &teamPlayers[i];
			if (teamPlayers[i].size() > maxPlayers->size()) maxPlayers = &teamPlayers[i];			
		}
		
		// Is the difference between the min and max teams >= 2 players
		if (maxPlayers->size() - minPlayers->size() >= 2)
		{
			check = true;
			ballanced = true;
			
			// Sort teams (if needed)
			if (ScorchedServer::instance()->getOptionsGame().getTeamBallance() ==
				OptionsGame::TeamBallanceAutoByScore)
			{
				std::sort(minPlayers->begin(), minPlayers->end(), lt_score); 
				std::sort(maxPlayers->begin(), maxPlayers->end(), lt_score); 
			}
			else if (ScorchedServer::instance()->getOptionsGame().getTeamBallance() ==
				OptionsGame::TeamBallanceAutoByBots)
			{
				std::sort(minPlayers->begin(), minPlayers->end(), lt_bots); 
				std::sort(maxPlayers->begin(), maxPlayers->end(), lt_bots); 
			}
		
			// Find out which team has the least players
			for (int i=0; i<ScorchedServer::instance()->getOptionsGame().getTeams(); i++)
			{	
				if (minPlayers == &teamPlayers[i])
				{
					// Ballance the teams
					Tank *tank = maxPlayers->back();
					maxPlayers->pop_back();
					minPlayers->push_back(tank);
					tank->setTeam(i+1);
				}
			}
		}
	}

	// Check if we needed to ballance
	if (ballanced)
	{
		if (ScorchedServer::instance()->getOptionsGame().getTeamBallance() ==
			OptionsGame::TeamBallanceAutoByScore)
		{
			ServerCommon::sendString(0, "Auto ballancing teams, by score");
		}
		else if (ScorchedServer::instance()->getOptionsGame().getTeamBallance() ==
			OptionsGame::TeamBallanceAutoByBots)
		{
			ServerCommon::sendString(0, "Auto ballancing teams, by bots");
		}
		else
		{
			ServerCommon::sendString(0, "Auto ballancing teams");
		}
	}
}

void ServerNewGameState::checkTeamsBotsVs()
{
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
			if (current->getDestinationId() == 0) current->setTeam(1);
			else current->setTeam(2);
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
						TankAIAdder::addTankAI(*ScorchedServer::instance(), playerType);
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

void ServerNewGameState::removeTargets()
{
	std::map<unsigned int, Target *> targets = // Note copy
		ScorchedServer::instance()->getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator itor;
	for (itor = targets.begin();
		itor != targets.end();
		itor++)
	{
		unsigned int playerId = (*itor).first;
		Target *target = (*itor).second;
		if (target->isTemp())
		{
			Target *removedTarget = 
				ScorchedServer::instance()->getTargetContainer().removeTarget(playerId);
			delete removedTarget;
		}
	}
}
