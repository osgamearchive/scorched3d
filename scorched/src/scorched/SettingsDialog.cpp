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
	SettingsFrame(bool server);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

protected:
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

SettingsFrame::SettingsFrame(bool server) :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(374, 370))
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
	OptionsGame::instance()->setNoMaxPlayers(maxPlayers);

	for (int i=0; i<24; i++)
	{
		(*SettingsPlayers::playerCombos[i])->Enable(
			(i < OptionsGame::instance()->getNoMaxPlayers()));
	}
}

bool SettingsFrame::TransferDataToWindow()
{
	// Player tab
	if (playersPanel_)
	{
		SettingsPlayers::IDC_EDIT3_CTRL->
			SetValue(OptionsGame::instance()->getBotNamePrefix());

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
		sprintf(buffer, "%i", OptionsGame::instance()->getNoMinPlayers());
		SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->SetValue(buffer);
		sprintf(buffer, "%i", OptionsGame::instance()->getNoMaxPlayers());
		SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->SetValue(buffer);

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
				OptionsGame::instance()->getPlayerType(i));
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
		sprintf(buffer, "%i", OptionsGame::instance()->getNoHills());
		SettingsLand::IDC_LANDCOVERAGE_CTRL->SetValue(buffer);

		// Land Height
		for (i=100; i>=0; i-=10)
		{	
			sprintf(buffer, "%i", i);
			SettingsLand::IDC_LANDHEIGHT_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", OptionsGame::instance()->getMaxHeight());
		SettingsLand::IDC_LANDHEIGHT_CTRL->SetValue(buffer);
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
		sprintf(buffer, "%i", OptionsGame::instance()->getBuyOnRound());
		SettingsEco::IDC_BUYONROUND_CTRL->SetValue(buffer);

		// Money per hit
		for (i=2000; i>=0; i-=100)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_MONEYPERHIT_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", OptionsGame::instance()->getMoneyWonPerHitPoint());
		SettingsEco::IDC_MONEYPERHIT_CTRL->SetValue(buffer);

		// Start Money
		for (i=250000; i>=0; i-=5000)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_STARTMONEY_CTRL->Append(buffer);
			SettingsEco::IDC_MONEYPERROUND_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", OptionsGame::instance()->getStartMoney());
		SettingsEco::IDC_STARTMONEY_CTRL->SetValue(buffer);
		sprintf(buffer, "%i", OptionsGame::instance()->getMoneyWonForRound());
		SettingsEco::IDC_MONEYPERROUND_CTRL->SetValue(buffer);

		// Interest
		for (i=100; i>=0; i-=5)
		{	
			sprintf(buffer, "%i", i);
			SettingsEco::IDC_INTEREST_CTRL->Append(buffer);
		}
		sprintf(buffer, "%i", OptionsGame::instance()->getInterest());
		SettingsEco::IDC_INTEREST_CTRL->SetValue(buffer);
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
			OptionsGame::instance()->getWindForce());

		// Wind changes
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Round", 
			(void *) OptionsGame::WindOnRound);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->Append("On Move", 
			(void *) OptionsGame::WindOnMove);
		SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->SetSelection(
			OptionsGame::instance()->getWindType());

		// Wall type
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Random", 
			(void *) OptionsGame::WallRandom);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Concrete", 
			(void *) OptionsGame::WallConcrete);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->Append("Bouncy", 
			(void *) OptionsGame::WallBouncy);
		SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->SetSelection(
			OptionsGame::instance()->getWallType());

		// Weapon Scale
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Small", 
			(void *) OptionsGame::ScaleSmall);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Medium", 
			(void *) OptionsGame::ScaleMedium);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->Append("Large", 
			(void *) OptionsGame::ScaleLarge);
		SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->SetSelection(
			OptionsGame::instance()->getWeapScale());

		// Weapon Scale
		for (int i=0; i<=10; i++)
		{
			char buffer[25];
			sprintf(buffer, "%i", i, i);
			SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->Append(buffer, (void *) i);

		}
		SettingsEnv::IDC_COMBO_ARMSLEVEL_CTRL->SetSelection(
			OptionsGame::instance()->getMaxArmsLevel());
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
		sprintf(string, "%i", OptionsGame::instance()->getNoMaxPlayers());
		SettingsMain::IDC_SERVER_PLAYERS_CTRL->SetValue(string);
		SettingsMain::IDC_SERVER_PLAYERS_CTRL->Enable(playersPanel_ == 0);

		// Rounds combo
		for (i=1; i<25; i++)
		{
			sprintf(string, "%i", i);	
			SettingsMain::IDC_SERVER_ROUNDS_CTRL->Append(string);
		}
		SettingsMain::IDC_SERVER_ROUNDS_CTRL->SetSelection(
			OptionsGame::instance()->getNoRounds() - 1);

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
			OptionsGame::instance()->getShotTime()/5);
		SettingsMain::IDC_WAIT_TIME_CTRL->SetSelection(
			OptionsGame::instance()->getWaitTime()/5);
		SettingsMain::IDC_IDLE_TIME_CTRL->SetSelection(
			OptionsGame::instance()->getIdleKickTime()/5);

		SettingsMain::IDC_SHOT_TIME_CTRL->Enable(playersPanel_ != 0);
		SettingsMain::IDC_WAIT_TIME_CTRL->Enable(playersPanel_ != 0);
		SettingsMain::IDC_IDLE_TIME_CTRL->Enable(playersPanel_ != 0);
	}

	return true;
}

bool SettingsFrame::TransferDataFromWindow()
{
	// Players
	if (playersPanel_)
	{
		OptionsGame::instance()->setBotNamePrefix(
			SettingsPlayers::IDC_EDIT3_CTRL->GetValue());

		// Read min + max players
		int minPlayers = 2;
		sscanf(SettingsPlayers::IDC_SERVER_MIN_PLAYERS_CTRL->GetValue(), 
			"%i", &minPlayers);
		int maxPlayers = 10;
		sscanf(SettingsPlayers::IDC_SERVER_MAX_PLAYERS_CTRL->GetValue(), 
			"%i", &maxPlayers);
		OptionsGame::instance()->setNoMinPlayers(minPlayers);
		OptionsGame::instance()->setNoMaxPlayers(maxPlayers);

		for (int i=0; i<24; i++)
		{
			OptionsGame::instance()->setPlayerType(i, 
				(*SettingsPlayers::playerCombos[i])->GetValue());
		}
	}

	// Land
	{
		int ilandCover = 2;
		sscanf(SettingsLand::IDC_LANDCOVERAGE_CTRL->GetValue(), "%i", &ilandCover);
		int ilandHeight = 2;
		sscanf(SettingsLand::IDC_LANDHEIGHT_CTRL->GetValue(), "%i", &ilandHeight);

		OptionsGame::instance()->setMaxHeight(ilandHeight);
		OptionsGame::instance()->setNoHills(ilandCover);
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

		OptionsGame::instance()->setStartMoney(startMoney);
		OptionsGame::instance()->setInterest(interest);
		OptionsGame::instance()->setBuyOnRound(buyonround);
		OptionsGame::instance()->setMoneyWonForRound(moneyperround);
		OptionsGame::instance()->setMoneyWonPerHitPoint(moneyperhit);
	}

	// Env
	{
		OptionsGame::instance()->setWindForce((OptionsGame::WindForce) (int)
			SettingsEnv::IDC_COMBO_FORCE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_FORCE_CTRL->GetSelection()));

		OptionsGame::instance()->setWindType((OptionsGame::WindType) (int)
			SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WINDCHANGES_CTRL->GetSelection()));

		OptionsGame::instance()->setWallType((OptionsGame::WallType) (int)
			SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WALLTYPE_CTRL->GetSelection()));

		OptionsGame::instance()->setWeapScale((OptionsGame::WeapScale) (int)
			SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetClientData(
				SettingsEnv::IDC_COMBO_WEAPONSCALE_CTRL->GetSelection()));

		OptionsGame::instance()->setMaxArmsLevel((int) 
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
		
		OptionsGame::instance()->setNoRounds(noRounds);
		OptionsGame::instance()->setShotTime(shotTime);
		OptionsGame::instance()->setWaitTime(waitTime);
		OptionsGame::instance()->setIdleKickTime(idleTime);
		if (playersPanel_ == 0)
		{
			OptionsGame::instance()->setNoMaxPlayers(noPlayers);
		}
	}

	return true;
}

bool showSettingsDialog(bool server)
{
	SettingsFrame frame(server);
	return (frame.ShowModal() == wxID_OK);
}
