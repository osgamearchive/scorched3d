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

static void createControls(wxWindow *parent, wxSizer *sizer)
{
	// Display settings
	wxStaticBox *displayBox = new wxStaticBox(parent, -1, "Display");
	wxStaticBoxSizer *displaySizer = new wxStaticBoxSizer(displayBox, wxHORIZONTAL);
	wxFlexGridSizer *displaySizer2 = new wxFlexGridSizer(2, 3, 5, 5);
	wxStaticText *resText = new wxStaticText(parent, -1, "Resolution :");
	IDC_DISPLAY_CTRL = 
		new wxComboBox(parent, -1,
		"",
		wxDefaultPosition, wxSize((int) 315, -1),
		0, 0, wxCB_READONLY);
	IDC_FULLSCREEN_CTRL = 
		new wxCheckBox(parent, -1, "Full Screen");
	displaySizer2->Add(resText, 0, wxALIGN_CENTER_VERTICAL);
	displaySizer2->Add(IDC_DISPLAY_CTRL, 0);
	displaySizer2->Add(IDC_FULLSCREEN_CTRL, 0, wxALIGN_CENTRE_VERTICAL);
	wxStaticText *brightText = new wxStaticText(parent, -1, "Brightness :");
	IDC_SLIDER1_CTRL = 
		new wxSlider(parent, -1,
		0,0,0,
		wxDefaultPosition, wxSize((int) 315, -1),
		wxSL_HORIZONTAL | wxSL_AUTOTICKS);
	displaySizer2->Add(brightText, 0, wxALIGN_CENTER_VERTICAL);
	displaySizer2->Add(IDC_SLIDER1_CTRL, 0);
	displaySizer->Add(displaySizer2, 0);
	sizer->Add(displaySizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// Texture sizes (small med large)
	wxStaticBox *textureBox = new wxStaticBox(parent, -1, "Level of Detail settings");
	wxStaticBoxSizer *textureSizer = new wxStaticBoxSizer(textureBox, wxHORIZONTAL);
	wxGridSizer *textureSizer2 = new wxGridSizer(3, 4, 10, 10);
	wxStaticText *texSizeText = new wxStaticText(parent, -1, "Texture Sizes :");
	textureSizer2->Add(texSizeText, 0, wxALIGN_CENTER_VERTICAL);
	IDC_SMALLTEX_CTRL = new wxRadioButton(parent, -1, "Small (3dfx/faster)", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	textureSizer2->Add(IDC_SMALLTEX_CTRL, 0);
	IDC_MEDIUMTEX_CTRL = new wxRadioButton(parent, -1, "Medium");
	textureSizer2->Add(IDC_MEDIUMTEX_CTRL, 0);
	IDC_LARGETEX_CTRL = new wxRadioButton(parent, -1, "Large (slower)");
	textureSizer2->Add(IDC_LARGETEX_CTRL, 0);
	wxStaticText *tankSizeText = new wxStaticText(parent, -1, "Tank Detail :");
	textureSizer2->Add(tankSizeText, 0, wxALIGN_CENTER_VERTICAL);
	IDC_LOWTANK_CTRL = new wxRadioButton(parent, -1, "Low (faster)", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	textureSizer2->Add(IDC_LOWTANK_CTRL, 0);
	IDC_MEDIUMTANK_CTRL = new wxRadioButton(parent, -1, "Medium");
	textureSizer2->Add(IDC_MEDIUMTANK_CTRL, 0);
	IDC_HIGHTANK_CTRL = new wxRadioButton(parent, -1, "Max (slower)");
	textureSizer2->Add(IDC_HIGHTANK_CTRL, 0);
	wxStaticText *effectSizeText = new wxStaticText(parent, -1, "Effects Detail :");
	textureSizer2->Add(effectSizeText, 0, wxALIGN_CENTER_VERTICAL);
	IDC_LOWEFFECTS_CTRL = new wxRadioButton(parent, -1, "Low (faster)", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	textureSizer2->Add(IDC_LOWEFFECTS_CTRL, 0);
	IDC_MEDIUMEFFECTS_CTRL = new wxRadioButton(parent, -1, "Medium");
	textureSizer2->Add(IDC_MEDIUMEFFECTS_CTRL, 0);
	IDC_HIGHEFFECTS_CTRL = new wxRadioButton(parent, -1, "High (slower)");
	textureSizer2->Add(IDC_HIGHEFFECTS_CTRL, 0);
	textureSizer->Add(textureSizer2, 0, wxGROW);
	sizer->Add(textureSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// Detail switches 
	wxStaticBox *detailBox = new wxStaticBox(parent, -1, 
		"Detail (Turn off to speed up the game)");
	wxStaticBoxSizer *detailSizer = new wxStaticBoxSizer(detailBox, wxHORIZONTAL);
	wxGridSizer *detailSizer2 = new wxGridSizer(3, 3, 10, 10);
	IDC_SINGLESKYLAYER_CTRL = 
		new wxCheckBox(parent, -1, "Single sky layer",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_SINGLESKYLAYER_CTRL, 0);
	IDC_NOSKYANI_CTRL = 
		new wxCheckBox(parent, -1, "Don't animate sky",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOSKYANI_CTRL, 0);
	IDC_NOWATERANI_CTRL = 
		new wxCheckBox(parent, -1, "Don't animate water",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOWATERANI_CTRL, 0);
	IDC_NOWATER_CTRL = 
		new wxCheckBox(parent, -1, "Don't draw water",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOWATER_CTRL, 0);
	IDC_NODETAILTEX_CTRL = 
		new wxCheckBox(parent, -1, "Don't use detail textures",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NODETAILTEX_CTRL, 0);
	IDC_NOSKINS_CTRL = 
		new wxCheckBox(parent, -1, "No tank skins",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOSKINS_CTRL, 0);
	detailSizer->Add(detailSizer2, 0, wxGROW);
	sizer->Add(detailSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// Troubleshooting switches 
	wxStaticBox *troubleBox = new wxStaticBox(parent, -1,
		"Trouble shooting (Turn off if Scorched crashes)");
	wxStaticBoxSizer *troubleSizer = new wxStaticBoxSizer(troubleBox, wxHORIZONTAL);
	wxGridSizer *troubleSizer2 = new wxGridSizer(3, 3, 10, 10);
	IDC_NOEXT_CTRL = 
		new wxCheckBox(parent, -1, "No GL Extensions",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOEXT_CTRL, 0);
	IDC_NOMULTITEX_CTRL = 
		new wxCheckBox(parent, -1, "No multi texture",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOMULTITEX_CTRL, 0);
	IDC_NOLANDSCAPESCORCH_CTRL = 
		new wxCheckBox(parent, -1, "No TexSubImaging",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOLANDSCAPESCORCH_CTRL, 0);
	IDC_NOCOMPILEDARRAYS_CTRL = 
		new wxCheckBox(parent, -1, "No compiled arrays",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOCOMPILEDARRAYS_CTRL, 0);
	IDC_NOENVCOMBINE_CTRL = 
		new wxCheckBox(parent, -1, "No combined textures",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOENVCOMBINE_CTRL, 0);
	IDC_NOCUBEMAP_CTRL = 
		new wxCheckBox(parent, -1, "No cube map textures",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOCUBEMAP_CTRL, 0);
	IDC_NOMIPMAPS_CTRL = 
		new wxCheckBox(parent, -1, "No HW mip maps",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOMIPMAPS_CTRL, 0);
	IDC_FULLCLEAR_CTRL = 
		new wxCheckBox(parent, -1, "Full Clear",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_FULLCLEAR_CTRL, 0);
	troubleSizer->Add(troubleSizer2, 0, wxGROW);
	sizer->Add(troubleSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// Misc switches 
	wxStaticBox *miscBox = new wxStaticBox(parent, -1, 
		"Misc. Options");
	wxStaticBoxSizer *miscSizer = new wxStaticBoxSizer(miscBox, wxHORIZONTAL);
	wxGridSizer *miscSizer2 = new wxGridSizer(3, 3, 10, 10);
	IDC_INVERT_CTRL = 
		new wxCheckBox(parent, -1, "Invert up/down keys",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_INVERT_CTRL, 0);
	IDC_TIMER_CTRL = 
		new wxCheckBox(parent, -1, "Show frames per second",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_TIMER_CTRL, 0);
	IDC_NOSOUND_CTRL = 
		new wxCheckBox(parent, -1, "No Sound",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_NOSOUND_CTRL, 0);
	miscSizer->Add(miscSizer2, 0, wxGROW);
	sizer->Add(miscSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// User id edit box
	wxStaticBox *userBox = new wxStaticBox(parent, -1, 
		"User ID (Uniquely identifies this player for stats, not generated from any user information.)");
	wxStaticBoxSizer *userSizer = new wxStaticBoxSizer(userBox, wxVERTICAL);
	IDC_USERID_CTRL = new wxTextCtrl(parent, -1, wxString(), wxDefaultPosition, wxSize(300, -1));
	userSizer->Add(IDC_USERID_CTRL, 0, wxALIGN_CENTER);
	IDC_HOSTDESC_CTRL = new wxTextCtrl(parent, -1, wxString(), wxDefaultPosition, wxSize(300, -1));
	userSizer->Add(IDC_HOSTDESC_CTRL, 0, wxALIGN_CENTER);
	sizer->Add(userSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(parent, wxID_OK, "Ok");
	IDCANCEL_CTRL = new wxButton(parent, wxID_CANCEL, "Cancel");
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxALL, 10);
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 10);
	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
}

