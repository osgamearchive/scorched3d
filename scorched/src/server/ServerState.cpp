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
#include <server/ServerWaitingForPlayersState.h>
#include <server/ServerNewGameState.h>
#include <server/ServerNextRoundState.h>
#include <server/ServerNextTurnState.h>
#include <server/ServerNextShotState.h>
#include <server/ServerReadyState.h>
#include <server/ServerResetState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ServerPlayingState.h>
#include <server/ServerBuyingState.h>
#include <server/ServerScoreState.h>
#include <server/ServerShotState.h>

void ServerState::setupStates(GameState &gameState)
{
	gameState.clear();

	ServerTooFewPlayersStimulus *tooFewPlayersStim =
		new ServerTooFewPlayersStimulus;

	// ServerStateWaitingForPlayers (start state)
	ServerWaitingForPlayersState *serverWaiting = 
		new ServerWaitingForPlayersState;
	gameState.addStateEntry(ServerStateWaitingForPlayers,
		serverWaiting);
	gameState.addStateStimulus(ServerStateWaitingForPlayers,
		serverWaiting, ServerStateReset);	
	gameState.setState(ServerStateWaitingForPlayers);

	// ServerStateReset
	ServerResetState *serverReset = new ServerResetState();
	gameState.addStateEntry(ServerStateReset,
		serverReset);
	gameState.addStateStimulus(ServerStateReset,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateReset, 
		ServerStimulusNewGame, ServerStateNewGame);

	// ServerStateNewGame
	ServerNewGameState *serverNewGame = new ServerNewGameState;
	gameState.addStateEntry(ServerStateNewGame,
		serverNewGame);
	gameState.addStateStimulus(ServerStateNewGame,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateNewGame, 
		ServerStimulusNextRound, ServerStateNextRound);

	// ServerStateNextRound
	ServerNextRoundState *serverNextRound = new ServerNextRoundState;
	gameState.addStateEntry(ServerStateNextRound,
		serverNextRound);
	gameState.addStateStimulus(ServerStateNextRound,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateNextRound, 
		ServerStimulusReady, ServerStateReady);

	// ServerStateReady
	ServerReadyState *serverReady = new ServerReadyState;
	gameState.addStateEntry(ServerStateReady,
		serverReady);
	gameState.addStateStimulus(ServerStateReady,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateReady, 
		serverReady, ServerStateNextShot);

	// ServerStateNextShot
	ServerNextShotState *serverNextShot = new ServerNextShotState;
	gameState.addStateEntry(ServerStateNextShot,
		serverNextShot);
	gameState.addStateStimulus(ServerStateNextShot,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
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
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
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
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStatePlaying,
		serverPlaying, ServerStateNextTurn);

	// ServerStateBuying
	ServerBuyingState *serverBuying = new ServerBuyingState;
	gameState.addStateEntry(ServerStateBuying,
		serverBuying);
	gameState.addStateStimulus(ServerStateBuying,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateBuying,
		serverBuying, ServerStateNextTurn);

	// ServerStateShot
	ServerShotState *serverShot = new ServerShotState;
	gameState.addStateEntry(ServerStateShot,
		serverShot);
	gameState.addStateStimulus(ServerStateShot,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateShot,
		serverShot, ServerStateReady);	

	// ServerStateScore
	ServerScoreState *serverScore = new ServerScoreState;
	gameState.addStateEntry(ServerStateScore,
		serverScore);
	gameState.addStateStimulus(ServerStateScore,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateScore,
		serverScore, ServerStateWaitingForPlayers);	
}
