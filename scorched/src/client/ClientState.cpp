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
#include <client/Main2DCamera.h>
#include <client/SpeedChange.h>
#include <client/MainCamera.h>
#include <client/MainBanner.h>
#include <client/ClientState.h>
#include <client/ShotTimer.h>
#include <client/MessageDisplay.h>
#include <client/ClientReadyState.h>
#include <client/ClientShotState.h>
#include <client/ClientWaitState.h>
#include <client/ClientNewGameState.h>
#include <client/ScorchedClient.h>
#include <tankgraph/TankRenderer.h>
#include <tankai/TankAIHumanCtrl.h>
#include <common/WindowManager.h>
#include <landscape/Landscape.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLOrderedItemRenderer.h>
#include <GLEXT/GLConsole.h>

void ClientState::addWindowManager(GameState &gameState, unsigned state)
{
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		WindowManager::instance());
	gameState.addStateMouseDownEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	gameState.addStateMouseDragEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	gameState.addStateMouseUpEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	gameState.addStateKeyEntry(state, WindowManager::instance());
}

void ClientState::addStandardComponents(GameState &gameState, unsigned state, bool network)
{
	gameState.addStateKeyEntry(state, GLConsole::instance());
	if (!network)
	{
		gameState.addStateKeyEntry(state, SpeedChange::instance());
	}
	gameState.addStateLoop(state, 
		MainCamera::instance(), GLCameraFrustum::instance());
	gameState.addStateLoop(state, 
		MainCamera::instance(), FrameTimer::instance());
	gameState.addStateLoop(state, 
		MainCamera::instance(), &TankRenderer::instance()->render3D);
	gameState.addStateLoop(state, 
		MainCamera::instance(), Landscape::instance());
	gameState.addStateLoop(state, 
		MainCamera::instance(), &TankRenderer::instance()->render3DSecond);
	gameState.addStateLoop(state, 
		Main2DCamera::instance(), &TankRenderer::instance()->render2D);
	if (!network)
	{
		gameState.addStateLoop(state, 
			Main2DCamera::instance(), SpeedChange::instance());
	}
	gameState.addStateLoop(state, MainCamera::instance(), 
		&ScorchedClient::instance()->getActionController());
	gameState.addStateLoop(state, MainCamera::instance(), 
		GLOrderedItemRenderer::instance());
	addWindowManager(gameState, state);
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		MainBanner::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		MessageDisplay::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		GLWToolTip::instance());
	gameState.addStateLoop(state, Main2DCamera::instance(), 
		GLConsole::instance());
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
	gameState.addStateStimulus(StateConnect, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateConnect, 
		StimGameStopped, StateConnect);
	gameState.addStateStimulus(StateConnect, 
		StimWait, StateWait);

	// StateNewGame
	addWindowManager(gameState, StateNewGame);
	gameState.addStateEntry(StateNewGame,
		ClientNewGameState::instance());
	gameState.addStateStimulus(StateNewGame, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateNewGame, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateNewGame, 
		StimWait, StateWait);

	// StateReady
	addStandardComponents(gameState, StateReady, network);
	gameState.addStateEntry(StateReady,
		ClientReadyState::instance());
	gameState.addStateStimulus(StateReady, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateReady, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateReady, 
		StimWait, StateWait);

	// StateWait
	addStandardComponents(gameState, StateWait, network);
	gameState.addStateEntry(StateWait, 
		TankAIHumanCtrl::instance());
	gameState.addStateEntry(StateWait,
		ClientWaitState::instance());
	gameState.addStateStimulus(StateWait, 
		StimReady, StateReady);
	gameState.addStateStimulus(StateWait, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateWait, 
		StimDisconnected, StateConnect);
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
		StimNewGame, StateNewGame);

	// StateBuyWeapons
	addStandardComponents(gameState, StateBuyWeapons, network);
	gameState.addStateEntry(StateBuyWeapons,
		ShotTimer::instance());
	gameState.addStateLoop(StateBuyWeapons,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StateBuyWeapons, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimAutoDefense, StateAutoDefense);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimWait, StateWait);
	gameState.addStateStimulus(StateBuyWeapons, 
		ShotTimer::instance(), StateWait);

	// StateAutoDefense
	addStandardComponents(gameState, StateAutoDefense, network);
	gameState.addStateLoop(StateAutoDefense,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StateAutoDefense, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateAutoDefense, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateAutoDefense, 
		StimWait, StateWait);
	gameState.addStateStimulus(StateAutoDefense, 
		ShotTimer::instance(), StateWait);

	// StatePlaying
	addStandardComponents(gameState, StatePlaying, network);
	gameState.addStateEntry(StatePlaying,
		ShotTimer::instance());
	gameState.addStateKeyEntry(StatePlaying, 
		TankAIHumanCtrl::instance());
	gameState.addStateLoop(StatePlaying,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StatePlaying, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StatePlaying, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StatePlaying, 
		StimWait, StateWait);
	gameState.addStateStimulus(StatePlaying, 
		ShotTimer::instance(), StateWait);

	// StateShot
	addStandardComponents(gameState, StateShot, network);
	gameState.addStateEntry(StateShot, 
		TankAIHumanCtrl::instance());
	gameState.addStateEntry(StateShot,
		ClientShotState::instance());
	gameState.addStateStimulus(StateShot, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateShot, 
		StimWait, StateWait);
	gameState.addStateStimulus(StateShot, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateShot, 
		ClientShotState::instance(), StateReady);

	// StateScore
	addStandardComponents(gameState, StateScore, network);
	gameState.addStateStimulus(StateScore, 
		StimDisconnected, StateConnect);
	gameState.addStateStimulus(StateScore, 
		StimGameStopped, StateWait);
	gameState.addStateStimulus(StateScore, 
		StimWait, StateWait);

	gameState.setState(StateConnect);
}
