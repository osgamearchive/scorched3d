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

#include "SettingsLand-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	wxSizer *sizer = new wxGridSizer(2, 2);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	sizer->Add(new wxStaticText(parent, -1,
		"Land Coverage :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_LANDCOVERAGE_CTRL = 
		new wxComboBox(parent, IDC_LANDCOVERAGE,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Land Height :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_LANDHEIGHT_CTRL = 
		new wxComboBox(parent, IDC_LANDHEIGHT,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Land Width X :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_LANDWIDTHX_CTRL = 
		new wxComboBox(parent, IDC_LANDWIDTHX,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
		
	sizer->Add(new wxStaticText(parent, -1,
		"Land Width Y :"), 0, wxALIGN_CENTER);
	sizer->Add(IDC_LANDWIDTHY_CTRL = 
		new wxComboBox(parent, IDC_LANDWIDTHY,
		"",
		wxDefaultPosition, wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY), 0, wxALIGN_CENTER);
}

