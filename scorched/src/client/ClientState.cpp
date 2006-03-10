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

#include <engine/FrameTimer.h>
#include <engine/ActionController.h>
#include <engine/ParticleEngine.h>
#include <client/Main2DCamera.h>
#include <client/SpeedChange.h>
#include <client/MainCamera.h>
#include <client/ClientState.h>
#include <client/ShotCountDown.h>
#include <client/SoftwareMouse.h>
#include <client/MessageDisplay.h>
#include <client/ClientShotState.h>
#include <client/ClientWaitState.h>
#include <client/ClientLoadPlayersState.h>
#include <client/ScorchedClient.h>
#include <tankgraph/RenderTargets.h>
#include <tankai/TankAIHumanCtrl.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWToolTip.h>
#include <landscape/Landscape.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLConsole.h>

void ClientState::addWindowManager(GameState &gameState, unsigned state)
{
	gameState.addStateKeyEntry(state, GLConsole::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		GLWWindowManager::instance());
	gameState.addStateMouseDownEntry(state, GameState::MouseButtonLeft, 
		GLWWindowManager::instance());
	gameState.addStateMouseDragEntry(state, GameState::MouseButtonLeft, 
		GLWWindowManager::instance());
	gameState.addStateMouseUpEntry(state, GameState::MouseButtonLeft, 
		GLWWindowManager::instance());
	gameState.addStateKeyEntry(state, GLWWindowManager::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(),
		GLConsole::instance());
}

void ClientState::addStandardComponents(GameState &gameState, unsigned state, bool network)
{
	if (!network)
	{
		gameState.addStateKeyEntry(state, SpeedChange::instance());
	}
	gameState.addStateLoop(state, 
		MainCamera::instance(), GLCameraFrustum::instance());
	gameState.addStateLoop(state, 
		MainCamera::instance(), &RenderTargets::instance()->render3D);
	gameState.addStateLoop(state, 
		MainCamera::instance(), Landscape::instance());
	gameState.addStateLoop(state, MainCamera::instance(), 
		&ScorchedClient::instance()->getActionController());
	gameState.addStateLoop(state, MainCamera::instance(), 
		&ScorchedClient::instance()->getParticleEngine());
	gameState.addStateLoop(state, 
		MainCamera::instance(), &MainCamera::instance()->precipitation_);
	gameState.addStateLoop(state, 
		Main2DCamera::instance(), &RenderTargets::instance()->render2D);
	gameState.addStateLoop(state, 
		MainCamera::instance(), FrameTimer::instance());
	if (!network)
	{
		gameState.addStateLoop(state, 
			Main2DCamera::instance(), SpeedChange::instance());
	}
	addWindowManager(gameState, state);
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		MessageDisplay::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		GLWToolTip::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		SoftwareMouse::instance());
	gameState.addStateMouseDownEntry(state,
		GameState::MouseButtonLeft, MainCamera::instance());
	gameState.addStateMouseDragEntry(state, 
		GameState::MouseButtonRight| GameState::MouseButtonMiddle, MainCamera::instance());
	gameState.addStateMouseWheelEntry(state, 
		MainCamera::instance());
	gameState.addStateKeyEntry(state, 
		MainCamera::instance());
	gameState.addStateLoop(state, 
		Main2DCamera::instance(), &MainCamera::instance()->saveScreen_);
}

void ClientState::setupGameState(bool network)
{
	GameState &gameState = ScorchedClient::instance()->getGameState();
	gameState.clear();

	// StateConnect
	addWindowManager(gameState, StateConnect);
	gameState.addStateLoop(StateConnect, 
		Main2DCamera::instance(), SoftwareMouse::instance());
	gameState.addStateStimulus(StateConnect, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateConnect, 
		StimGameStopped, StateConnect);
	gameState.addStateStimulus(StateConnect, 
		StimGetPlayers, StateGetPlayers);
	gameState.addStateStimulus(StateConnect, 
		StimLoadPlayers, StateLoadPlayers);

	// StateGetPlayers
	addWindowManager(gameState, StateGetPlayers);
	gameState.addStateLoop(StateGetPlayers, 
		Main2DCamera::instance(), GLWToolTip::instance());
	gameState.addStateLoop(StateGetPlayers, 
		Main2DCamera::instance(), SoftwareMouse::instance());
	gameState.addStateStimulus(StateGetPlayers, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StateGetPlayers, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateGetPlayers, 
		StimWait, StateWait);

	// StateLoadPlayers (Single Player Only)
	addWindowManager(gameState, StateLoadPlayers);
	gameState.addStateEntry(StateLoadPlayers,
		ClientLoadPlayersState::instance());
	gameState.addStateStimulus(StateLoadPlayers,
		StimWait, StateWait);

	// StateWait
	addStandardComponents(gameState, StateWait, network);
	gameState.addStateLoop(StateWait,
		Main2DCamera::instance(), ShotCountDown::instance());
	gameState.addStateEntry(StateWait, 
		TankAIHumanCtrl::instance());
	gameState.addStateEntry(StateWait,
		ClientWaitState::instance());
	gameState.addStateStimulus(StateWait, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StateWait, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateWait, 
		StimAutoDefense, StateWait);
	gameState.addStateStimulus(StateWait, 
		StimBuyWeapons, StateBuyWeapons);
	gameState.addStateStimulus(StateWait, 
		StimPlaying, StatePlaying);
	gameState.addStateStimulus(StateWait, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateWait, 
		StimWait, StateWait);
	gameState.addStateStimulus(StateWait, 
		StimScore, StateScore);
	gameState.addStateStimulus(StateWait, 
		StimGetPlayers, StateGetPlayers);

	// StateBuyWeapons
	addStandardComponents(gameState, StateBuyWeapons, network);
	gameState.addStateLoop(StateBuyWeapons,
		Main2DCamera::instance(), ShotCountDown::instance());
	gameState.addStateStimulus(StateBuyWeapons, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimAutoDefense, StateAutoDefense);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimWait, StateWait);

	// StateAutoDefense
	addStandardComponents(gameState, StateAutoDefense, network);
	gameState.addStateLoop(StateAutoDefense,
		Main2DCamera::instance(), ShotCountDown::instance());
	gameState.addStateStimulus(StateAutoDefense, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateAutoDefense, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StateAutoDefense, 
		StimWait, StateWait);

	// StatePlaying
	addStandardComponents(gameState, StatePlaying, network);
	gameState.addStateKeyEntry(StatePlaying, 
		TankAIHumanCtrl::instance());
	gameState.addStateLoop(StatePlaying,
		Main2DCamera::instance(), ShotCountDown::instance());
	gameState.addStateStimulus(StatePlaying, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StatePlaying, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StatePlaying, 
		StimWait, StateWait);

	// StateShot
	addStandardComponents(gameState, StateShot, network);
	gameState.addStateEntry(StateShot, 
		TankAIHumanCtrl::instance());
	gameState.addStateEntry(StateShot,
		ClientShotState::instance());
	gameState.addStateStimulus(StateShot,
		ClientShotState::instance(), StateWait);
	gameState.addStateStimulus(StateShot, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateShot, 
		StimWait, StateWait);
	gameState.addStateStimulus(StateShot, 
		StimGameStopped, StateGetPlayers);

	// StateScore
	addStandardComponents(gameState, StateScore, network);
	gameState.addStateStimulus(StateScore, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateScore, 
		StimGameStopped, StateGetPlayers);
	gameState.addStateStimulus(StateScore, 
		StimWait, StateWait);

	// StateDisconnected
	addWindowManager(gameState, StateDisconnected);
	gameState.addStateLoop(StateDisconnected, 
		Main2DCamera::instance(), SoftwareMouse::instance());
	gameState.addStateStimulus(StateDisconnected, 
		StimDisconnected, StateDisconnected);
	gameState.addStateStimulus(StateDisconnected, 
		StimGameStopped, StateDisconnected);
	gameState.addStateStimulus(StateDisconnected, 
		StimGetPlayers, StateDisconnected);
	gameState.addStateStimulus(StateDisconnected, 
		StimWait, StateDisconnected);

	// Set the start state
	gameState.setState(StateConnect);
}
