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

#include "SettingsPlayers-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	topsizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 10);

	buttonSizer->Add(new wxStaticText(parent, -1,
		wxT("Min Players :")), 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	buttonSizer->Add(IDC_SERVER_MIN_PLAYERS_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxSize(70, -1),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	buttonSizer->Add(new wxStaticText(parent, -1,
		wxT("Max Players :")), 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	buttonSizer->Add(IDC_SERVER_MAX_PLAYERS_CTRL = 
		new wxComboBox(parent, IDC_SERVER_MAX_PLAYERS,
		wxT(""),
		wxDefaultPosition, wxSize(70, -1),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);	

	wxBoxSizer *buttonSizer2 = new wxBoxSizer(wxHORIZONTAL);
	topsizer->Add(buttonSizer2, 0, wxALIGN_CENTER | wxTOP, 10);
	buttonSizer2->Add(new wxStaticText(parent, -1,
		wxT("Remove bots after players :")), 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	buttonSizer2->Add(IDC_SERVER_REMOVEBOT_PLAYERS_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);

	IDC_SERVER_RESIDUAL_CTRL = new wxCheckBox(parent, -1, 
		wxT("Players are persistent for game"));
	topsizer->Add(IDC_SERVER_RESIDUAL_CTRL, 0, wxALIGN_CENTER | wxTOP, 10);
		
	wxSizer *sizer = new wxGridSizer(3, 3);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	for (int i=0; i<24; i++)
	{
		char buffer[256];
		sprintf(buffer, "%i", (i+1));
	
		wxBoxSizer *playerSizer = new wxBoxSizer(wxHORIZONTAL);
		playerSizer->Add(new wxStaticText(parent, -1, wxString(buffer, wxConvUTF8)));
		playerSizer->Add(IDC_COMBO_PTYPE_CTRL[i] = 
			new wxComboBox(parent, -1,
			wxT(""),
			wxDefaultPosition, wxSize(70, -1),
			0, 0, wxCB_READONLY));
		sizer->Add(playerSizer, 0, wxALIGN_RIGHT | wxALL, 2);
	}

	wxBoxSizer *botSizer = new wxBoxSizer(wxHORIZONTAL);
	topsizer->Add(botSizer, 0, wxALIGN_CENTER | wxALL, 10);
	botSizer->Add(new wxStaticText(parent, -1,
		wxT("Bot Name Prefix :")), 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	botSizer->Add(IDC_EDIT3_CTRL = 
		new wxTextCtrl(parent, -1,
		wxT(""),
		wxDefaultPosition, wxSize(100, -1)), 
		0, wxALIGN_CENTER);
}

