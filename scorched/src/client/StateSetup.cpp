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


// MainGameState.cpp: implementation of the MainGameState class.
//
//////////////////////////////////////////////////////////////////////

#include <engine/ActionController.h>
#include <engine/FrameTimer.h>
#include <engine/MainLoop.h>
#include <engine/ActionController.h>
#include <client/Main2DCamera.h>
#include <client/MainCamera.h>
#include <client/MainBanner.h>
#include <client/StateSetup.h>
#include <client/ShotTimer.h>
#include <client/ClientNextRoundState.h>
#include <client/ClientShotState.h>
#include <tankgraph/TankRenderer.h>
#include <tank/TankController.h>
#include <common/WindowManager.h>
#include <landscape/Landscape.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLConsole.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void StateSetup::addWindowManager(unsigned state)
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

void StateSetup::addStandardComponents(unsigned state)
{
	GameState::instance()->addStateKeyEntry(state, 
		GLConsole::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), GLCameraFrustum::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), FrameTimer::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), Landscape::instance());
	GameState::instance()->addStateLoop(state, 
		MainCamera::instance(), &TankRenderer::instance()->render3D);
	GameState::instance()->addStateLoop(state, 
		Main2DCamera::instance(), &TankRenderer::instance()->render2D);
	GameState::instance()->addStateLoop(state, MainCamera::instance(), 
		ActionController::instance());
	addWindowManager(state);
	GameState::instance()->addStateLoop(state, Main2DCamera::instance(), 
		MainBanner::instance());
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
}

void StateSetup::setupGameState(bool network)
{
	MainLoop::instance()->addMainLoop(GameState::instance());

	// StateOptions (Single player only, start state)
	addWindowManager(MainGameState::StateOptions);
	//GameState::instance()->addStateStimulus(MainGameState::StateOptions, 
	//	MainGameState::StimNext, MainGameState::StatePlayerOptions);

	/*
	addWindowManager(MainGameState::StatePlayerOptions);
	GameState::instance()->addStateStimulus(MainGameState::StatePlayerOptions, 
	MainGameState::StimNext, 
	MainGameState::StateNewGame);*/

	// StateClientConnectPlayer (Multi player only, start state)
	addWindowManager(MainGameState::StateClientConnectPlayer);
	GameState::instance()->addStateStimulus(MainGameState::StateClientConnectPlayer, 
		MainGameState::StimClientConnect, MainGameState::StateClientConnect);

	// StateClientConnect (Multi player only)
	addWindowManager(MainGameState::StateClientConnect);
	GameState::instance()->addStateStimulus(MainGameState::StateClientConnect, 
		MainGameState::StimNextRound, MainGameState::StateNextRound);
	GameState::instance()->addStateStimulus(MainGameState::StateClientConnect, 
		MainGameState::StimDisconnected, MainGameState::StateClientConnect);

	// StateNextRound
	addStandardComponents(MainGameState::StateNextRound);
	GameState::instance()->addStateEntry(MainGameState::StateNextRound,
		ClientNextRoundState::instance());
	GameState::instance()->addStateStimulus(MainGameState::StateNextRound, 
		MainGameState::StimMain, MainGameState::StateMain);
	GameState::instance()->addStateStimulus(MainGameState::StateNextRound, 
		MainGameState::StimShot, MainGameState::StateShot);
	GameState::instance()->addStateStimulus(MainGameState::StateNextRound, 
		MainGameState::StimDisconnected, MainGameState::StateClientConnect);

	// StateMain
	addStandardComponents(MainGameState::StateMain);
	GameState::instance()->addStateEntry(MainGameState::StateMain,
		ShotTimer::instance());
	GameState::instance()->addStateKeyEntry(MainGameState::StateMain, 
		TankController::instance());
	GameState::instance()->addStateLoop(MainGameState::StateMain,
		Main2DCamera::instance(), ShotTimer::instance());
	GameState::instance()->addStateStimulus(MainGameState::StateMain, 
		MainGameState::StimShot, MainGameState::StateShot);
	GameState::instance()->addStateStimulus(MainGameState::StateMain, 
		ShotTimer::instance(), MainGameState::StateShot);
	GameState::instance()->addStateStimulus(MainGameState::StateMain, 
		MainGameState::StimDisconnected, MainGameState::StateClientConnect);

	// StateShot
	addStandardComponents(MainGameState::StateShot);
	GameState::instance()->addStateStimulus(MainGameState::StateShot, 
		MainGameState::StimShot, MainGameState::StateShot);
	GameState::instance()->addStateStimulus(MainGameState::StateShot, 
		ClientShotState::instance(), MainGameState::StateNextRound);
	GameState::instance()->addStateStimulus(MainGameState::StateShot, 
		MainGameState::StimDisconnected, MainGameState::StateClientConnect);

	if (network) GameState::instance()->setState(MainGameState::StateClientConnectPlayer);
	else GameState::instance()->setState(MainGameState::StateOptions);
}

