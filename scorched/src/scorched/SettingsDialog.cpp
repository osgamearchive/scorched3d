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

#include <scorched/DisplayDialog.h>
#include <scorched/MainDialog.h>
#include <engine/ScorchedContext.h>
#include <tankai/TankAIStore.h>
#include <common/OptionsGame.h>
#include <wx/wx.h>
#include <wx/notebook.h>

namespace SettingsEco 
{ 
#include "SettingsEco.cpp" 
};
namespace SettingsEnv 
{ 
#include "SettingsEnv.cpp" 
};
namespace SettingsLand 
{
#include "SettingsLand.cpp" 
};
namespace SettingsMain 
{ 
#include "SettingsMain.cpp" 
};
namespace SettingsPlayers 
{ 
#include "SettingsPlayers.cpp" 

wxComboBox **playerCombos[] = {
 &IDC_COMBO_PTYPE1_CTRL, &IDC_COMBO_PTYPE2_CTRL, &IDC_COMBO_PTYPE3_CTRL, &IDC_COMBO_PTYPE4_CTRL, &IDC_COMBO_PTYPE5_CTRL,
 &IDC_COMBO_PTYPE6_CTRL, &IDC_COMBO_PTYPE7_CTRL, &IDC_COMBO_PTYPE8_CTRL, &IDC_COMBO_PTYPE9_CTRL, &IDC_COMBO_PTYPE10_CTRL,
 &IDC_COMBO_PTYPE11_CTRL, &IDC_COMBO_PTYPE12_CTRL, &IDC_COMBO_PTYPE13_CTRL, &IDC_COMBO_PTYPE14_CTRL, &IDC_COMBO_PTYPE15_CTRL,
 &IDC_COMBO_PTYPE16_CTRL, &IDC_COMBO_PTYPE17_CTRL, &IDC_COMBO_PTYPE18_CTRL, &IDC_COMBO_PTYPE19_CTRL, &IDC_COMBO_PTYPE20_CTRL,
 &IDC_COMBO_PTYPE21_CTRL, &IDC_COMBO_PTYPE22_CTRL, &IDC_COMBO_PTYPE23_CTRL, &IDC_COMBO_PTYPE24_CTRL };
};

extern char scorched3dAppName[128];

class SettingsFrame: public wxDialog
{
public:
	SettingsFrame(bool server, ScorchedContext &context);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

protected:
	ScorchedContext &context_;
	wxNotebook *book_;
	wxPanel *mainPanel_;
	wxPanel *ecoPanel_;
	wxPanel *envPanel_;
	wxPanel *landPanel_;
	wxPanel *playersPanel_;

	void setupPlayers();
	void onMaxPlayerChange();

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SettingsFrame, wxDialog)
	EVT_TEXT(SettingsPlayers::IDC_SERVER_MAX_PLAYERS,  SettingsFrame::onMaxPlayerChange)
END_EVENT_TABLE()

SettingsFrame::SettingsFrame(bool server, ScorchedContext &context) :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(374, 370)),
	context_(context)
{
	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create all the display controls
	book_ = new wxNotebook(this, -1, wxPoint(0, 0), wxSize(368, 300));

	mainPanel_ = new wxPanel(book_, -1, wxPoint(0, 0), wxSize(300, 300));
	SettingsMain::createControls(mainPanel_);
	book_->AddPage(mainPanel_, "Main");

	ecoPanel_ = new wxPanel(book_, -1, wxPoint(0, 0), wxSize(300, 300));
	SettingsEco::createControls(ecoPanel_);
	book_->AddPage(ecoPanel_, "Eco");

	envPanel_ = new wxPanel(book_, -1, wxPoint(0, 0), wxSize(300, 300));
	SettingsEnv::createControls(envPanel_);
	book_->AddPage(envPanel_, "Env");

	landPanel_ = new wxPanel(book_, -1, wxPoint(0, 0), wxSize(300, 300));
	SettingsLand::createControls(landPanel_);
	book_->AddPage(landPanel_, "Land");

	if (server)
	{
		playersPanel_ = new wxPanel(book_, -1, wxPoint(0, 0), wxSize(300, 300));
		SettingsPlayers::createControls(playersPanel_);
		book_->AddPage(playersPanel_, "Players");
	}
	else playersPanel_ = 0;

	new wxButton(this, wxID_OK,
		"Ok",
		wxPoint((int) 285, (int) 310), wxSize((int) 75, (int) 21));
	new wxButton(this, wxID_CANCEL,
		"Cancel",
		wxPoint((int) 205, (int) 310), wxSize((int) 75, (int) 21));
}

void SettingsFrame::onMaxPlayerChange()
{
	setupPlayers();
}

void SettingsFrame::setupPlayers()
{
	int maxPlayers = 10;
	sscanf(SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->GetValue(), "%i", &maxPlayers);
	context_.optionsGame.setNoMaxPlayers(maxPlayers);

	for (int i=0; i<24; i++)
	{
		(*SettingsPlayers::playerCombos[i])->Enable(
			(i < context_.optionsGame.getNoMaxPlayers()));
	}
}

bool SettingsFrame::TransferDataToWindow()
{
	// Player tab
	if (playersPanel_)
	{
		SettingsPlayers::IDC_EDIT3_CTRL->
			SetValue(context_.optionsGame.getBotNamePrefix());
		SettingsPlayers::IDC_EDIT3_CTRL->SetToolTip(
			wxString("The text prefixed onto any player that is a bot."));

		// Min max players are rounds combos
		char buffer[25];
		int i;
		for (i=24; i>1; i--)
		{
			char string[20];
			sprintf(string, "%i", i);

			SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->Append(string);
			SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->Append(string);
		}
		sprintf(buffer, "%i", context_.optionsGame.getNoMinPlayers());
		SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->SetValue(buffer);
		SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->SetToolTip(
			wxString("The number of players that must be on the server before a game starts."));


		sprintf(buffer, "%i", context_.optionsGame.getNoMaxPlayers());
		SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->SetValue(buffer);
		SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->SetToolTip(
			wxString("The maximum number of players that can be on the server."));

		std::list<TankAIComputer *> &ais = 
			TankAIStore::instance()->getAis();
		for (int i=0; i<24; i++)
		{
			std::list<TankAIComputer *>::iterator itor;
			for (itor = ais.begin();
				itor != ais.end();
				itor++)
			{
				TankAIComputer *ai = *itor;
				(*SettingsPlayers::playerCombos[i])->Append(ai->getName());
			}
			(*SettingsPlayers::playerCombos[i])->Append("Human");
			(*SettingsPlayers::playerCombos[i])->SetValue(
				context_.optionsGame.getPlayerType(i));
		}
		setupPlayers();
	}

	// Land 
	{
		// Coverage
		char buffer[25];
		int i;
		for (i=500; i>=10; i-=10)
		{	
			sprintf(buffer, "%i", i);
			SettingsLand::IDC_LANDCOVERAGE_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getNoHills());
		SettingsLand::IDC_LANDCOVERAGE_CTRL->SetValue(buffer);
		SettingsLand::IDC_LANDCOVERAGE_CTRL->SetToolTip(
			wxString("Specifies how large the island is."));

		// Land Height
		for (i=100; i>=0; i-=10)
		{	
			sprintf(buffer, "%i", i);
			SettingsLand::IDC_LANDHEIGHT_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getMaxHeight());
		SettingsLand::IDC_LANDHEIGHT_CTRL->SetValue(buffer);
		SettingsLand::IDC_LANDHEIGHT_CTRL->SetToolTip(
			wxString("Specifies how hilly the landscape is."));
	}

	// Eco
	{
		// Buy on round
		char buffer[25];
		int i;
		for (i=25; i>=1; i-=1)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_BUYONROUND_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getBuyOnRound());
		SettingsEco::IDC_BUYONROUND_CTRL->SetValue(buffer);
		SettingsEco::IDC_BUYONROUND_CTRL->SetToolTip(
			wxString("Players will be allowed to buy more weapons on this round."));

		// Money per hit
		for (i=2000; i>=0; i-=100)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_MONEYPERHIT_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getMoneyWonPerHitPoint());
		SettingsEco::IDC_MONEYPERHIT_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYPERHIT_CTRL->SetToolTip(
			wxString("The money awarded for killing another tank.\n"
				"This is multiplied by the weapons arms level"));

		// Start Money
		for (i=250000; i>=0; i-=5000)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_STARTMONEY_CTRL->Append(buffer);
			SettingsEco::IDC_MONEYPERROUND_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getStartMoney());
		SettingsEco::IDC_STARTMONEY_CTRL->SetValue(buffer);
		SettingsEco::IDC_STARTMONEY_CTRL->SetToolTip(
			wxString("The money each tank will start the game with."));
		sprintf(buffer, "%i", context_.optionsGame.getMoneyWonForRound());
		SettingsEco::IDC_MONEYPERROUND_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYPERROUND_CTRL->SetToolTip(
			wxString("The money awarded to the last tank surviving a round."));

		// Interest
		for (i=100; i>=0; i-=5)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_INTEREST_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.optionsGame.getInterest());
		SettingsEco::IDC_INTEREST_CTRL->SetValue(buffer);
		SettingsEco::IDC_INTEREST_CTRL->SetToolTip(
			wxString("The amount of monetary interest gained at the end of each round."));
	}

	// Env
	{
		// Wind force
		SettingsEnv::IDC_COMBO_FORCE_CTRL->Append("Random", (void *) 0);
		for (int i=0; i<=5; i++)
		{
			char buffer[25];
			sprintf(buffer, "Force %i%s", i, ((i==0)?" (No Wind)":""));
			SettingsEnv::IDC_COMBO_FORCE_CTRL->Append(
				buffer, (void *) (i+1));
		}
		SettingsEnv::IDC_COMBO_FORCE_CTRL->SetSelection(
			context_.optionsGame.getWindForce());
		SettingsEnv::IDC_COMBO_FORCE_CTRL->SetToolTip(
			wxString("The force of the wind."));

		// Wind changes
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Round", 
			(void *) OptionsGame::WindOnRound);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Move", 
			(void *) OptionsGame::WindOnMove);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->SetSelection(
			context_.optionsGame.getWindType());
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->SetToolTip(
			wxString("Specifies when the wind is allowed to change direction."));

		// Wall type
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Random", 
			(void *) OptionsGame::WallRandom);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Concrete", 
			(void *) OptionsGame::WallConcrete);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Bouncy", 
			(void *) OptionsGame::WallBouncy);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->SetSelection(
			context_.optionsGame.getWallType());
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->SetToolTip(
			wxString("Specifies the behaviour of the walls that surround the island."));

		// Weapon Scale
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Small", 
			(void *) OptionsGame::ScaleSmall);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Medium", 
			(void *) OptionsGame::ScaleMedium);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Large", 
			(void *) OptionsGame::ScaleLarge);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->SetSelection(
			context_.optionsGame.getWeapScale());
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->SetToolTip(
			wxString("Specifies the size of the blast radius for explosive weapons."));

		// Weapon Scale
		for (int i=0; i<=10; i++)
		{
			char buffer[25];
			sprintf(buffer, "%i", i, i);
			SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->Append(buffer, (void *) i);

		}
		SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->SetSelection(
			context_.optionsGame.getMaxArmsLevel());
		SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->SetToolTip(
			wxString("Specifies the most powerful weapon that will be available to buy."));
	}

	// Main
	{
		int i;
		char string[20];
		for (i=24; i>1; i--)
		{
			char string[20];
			sprintf(string, "%i", i);
			SettingsMain::IDC_SERVER_PLAYERS_CTRL->Append(string);
		}
		sprintf(string, "%i", context_.optionsGame.getNoMaxPlayers());
		SettingsMain::IDC_SERVER_PLAYERS_CTRL->SetValue(string);
		SettingsMain::IDC_SERVER_PLAYERS_CTRL->Show(playersPanel_ == 0);
		SettingsMain::IDC_SERVER_PLAYERS_CTRL_TEXT->Show(playersPanel_ == 0);
		SettingsMain::IDC_SERVER_PLAYERS_CTRL->SetToolTip(
			wxString("The number of players that will play in this game.\n"
				"This number should include computer players"));

		// Rounds combo
		for (i=1; i<25; i++)
		{
			sprintf(string, "%i", i);	
			SettingsMain::IDC_SERVER_ROUNDS_CTRL->Append(string);
		}
		SettingsMain::IDC_SERVER_ROUNDS_CTRL->SetSelection(
			context_.optionsGame.getNoRounds() - 1);
		SettingsMain::IDC_SERVER_ROUNDS_CTRL->SetToolTip(
			wxString("The number of rounds that will be played in this game."));


		// The waiting time
		for (i=0; i<=90; i+=5)
		{
			sprintf(string, "%i Seconds", i);
			SettingsMain::IDC_WAIT_TIME_CTRL->Append(string);

			if (i==0) sprintf(string, "%i (Infinite)", i);	
			SettingsMain::IDC_SHOT_TIME_CTRL->Append(string);
			SettingsMain::IDC_IDLE_TIME_CTRL->Append(string);
		}
		SettingsMain::IDC_SHOT_TIME_CTRL->SetSelection(
			context_.optionsGame.getShotTime()/5);
		SettingsMain::IDC_SHOT_TIME_CTRL->SetToolTip(
			wxString("The maximum amount of time allowed for each player to make a move."));
		SettingsMain::IDC_WAIT_TIME_CTRL->SetSelection(
			context_.optionsGame.getWaitTime()/5);
		SettingsMain::IDC_WAIT_TIME_CTRL->SetToolTip(
			wxString("The amount of time to wait for more players before the game starts."));
		SettingsMain::IDC_IDLE_TIME_CTRL->SetSelection(
			context_.optionsGame.getIdleKickTime()/5);
		SettingsMain::IDC_IDLE_TIME_CTRL->SetToolTip(
			wxString("The amount of time to wait for a client to respond before kicking it."));

		SettingsMain::IDC_SHOT_TIME_CTRL->Show(playersPanel_ != 0);
		SettingsMain::IDC_WAIT_TIME_CTRL->Show(playersPanel_ != 0);
		SettingsMain::IDC_IDLE_TIME_CTRL->Show(playersPanel_ != 0);
		SettingsMain::IDC_SHOT_TIME_CTRL_TEXT->Show(playersPanel_ != 0);
		SettingsMain::IDC_WAIT_TIME_CTRL_TEXT->Show(playersPanel_ != 0);
		SettingsMain::IDC_IDLE_TIME_CTRL_TEXT->Show(playersPanel_ != 0);
	}

	return true;
}

bool SettingsFrame::TransferDataFromWindow()
{
	// Players
	if (playersPanel_)
	{
		context_.optionsGame.setBotNamePrefix(
			SettingsPlayers::IDC_EDIT3_CTRL->GetValue());

		// Read min + max players
		int minPlayers = 2;
		sscanf(SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->GetValue(), 
			"%i", &minPlayers);
		int maxPlayers = 10;
		sscanf(SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->GetValue(), 
			"%i", &maxPlayers);
		context_.optionsGame.setNoMinPlayers(minPlayers);
		context_.optionsGame.setNoMaxPlayers(maxPlayers);

		for (int i=0; i<24; i++)
		{
			context_.optionsGame.setPlayerType(i, 
				(*SettingsPlayers::playerCombos[i])->GetValue());
		}
	}

	// Land
	{
		int ilandCover = 2;
		sscanf(SettingsLand::IDC_LANDCOVERAGE_CTRL->GetValue(), "%i", &ilandCover);
		int ilandHeight = 2;
		sscanf(SettingsLand::IDC_LANDHEIGHT_CTRL->GetValue(), "%i", &ilandHeight);

		context_.optionsGame.setMaxHeight(ilandHeight);
		context_.optionsGame.setNoHills(ilandCover);
	}

	// Eco
	{
		int buyonround = 2;
		int moneyperhit = 2;
		int moneyperround = 2;
		int startMoney = 2;
		int interest = 2;

		sscanf(SettingsEco::IDC_BUYONROUND_CTRL->GetValue(), "%i", &buyonround);
		sscanf(SettingsEco::IDC_MONEYPERHIT_CTRL->GetValue(), "%i", &moneyperhit);
		sscanf(SettingsEco::IDC_MONEYPERROUND_CTRL->GetValue(), "%i", &moneyperround);
		sscanf(SettingsEco::IDC_STARTMONEY_CTRL->GetValue(), "%i", &startMoney);
		sscanf(SettingsEco::IDC_INTEREST_CTRL->GetValue(), "%i", &interest);

		context_.optionsGame.setStartMoney(startMoney);
		context_.optionsGame.setInterest(interest);
		context_.optionsGame.setBuyOnRound(buyonround);
		context_.optionsGame.setMoneyWonForRound(moneyperround);
		context_.optionsGame.setMoneyWonPerHitPoint(moneyperhit);
	}

	// Env
	{
		context_.optionsGame.setWindForce((OptionsGame::WindForce) (int)
			SettingsEnv::IDC_COMBO_FORCE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_FORCE_CTRL->GetSelection()));

		context_.optionsGame.setWindType((OptionsGame::WindType) (int)
			SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetSelection()));

		context_.optionsGame.setWallType((OptionsGame::WallType) (int)
			SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetSelection()));

		context_.optionsGame.setWeapScale((OptionsGame::WeapScale) (int)
			SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetSelection()));

		context_.optionsGame.setMaxArmsLevel((int) 
			SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->GetSelection()));
	}

	// Main
	{
		int noRounds = 5;
		int shotTime = 30;
		int waitTime = 30;
		int idleTime = 30;
		int noPlayers = 2;

		sscanf(SettingsMain::IDC_SERVER_PLAYERS_CTRL->GetValue(), "%i", &noPlayers);
		sscanf(SettingsMain::IDC_SERVER_ROUNDS_CTRL->GetValue(), "%i", &noRounds);
		sscanf(SettingsMain::IDC_SHOT_TIME_CTRL->GetValue(), "%i", &shotTime);
		sscanf(SettingsMain::IDC_WAIT_TIME_CTRL->GetValue(), "%i", &waitTime);
		sscanf(SettingsMain::IDC_IDLE_TIME_CTRL->GetValue(), "%i", &idleTime);
		
		context_.optionsGame.setNoRounds(noRounds);
		context_.optionsGame.setShotTime(shotTime);
		context_.optionsGame.setWaitTime(waitTime);
		context_.optionsGame.setIdleKickTime(idleTime);
		if (playersPanel_ == 0)
		{
			context_.optionsGame.setNoMaxPlayers(noPlayers);
		}
	}

	return true;
}

bool showSettingsDialog(bool server, ScorchedContext &context)
{
	SettingsFrame frame(server, context);
	return (frame.ShowModal() == wxID_OK);
}
