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


#include <common/WindowManager.h>
#include <common/Keyboard.h>
#include <client/ClientState.h>
#include <client/WindowSetup.h>
#include <dialogs/AutoDefenseDialog.h>
#include <dialogs/MainMenuDialog.h>
#include <dialogs/WindDialog.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/PlayersDialog.h>
#include <dialogs/PlanViewDialog.h>
#include <dialogs/QuitDialog.h>
#include <dialogs/KillDialog.h>
#include <dialogs/ConnectDialog.h>
#include <dialogs/BackdropDialog.h>
#include <dialogs/HelpDialog.h>
#include <dialogs/BuyAccessoryDialog.h>
#include <dialogs/TalkDialog.h>
#include <dialogs/TankDialog.h>
#include <dialogs/ScoreDialog.h>
#include <dialogs/KibitzingDialog.h>

void WindowSetup::addCommonComponents(unsigned state, bool server)
{
	KEYBOARDKEY("SHOW_QUIT_DIALOG", quitKey);
	if (server || state == ClientState::StateScore)
	{
		WindowManager::instance()->addWindow(state, 
 			QuitDialog::instance(), quitKey, false);
	}
	else
	{
		WindowManager::instance()->addWindow(state, 
			KillDialog::instance(), quitKey, false);
	}

	KEYBOARDKEY("SHOW_HELP_DIALOG", helpKey);
	WindowManager::instance()->addWindow(state, 
		HelpDialog::instance(), helpKey, false);
	if (state != ClientState::StateScore)
	{
		KEYBOARDKEY("SHOW_SCORE_DIALOG", scoreKey);
		WindowManager::instance()->addWindow(state, 
			ScoreDialog::instance(), scoreKey, false);
	}
	if (server)
	{
		KEYBOARDKEY("SHOW_TALK_DIALOG", talkKey);
		WindowManager::instance()->addWindow(state, 
			TalkDialog::instance(), talkKey, false);
	}
	if (state != ClientState::StateScore)
	{
		WindowManager::instance()->addWindow(state, 
			MainMenuDialog::instance(), 0, true);
	}
}

void WindowSetup::setup(bool server)
{
	KEYBOARDKEY("SHOW_TALK_DIALOG", talkKey);
	KEYBOARDKEY("SHOW_QUIT_DIALOG", quitKey);
	KEYBOARDKEY("SHOW_WIND_DIALOG", windKey);
	KEYBOARDKEY("SHOW_PLAN_DIALOG", planKey);
	KEYBOARDKEY("SHOW_PLAYER_DIALOG", playerKey);
	KEYBOARDKEY("SHOW_KIBITZ_DIALOG", kibitzKey);
	
	GLWWindow *planView = new PlanViewDialog;

	// StatePlayerOptions
	WindowManager::instance()->addWindow(ClientState::StatePlayerOptions, 
		BackdropDialog::instance(), 0, true);
	WindowManager::instance()->addWindow(ClientState::StatePlayerOptions, 
		PlayersDialog::instance(), 0, true);
	WindowManager::instance()->addWindow(ClientState::StatePlayerOptions, 
		QuitDialog::instance(), quitKey, false);

	// StateClientConnectPlayer
	WindowManager::instance()->addWindow(ClientState::StateClientConnectPlayer, 
		BackdropDialog::instance(), 0, true);
	WindowManager::instance()->addWindow(ClientState::StateClientConnectPlayer, 
		QuitDialog::instance(), quitKey, false);
	WindowManager::instance()->addWindow(ClientState::StateClientConnectPlayer, 
		PlayerDialog::instance(), 0, true);

	// StateClientConnect
	WindowManager::instance()->addWindow(ClientState::StateClientConnect, 
		TalkDialog::instance(), talkKey, false);
	WindowManager::instance()->addWindow(ClientState::StateClientConnect, 
		BackdropDialog::instance(), 0, true);
	WindowManager::instance()->addWindow(ClientState::StateClientConnect, 
		QuitDialog::instance(), quitKey, false);
	WindowManager::instance()->addWindow(ClientState::StateClientConnect, 
		ConnectDialog::instance(), 0, true);

	// StateNextRound
	addCommonComponents(ClientState::StateNextRound, server);

	// StateBuyWeapons
	addCommonComponents(ClientState::StateBuyWeapons, server);
	WindowManager::instance()->addWindow(ClientState::StateBuyWeapons, 
		new BuyAccessoryDialog, 0, false);

	// StateAutoDefense
	addCommonComponents(ClientState::StateAutoDefense, server);
	WindowManager::instance()->addWindow(ClientState::StateAutoDefense, 
		new AutoDefenseDialog, 0, false);

	// StateMain
	WindowManager::instance()->addWindow(ClientState::StateMain, 
		planView, planKey, true);
	WindowManager::instance()->addWindow(ClientState::StateMain, 
		WindDialog::instance(), windKey, true);
	WindowManager::instance()->addWindow(ClientState::StateMain, 
		TankDialog::instance(), playerKey, true);
	WindowManager::instance()->addWindow(ClientState::StateMain, 
		new KibitzingDialog, kibitzKey, false);
	addCommonComponents(ClientState::StateMain, server);
	
	// StateShot
	WindowManager::instance()->addWindow(ClientState::StateShot, 
		planView, planKey, true);
	WindowManager::instance()->addWindow(ClientState::StateShot, 
		WindDialog::instance(), windKey, true);
	addCommonComponents(ClientState::StateShot, server);

	// StateScore
	addCommonComponents(ClientState::StateScore, server);
	WindowManager::instance()->addWindow(ClientState::StateScore,
		ScoreDialog::instance2(), 0, true);
}
