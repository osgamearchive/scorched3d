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
		"Start Money :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_STARTMONEY_CTRL = 
		new wxComboBox(parent, IDC_STARTMONEY,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Percentage Interest :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_INTEREST_CTRL = 
		new wxComboBox(parent, IDC_INTEREST,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 148.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Buy On Round :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_BUYONROUND_CTRL = 
		new wxComboBox(parent, IDC_BUYONROUND,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Money Per hit Point :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERHIT_CTRL = 
		new wxComboBox(parent, IDC_MONEYPERHIT,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Money Per kill Point :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERKILL_CTRL = 
		new wxComboBox(parent, IDC_MONEYPERKILL,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Money Per Round Won :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MONEYPERROUND_CTRL = 
		new wxComboBox(parent, IDC_MONEYPERROUND,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Scoring Mode :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_SCOREMODE_CTRL = 
		new wxComboBox(parent, IDC_SCOREMODE,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	topsizer->Add(IDC_MONEYPERHEALTH_CTRL = 
		new wxCheckBox(parent, IDC_MONEYPERHEALTH,
		"Money Awarded by Health Taken"), 0, wxALIGN_CENTER);
}

