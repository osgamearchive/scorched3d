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

void ServerState::setupStates()
{
	GameState::instance()->clear();

	ServerTooFewPlayersStimulus *tooFewPlayersStim =
		new ServerTooFewPlayersStimulus;

	// ServerStateWaitingForPlayers (start state)
	ServerWaitingForPlayersState *serverWaiting = 
		new ServerWaitingForPlayersState;
	GameState::instance()->addStateEntry(ServerStateWaitingForPlayers,
		serverWaiting);
	GameState::instance()->addStateStimulus(ServerStateWaitingForPlayers,
		serverWaiting, ServerStateNewGame);	
	GameState::instance()->setState(ServerStateWaitingForPlayers);

	// ServerStateNewGame
	ServerNewGameState *serverNewGame = new ServerNewGameState;
	GameState::instance()->addStateEntry(ServerStateNewGame,
		serverNewGame);
	GameState::instance()->addStateStimulus(ServerStateNewGame, 
		ServerStimulusNextRound, ServerStateNextRound);
	GameState::instance()->addStateStimulus(ServerStateNewGame,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	

	// ServerStateNextRound
	ServerNextRoundState *serverNextRound = new ServerNextRoundState;
	GameState::instance()->addStateEntry(ServerStateNextRound,
		serverNextRound);
	GameState::instance()->addStateEntry(ServerStateNextRound,
		TankAIComputerCtrl::instance());
	GameState::instance()->addStateStimulus(ServerStateNextRound,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	GameState::instance()->addStateStimulus(ServerStateNextRound,
		ServerStimulusNewGame, ServerStateNewGame);	
	GameState::instance()->addStateStimulus(ServerStateNextRound,
		ServerStimulusScore, ServerStateScore);	
	GameState::instance()->addStateStimulus(ServerStateNextRound, 
		serverNextRound, ServerStatePlaying);

	// ServerStatePlaying
	ServerPlayingState *serverPlaying = new ServerPlayingState;
	GameState::instance()->addStateEntry(ServerStatePlaying,
		serverPlaying);
	GameState::instance()->addStateEntry(ServerStatePlaying,
		TankAIComputerCtrl::instance());
	GameState::instance()->addStateStimulus(ServerStatePlaying,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	GameState::instance()->addStateStimulus(ServerStatePlaying,
		serverPlaying, ServerStateShot);

	// ServerStateShot
	ServerShotState *serverShot = new ServerShotState;
	GameState::instance()->addStateEntry(ServerStateShot,
		serverShot);
	GameState::instance()->addStateStimulus(ServerStateShot,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	GameState::instance()->addStateStimulus(ServerStateShot,
		serverShot, ServerStateNextRound);	

	// ServerStateScore
	ServerScoreState *serverScore = new ServerScoreState;
	GameState::instance()->addStateEntry(ServerStateScore,
		serverScore);
	GameState::instance()->addStateStimulus(ServerStateScore,
		tooFewPlayersStim, ServerStateWaitingForPlayers);	
	GameState::instance()->addStateStimulus(ServerStateScore,
		serverScore, ServerStateWaitingForPlayers);	
}
