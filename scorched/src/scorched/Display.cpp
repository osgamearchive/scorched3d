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

static void createMainControls(wxWindow *parent, wxSizer *sizer)
{
	{
	// Display settings
	wxStaticBox *displayBox = new wxStaticBox(parent, -1, "Display");
	wxStaticBoxSizer *displaySizer = new wxStaticBoxSizer(displayBox, wxVERTICAL);
	wxFlexGridSizer *displaySizer2 = new wxFlexGridSizer(2, 3, 5, 5);
	wxStaticText *resText = new wxStaticText(parent, -1, "Resolution :");
	IDC_DISPLAY_CTRL = 
		new wxComboBox(parent, -1,
		"",
		wxDefaultPosition, wxSize((int) 315, -1),
		0, 0, wxCB_READONLY);
	IDC_FULLSCREEN_CTRL = 
		new wxCheckBox(parent, -1, "Full Screen");
	IDC_MORERES_CTRL =
		new wxCheckBox(parent, ID_MORERES, "More Resolutions");
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
	displaySizer2->Add(IDC_MORERES_CTRL, 0, wxALIGN_CENTRE_VERTICAL);
	displaySizer->Add(displaySizer2, 0);

	wxFlexGridSizer *displaySizer3 = new wxFlexGridSizer(1, 6, 5, 5);
	IDC_TINYDIALOGS_CTRL = new wxRadioButton(parent, -1, "Tiny", 
		wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	IDC_SMALLDIALOGS_CTRL = new wxRadioButton(parent, -1, "Small");
	IDC_MEDIUMDIALOGS_CTRL = new wxRadioButton(parent, -1, "Medium");
	IDC_LARGEDIALOGS_CTRL = new wxRadioButton(parent, -1, "Large");
	IDC_HUGEDIALOGS_CTRL = new wxRadioButton(parent, -1, "Huge");
	displaySizer3->Add(new wxStaticText(parent, -1, "Dialog/Font Sizes :"));
	displaySizer3->Add(IDC_TINYDIALOGS_CTRL);
	displaySizer3->Add(IDC_SMALLDIALOGS_CTRL);
	displaySizer3->Add(IDC_MEDIUMDIALOGS_CTRL);
	displaySizer3->Add(IDC_LARGEDIALOGS_CTRL);
	displaySizer3->Add(IDC_HUGEDIALOGS_CTRL);
	displaySizer->Add(displaySizer3, 0, wxTOP | wxBOTTOM, 10);
	sizer->Add(displaySizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
	}

	{
	// Sound settings
	wxStaticBox *soundBox = new wxStaticBox(parent, -1, "Sound");
	wxStaticBoxSizer *soundSizer = new wxStaticBoxSizer(soundBox, wxVERTICAL);
	wxBoxSizer *soundSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *volumeText = new wxStaticText(parent, -1, "Volume :");
	IDC_NOSOUND_CTRL = 
		new wxCheckBox(parent, -1, "No Sound");
	IDC_VOLUME_CTRL = 
		new wxSlider(parent, -1,
		0,0,0,
		wxDefaultPosition, wxSize((int) 250, -1),
		wxSL_HORIZONTAL | wxSL_AUTOTICKS);
	IDC_SOUNDCHANNELS_CTRL = 
		new wxComboBox(parent, -1,
			"",
			wxDefaultPosition, wxSize((int) 50, (int) 91.5),
			0, 0, wxCB_READONLY);
	soundSizer2->Add(volumeText, 0, wxRIGHT, 10);
	soundSizer2->Add(IDC_VOLUME_CTRL, 0, wxRIGHT, 10);
	soundSizer2->Add(IDC_NOSOUND_CTRL, 0, wxRIGHT, 10);
	soundSizer2->Add(IDC_SOUNDCHANNELS_CTRL, 0, wxRIGHT, 5);
	soundSizer2->Add(new wxStaticText(parent, -1, "Channels"), 0, wxALIGN_CENTRE_VERTICAL);
	soundSizer->Add(soundSizer2, 0);
	sizer->Add(soundSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
	}

	{
	// Misc switches 
	wxStaticBox *miscBox = new wxStaticBox(parent, -1, 
		"Misc. Options");
	wxStaticBoxSizer *miscSizer = new wxStaticBoxSizer(miscBox, wxHORIZONTAL);
	wxGridSizer *miscSizer2 = new wxGridSizer(3, 3, 10, 10);
	IDC_INVERT_CTRL = 
		new wxCheckBox(parent, -1, "Invert key elevation",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_INVERT_CTRL, 0);
	IDC_INVERTMOUSE_CTRL = 
		new wxCheckBox(parent, -1, "Invert mouse elevation",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_INVERTMOUSE_CTRL, 0);
	IDC_SWAPYAXIS_CTRL = 
		new wxCheckBox(parent, -1, "Invert mouse y axis (OS X)",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_SWAPYAXIS_CTRL, 0);
	IDC_TIMER_CTRL = 
		new wxCheckBox(parent, -1, "Show frames per second",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_TIMER_CTRL, 0);
	IDC_TOOLTIP_CTRL = 
		new wxCheckBox(parent, -1, "Show ToolTips",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_TOOLTIP_CTRL, 0);
	IDC_SMOUSE_CTRL = 
		new wxCheckBox(parent, -1, "Software Mouse",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_SMOUSE_CTRL, 0);
	IDC_SIDESCROLL_CTRL = 
		new wxCheckBox(parent, -1, "Side Scrolling",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	miscSizer2->Add(IDC_SIDESCROLL_CTRL, 0);
	miscSizer->Add(miscSizer2, 0, wxGROW);
	sizer->Add(miscSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
	}

	{
	// Load settings
	wxBoxSizer *loadSizer = new wxBoxSizer(wxHORIZONTAL);
	IDC_LOADDEFAULTS_CTRL = new wxButton(parent, ID_LOADDEFAULTS, "Restore Defaults");
	IDC_LOADSAFE_CTRL = new wxButton(parent, ID_LOADSAFE, "Safe Options");
	loadSizer->Add(IDC_LOADDEFAULTS_CTRL, 0, wxALL, 10);
	loadSizer->Add(IDC_LOADSAFE_CTRL, 0, wxALL, 10);
	sizer->Add(loadSizer, 0, wxALIGN_CENTER);
	}
}

static void createTroubleControls(wxWindow *parent, wxSizer *sizer)
{
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
		new wxCheckBox(parent, -1, "Draw simplistic water",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOWATERANI_CTRL, 0);
	IDC_NOWATER_CTRL = 
		new wxCheckBox(parent, -1, "Don't draw water",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOWATER_CTRL, 0);
	IDC_NOSURROUND_CTRL = 
		new wxCheckBox(parent, -1, "Don't draw surround",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOSURROUND_CTRL, 0);
	IDC_NODETAILTEX_CTRL = 
		new wxCheckBox(parent, -1, "Don't use detail textures",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NODETAILTEX_CTRL, 0);
	IDC_NOSKINS_CTRL = 
		new wxCheckBox(parent, -1, "No tank skins",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOSKINS_CTRL, 0);
	IDC_NOTREES_CTRL = 
		new wxCheckBox(parent, -1, "No trees",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOTREES_CTRL, 0);
	IDC_NOPRECIPITATION_CTRL = 
		new wxCheckBox(parent, -1, "No precipitation",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOPRECIPITATION_CTRL, 0);
	IDC_NODEPTHSORT_CTRL = 
		new wxCheckBox(parent, -1, "No sprite depth sorting",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NODEPTHSORT_CTRL, 0);
	IDC_NOWAVES_CTRL = 
		new wxCheckBox(parent, -1, "No waves at shore",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	detailSizer2->Add(IDC_NOWAVES_CTRL, 0);
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
	IDC_NOSPHEREMAP_CTRL = 
		new wxCheckBox(parent, -1, "No sphere map textures",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOSPHEREMAP_CTRL, 0);
	IDC_NOVBO_CTRL = 
		new wxCheckBox(parent, -1, "No VBO",
		wxDefaultPosition, wxSize((int) 150, (int) -1));
	troubleSizer2->Add(IDC_NOVBO_CTRL, 0);

	troubleSizer->Add(troubleSizer2, 0, wxGROW);
	sizer->Add(troubleSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
}

static void createIdentControls(wxWindow *parent, wxSizer *sizer)
{
	{
	// User name edit box
	wxStaticBox *userNameBox = new wxStaticBox(parent, -1, 
		"Online User Name");
	wxStaticBoxSizer *userNameSizer = new wxStaticBoxSizer(userNameBox, wxVERTICAL);
	IDC_USERNAME_CTRL = new wxTextCtrl(parent, -1, wxString(), wxDefaultPosition, wxSize(300, -1));
	userNameSizer->Add(IDC_USERNAME_CTRL, 0, wxALIGN_CENTER);
	sizer->Add(userNameSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
	}

	{
	// TankModel edit box
	wxStaticBox *tankModelBox = new wxStaticBox(parent, -1, 
		"Online Tank Model");
	wxStaticBoxSizer *tankModelSizer = new wxStaticBoxSizer(tankModelBox, wxVERTICAL);
	IDC_TANKMODEL_CTRL = new wxTextCtrl(parent, -1, wxString(), wxDefaultPosition, wxSize(300, -1));
	tankModelSizer->Add(IDC_TANKMODEL_CTRL, 0, wxALIGN_CENTER);
	sizer->Add(tankModelSizer, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
	}

	UniqueIdStore idStore;
	idStore.loadStore();

	// User id edit box
	wxStaticBox *userBox = new wxStaticBox(parent, -1, 
		"User ID (Uniquely identifies this player for stats, not generated from any user information.)");
	wxStaticBoxSizer *userSizer = new wxStaticBoxSizer(userBox, wxVERTICAL);
	IDC_USERID_CTRL = new wxGrid(parent, -1, wxDefaultPosition, wxDefaultSize);
	IDC_USERID_CTRL->CreateGrid(idStore.getIds().size(), 3);
	IDC_USERID_CTRL->SetColLabelValue(0,"Published Ip");
	IDC_USERID_CTRL->SetColLabelValue(1, "Current Ip");
	IDC_USERID_CTRL->SetColLabelValue(2, "Unique Id");
	IDC_USERID_CTRL->SetColSize(0, 200);
	IDC_USERID_CTRL->SetColSize(1, 100);
	IDC_USERID_CTRL->SetColSize(2, 130);
	IDC_USERID_CTRL->SetColLabelSize(20);
	IDC_USERID_CTRL->SetRowLabelSize(0);
	int pos = 0;
	std::list<UniqueIdStore::Entry>::iterator itor;
	for (itor = idStore.getIds().begin();
		itor != idStore.getIds().end();
		itor++, pos++)
	{
		UniqueIdStore::Entry &entry = *itor;
		IDC_USERID_CTRL->SetCellValue(pos, 0, entry.published.c_str());
		IDC_USERID_CTRL->SetCellValue(pos, 1, NetInterface::getIpName(entry.ip));
		IDC_USERID_CTRL->SetCellValue(pos, 2, entry.id.c_str());
	}
	IDC_USERID_CTRL->EnableEditing(false);
	IDC_USERID_CTRL->ForceRefresh();

	userSizer->Add(IDC_USERID_CTRL, 1, wxALIGN_CENTER | wxGROW);
	IDC_HOSTDESC_CTRL = new wxTextCtrl(parent, -1, wxString(), wxDefaultPosition, wxSize(300, -1), wxTE_READONLY);
	userSizer->Add(IDC_HOSTDESC_CTRL, 0, wxALIGN_CENTER);
	sizer->Add(userSizer, 1, wxGROW | wxLEFT | wxRIGHT | wxTOP, 5);
}

class KeyButtonData : public wxObjectRefData
{
public:
	KeyButtonData(const char *key, unsigned int position);
	virtual ~KeyButtonData();

	std::string key_;
	unsigned int position_;
};

KeyButtonData::KeyButtonData(const char *key, unsigned int position) :
	wxObjectRefData(),
	key_(key), position_(position)
{
}

KeyButtonData::~KeyButtonData()
{
}

static std::list<wxButton *> keyboardKeyList;

static void createKeysControls(wxWindow *parent, wxSizer *topsizer)
{
	wxScrolledWindow *scrolledWindow = new wxScrolledWindow(parent, -1);
	wxSizer *sizer = new wxFlexGridSizer(5, 1);
	
	keyboardKeyList.clear();
	if (!Keyboard::instance()->loadKeyFile())
	{
		dialogExit("Keyboard", "Failed to process keyboad file keys.xml");
	}

	int lastGroup = 0;
	std::list<KeyboardKey *> &keys = 
		Keyboard::instance()->getKeyList();
	std::list<KeyboardKey *>::iterator itor;
	for (itor = keys.begin();
		itor != keys.end();
		itor++)
	{
		KeyboardKey *key = (*itor);

		// Add a spacer line
		if (key->getGroup() != lastGroup)
		{
			lastGroup = key->getGroup();
			for (unsigned int i=0; i<5; i++)
			{
				sizer->Add(new wxStaticText(scrolledWindow, -1, ""), 0, wxALIGN_LEFT);
			}
		}

		// Add the key name
		wxStaticText *text = new wxStaticText(scrolledWindow, -1, key->getTitle());
		text->SetToolTip(key->getDescription());
		sizer->Add(text, 0, wxALIGN_LEFT);

		// Add the keys
		for (unsigned int i=0; i<4; i++)
		{
			wxButton *button = new wxButton(scrolledWindow, ID_KEY, "", 
				wxDefaultPosition, wxSize(120, -1));
			button->SetRefData(new KeyButtonData(key->getName(), i));
			button->SetToolTip(key->getDescription());
			sizer->Add(button, 0, wxLEFT | wxALIGN_CENTER, 5);
			keyboardKeyList.push_back(button);
		}
	}
	
	// Setup the scrolled area size
	scrolledWindow->SetAutoLayout(TRUE);
	scrolledWindow->SetSizer(sizer);
	wxSize minSize = sizer->CalcMin();
	scrolledWindow->SetScrollbars(10, 10, 
		(minSize.GetWidth() + 10) / 10, (minSize.GetHeight() + 10) / 10);
	topsizer->Add(scrolledWindow, 1, wxGROW | wxALL, 2);

	// Add the load default keys button
	IDC_LOADKEYDEFAULTS_CTRL = new wxButton(parent, ID_KEYDEFAULTS, "Load Default Keys");
	topsizer->Add(IDC_LOADKEYDEFAULTS_CTRL, 0, wxCENTER | wxALL, 2);
}


