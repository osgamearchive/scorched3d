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
#include <client/ClientNextRoundState.h>
#include <client/ClientShotState.h>
#include <client/ScorchedClient.h>
#include <tankgraph/TankRenderer.h>
#include <tankai/TankAIHumanCtrl.h>
#include <common/WindowManager.h>
#include <landscape/Landscape.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBilboardRenderer.h>
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
		GLBilboardRenderer::instance());
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

	// StatePlayerOptions (Single player only)
	addWindowManager(gameState, StatePlayerOptions);
	gameState.addStateLoop(StatePlayerOptions, Main2DCamera::instance(), 
		GLWToolTip::instance());
	gameState.addStateStimulus(StatePlayerOptions, 
		StimNextRound, 
		StateNextRound);

	// StateClientConnectPlayer (Multi player only, start state)
	addWindowManager(gameState, StateClientConnectPlayer);
	gameState.addStateStimulus(StateClientConnectPlayer, 
		StimClientConnect, StateClientConnect);

	// StateClientConnect (Multi player only)
	addWindowManager(gameState, StateClientConnect);
	gameState.addStateStimulus(StateClientConnect, 
		StimNextRound, StateNextRound);
	gameState.addStateStimulus(StateClientConnect, 
		StimDisconnected, StateClientConnect);

	// StateNextRound
	addStandardComponents(gameState, StateNextRound, network);
	gameState.addStateEntry(StateNextRound,
		ClientNextRoundState::instance());
	gameState.addStateStimulus(StateNextRound, 
		StimBuyWeapons, StateBuyWeapons);
	gameState.addStateStimulus(StateNextRound, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateNextRound, 
		StimScore, StateScore);
	gameState.addStateStimulus(StateNextRound, 
		StimNextRound, StateNextRound);
	gameState.addStateStimulus(StateNextRound, 
		StimDisconnected, StateClientConnect);

	// StateBuyWeapons
	addStandardComponents(gameState, StateBuyWeapons, network);
	gameState.addStateEntry(StateBuyWeapons,
		ShotTimer::instance());
	gameState.addStateLoop(StateBuyWeapons,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StateBuyWeapons, 
		StimAutoDefense, StateAutoDefense);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateBuyWeapons, 
		ShotTimer::instance(), StateShot);
	gameState.addStateStimulus(StateBuyWeapons, 
		StimDisconnected, StateClientConnect);

	// StateAutoDefense
	addStandardComponents(gameState, StateAutoDefense, network);
	gameState.addStateLoop(StateAutoDefense,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StateAutoDefense, 
		StimMain, StateMain);
	gameState.addStateStimulus(StateAutoDefense, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateAutoDefense, 
		ShotTimer::instance(), StateShot);
	gameState.addStateStimulus(StateAutoDefense, 
		StimDisconnected, StateClientConnect);

	// StateMain
	addStandardComponents(gameState, StateMain, network);
	gameState.addStateKeyEntry(StateMain, 
		TankAIHumanCtrl::instance());
	gameState.addStateLoop(StateMain,
		Main2DCamera::instance(), ShotTimer::instance());
	gameState.addStateStimulus(StateMain, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateMain, 
		ShotTimer::instance(), StateShot);
	gameState.addStateStimulus(StateMain, 
		StimDisconnected, StateClientConnect);

	// StateShot
	addStandardComponents(gameState, StateShot, network);
	gameState.addStateEntry(StateShot, 
		TankAIHumanCtrl::instance());
	gameState.addStateEntry(StateShot,
		ClientShotState::instance());
	gameState.addStateStimulus(StateShot, 
		StimShot, StateShot);
	gameState.addStateStimulus(StateShot, 
		ClientShotState::instance(), StateNextRound);
	gameState.addStateStimulus(StateShot, 
		StimDisconnected, StateClientConnect);

	// StateScore
	addStandardComponents(gameState, StateScore, network);
	gameState.addStateStimulus(StateScore, 
		StimDisconnected, StateClientConnect);
	gameState.addStateStimulus(StateScore, 
		StimNextRound, StateNextRound);

	if (network) gameState.setState(StateClientConnectPlayer);
	else gameState.setState(StatePlayerOptions);
}
