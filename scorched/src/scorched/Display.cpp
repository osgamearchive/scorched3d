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


#include "Display-def.cpp"

static void createControls(wxWindow *parent)
{
	IDC_FULLCLEAR_CTRL = 
		new wxCheckBox(parent, IDC_FULLCLEAR,
		"Full Clear - Clears screen instead of overwriting for each frame (slower)",
		wxPoint((int) 27, (int) 228));
	new wxStaticBox(parent, -1,
		"Switches (Switches used to diagnose problems)",
		wxPoint((int) 10.5, (int) 213), wxSize((int) 519, (int) 114));
	IDC_NOEXT_CTRL = 
		new wxCheckBox(parent, IDC_NOEXT,
		"No GL Extensions - Don't use any OpenGL extensions (3dfx)",
		wxPoint((int) 27, (int) 248));
	IDC_NOSOUND_CTRL = 
		new wxCheckBox(parent, IDC_NOSOUND,
		"No Sound - Don't use any sound",
		wxPoint((int) 27, (int) 288));
	IDC_NOMULTITEX_CTRL = 
		new wxCheckBox(parent, IDC_NOMULTITEX,
		"No Multi Texture - Only use one texture per face (faster)",
		wxPoint((int) 27, (int) 268));
	new wxStaticBox(parent, -1,
		"Texture Sizes (Larger is slower but of better quality)",
		wxPoint((int) 10.5, (int) 84), wxSize((int) 519, (int) 43.5));
	IDC_RADIO_SMALL_CTRL = 
		new wxRadioButton(parent, IDC_RADIO_SMALL,
		"Small (3dfx and fastest)",
		wxPoint((int) 66, (int) 103.5));
	IDC_RADIO_MEDIUM_CTRL = 
		new wxRadioButton(parent, IDC_RADIO_MEDIUM,
		"Medium",
		wxPoint((int) 247.5, (int) 103.5));
	IDC_RADIO_LARGE_CTRL = 
		new wxRadioButton(parent, IDC_RADIO_LARGE,
		"Large (slowest)",
		wxPoint((int) 387, (int) 105));
	new wxStaticBox(parent, -1,
		"Display",
		wxPoint((int) 10.5, (int) 4.5), wxSize((int) 519, (int) 76.5));
	IDC_FULLSCREEN_CTRL = 
		new wxCheckBox(parent, IDC_FULLSCREEN,
		"Full Screen",
		wxPoint((int) 27, (int) 52.5));
	IDC_DISPLAY_CTRL = 
		new wxComboBox(parent, IDC_DISPLAY,
		"",
		wxPoint((int) 198, (int) 21), wxSize((int) 301.5, (int) 129),
		0, 0, wxCB_READONLY);
	new wxStaticText(parent, -1,
		"Brightness :",
		wxPoint((int) 129, (int) 54), wxSize((int) 58.5, (int) 12));
	IDC_SLIDER1_CTRL = 
		new wxSlider(parent, IDC_SLIDER1,
		0,0,0,
		wxPoint((int) 190.5, (int) 43.5), wxSize((int) 315, (int) 33),
		wxSL_HORIZONTAL | wxSL_AUTOTICKS);
	new wxStaticText(parent, -1,
		"Resolution :",
		wxPoint((int) 129, (int) 27), wxSize((int) 58.5, (int) 12));
	new wxStaticBox(parent, -1,
		"Misc. Settings",
		wxPoint((int) 10.5, (int) 130.5), wxSize((int) 519, (int) 79.5));
	IDC_SINGLESKYLAYER_CTRL = 
		new wxCheckBox(parent, IDC_SINGLESKYLAYER,
		"Single sky layer",
		wxPoint((int) 27, (int) 147));
	IDC_NOSKYANI_CTRL = 
		new wxCheckBox(parent, IDC_NOSKYANI,
		"Don't animate sky",
		wxPoint((int) 27, (int) 167));
	IDC_NOWATERANI_CTRL = 
		new wxCheckBox(parent, IDC_NOWATERANI,
		"Don't animate water",
		wxPoint((int) 27, (int) 187));
	IDC_NOWATER_CTRL = 
		new wxCheckBox(parent, IDC_NOWATER,
		"Don't draw water",
		wxPoint((int) 200, (int) 147));
	IDC_LANDSCAPETEX_CTRL = 
		new wxCheckBox(parent, IDC_LANDSCAPETEX,
		"Don't texture landscape",
		wxPoint((int) 200, (int) 167));
	IDC_INVERT_CTRL = 
		new wxCheckBox(parent, IDC_INVERT,
		"Invert up/down keys",
		wxPoint((int) 200, (int) 187));
	IDC_TIMER_CTRL = 
		new wxCheckBox(parent, IDC_TIMER,
		"Show frames per second",
		wxPoint((int) 370, (int) 147));
	IDC_NODETAILTEX_CTRL = 
		new wxCheckBox(parent, IDC_NODETAILTEX,
		"Don't use detail textures",
		wxPoint((int) 370, (int) 167));
	IDC_NOSKINS_CTRL = 
		new wxCheckBox(parent, IDC_NOSKINS,
		"No tank skins",
		wxPoint((int) 370, (int) 187));
	IDC_NOLANDSCAPESCORCH_CTRL = 
		new wxCheckBox(parent, IDC_NOLANDSCAPESCORCH,
		"Don't use TexSubImaging",
		wxPoint((int) 27, (int) 308));
	new wxStaticBox(parent, -1,
		"User ID (Uniquely identifies this player for stats, not generated from any user information.)",
		wxPoint((int) 10.5, (int) 332), wxSize((int) 519, (int) 43.5));
	IDC_USERID_CTRL = 
		new wxTextCtrl(parent, IDC_USERID,
		"", 
		wxPoint((int) 27, (int) 345), wxSize((int) 300, (int) 23.5));
	IDOK_CTRL = 
		new wxButton(parent, wxID_OK,
		"Ok",
		wxPoint((int) 454.5, (int) 384.5), wxSize((int) 75, (int) 21));
	IDCANCEL_CTRL = 
		new wxButton(parent, wxID_CANCEL,
		"Cancel",
		wxPoint((int) 373.5, (int) 384.5), wxSize((int) 75, (int) 21));
}

