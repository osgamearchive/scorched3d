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


// WARNING: This was autogenerated from a windows .rc file!!
// Do not edit directly

#include "SettingsLand-def.cpp"

static void createControls(wxWindow *parent)
{
	new wxStaticText(parent, -1,
		"Land Coverage :",
		wxPoint((int) 93, (int) 54));
	IDC_LANDCOVERAGE_CTRL = 
		new wxComboBox(parent, IDC_LANDCOVERAGE,
		"",
		wxPoint((int) 181.5, (int) 51), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticText(parent, -1,
		"Land Height :",
		wxPoint((int) 108, (int) 75));
	IDC_LANDHEIGHT_CTRL = 
		new wxComboBox(parent, IDC_LANDHEIGHT,
		"",
		wxPoint((int) 181.5, (int) 74), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticText(parent, -1,
		"Land Width X :",
		wxPoint((int) 108, (int) 96));
	IDC_LANDWIDTHX_CTRL = 
		new wxComboBox(parent, IDC_LANDWIDTHX,
		"",
		wxPoint((int) 181.5, (int) 97), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticText(parent, -1,
		"Land Width Y :",
		wxPoint((int) 108, (int) 117));
	IDC_LANDWIDTHY_CTRL = 
		new wxComboBox(parent, IDC_LANDWIDTHY,
		"",
		wxPoint((int) 181.5, (int) 120), wxSize((int) 72, (int) 124.5),
		0, 0, wxCB_READONLY);
	new wxStaticBox(parent, -1,
		"Land",
		wxPoint((int) 10.5, (int) 10.5), wxSize((int) 339, (int) 140.5));

}

