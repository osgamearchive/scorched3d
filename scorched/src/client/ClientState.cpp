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

#include <engine/ActionController.h>
#include <engine/FrameTimer.h>
#include <engine/MainLoop.h>
#include <client/Main2DCamera.h>
#include <client/SpeedChange.h>
#include <client/MainCamera.h>
#include <client/MainBanner.h>
#include <client/ClientState.h>
#include <client/ShotTimer.h>
#include <client/ClientNextRoundState.h>
#include <client/ClientShotState.h>
#include <tankgraph/TankRenderer.h>
#include <tankai/TankAIHumanCtrl.h>
#include <common/WindowManager.h>
#include <landscape/Landscape.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBilboardRenderer.h>
#include <GLEXT/GLConsole.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void ClientState::addWindowManager(unsigned state)
{
	GameState::instance()->addStateLoop(state, Main2DCamera::instance(), 
		WindowManager::instance());
	GameState::instance()->addStateMouseDownEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	GameState::instance()->addStateMouseDragEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	GameState::instance()->addStateMouseUpEntry(state, GameState::MouseButtonLeft, 
		WindowManager::instance());
	GameState::instance()->addStateKeyEntry(state, WindowManager::instance());
}

void ClientState::addStandardComponents(unsigned state, bool network)
{
	GameState::instance()->addStateKeyEntry(state, 
		GLConsole::instance());
	if (!network)
	{
		GameState::instance()->addStateKeyEntry(state, 
			SpeedChange::instance());
	}
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), GLCameraFrustum::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), FrameTimer::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), &TankRenderer::instance()->render3D);
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), Landscape::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), &TankRenderer::instance()->render3DSecond);
	GameState::instance()->addStateLoop(state, 
		Main2DCamera::instance(), &TankRenderer::instance()->render2D);
	if (!network)
	{
		GameState::instance()->addStateLoop(state, 
			Main2DCamera::instance(), SpeedChange::instance());
	}
	GameState::instance()->addStateLoop(state, MainCamera::instance(), 
		ActionController::instance());
	GameState::instance()->addStateLoop(state, MainCamera::instance(), 
		GLBilboardRenderer::instance());
	addWindowManager(state);
	GameState::instance()->addStateLoop(state, Main2DCamera::instance(), 
		MainBanner::instance());
	GameState::instance()->addStateLoop(state, Main2DCamera::instance(), 
		GLWToolTip::instance());
	GameState::instance()->addStateLoop(state, Main2DCamera::instance(), 
		GLConsole::instance());
	GameState::instance()->addStateMouseDownEntry(state,
		GameState::MouseButtonLeft, MainCamera::instance());
	GameState::instance()->addStateMouseDragEntry(state, 
		GameState::MouseButtonRight| GameState::MouseButtonMiddle, MainCamera::instance());
	GameState::instance()->addStateMouseWheelEntry(state, 
		MainCamera::instance());
	GameState::instance()->addStateKeyEntry(state, 
		MainCamera::instance());
	GameState::instance()->addStateLoop(state, 
		Main2DCamera::instance(), &MainCamera::instance()->saveScreen_);
}

void ClientState::setupGameState(bool network)
{
	MainLoop::instance()->addMainLoop(GameState::instance());

	// StatePlayerOptions (Single player only)
	addWindowManager(StatePlayerOptions);
	GameState::instance()->addStateStimulus(StatePlayerOptions, 
		StimNextRound, 
		StateNextRound);

	// StateClientConnectPlayer (Multi player only, start state)
	addWindowManager(StateClientConnectPlayer);
	GameState::instance()->addStateStimulus(StateClientConnectPlayer, 
		StimClientConnect, StateClientConnect);

	// StateClientConnect (Multi player only)
	addWindowManager(StateClientConnect);
	GameState::instance()->addStateStimulus(StateClientConnect, 
		StimNextRound, StateNextRound);
	GameState::instance()->addStateStimulus(StateClientConnect, 
		StimDisconnected, StateClientConnect);

	// StateNextRound
	addStandardComponents(StateNextRound, network);
	GameState::instance()->addStateEntry(StateNextRound,
		ClientNextRoundState::instance());
	GameState::instance()->addStateStimulus(StateNextRound, 
		StimBuyWeapons, StateBuyWeapons);
	GameState::instance()->addStateStimulus(StateNextRound, 
		StimShot, StateShot);
	GameState::instance()->addStateStimulus(StateNextRound, 
		StimScore, StateScore);
	GameState::instance()->addStateStimulus(StateNextRound, 
		StimNextRound, StateNextRound);
	GameState::instance()->addStateStimulus(StateNextRound, 
		StimDisconnected, StateClientConnect);

	// StateBuyWeapons
	addStandardComponents(StateBuyWeapons, network);
	GameState::instance()->addStateEntry(StateBuyWeapons,
		ShotTimer::instance());
	GameState::instance()->addStateLoop(StateBuyWeapons,
		Main2DCamera::instance(), ShotTimer::instance());
	GameState::instance()->addStateStimulus(StateBuyWeapons, 
		StimAutoDefense, StateAutoDefense);
	GameState::instance()->addStateStimulus(StateBuyWeapons, 
		StimShot, StateShot);
	GameState::instance()->addStateStimulus(StateBuyWeapons, 
		ShotTimer::instance(), StateShot);
	GameState::instance()->addStateStimulus(StateBuyWeapons, 
		StimDisconnected, StateClientConnect);

	// StateAutoDefense
	addStandardComponents(StateAutoDefense, network);
	GameState::instance()->addStateLoop(StateAutoDefense,
		Main2DCamera::instance(), ShotTimer::instance());
	GameState::instance()->addStateStimulus(StateAutoDefense, 
		StimMain, StateMain);
	GameState::instance()->addStateStimulus(StateAutoDefense, 
		StimShot, StateShot);
	GameState::instance()->addStateStimulus(StateAutoDefense, 
		ShotTimer::instance(), StateShot);
	GameState::instance()->addStateStimulus(StateAutoDefense, 
		StimDisconnected, StateClientConnect);

	// StateMain
	addStandardComponents(StateMain, network);
	GameState::instance()->addStateKeyEntry(StateMain, 
		TankAIHumanCtrl::instance());
	GameState::instance()->addStateLoop(StateMain,
		Main2DCamera::instance(), ShotTimer::instance());
	GameState::instance()->addStateStimulus(StateMain, 
		StimShot, StateShot);
	GameState::instance()->addStateStimulus(StateMain, 
		ShotTimer::instance(), StateShot);
	GameState::instance()->addStateStimulus(StateMain, 
		StimDisconnected, StateClientConnect);

	// StateShot
	addStandardComponents(StateShot, network);
	GameState::instance()->addStateEntry(StateShot, 
		TankAIHumanCtrl::instance());
	GameState::instance()->addStateEntry(StateShot,
		ClientShotState::instance());
	GameState::instance()->addStateStimulus(StateShot, 
		StimShot, StateShot);
	GameState::instance()->addStateStimulus(StateShot, 
		ClientShotState::instance(), StateNextRound);
	GameState::instance()->addStateStimulus(StateShot, 
		StimDisconnected, StateClientConnect);

	// StateScore
	addStandardComponents(StateScore, network);
	GameState::instance()->addStateStimulus(StateScore, 
		StimDisconnected, StateClientConnect);
	GameState::instance()->addStateStimulus(StateScore, 
		StimNextRound, StateNextRound);

	if (network) GameState::instance()->setState(StateClientConnectPlayer);
	else GameState::instance()->setState(StatePlayerOptions);
}
