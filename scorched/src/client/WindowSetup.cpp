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

#include <GLW/GLWWindowManager.h>
#include <GLW/GLWWindowSkinManager.h>
#include <GLW/GLWSelector.h>
#include <common/Keyboard.h>
#include <common/OptionsParam.h>
#include <client/ClientState.h>
#include <client/WindowSetup.h>
#include <dialogs/AutoDefenseDialog.h>
#include <dialogs/MainMenuDialog.h>
#include <dialogs/CameraDialog.h>
#include <dialogs/PlayerDialog.h>
#include <dialogs/QuitDialog.h>
#include <dialogs/SaveDialog.h>
#include <dialogs/LogDialog.h>
#include <dialogs/ConnectDialog.h>
#include <dialogs/BackdropDialog.h>
#include <dialogs/BuyAccessoryDialog.h>
#include <dialogs/RulesDialog.h>
#include <dialogs/TalkDialog.h>
#include <dialogs/ScoreDialog.h>
#include <dialogs/KibitzingDialog.h>
#include <dialogs/HUDDialog.h>
#include <dialogs/HelpButtonDialog.h>

void WindowSetup::addCommonComponents(unsigned state)
{
	std::list<GLWWindowSkin *> allStateWindows = 
		GLWWindowSkinManager::instance()->getAllStateWindows();
	std::list<GLWWindowSkin *>::iterator itor;
	for (itor = allStateWindows.begin();
		itor != allStateWindows.end();
		itor++)
	{
		GLWWindowSkin *window = *itor;
		KeyboardKey *key = 0;
		if (window->getKey()[0]) key = 
			Keyboard::instance()->getKey(window->getKey());
		GLWWindowManager::instance()->addWindow(state, 
			window, key, window->getVisible());
	}

	KEYBOARDKEY("HUD_ITEMS", hudItemsKey);
	GLWWindowManager::instance()->addWindow(state, 
 		HUDDialog::instance(), hudItemsKey, false);

	KEYBOARDKEY("SHOW_QUIT_DIALOG", quitKey);
	GLWWindowManager::instance()->addWindow(state, 
 		QuitDialog::instance(), quitKey, false);

	KEYBOARDKEY("SHOW_KIBITZ_DIALOG", kibitzKey);
	GLWWindowManager::instance()->addWindow(state, 
		new KibitzingDialog, kibitzKey, false);

	if (!OptionsParam::instance()->getConnectedToServer())
	{
		KEYBOARDKEY("SHOW_SAVE_DIALOG", saveKey);
		GLWWindowManager::instance()->addWindow(state, 
 			SaveDialog::instance(), saveKey, false);
		KEYBOARDKEY("SHOW_KILL_DIALOG", killKey);
		GLWWindowManager::instance()->addWindow(state, 
 			QuitDialog::instance(), killKey, false);
	}

	KEYBOARDKEY("SHOW_CAMERA_DIALOG", cameraKey);
	GLWWindowManager::instance()->addWindow(state, 
		CameraDialog::instance(), cameraKey, false);

	if (state != ClientState::StateScore)
	{
		KEYBOARDKEY("SHOW_SCORE_DIALOG", scoreKey);
		GLWWindowManager::instance()->addWindow(state, 
			ScoreDialog::instance(), scoreKey, false);
	}
	KEYBOARDKEY("SHOW_TALK_DIALOG", talkKey);
	GLWWindowManager::instance()->addWindow(state, 
		TalkDialog::instance(), talkKey, false);
	KEYBOARDKEY("SHOW_TEAM_TALK_DIALOG", teamTalkKey);
	GLWWindowManager::instance()->addWindow(state, 
		TeamTalkDialog::instance(), teamTalkKey, false);

	if (state != ClientState::StateScore)
	{
		GLWWindowManager::instance()->addWindow(state, 
			MainMenuDialog::instance(), 0, true);
	}
	GLWWindowManager::instance()->addWindow(state,
		GLWSelector::instance(), 0, true);

	if (OptionsParam::instance()->getConnectedToServer())
	{
		KEYBOARDKEY("SHOW_TEAM_DIALOG", teamKey);
		GLWWindowManager::instance()->addWindow(state,
			PlayerDialog::instance(), teamKey, false);
	}

	KEYBOARDKEY("SHOW_RULES_DIALOG", rulesKey);
	GLWWindowManager::instance()->addWindow(state, 
		RulesDialog::instance(), rulesKey, true);

	GLWWindowManager::instance()->addWindow(state, 
		HelpButtonDialog::instance(), 0, true);
}

void WindowSetup::setup()
{
	KEYBOARDKEY("SHOW_QUIT_DIALOG", quitKey);
	KEYBOARDKEY("SHOW_TALK_DIALOG", talkKey);
	KEYBOARDKEY("SHOW_TEAM_TALK_DIALOG", teamTalkKey);
	KEYBOARDKEY("SHOW_PLAYER_DIALOG", playerKey);
	KEYBOARDKEY("SHOW_AIM_DIALOG", aimKey);
	KEYBOARDKEY("SHOW_WEAPON_DIALOG", weaponKey);

	// StateConnect
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		TalkDialog::instance(), talkKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		TeamTalkDialog::instance(), teamTalkKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		BackdropDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		QuitDialog::instance(), quitKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		LogDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
		ConnectDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateConnect, 
			GLWSelector::instance(), 0, true);

	// StateGetPlayers
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		TalkDialog::instance(), talkKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		TeamTalkDialog::instance(), teamTalkKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		BackdropDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		PlayerDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		QuitDialog::instance(), quitKey, false);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
		HelpButtonDialog::instance(), 0, true);
	GLWWindowManager::instance()->addWindow(ClientState::StateGetPlayers, 
			GLWSelector::instance(), 0, true);

	// StateLoadPlayers
	GLWWindowManager::instance()->addWindow(ClientState::StateLoadPlayers,
		BackdropDialog::instance(), 0, true);

	// StateWait
	addCommonComponents(ClientState::StateWait);

	// StateReady
	addCommonComponents(ClientState::StateReady);

	// StateBuyWeapons
	addCommonComponents(ClientState::StateBuyWeapons);
	GLWWindowManager::instance()->addWindow(ClientState::StateBuyWeapons, 
		new BuyAccessoryDialog, 0, true);

	// StateAutoDefense
	addCommonComponents(ClientState::StateAutoDefense);
	GLWWindowManager::instance()->addWindow(ClientState::StateAutoDefense, 
		new AutoDefenseDialog, 0, false);

	// StatePlaying
	std::list<GLWWindowSkin *> playerStateWindows = 
		GLWWindowSkinManager::instance()->getPlayerStateWindows();
	std::list<GLWWindowSkin *>::iterator itor;
	for (itor = playerStateWindows.begin();
		itor != playerStateWindows.end();
		itor++)
	{
		GLWWindowSkin *window = *itor;
		KeyboardKey *key = 0;
		if (window->getKey()[0]) key = 
			Keyboard::instance()->getKey(window->getKey());
		GLWWindowManager::instance()->addWindow(ClientState::StatePlaying, 
			window, key, window->getVisible());
	}
	addCommonComponents(ClientState::StatePlaying);

	// StateShot
	addCommonComponents(ClientState::StateShot);

	// StateScore
	addCommonComponents(ClientState::StateScore);
	GLWWindowManager::instance()->addWindow(ClientState::StateScore,
		ScoreDialog::instance2(), 0, true);
}
