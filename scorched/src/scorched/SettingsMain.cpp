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
		"Number Teams :");
	sizer->Add(IDC_TEAMS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_TEAMS_CTRL = 
		new wxComboBox(parent, IDC_TEAMS,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_TEAMS_CTRL, 0, wxALIGN_CENTER);

	sizer->Add(new wxStaticText(parent, -1, ""), 0, wxALIGN_CENTER);
	sizer->Add(IDC_AUTOBALANCETEAMS_CTRL = 
		new wxCheckBox(parent, IDC_AUTOBALANCETEAMS,
		"Auto ballance teams"), 0, wxALIGN_CENTER);
		
	IDC_TYPE_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Game Type :");
	sizer->Add(IDC_TYPE_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_TYPE_CTRL = 
		new wxComboBox(parent, IDC_GAME_TYPE,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_TYPE_CTRL, 0, wxALIGN_CENTER);
		
	IDC_SERVER_ROUNDS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Number Rounds :");
	sizer->Add(IDC_SERVER_ROUNDS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SERVER_ROUNDS_CTRL = 
		new wxComboBox(parent, IDC_SERVER_ROUNDS,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_SERVER_ROUNDS_CTRL, 0, wxALIGN_CENTER);
		
	IDC_NOSHOTS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Number Turns :");
	sizer->Add(IDC_NOSHOTS_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_NOSHOTS_CTRL = 
		new wxComboBox(parent, IDC_NOSHOTS,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_NOSHOTS_CTRL, 0, wxALIGN_CENTER);	
		
	IDC_SHOT_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Shot Time :");
	sizer->Add(IDC_SHOT_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SHOT_TIME_CTRL = 
		new wxComboBox(parent, IDC_SHOT_TIME,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_SHOT_TIME_CTRL, 0, wxALIGN_CENTER);
		
	IDC_IDLE_TIME_CTRL_TEXT =
		new wxStaticText(parent, -1,
		"Level Idle Time :");
	sizer->Add(IDC_IDLE_TIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_IDLE_TIME_CTRL = 
		new wxComboBox(parent, IDC_IDLE_TIME,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_IDLE_TIME_CTRL, 0, wxALIGN_CENTER);
	IDC_IDLE_SHOTTIME_CTRL_TEXT =
		new wxStaticText(parent, -1,
		"Shot Idle Time :");
	sizer->Add(IDC_IDLE_SHOTTIME_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_IDLE_SHOTTIME_CTRL = 
		new wxComboBox(parent, IDC_IDLE_SHOTTIME,
		"",
		wxDefaultPosition, wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	sizer->Add(IDC_IDLE_SHOTTIME_CTRL, 0, wxALIGN_CENTER);
	IDC_SERVER_PASSWORD_CTRL_TEXT = 
		new wxStaticText(parent, -1, "Password :");
	sizer->Add(IDC_SERVER_PASSWORD_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SERVER_PASSWORD_CTRL = 
		new wxTextCtrl(parent, -1,
		"",
		wxDefaultPosition, wxSize((int) 100, -1), wxTE_PASSWORD);
	sizer->Add(IDC_SERVER_PASSWORD_CTRL, 0, wxALIGN_CENTER);
	IDC_SERVERADMIN_PASSWORD_CTRL_TEXT =
		new wxStaticText(parent, -1, "Admin Password :");
	sizer->Add(IDC_SERVERADMIN_PASSWORD_CTRL_TEXT, 0, wxALIGN_CENTER);
	IDC_SERVERADMIN_PASSWORD_CTRL =
		new wxTextCtrl(parent, -1,
		"",
		wxDefaultPosition, wxSize((int) 100, -1), wxTE_PASSWORD);
	sizer->Add(IDC_SERVERADMIN_PASSWORD_CTRL, 0, wxALIGN_CENTER);
}

