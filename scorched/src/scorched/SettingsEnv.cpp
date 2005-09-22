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

#include "SettingsEnv-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	wxSizer *sizer = new wxGridSizer(2, 2);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	sizer->Add(new wxStaticText(parent, -1,
		wxT("Wind Force :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_FORCE_CTRL = 
		new wxComboBox(parent, IDC_COMBO_FORCE,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Wind Changes :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_WINDCHANGES_CTRL = 
		new wxComboBox(parent, IDC_COMBO_WINDCHANGES,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Wall Type :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_WALLTYPE_CTRL = 
		new wxComboBox(parent, IDC_COMBO_WALLTYPE,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Weapon Scale :")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_WEAPONSCALE_CTRL = 
		new wxComboBox(parent, IDC_COMBO_WEAPONSCALE,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);

	sizer->Add(new wxStaticText(parent, -1,
		wxT("Arms Level (Start Round):")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_STARTARMSLEVEL_CTRL = 
		new wxComboBox(parent, IDC_COMBO_STARTARMSLEVEL,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Arms Level (End Round):")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_ENDARMSLEVEL_CTRL = 
		new wxComboBox(parent, IDC_COMBO_ENDARMSLEVEL,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Falling Distance")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_FALLINGDISTANCE_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Climbing Distance")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_COMBO_CLIMBINGDISTANCE_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Resign Mode")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_RESIGNENDROUND_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(parent, -1,
		wxT("Movement Restrictions")), 0, wxALIGN_CENTER);
	sizer->Add(IDC_MOVEMENTRESTRICTION_CTRL = 
		new wxComboBox(parent, -1,
		wxT(""),
		wxDefaultPosition, wxDefaultSize,
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
	topsizer->Add(IDC_GIVEALLWEAPONS_CTRL = 
		new wxCheckBox(parent, -1,
			wxT("Give all weapons"),
			wxDefaultPosition, wxDefaultSize),
			0, wxALIGN_CENTER);
}

