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

static void createControls(wxWindow *parent)
{
	IDC_SERVER_PLAYERS_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"No Players :",
		wxPoint((int) 28.5, (int) 55.5));
	IDC_SERVER_PLAYERS_CTRL = 
		new wxComboBox(parent, IDC_SERVER_PLAYERS,
		"",
		wxPoint((int) 97.5, (int) 52.5), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	IDC_TYPE_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Game Type :",
		wxPoint((int) 28.5, (int) 78));
	IDC_TYPE_CTRL = 
		new wxComboBox(parent, IDC_GAME_TYPE,
		"",
		wxPoint((int) 97.5, (int) 78), wxSize((int) 100, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticText(parent, -1,
		"No Rounds :",
		wxPoint((int) 28.5, (int) 30));
	IDC_SERVER_ROUNDS_CTRL = 
		new wxComboBox(parent, IDC_SERVER_ROUNDS,
		"",
		wxPoint((int) 97.5, (int) 27), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticBox(parent, -1,
		"Game Options",
		wxPoint((int) 10.5, (int) 10.5), wxSize((int) 339, (int) 99));
	IDC_WAIT_TIME_CTRL = 
		new wxComboBox(parent, IDC_WAIT_TIME,
		"",
		wxPoint((int) 250.5, (int) 27), wxSize((int) 87, (int) 124.5),
		0, 0, wxCB_READONLY);
	IDC_WAIT_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Wait Time :",
		wxPoint((int) 195, (int) 30));
	IDC_SHOT_TIME_CTRL = 
		new wxComboBox(parent, IDC_SHOT_TIME,
		"",
		wxPoint((int) 250.5, (int) 52.5), wxSize((int) 87, (int) 124.5),
		0, 0, wxCB_READONLY);
	IDC_SHOT_TIME_CTRL_TEXT = 
		new wxStaticText(parent, -1,
		"Shot Time :",
		wxPoint((int) 195, (int) 55.5));
	IDC_IDLE_TIME_CTRL = 
		new wxComboBox(parent, IDC_IDLE_TIME,
		"",
		wxPoint((int) 250.5, (int) 78), wxSize((int) 87, (int) 124.5),
		0, 0, wxCB_READONLY);
	IDC_IDLE_TIME_CTRL_TEXT =
		new wxStaticText(parent, -1,
		"Idle Time :",
		wxPoint((int) 199.5, (int) 81));
}

