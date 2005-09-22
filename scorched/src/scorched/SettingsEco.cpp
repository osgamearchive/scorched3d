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

#include "SettingsEco-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	wxSizer *sizer = new wxGridSizer(2, 2);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	sizer->Add(new wxStaticText(parent, -1,
		wxT("Start Money :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_STARTMONEY_CTRL = 
		new wxComboBox(parent, IDC_STARTMONEY,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Percentage Interest :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_INTEREST_CTRL = 
		new wxComboBox(parent, IDC_INTEREST,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Buy On Round :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_BUYONROUND_CTRL = 
		new wxComboBox(parent, IDC_BUYONROUND,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Money Per hit Point :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERHIT_CTRL = 
		new wxComboBox(parent, IDC_MONEYPERHIT,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Money Per kill Point :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERKILL_CTRL = 
		new wxComboBox(parent, IDC_MONEYPERKILL,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Money Per Round Won :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERROUND_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);

	sizer->Add(new wxStaticText(parent, -1,
		wxT("Money Per Round :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYROUND_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Scoring Mode :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_SCOREMODE_CTRL = 
		new wxComboBox(parent, IDC_SCOREMODE,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);

        sizer->Add(new wxStaticText(parent, -1,
                wxT("Economy :")), 0, wxALIGN_CENTER);
        sizer->Add(IDC_ECONOMY_CTRL = 
                new wxComboBox(parent, IDC_ECONOMY,
                wxT(""), 
                wxDefaultPosition, wxDefaultSize,
                0, 0, wxCB_READONLY), 0, wxALIGN_CENTER); 
		
	topsizer->Add(IDC_MONEYPERHEALTH_CTRL = 
		new wxCheckBox(parent, IDC_MONEYPERHEALTH,
		wxT("Money Awarded by Health Taken")), 0, wxALIGN_CENTER);
}

