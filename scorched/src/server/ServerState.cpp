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
#include <tankai/TankAIComputerCtrl.h>
#include <server/ServerState.h>
#include <server/ServerWaitingForPlayersState.h>
#include <server/ServerNewGameState.h>
#include <server/ServerNextRoundState.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <server/ServerPlayingState.h>
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
		serverWaiting, ServerStateNewGame);	
	gameState.setState(ServerStateWaitingForPlayers);

	// ServerStateNewGame
	ServerNewGameState *serverNewGame = new ServerNewGameState;
	gameState.addStateEntry(ServerStateNewGame,
		serverNewGame);
	gameState.addStateStimulus(ServerStateNewGame, 
		ServerStimulusNextRound, ServerStateNextRound);
	gameState.addStateStimulus(ServerStateNewGame,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	

	// ServerStateNextRound
	ServerNextRoundState *serverNextRound = new ServerNextRoundState;
	gameState.addStateEntry(ServerStateNextRound,
		serverNextRound);
	gameState.addStateEntry(ServerStateNextRound,
		TankAIComputerCtrl::instance());
	gameState.addStateStimulus(ServerStateNextRound,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateNextRound,
		ServerStimulusNewGame, ServerStateNewGame);	
	gameState.addStateStimulus(ServerStateNextRound,
		ServerStimulusScore, ServerStateScore);	
	gameState.addStateStimulus(ServerStateNextRound, 
		serverNextRound, ServerStatePlaying);

	// ServerStatePlaying
	ServerPlayingState *serverPlaying = new ServerPlayingState;
	gameState.addStateEntry(ServerStatePlaying,
		serverPlaying);
	gameState.addStateEntry(ServerStatePlaying,
		TankAIComputerCtrl::instance());
	gameState.addStateStimulus(ServerStatePlaying,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStatePlaying,
		serverPlaying, ServerStateShot);

	// ServerStateShot
	ServerShotState *serverShot = new ServerShotState;
	gameState.addStateEntry(ServerStateShot,
		serverShot);
	gameState.addStateStimulus(ServerStateShot,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateShot,
		serverShot, ServerStateNextRound);	

	// ServerStateScore
	ServerScoreState *serverScore = new ServerScoreState;
	gameState.addStateEntry(ServerStateScore,
		serverScore);
	gameState.addStateStimulus(ServerStateScore,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	gameState.addStateStimulus(ServerStateScore,
		serverScore, ServerStateWaitingForPlayers);	
}
