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
#include <server/ScorchedServer.h>
#include <weapons/AccessoryStore.h>
#include <landscape/LandscapeDefinitions.h>
#include <tankai/TankAIStore.h>
#include <common/OptionsGame.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/utils.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>

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
static LandscapeDefinitions landscapeDefinitions;
#include "SettingsLand.cpp" 
};
namespace SettingsMain 
{ 
#include "SettingsMain.cpp" 
};
namespace SettingsPlayers 
{ 
static AccessoryStore accessoryStore;
static TankAIStore tankAIStore;
#include "SettingsPlayers.cpp" 
}
namespace SettingsMOTD
{ 
#include "SettingsMOTD.cpp" 
}

extern char scorched3dAppName[128];

class SettingsFrame: public wxDialog
{
public:
	SettingsFrame(bool server, OptionsGame &context);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

protected:
	OptionsGame &context_;
	wxNotebook *book_;
	wxPanel *mainPanel_;
	wxPanel *ecoPanel_;
	wxPanel *envPanel_;
	wxPanel *landPanel_;
	wxPanel *playersPanel_;
	wxPanel *motdPanel_;

	void setupPlayers();
	void onMaxPlayerChange(wxCommandEvent &event);
	void onSelectAll(wxCommandEvent &event);
	void onDeselectAll(wxCommandEvent &event);

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SettingsFrame, wxDialog)
	EVT_TEXT(SettingsPlayers::IDC_SERVER_MAX_PLAYERS,  SettingsFrame::onMaxPlayerChange)
	EVT_BUTTON(SettingsLand::IDC_SELECTALL,  SettingsFrame::onSelectAll)
	EVT_BUTTON(SettingsLand::IDC_DESELECTALL,  SettingsFrame::onDeselectAll)
END_EVENT_TABLE()

SettingsFrame::SettingsFrame(bool server, OptionsGame &context) :
	wxDialog(getMainDialog(), -1, scorched3dAppName,
		wxDefaultPosition, wxDefaultSize),
	context_(context)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controls
	book_ = new wxNotebook(this, -1);
	wxNotebookSizer *nbs = new wxNotebookSizer(book_);

	mainPanel_ = new wxPanel(book_, -1);
	book_->AddPage(mainPanel_, "Main");
	wxSizer *mainPanelSizer = new wxBoxSizer(wxVERTICAL);
	SettingsMain::createControls(mainPanel_, mainPanelSizer);
	mainPanel_->SetAutoLayout(TRUE);
	mainPanel_->SetSizer(mainPanelSizer);

	ecoPanel_ = new wxPanel(book_, -1);
	wxSizer *ecoPanelSizer = new wxBoxSizer(wxVERTICAL);
	SettingsEco::createControls(ecoPanel_, ecoPanelSizer);
	book_->AddPage(ecoPanel_, "Eco");
	ecoPanel_->SetAutoLayout(TRUE);
	ecoPanel_->SetSizer(ecoPanelSizer);

	envPanel_ = new wxPanel(book_, -1);
	wxSizer *envPanelSizer = new wxBoxSizer(wxVERTICAL);
	SettingsEnv::createControls(envPanel_, envPanelSizer);
	book_->AddPage(envPanel_, "Env");
	envPanel_->SetAutoLayout(TRUE);
	envPanel_->SetSizer(envPanelSizer);

	landPanel_ = new wxPanel(book_, -1);
	wxSizer *landPanelSizer = new wxBoxSizer(wxVERTICAL);
	SettingsLand::createControls(landPanel_, landPanelSizer);
	book_->AddPage(landPanel_, "Land");
	landPanel_->SetAutoLayout(TRUE);
	landPanel_->SetSizer(landPanelSizer);

	if (server)
	{
		if (SettingsPlayers::IDC_COMBO_PTYPE_CTRL) 
			delete [] SettingsPlayers::IDC_COMBO_PTYPE_CTRL;
		SettingsPlayers::IDC_COMBO_PTYPE_CTRL = new wxComboBox*[24];
	
		playersPanel_ = new wxPanel(book_, -1);
		wxSizer *playersPanelSizer = new wxBoxSizer(wxVERTICAL);
		SettingsPlayers::createControls(playersPanel_, playersPanelSizer);
		book_->AddPage(playersPanel_, "Players");
		playersPanel_->SetAutoLayout(TRUE);
		playersPanel_->SetSizer(playersPanelSizer);
	}
	else playersPanel_ = 0;

	motdPanel_ = new wxPanel(book_, -1);
	wxSizer *motdPanelSizer = new wxBoxSizer(wxVERTICAL);
	SettingsMOTD::createControls(motdPanel_, motdPanelSizer);
	book_->AddPage(motdPanel_, "MOTD");
	motdPanel_->SetAutoLayout(TRUE);
	motdPanel_->SetSizer(motdPanelSizer);

	topsizer->Add(nbs, 0, wxALL, 10);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *okButton = new wxButton(this, wxID_OK, "Ok");
	wxButton *cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
	buttonSizer->Add(cancelButton, 0, wxALL, 10);
	buttonSizer->Add(okButton, 0, wxALL, 10);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	okButton->SetDefault();
		
	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SettingsFrame::onSelectAll(wxCommandEvent &event)
{
	std::list<LandscapeDefinitionsEntry> &defns =
		SettingsLand::landscapeDefinitions.getAllLandscapes();
	std::list<LandscapeDefinitionsEntry>::iterator itor = 
		defns.begin();
	for (int i = 0; i<(int) defns.size(); i++, itor++)
	{
		SettingsLand::landscapes[i]->SetValue(true);
	}
}

void SettingsFrame::onDeselectAll(wxCommandEvent &event)
{
	std::list<LandscapeDefinitionsEntry> &defns =
		SettingsLand::landscapeDefinitions.getAllLandscapes();
	std::list<LandscapeDefinitionsEntry>::iterator itor =
		defns.begin();  
	for (int i = 0; i<(int) defns.size(); i++, itor++)
	{           
		SettingsLand::landscapes[i]->SetValue(false);
	}
}

void SettingsFrame::onMaxPlayerChange(wxCommandEvent &event)
{
	setupPlayers();
}

void SettingsFrame::setupPlayers()
{
	int maxPlayers = 10;
	sscanf(SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->GetValue(), "%i", &maxPlayers);
	context_.setNoMaxPlayers(maxPlayers);

	for (int i=0; i<24; i++)
	{
		SettingsPlayers::IDC_COMBO_PTYPE_CTRL[i]->Enable(
			(i < context_.getNoMaxPlayers()));
	}
}

bool SettingsFrame::TransferDataToWindow()
{
	// Player tab
	if (playersPanel_)
	{
		SettingsPlayers::IDC_EDIT3_CTRL->
			SetValue(context_.getBotNamePrefix());
		SettingsPlayers::IDC_EDIT3_CTRL->SetToolTip(
			wxString("The text prefixed onto any player that is a bot."));

		// Min max players are rounds combos
		char buffer[25];
		int i;
		for (i=24; i>=0; i--)
		{
			char string[20];
			sprintf(string, "%i", i);

			if (i > 1)
			{
				SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->Append(string);
				SettingsPlayers::IDC_SERVER_REMOVEBOT_PLAYERS_CTRL->Append(string);
			}
			SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->Append(string);
		}
		SettingsPlayers::IDC_SERVER_REMOVEBOT_PLAYERS_CTRL->Append("0");

		sprintf(buffer, "%i", context_.getNoMinPlayers());
		SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->SetValue(buffer);
		SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->SetToolTip(
			wxString("The number of players that must be on the server before a game starts."));

		sprintf(buffer, "%i", context_.getNoMaxPlayers());
		SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->SetValue(buffer);
		SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->SetToolTip(
			wxString("The maximum number of players that can be on the server."));

		sprintf(buffer, "%i", context_.getRemoveBotsAtPlayers());
		SettingsPlayers::IDC_SERVER_REMOVEBOT_PLAYERS_CTRL->SetValue(buffer);
		SettingsPlayers::IDC_SERVER_REMOVEBOT_PLAYERS_CTRL->SetToolTip(
			wxString("The number of players to allow before remvoing bots."));

		SettingsPlayers::IDC_SERVER_RESIDUAL_CTRL->SetValue(
				context_.getResidualPlayers());
		SettingsPlayers::IDC_SERVER_RESIDUAL_CTRL->SetToolTip(
			wxString("Players re-connect with the same money and weapons."));

		// Reload the AIs in case a new mod has been loaded
		SettingsPlayers::tankAIStore.clearAIs();
		SettingsPlayers::accessoryStore.clearAccessories();
		SettingsPlayers::accessoryStore.parseFile(context_);
		SettingsPlayers::tankAIStore.loadAIs(SettingsPlayers::accessoryStore);

		std::list<TankAI *> &ais = 
			SettingsPlayers::tankAIStore.getAis();
		for (int i=0; i<24; i++)
		{
			std::list<TankAI *>::iterator itor;
			for (itor = ais.begin();
				itor != ais.end();
				itor++)
			{
				TankAI *ai = *itor;
				SettingsPlayers::IDC_COMBO_PTYPE_CTRL[i]->Append(ai->getName());
			}
			SettingsPlayers::IDC_COMBO_PTYPE_CTRL[i]->Append("Human");
			SettingsPlayers::IDC_COMBO_PTYPE_CTRL[i]->SetValue(
				context_.getPlayerType(i));
		}
		setupPlayers();
	}

	// Land 
	{
		std::list<LandscapeDefinitionsEntry> &defns =
			SettingsLand::landscapeDefinitions.getAllLandscapes();
		std::list<LandscapeDefinitionsEntry>::iterator itor = 
			defns.begin();
		for (int i = 0; i<(int) defns.size(); i++, itor++)
		{
			SettingsLand::landscapes[i]->SetValue(
				SettingsLand::landscapeDefinitions.landscapeEnabled(
					context_,
					(*itor).name.c_str()));
		}
		SettingsLand::IDC_CYCLEMAPS_CTRL->SetValue(
			context_.getCycleMaps());
		SettingsLand::IDC_CYCLEMAPS_CTRL->SetToolTip(
			wxString(context_.getCycleMapsToolTip()));
	}

	// Eco
	{
		// Buy on round
		char buffer[25];
		int i;
		for (i=50; i>=1; i-=1)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_BUYONROUND_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.getBuyOnRound());
		SettingsEco::IDC_BUYONROUND_CTRL->SetValue(buffer);
		SettingsEco::IDC_BUYONROUND_CTRL->SetToolTip(
			wxString("Players will be allowed to buy more weapons on this round."));

		// Economy
		SettingsEco::IDC_ECONOMY_CTRL->Append("EconomyFreeMarket");
		SettingsEco::IDC_ECONOMY_CTRL->Append("EconomyNone");
		SettingsEco::IDC_ECONOMY_CTRL->SetValue(context_.getEconomy());
		SettingsEco::IDC_ECONOMY_CTRL->SetToolTip(
			wxString("Economic factors for weapon prices"));

		// Money per hit
		for (i=2000; i>=0; i-=50)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_MONEYPERHIT_CTRL->Append(buffer);
			SettingsEco::IDC_MONEYPERKILL_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.getMoneyWonPerHitPoint());
		SettingsEco::IDC_MONEYPERHIT_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYPERHIT_CTRL->SetToolTip(
			wxString("The money awarded for HITTING another tank.\n"
				"This is multiplied by the weapons arms level\n"
					 "and health points removed (if switched on)."));
		sprintf(buffer, "%i", context_.getMoneyWonPerKillPoint());
		SettingsEco::IDC_MONEYPERKILL_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYPERKILL_CTRL->SetToolTip(
			wxString("The money awarded for KILLING another tank.\n"
				"This is multiplied by the weapons arms level\n"
				"and health points removed (if switched on)."));

		SettingsEco::IDC_MONEYPERHEALTH_CTRL->SetValue(
			context_.getMoneyPerHealthPoint());
		SettingsEco::IDC_MONEYPERHEALTH_CTRL->SetToolTip(
			wxString("Switchs on money being awarded by health points removed"));

		// Start Money
		for (i=250000; i>=0; i-=5000)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_STARTMONEY_CTRL->Append(buffer);
			SettingsEco::IDC_MONEYPERROUND_CTRL->Append(buffer);
			SettingsEco::IDC_MONEYROUND_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.getStartMoney());
		SettingsEco::IDC_STARTMONEY_CTRL->SetValue(buffer);
		SettingsEco::IDC_STARTMONEY_CTRL->SetToolTip(
			wxString("The money each tank will start the game with."));
		sprintf(buffer, "%i", context_.getMoneyWonForRound());
		SettingsEco::IDC_MONEYPERROUND_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYPERROUND_CTRL->SetToolTip(
			wxString("The money awarded to the last tank surviving a round."));
		sprintf(buffer, "%i", context_.getMoneyPerRound());
		SettingsEco::IDC_MONEYROUND_CTRL->SetValue(buffer);
		SettingsEco::IDC_MONEYROUND_CTRL->SetToolTip(
			wxString("The money given to every tank at the end of each round."));

		// Interest
		for (i=100; i>=0; i-=5)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_INTEREST_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", context_.getInterest());
		SettingsEco::IDC_INTEREST_CTRL->SetValue(buffer);
		SettingsEco::IDC_INTEREST_CTRL->SetToolTip(
			wxString("The amount of monetary interest gained at the end of each round."));

		// Interest
		SettingsEco::IDC_SCOREMODE_CTRL->Append("Most Wins");
		SettingsEco::IDC_SCOREMODE_CTRL->Append("Most Kills");
		SettingsEco::IDC_SCOREMODE_CTRL->Append("Most Money");
		SettingsEco::IDC_SCOREMODE_CTRL->SetSelection(
			(int) context_.getScoreType());
		SettingsEco::IDC_SCOREMODE_CTRL->SetToolTip(
			wxString("The ranking that determines the winner."));
	}

	// Env
	{
		// Wind force
		SettingsEnv::IDC_COMBO_FORCE_CTRL->Append("Random", 
			(void *) (OptionsGame::WindRandom));
		for (int i=0; i<=5; i++)
		{
			char buffer[25];
			sprintf(buffer, "Force %i%s", i, ((i==0)?" (No Wind)":""));
			SettingsEnv::IDC_COMBO_FORCE_CTRL->Append(
				buffer, (void *) (i+1));
		}
		SettingsEnv::IDC_COMBO_FORCE_CTRL->Append("Breezy (Force 0->2)", 
			(void *) (OptionsGame::WindBreezy));
		SettingsEnv::IDC_COMBO_FORCE_CTRL->Append("Gale (Force 3->5)", 
			(void *) (OptionsGame::WindGale));
		SettingsEnv::IDC_COMBO_FORCE_CTRL->SetSelection(
			context_.getWindForce());
		SettingsEnv::IDC_COMBO_FORCE_CTRL->SetToolTip(
			wxString("The force of the wind."));

		// Wind changes
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Round", 
			(void *) OptionsGame::WindOnRound);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Move", 
			(void *) OptionsGame::WindOnMove);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->SetSelection(
			context_.getWindType());
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->SetToolTip(
			wxString("Specifies when the wind is allowed to change direction."));

		// Wall type
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Random", 
			(void *) OptionsGame::WallRandom);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Concrete", 
			(void *) OptionsGame::WallConcrete);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Bouncy", 
			(void *) OptionsGame::WallBouncy);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Wrap", 
			(void *) OptionsGame::WallWrapAround);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->SetSelection(
			context_.getWallType());
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
			context_.getWeapScale());
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->SetToolTip(
			wxString("Specifies the size of the blast radius for explosive weapons."));

		// Weapon Scale
		for (int i=0; i<=20; i++)
		{
			char buffer[25];
			sprintf(buffer, "%i", i, i);
			if (i<=10)
			{
				SettingsEnv::IDC_COMBO_STARTARMSLEVEL_CTRL->Append(buffer, (void *) i);
				SettingsEnv::IDC_COMBO_ENDARMSLEVEL_CTRL->Append(buffer, (void *) i);
			}
			SettingsEnv::IDC_COMBO_CLIMBINGDISTANCE_CTRL->Append(buffer, (void *) i);
			SettingsEnv::IDC_COMBO_FALLINGDISTANCE_CTRL->Append(buffer, (void *) i);
		}
		SettingsEnv::IDC_COMBO_STARTARMSLEVEL_CTRL->SetSelection(
			context_.getStartArmsLevel());
		SettingsEnv::IDC_COMBO_STARTARMSLEVEL_CTRL->SetToolTip(
			wxString("Specifies the most powerful weapon that will be available to buy from round 0."));
		SettingsEnv::IDC_COMBO_ENDARMSLEVEL_CTRL->SetSelection(
			context_.getEndArmsLevel());
		SettingsEnv::IDC_COMBO_ENDARMSLEVEL_CTRL->SetToolTip(
			wxString("Specifies the most powerful weapon that will be available to buy in the final round."));
		SettingsEnv::IDC_COMBO_FALLINGDISTANCE_CTRL->SetSelection(
			context_.getMinFallingDistance());
		SettingsEnv::IDC_COMBO_FALLINGDISTANCE_CTRL->SetToolTip(
			wxString("Specifies the largest fall without taking damage."));
		SettingsEnv::IDC_COMBO_CLIMBINGDISTANCE_CTRL->SetSelection(
			context_.getMaxClimbingDistance());
		SettingsEnv::IDC_COMBO_CLIMBINGDISTANCE_CTRL->SetToolTip(
			wxString("Specifies the largest distance the tank can climb in one step."));

		SettingsEnv::IDC_GIVEALLWEAPONS_CTRL->SetValue(
			context_.getGiveAllWeapons());
		SettingsEnv::IDC_GIVEALLWEAPONS_CTRL->SetToolTip(
			wxString("Gives everyone an infinite number of all the weapons."));

		// Resign Mode
		SettingsEnv::IDC_RESIGNENDROUND_CTRL->Append("Round Start", 
			(void *) OptionsGame::ResignStart);
		SettingsEnv::IDC_RESIGNENDROUND_CTRL->Append("Round End", 
			(void *) OptionsGame::ResignEnd);
		SettingsEnv::IDC_RESIGNENDROUND_CTRL->SetSelection(
			context_.getResignMode());
		SettingsEnv::IDC_RESIGNENDROUND_CTRL->SetToolTip(
			wxString("Players resign at the end of the round."));	
	}

	// MOTD
	{
		SettingsMOTD::IDC_MOTD_CTRL->SetValue(
			context_.getMOTD());
		SettingsMOTD::IDC_MOTD_CTRL->SetToolTip(
			wxString("The Message Of The Day."));
	}

	// Main
	{
		SettingsMain::IDC_AUTOBALANCETEAMS_CTRL->SetValue(
			context_.getAutoBallanceTeams());
		SettingsMain::IDC_AUTOBALANCETEAMS_CTRL->SetToolTip(
			wxString("Players are moved between teams to ballance them."));

		// Teams combo
		SettingsMain::IDC_TEAMS_CTRL->Append("None");
		int i;
		char string[256];
		for (i=2; i<=2; i++)
		{
			sprintf(string, "%i", i);	
			SettingsMain::IDC_TEAMS_CTRL->Append(string);
		}
		SettingsMain::IDC_TEAMS_CTRL->SetSelection(
			context_.getTeams() - 1);
		SettingsMain::IDC_TEAMS_CTRL->SetToolTip(
			wxString("The number of teams that will play in this game."));

		// Rounds combo
		for (i=1; i<50; i++)
		{
			sprintf(string, "%i", i);	
			SettingsMain::IDC_SERVER_ROUNDS_CTRL->Append(string);
		}
		SettingsMain::IDC_SERVER_ROUNDS_CTRL->SetSelection(
			context_.getNoRounds() - 1);
		SettingsMain::IDC_SERVER_ROUNDS_CTRL->SetToolTip(
			wxString("The number of rounds that will be played in this game."));

		// Shots combo
		for (i=0; i<50; i++)
		{
			sprintf(string, "%i", i);	
			SettingsMain::IDC_NOSHOTS_CTRL->Append(string);
		}
		SettingsMain::IDC_NOSHOTS_CTRL->SetSelection(
			context_.getNoMaxRoundTurns());
		SettingsMain::IDC_NOSHOTS_CTRL->SetToolTip(
			wxString("The maximum number of turns that will be played in each round (0 = infinite)."));

		// Type combo
		SettingsMain::IDC_TYPE_CTRL->Append("Simultaneous", 
			(void *) OptionsGame::TurnSimultaneous);
		SettingsMain::IDC_TYPE_CTRL->Append("Sequential (Loser First)", 
			(void *) OptionsGame::TurnSequentialLooserFirst);
		SettingsMain::IDC_TYPE_CTRL->Append("Sequential (Random Order)", 
			(void *) OptionsGame::TurnSequentialRandom);
		SettingsMain::IDC_TYPE_CTRL->SetSelection(
			context_.getTurnType());
		SettingsMain::IDC_TYPE_CTRL->SetToolTip(
			wxString("Specifies the order of play."));	

		// The waiting time
		for (i=0; i<=90; i+=5)
		{
			sprintf(string, "%i Seconds", i);
			if (i==0) sprintf(string, "%i (Infinite)", i);	
			SettingsMain::IDC_SHOT_TIME_CTRL->Append(string);
			SettingsMain::IDC_BUYING_TIME_CTRL->Append(string);
			SettingsMain::IDC_START_TIME_CTRL->Append(string);
			SettingsMain::IDC_IDLE_SHOTTIME_CTRL->Append(string);
			SettingsMain::IDC_IDLE_TIME_CTRL->Append(string);
		}
		SettingsMain::IDC_START_TIME_CTRL->SetSelection(
			context_.getStartTime()/5);
		SettingsMain::IDC_START_TIME_CTRL->SetToolTip(
			wxString("The time to wait before starting a new game."));
		SettingsMain::IDC_SHOT_TIME_CTRL->SetSelection(
			context_.getShotTime()/5);
		SettingsMain::IDC_SHOT_TIME_CTRL->SetToolTip(
			wxString("The maximum amount of time allowed for each player to make a move."));
		SettingsMain::IDC_BUYING_TIME_CTRL->SetSelection(
			context_.getBuyingTime()/5);
		SettingsMain::IDC_BUYING_TIME_CTRL->SetToolTip(
			wxString("The maximum amount of time allowed for each player to buy accessories."));
		SettingsMain::IDC_IDLE_TIME_CTRL->SetSelection(
			context_.getIdleKickTime()/5);
		SettingsMain::IDC_IDLE_TIME_CTRL->SetToolTip(
			wxString("The amount of time to wait for a client to respond after level loading before kicking it."));
		SettingsMain::IDC_IDLE_SHOTTIME_CTRL->SetSelection(
			context_.getIdleShotKickTime()/5);
		SettingsMain::IDC_IDLE_SHOTTIME_CTRL->SetToolTip(
			wxString("The amount of time to wait for a client to respond after shots before kicking it."));

		// Download speed
		for (i=0; i<=250000; i+=5000)
		{
			sprintf(string, "%i bytes/sec", i);
			if (i==0) sprintf(string, "%i (No download)", i);	
			SettingsMain::IDC_DOWNLOAD_SPEED_CTRL->Append(string);
		}
		SettingsMain::IDC_DOWNLOAD_SPEED_CTRL->SetSelection(
			context_.getModDownloadSpeed()/5000);
		SettingsMain::IDC_DOWNLOAD_SPEED_CTRL->SetToolTip(
			wxString(context_.getModDownloadSpeedToolTip()));

		// Password
		SettingsMain::IDC_SERVER_PASSWORD_CTRL->SetValue(
			context_.getServerPassword());
		SettingsMain::IDC_SERVER_PASSWORD_CTRL->SetToolTip(
			context_.getServerPasswordToolTip());

		// Turn on/off settings if server or client
		SettingsMain::IDC_SERVER_PASSWORD_CTRL->Show(playersPanel_ != 0);
		SettingsMain::IDC_SERVER_PASSWORD_CTRL_TEXT->Show(playersPanel_ != 0);
	}

	return true;
}

bool SettingsFrame::TransferDataFromWindow()
{
	// Players
	if (playersPanel_)
	{
		context_.setBotNamePrefix(
			SettingsPlayers::IDC_EDIT3_CTRL->GetValue());

		// Read min + max players
		int minPlayers = 2;
		sscanf(SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->GetValue(), 
			"%i", &minPlayers);
		int maxPlayers = 10;
		sscanf(SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->GetValue(), 
			"%i", &maxPlayers);
		int maxBotPlayers = 10;
		sscanf(SettingsPlayers::IDC_SERVER_REMOVEBOT_PLAYERS_CTRL->GetValue(), 
			"%i", &maxBotPlayers);

		context_.setRemoveBotsAtPlayers(maxBotPlayers);
		context_.setNoMinPlayers(minPlayers);
		context_.setNoMaxPlayers(maxPlayers);

		context_.setResidualPlayers(
				SettingsPlayers::IDC_SERVER_RESIDUAL_CTRL->GetValue());

		for (int i=0; i<24; i++)
		{
			context_.setPlayerType(i, 
				SettingsPlayers::IDC_COMBO_PTYPE_CTRL[i]->GetValue());
		}
	}

	// Land
	{
		std::string landscapes;
		std::list<LandscapeDefinitionsEntry> &defns =
			SettingsLand::landscapeDefinitions.getAllLandscapes();
		std::list<LandscapeDefinitionsEntry>::iterator itor = 
			defns.begin();
		for (int i = 0; i<(int) defns.size(); i++, itor++)
		{
			if (SettingsLand::landscapes[i]->GetValue())
			{
				if (!landscapes.empty()) landscapes += ":";
				landscapes += (*itor).name.c_str();
			}
		}
		context_.setLandscapes(landscapes.c_str());
		context_.setCycleMaps(
			SettingsLand::IDC_CYCLEMAPS_CTRL->GetValue());
	}

	// Eco
	{
		int buyonround = 2;
		int moneyperhit = 2;
		int moneyperkill = 2;
		int moneyperround = 2;
		int moneyround = 2;
		int startMoney = 2;
		int interest = 2;

		sscanf(SettingsEco::IDC_BUYONROUND_CTRL->GetValue(), "%i", &buyonround);
		sscanf(SettingsEco::IDC_MONEYPERHIT_CTRL->GetValue(), "%i", &moneyperhit);
		sscanf(SettingsEco::IDC_MONEYPERKILL_CTRL->GetValue(), "%i", &moneyperkill);
		sscanf(SettingsEco::IDC_MONEYPERROUND_CTRL->GetValue(), "%i", &moneyperround);
		sscanf(SettingsEco::IDC_MONEYROUND_CTRL->GetValue(), "%i", &moneyround);
		sscanf(SettingsEco::IDC_STARTMONEY_CTRL->GetValue(), "%i", &startMoney);
		sscanf(SettingsEco::IDC_INTEREST_CTRL->GetValue(), "%i", &interest);

		context_.setScoreType(
			(OptionsGame::ScoreType) SettingsEco::IDC_SCOREMODE_CTRL->GetSelection());
		context_.setStartMoney(startMoney);
		context_.setInterest(interest);
		context_.setBuyOnRound(buyonround);
		context_.setMoneyWonForRound(moneyperround);
		context_.setMoneyPerRound(moneyround);
		context_.setMoneyWonPerHitPoint(moneyperhit);
		context_.setMoneyWonPerKillPoint(moneyperkill);
		context_.setMoneyPerHealthPoint(SettingsEco::IDC_MONEYPERHEALTH_CTRL->GetValue());
		context_.setEconomy(SettingsEco::IDC_ECONOMY_CTRL->GetValue());
	}

	// Env
	{
		context_.setWindForce((OptionsGame::WindForce) (int)
			SettingsEnv::IDC_COMBO_FORCE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_FORCE_CTRL->GetSelection()));

		context_.setWindType((OptionsGame::WindType) (int)
			SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetSelection()));

		context_.setWallType((OptionsGame::WallType) (int)
			SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetSelection()));

		context_.setWeapScale((OptionsGame::WeapScale) (int)
			SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetSelection()));

		context_.setStartArmsLevel((int) 
			SettingsEnv::IDC_COMBO_STARTARMSLEVEL_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_STARTARMSLEVEL_CTRL->GetSelection()));

		context_.setEndArmsLevel((int) 
			SettingsEnv::IDC_COMBO_ENDARMSLEVEL_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_ENDARMSLEVEL_CTRL->GetSelection()));
		context_.setMinFallingDistance((int) 
			SettingsEnv::IDC_COMBO_FALLINGDISTANCE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_FALLINGDISTANCE_CTRL->GetSelection()));
		context_.setMaxClimbingDistance((int) 
			SettingsEnv::IDC_COMBO_CLIMBINGDISTANCE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_CLIMBINGDISTANCE_CTRL->GetSelection()));

		context_.setGiveAllWeapons(
			SettingsEnv::IDC_GIVEALLWEAPONS_CTRL->GetValue());

		context_.setResignMode((OptionsGame::ResignType) (int)
			SettingsEnv::IDC_RESIGNENDROUND_CTRL->GetClientData(
				SettingsEnv::IDC_RESIGNENDROUND_CTRL->GetSelection()));
	}

	// MOTD
	{
		context_.setMOTD(
			SettingsMOTD::IDC_MOTD_CTRL->GetValue());
	}

	// Main
	{
		int noRounds = 5;
		int shotTime = 30;
		int buyingTime = 30;
		int waitTime = 30;
		int idleTime = 30;
		int startTime = 10;
		int idleShotTime = 10;
		int maxRoundTurns = 15;
		int downloadSpeed = 0;

		context_.setTurnType((OptionsGame::TurnType) (int) 
			SettingsMain::IDC_TYPE_CTRL->GetClientData(
				SettingsMain::IDC_TYPE_CTRL->GetSelection()));
		context_.setTeams((int) SettingsMain::IDC_TEAMS_CTRL->GetSelection() + 1);

		sscanf(SettingsMain::IDC_NOSHOTS_CTRL->GetValue(), "%i", &maxRoundTurns);
		sscanf(SettingsMain::IDC_SERVER_ROUNDS_CTRL->GetValue(), "%i", &noRounds);
		sscanf(SettingsMain::IDC_SHOT_TIME_CTRL->GetValue(), "%i", &shotTime);
		sscanf(SettingsMain::IDC_BUYING_TIME_CTRL->GetValue(), "%i", &buyingTime);
		sscanf(SettingsMain::IDC_START_TIME_CTRL->GetValue(), "%i", &startTime);
		sscanf(SettingsMain::IDC_IDLE_TIME_CTRL->GetValue(), "%i", &idleTime);
		sscanf(SettingsMain::IDC_IDLE_SHOTTIME_CTRL->GetValue(), "%i", &idleShotTime);
		sscanf(SettingsMain::IDC_DOWNLOAD_SPEED_CTRL->GetValue(), "%i", &downloadSpeed);
		context_.setAutoBallanceTeams(SettingsMain::IDC_AUTOBALANCETEAMS_CTRL->GetValue());
		
		context_.setNoRounds(noRounds);
		context_.setShotTime(shotTime);
		context_.setBuyingTime(buyingTime);
		context_.setStartTime(startTime);
		context_.setIdleKickTime(idleTime);
		context_.setIdleShotKickTime(idleShotTime);
		context_.setNoMaxRoundTurns(maxRoundTurns);
		context_.setModDownloadSpeed(downloadSpeed);
		
		context_.setServerPassword(
			SettingsMain::IDC_SERVER_PASSWORD_CTRL->GetValue());
	}

	return true;
}

bool showSettingsDialog(bool server, OptionsGame &context)
{
	// Set the current mod
	std::string modValue = getDataFileMod();
	setDataFileMod(context.getMod());

	// Show the settings
	SettingsFrame frame(server, context);
	bool result = (frame.ShowModal() == wxID_OK);

	// Reset the mod
	setDataFileMod(modValue.c_str());

	return result;
}
