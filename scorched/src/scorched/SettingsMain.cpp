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

#include "SettingsMain-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	wxSizer *sizer = new wxFlexGridSizer(2, 2);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	IDC_TEAMS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Number Teams :"));
	sizer->Add(IDC_TEAMS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_TEAMS_CTRL = 
		new wxComboBox(parent, IDC_TEAMS,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_TEAMS_CTRL, 0, wxALIGN_CENTER);

	sizer->Add(new wxStaticText(parent, -1, 
		wxT("Team Ballance")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_TEAMBALLANCE_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	IDC_TYPE_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Game Type :"));
	sizer->Add(IDC_TYPE_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_TYPE_CTRL = 
		new wxComboBox(parent, IDC_GAME_TYPE,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_TYPE_CTRL, 0, wxALIGN_CENTER);
		
	IDC_SERVER_ROUNDS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Number Rounds :"));
	sizer->Add(IDC_SERVER_ROUNDS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SERVER_ROUNDS_CTRL = 
		new wxComboBox(parent, IDC_SERVER_ROUNDS,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_SERVER_ROUNDS_CTRL, 0, wxALIGN_CENTER);
		
	IDC_NOSHOTS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Number Turns :"));
	sizer->Add(IDC_NOSHOTS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_NOSHOTS_CTRL = 
		new wxComboBox(parent, IDC_NOSHOTS,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_NOSHOTS_CTRL, 0, wxALIGN_CENTER);	

	sizer->Add(new wxStaticText(parent, -1,
		wxT("Keep Alive Time :")), 0, wxALIGN_CENTER);
	IDC_KEEPALIVE_TIME_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_KEEPALIVE_TIME_CTRL, 0, wxALIGN_CENTER);

	IDC_START_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Start Time :"));
	sizer->Add(IDC_START_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_START_TIME_CTRL = 
		new wxComboBox(parent, IDC_START_TIME,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_START_TIME_CTRL, 0, wxALIGN_CENTER);
		
	IDC_SHOT_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Shot Time :"));
	sizer->Add(IDC_SHOT_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SHOT_TIME_CTRL = 
		new wxComboBox(parent, IDC_SHOT_TIME,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_SHOT_TIME_CTRL, 0, wxALIGN_CENTER);
		
	IDC_BUYING_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		wxT("Buying Time :"));
	sizer->Add(IDC_BUYING_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_BUYING_TIME_CTRL = 
		new wxComboBox(parent, IDC_SHOT_TIME,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_BUYING_TIME_CTRL, 0, wxALIGN_CENTER);

	IDC_IDLE_TIME_CTRL_TEXT =
		new wxStaticText(parent, -1,
		wxT("Level Idle Time :"));
	sizer->Add(IDC_IDLE_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_IDLE_TIME_CTRL = 
		new wxComboBox(parent, IDC_IDLE_TIME,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_IDLE_TIME_CTRL, 0, wxALIGN_CENTER);
	IDC_IDLE_SHOTTIME_CTRL_TEXT =
		new wxStaticText(parent, -1,
		wxT("Shot Idle Time :"));
	sizer->Add(IDC_IDLE_SHOTTIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_IDLE_SHOTTIME_CTRL = 
		new wxComboBox(parent, IDC_IDLE_SHOTTIME,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_IDLE_SHOTTIME_CTRL, 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Mod Download Speed :")), 0, wxALIGN_CENTER);
	IDC_DOWNLOAD_SPEED_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_DOWNLOAD_SPEED_CTRL, 0, wxALIGN_CENTER);
	IDC_SERVER_PASSWORD_CTRL_TEXT = 
		new wxStaticText(parent, -1, wxT("Password :"));
	sizer->Add(IDC_SERVER_PASSWORD_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SERVER_PASSWORD_CTRL = 
		new wxTextCtrl(parent, -1,
		wxT(""),
		wxDefaultPosition, wxSize((int) 132, -1), wxTE_PASSWORD);
	sizer->Add(IDC_SERVER_PASSWORD_CTRL, 0, wxALIGN_CENTER);
}

