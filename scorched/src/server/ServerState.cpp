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

#include <engine/GameState.h>
#include <server/ServerState.h>
#include <server/ServerNewGameState.h>
#include <server/ServerNextRoundState.h>
#include <server/ServerNextTurnState.h>
#include <server/ServerNextShotState.h>
#include <server/ServerReadyState.h>
#include <server/ServerResetState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ServerStateTooFewPlayersState.h>
#include <server/ServerCheckForWinnersState.h>
#include <server/ServerPlayingState.h>
#include <server/ServerStartingState.h>
#include <server/ServerScoreState.h>
#include <server/ServerShotState.h>

void ServerState::setupStates(GameState &gameState)
{
	gameState.clear();

	// ServerStateTooFewPlayers
	ServerStateTooFewPlayersState *serverTooFewPlayers = 
		new ServerStateTooFewPlayersState();
	gameState.addStateEntry(ServerStateTooFewPlayers,
		serverTooFewPlayers);
	gameState.addStateStimulus(ServerStateTooFewPlayers, 
		serverTooFewPlayers, ServerStateStarting);

	// ServerStateStarting
	ServerStartingState *serverStarting = 
		new ServerStartingState();
	gameState.addStateEntry(ServerStateStarting,
		serverStarting);
	gameState.addStateStimulus(ServerStateStarting, 
		ServerTooFewPlayersStimulus::instance(), ServerStateTooFewPlayers);	
	gameState.addStateStimulus(ServerStateStarting, 
		serverStarting, ServerStateReset);

	// ServerStateReset (Start State)
	ServerResetState *serverReset = new ServerResetState();
	gameState.addStateEntry(ServerStateReset,
		serverReset);
	gameState.addStateStimulus(ServerStateReset, 
		ServerStimulusNewGame, ServerStateNewGame);

	// ServerStateNewGame
	ServerNewGameState *serverNewGame = new ServerNewGameState;
	gameState.addStateEntry(ServerStateNewGame,
		serverNewGame);
	gameState.addStateStimulus(ServerStateNewGame, 
		ServerStimulusNewGameReady, ServerStateNewGameReady);

	ServerShotState *serverShot = new ServerShotState();

	// ServerStateNewGameReady
	ServerReadyState *serverNewGameReady = new ServerReadyState(serverShot);
	gameState.addStateEntry(ServerStateNewGameReady,
		serverNewGameReady);
	gameState.addStateStimulus(ServerStateNewGameReady, 
		serverNewGameReady, ServerStateNextRound);

	// ServerStateNextRound
	ServerNextRoundState *serverNextRound = new ServerNextRoundState();
	gameState.addStateEntry(ServerStateNextRound,
		serverNextRound);
	gameState.addStateStimulus(ServerStateNextRound, 
		ServerStimulusNextShot, ServerStateNextShot);

	// ServerStateNextShot
	ServerNextShotState *serverNextShot = new ServerNextShotState;
	gameState.addStateEntry(ServerStateNextShot,
		serverNextShot);
	gameState.addStateStimulus(ServerStateNextShot, 
		ServerStimulusTooFewPlayers, ServerStateCheckForWinners);
	gameState.addStateStimulus(ServerStateNextShot, 
		ServerStimulusNextTurn, ServerStateNextTurn);
	gameState.addStateStimulus(ServerStateNextShot,
		ServerStimulusNewGame, ServerStateNewGame);	
	gameState.addStateStimulus(ServerStateNextShot,
		ServerStimulusScore, ServerStateScore);	
	gameState.addStateStimulus(ServerStateNextShot, 
		ServerStimulusNextRound, ServerStateNextRound);

	// ServerStateNextTurn
	ServerNextTurnState *serverNextTurn = new ServerNextTurnState;
	gameState.addStateEntry(ServerStateNextTurn,
		serverNextTurn);
	gameState.addStateStimulus(ServerStateNextTurn,
		ServerTooFewPlayersStimulus::instance(), ServerStateCheckForWinners);	
	gameState.addStateStimulus(ServerStateNextTurn,
		ServerStimulusBuying, ServerStateBuying);	
	gameState.addStateStimulus(ServerStateNextTurn,
		ServerStimulusPlaying, ServerStatePlaying);	
	gameState.addStateStimulus(ServerStateNextTurn,
		ServerStimulusShot, ServerStateShot);	

	// ServerStatePlaying
	ServerPlayingState *serverPlaying = new ServerPlayingState;
	gameState.addStateEntry(ServerStatePlaying,
		serverPlaying);
	gameState.addStateStimulus(ServerStatePlaying,
		ServerTooFewPlayersStimulus::instance(), ServerStateShot);	
	gameState.addStateStimulus(ServerStatePlaying,
		serverPlaying, ServerStateNextTurn);

	// ServerStateBuying
	ServerPlayingState *serverBuying = new ServerPlayingState;
	gameState.addStateEntry(ServerStateBuying,
		serverBuying);
	gameState.addStateStimulus(ServerStateBuying,
		ServerTooFewPlayersStimulus::instance(), ServerStateShot);	
	gameState.addStateStimulus(ServerStateBuying,
		serverBuying, ServerStateNextTurn);

	// ServerStateShot
	gameState.addStateEntry(ServerStateShot,
		serverShot);
	gameState.addStateStimulus(ServerStateShot,
		serverShot, ServerStateShotReady);

	// ServerStateShotReady
	ServerReadyState *serverShotReady = new ServerReadyState(serverShot);
	gameState.addStateEntry(ServerStateShotReady,
		serverShotReady);
	gameState.addStateStimulus(ServerStateShotReady, 
		serverShotReady, ServerStateNextShot);

	// ServerStateCheckForWinners
	ServerCheckForWinnersState *serverCheckForWinners = new ServerCheckForWinnersState;
	gameState.addStateEntry(ServerStateCheckForWinners,
		serverCheckForWinners);
	gameState.addStateStimulus(ServerStateCheckForWinners,
		ServerStimulusTooFewPlayers, ServerStateTooFewPlayers);	
	gameState.addStateStimulus(ServerStateCheckForWinners,
		ServerStimulusScore, ServerStateScore);	

	// ServerStateScore
	ServerScoreState *serverScore = new ServerScoreState;
	gameState.addStateEntry(ServerStateScore,
		serverScore);
	gameState.addStateStimulus(ServerStateScore,
		serverScore, ServerStateStarting);	

	// Set the start state
	gameState.setState(ServerStateReset);
}
