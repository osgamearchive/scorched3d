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

#include <scorched/DisplayDialog.h>
#include <scorched/MainDialog.h>
#include <scorched/KeyDialog.h>
#include <coms/NetInterface.h>
#include <engine/ModFiles.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <common/Keyboard.h>
#include <client/UniqueIdStore.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/listbox.h>
#include <set>
#include <stdio.h>
#include "Display.cpp"

extern char scorched3dAppName[128];
extern char *displayOptions;

class DisplayFrame: public wxDialog
{
public:
	DisplayFrame();

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onLoadDefaultsButton(wxCommandEvent &event);
	void onLoadSafeButton(wxCommandEvent &event);
	void onLoadDefaultKeysButton(wxCommandEvent &event);
	void onKeyButton(wxCommandEvent &event);
	void onImportMod(wxCommandEvent &event);
	void onExportMod(wxCommandEvent &event);
	void onMoreRes(wxCommandEvent &event);
	void onKey();

	wxNotebook *book_;
	wxPanel *mainPanel_;
	wxPanel *troublePanel_;
	wxPanel *identPanel_;
	wxPanel *keysPanel_;
	wxPanel *modsPanel_;

protected:
	DECLARE_EVENT_TABLE()

	void refreshScreen();
	void refreshResolutions();
	void refreshKeysControls();
};

BEGIN_EVENT_TABLE(DisplayFrame, wxDialog)
    EVT_BUTTON(ID_LOADDEFAULTS,  DisplayFrame::onLoadDefaultsButton)
	EVT_BUTTON(ID_KEYDEFAULTS,  DisplayFrame::onLoadDefaultKeysButton)
    EVT_BUTTON(ID_LOADSAFE,  DisplayFrame::onLoadSafeButton)
    EVT_BUTTON(ID_KEY,  DisplayFrame::onKeyButton)
	EVT_BUTTON(ID_IMPORT,  DisplayFrame::onImportMod)
	EVT_BUTTON(ID_EXPORT,  DisplayFrame::onExportMod)
	EVT_CHECKBOX(ID_MORERES, DisplayFrame::onMoreRes) 
END_EVENT_TABLE()

DisplayFrame::DisplayFrame() :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8))
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the size for the layout
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controls
	book_ = new wxNotebook(this, -1);
	wxNotebookSizer *nbs = new wxNotebookSizer(book_);

	// Main Panel
	mainPanel_ = new wxPanel(book_, -1);
	book_->AddPage(mainPanel_, wxT("Main"));
	wxSizer *mainPanelSizer = new wxBoxSizer(wxVERTICAL);
	createMainControls(mainPanel_, mainPanelSizer);
	mainPanel_->SetAutoLayout(TRUE);
	mainPanel_->SetSizer(mainPanelSizer);
	
	// Trouble Panel
	troublePanel_ = new wxPanel(book_, -1);
	wxSizer *troublePanelSizer = new wxBoxSizer(wxVERTICAL);
	createTroubleControls(troublePanel_, troublePanelSizer);
	book_->AddPage(troublePanel_, wxT("Troubleshooting / Detail"));
	troublePanel_->SetAutoLayout(TRUE);
	troublePanel_->SetSizer(troublePanelSizer);

	// Keys Panel
	keysPanel_ = new wxPanel(book_, -1);
	wxSizer *keysPanelSizer = new wxBoxSizer(wxVERTICAL);
	createKeysControls(keysPanel_, keysPanelSizer);
	book_->AddPage(keysPanel_, wxT("Keys"));
	keysPanel_->SetAutoLayout(TRUE);
#if wxCHECK_VERSION(2,6,0)
	keysPanel_->SetSizer(keysPanelSizer);
#else
#if wxCHECK_VERSION(2,5,0)
#else
	keysPanel_->SetSizer(keysPanelSizer);
#endif
#endif

	// Ident Panel
	identPanel_ = new wxPanel(book_, -1);
	wxSizer *identPanelSizer = new wxBoxSizer(wxVERTICAL);
	createIdentControls(identPanel_, identPanelSizer);
	book_->AddPage(identPanel_, wxT("Identity"));
	identPanel_->SetAutoLayout(TRUE);
	identPanel_->SetSizer(identPanelSizer);

	// Mods Panel
	modsPanel_ = new wxPanel(book_, -1);
    wxSizer *modsPanelSizer = new wxBoxSizer(wxVERTICAL);
    createModsControls(modsPanel_, modsPanelSizer);
    book_->AddPage(modsPanel_, wxT("Mods"));
    modsPanel_->SetAutoLayout(TRUE);
    modsPanel_->SetSizer(modsPanelSizer);

	// Notebook
	topsizer->Add(nbs, 0, wxALL, 10);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(this, wxID_OK, wxT("Ok"));
	IDCANCEL_CTRL = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxRIGHT, 5);
	buttonSizer->Add(IDOK_CTRL, 0, wxLEFT, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void DisplayFrame::onLoadDefaultsButton(wxCommandEvent &event)
{
	OptionsDisplay::instance()->loadDefaultValues();
	refreshScreen();
}

void DisplayFrame::onLoadSafeButton(wxCommandEvent &event)
{
	OptionsDisplay::instance()->loadSafeValues();
	refreshScreen();
}

void DisplayFrame::onLoadDefaultKeysButton(wxCommandEvent &event)
{
	Keyboard::instance()->loadKeyFile(true);
	refreshKeysControls();
}

void DisplayFrame::onMoreRes(wxCommandEvent &event)
{
	refreshResolutions();
}

void DisplayFrame::onKeyButton(wxCommandEvent &event)
{
	// Find which button was pressed
	wxButton *chosenButton = (wxButton *) event.GetEventObject();
	KeyButtonData *chosenData = (KeyButtonData *) chosenButton->GetRefData();
	KeyboardKey *chosenKey = Keyboard::instance()->getKey(chosenData->key_.c_str());
	unsigned int chosenPosition = chosenData->position_;
	if (!chosenKey) return;

	// Ask the user for a key
	showKeyDialog(this);
	
	// Translate the key code from wx to SDL	
	unsigned int resultKey = getKeyDialogKey();
	unsigned int wxKey = (unsigned int) resultKey;
	unsigned int sdlKey = 0;
	if (!KeyboardKey::translateKeyNameWX(wxKey, sdlKey)) return;
	unsigned int sdlState = 0;
	if (getKeyDialogAlt()) sdlState = KMOD_LALT;
	else if (getKeyDialogControl()) sdlState = KMOD_LCTRL;
	else if (getKeyDialogShift()) sdlState = KMOD_LSHIFT;

	// Remove any existing occurances of this key 
	// (except when it exists on the chosen key)
	bool found = false;
	std::list<wxButton *>::iterator itor;
	for (itor = keyboardKeyList.begin();
		itor != keyboardKeyList.end();
		itor++)
	{
		wxButton *button = (*itor);
		KeyButtonData *data = (KeyButtonData *) button->GetRefData();
		KeyboardKey *key = Keyboard::instance()->getKey(data->key_.c_str());
		unsigned int position = data->position_;

		if (key)
		{
			std::vector<KeyboardKey::KeyEntry> &keyEntries = key->getKeys();
			if (position < keyEntries.size())
			{
				if (keyEntries[position].key == sdlKey &&
					keyEntries[position].state == sdlState)
				{
					if (key == chosenKey) found = true;
					else key->removeKey(position);
				}
			}
		}
	}

	// Add this key to the chosen item (unless it already exists)
	if (!found)
	{
		chosenKey->addKey(chosenPosition, sdlKey, sdlState);
	}

	// Rerfresh the key view
	refreshKeysControls();
}

void DisplayFrame::refreshKeysControls()
{
	std::list<wxButton *>::iterator buttonitor;
	for (buttonitor = keyboardKeyList.begin();
		buttonitor != keyboardKeyList.end();
		buttonitor++)
	{
		wxButton *button = (*buttonitor);			

		KeyButtonData *data = (KeyButtonData *) button->GetRefData();
		KeyboardKey *key = Keyboard::instance()->getKey(data->key_.c_str());
		unsigned int position = data->position_;

		char buffer[256];
		buffer[0] = '\0';
		if (key && position < key->getKeys().size())
		{
			const char *keyName = "";
			const char *stateName = "";
			KeyboardKey::translateKeyNameValue(key->getKeys()[position].key, keyName);
			KeyboardKey::translateKeyStateValue(key->getKeys()[position].state, stateName);
			if (strcmp(stateName, "NONE") == 0) sprintf(buffer, "%s", keyName);
			else sprintf(buffer, "<%s> %s", stateName, keyName);
		}
		button->SetLabel(wxString(buffer, wxConvUTF8));
	}
}

bool DisplayFrame::TransferDataToWindow()
{
	// Read display options from a file
	OptionsDisplay::instance()->readOptionsFromFile();

	refreshScreen();
	return true;
}

void DisplayFrame::refreshScreen()
{
	IDC_FULLCLEAR_CTRL->SetValue(OptionsDisplay::instance()->getFullClear());
	IDC_FULLCLEAR_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getFullClearToolTip(), wxConvUTF8));
	IDC_NOEXT_CTRL->SetValue(OptionsDisplay::instance()->getNoGLExt());
	IDC_NOEXT_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLExtToolTip(), wxConvUTF8));
	IDC_NOLANDSCAPESCORCH_CTRL->SetValue(OptionsDisplay::instance()->getNoGLTexSubImage());
	IDC_NOLANDSCAPESCORCH_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLTexSubImageToolTip(), wxConvUTF8));
	IDC_NOMULTITEX_CTRL->SetValue(OptionsDisplay::instance()->getNoGLMultiTex());
	IDC_NOMULTITEX_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLMultiTexToolTip(), wxConvUTF8));
	IDC_NOCOMPILEDARRAYS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCompiledArrays());
	IDC_NOCOMPILEDARRAYS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLCompiledArraysToolTip(), wxConvUTF8));
	IDC_NOENVCOMBINE_CTRL->SetValue(OptionsDisplay::instance()->getNoGLEnvCombine());
	IDC_NOENVCOMBINE_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLEnvCombineToolTip(), wxConvUTF8));
	IDC_NOCUBEMAP_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCubeMap());
	IDC_NOCUBEMAP_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLCubeMapToolTip(), wxConvUTF8));
	IDC_NOSPHEREMAP_CTRL->SetValue(OptionsDisplay::instance()->getNoGLSphereMap());
	IDC_NOSPHEREMAP_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLSphereMapToolTip(), wxConvUTF8));
	IDC_NOVBO_CTRL->SetValue(OptionsDisplay::instance()->getNoVBO());
	IDC_NOVBO_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoVBOToolTip(), wxConvUTF8));
	IDC_NOMIPMAPS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLHardwareMipmaps());
	IDC_NOMIPMAPS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoGLHardwareMipmapsToolTip(), wxConvUTF8));
	IDC_NOSOUND_CTRL->SetValue(OptionsDisplay::instance()->getNoSound());
	IDC_NOSOUND_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoSoundToolTip(), wxConvUTF8));
	IDC_NOAMBIENTSOUND_CTRL->SetValue(OptionsDisplay::instance()->getNoAmbientSound());
	IDC_NOAMBIENTSOUND_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoAmbientSoundToolTip(), wxConvUTF8));
	IDC_NOBOIDSOUND_CTRL->SetValue(OptionsDisplay::instance()->getNoBoidSound());
	IDC_NOBOIDSOUND_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoBoidSoundToolTip(), wxConvUTF8));
	IDC_NOSKINS_CTRL->SetValue(OptionsDisplay::instance()->getNoSkins());
	IDC_NOSKINS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoSkinsToolTip(), wxConvUTF8));
	IDC_FULLSCREEN_CTRL->SetValue(OptionsDisplay::instance()->getFullScreen());
	IDC_FULLSCREEN_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getFullScreenToolTip(), wxConvUTF8));
	IDC_SINGLESKYLAYER_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyLayers());
	IDC_SINGLESKYLAYER_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoSkyLayersToolTip(), wxConvUTF8));
	IDC_NOSKYANI_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyMovement());
	IDC_NOSKYANI_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoSkyMovementToolTip(), wxConvUTF8));
	IDC_NOWATERANI_CTRL->SetValue(OptionsDisplay::instance()->getNoWaterMovement());
	IDC_NOWATERANI_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoWaterMovementToolTip(), wxConvUTF8));
	IDC_NOWATER_CTRL->SetValue(!OptionsDisplay::instance()->getDrawWater());
	IDC_NOWATER_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDrawWaterToolTip(), wxConvUTF8));
	IDC_NOSURROUND_CTRL->SetValue(!OptionsDisplay::instance()->getDrawSurround());
	IDC_NOSURROUND_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDrawSurroundToolTip(), wxConvUTF8));
	IDC_NOTREES_CTRL->SetValue(OptionsDisplay::instance()->getNoTrees());
	IDC_NOTREES_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoTreesToolTip(), wxConvUTF8));
	IDC_NOPRECIPITATION_CTRL->SetValue(OptionsDisplay::instance()->getNoPrecipitation());
	IDC_NOPRECIPITATION_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoPrecipitationToolTip(), wxConvUTF8));
	IDC_NOWAVES_CTRL->SetValue(OptionsDisplay::instance()->getNoWaves());
	IDC_NOWAVES_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoWavesToolTip(), wxConvUTF8));
	IDC_NODEPTHSORT_CTRL->SetValue(OptionsDisplay::instance()->getNoDepthSorting());
	IDC_NODEPTHSORT_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoDepthSortingToolTip(), wxConvUTF8));
	IDC_INVERT_CTRL->SetValue(OptionsDisplay::instance()->getInvertElevation());
	IDC_INVERT_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getInvertElevationToolTip(), wxConvUTF8));
	IDC_INVERTMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getInvertMouse());
	IDC_INVERTMOUSE_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getInvertMouseToolTip(), wxConvUTF8));
	IDC_SMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getSoftwareMouse());
	IDC_SMOUSE_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getSoftwareMouseToolTip(), wxConvUTF8));
	IDC_TOOLTIP_CTRL->SetValue(OptionsDisplay::instance()->getShowContextHelp());
	IDC_TOOLTIP_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getShowContextHelpToolTip(), wxConvUTF8));
	IDC_TIMER_CTRL->SetValue(OptionsDisplay::instance()->getFrameTimer());
	IDC_TIMER_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getFrameTimerToolTip(), wxConvUTF8));
	IDC_SIDESCROLL_CTRL->SetValue(OptionsDisplay::instance()->getSideScroll());
	IDC_SIDESCROLL_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getSideScrollToolTip(), wxConvUTF8));
	IDC_VALIDATESERVER_CTRL->SetValue(OptionsDisplay::instance()->getValidateServerIp());
	IDC_VALIDATESERVER_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getValidateServerIpToolTip(), wxConvUTF8));	
	IDC_SLIDER1_CTRL->SetRange(3, 40);
	IDC_SLIDER1_CTRL->SetValue(OptionsDisplay::instance()->getBrightness());
	IDC_SLIDER1_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getBrightnessToolTip(), wxConvUTF8));
	IDC_VOLUME_CTRL->SetRange(0, 128);
	IDC_VOLUME_CTRL->SetValue(OptionsDisplay::instance()->getSoundVolume());
	IDC_VOLUME_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getSoundVolumeToolTip(), wxConvUTF8));
	IDC_USERNAME_CTRL->SetValue(wxString(OptionsDisplay::instance()->getOnlineUserName(), wxConvUTF8));
	IDC_USERNAME_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getOnlineUserNameToolTip(), wxConvUTF8));
	IDC_TANKMODEL_CTRL->SetValue(wxString(OptionsDisplay::instance()->getOnlineTankModel(), wxConvUTF8));
	IDC_TANKMODEL_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getOnlineTankModelToolTip(), wxConvUTF8));
	IDC_HOSTDESC_CTRL->SetValue(wxString(OptionsDisplay::instance()->getHostDescription(), wxConvUTF8));
	IDC_HOSTDESC_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getHostDescriptionToolTip(), wxConvUTF8));
	IDC_NODETAILTEX_CTRL->SetValue(!OptionsDisplay::instance()->getDetailTexture());
	IDC_NODETAILTEX_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDetailTextureToolTip(), wxConvUTF8));
	IDC_NOBOIDS_CTRL->SetValue(OptionsDisplay::instance()->getNoBOIDS());
	IDC_NOBOIDS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getNoBOIDSToolTip(), wxConvUTF8));
	IDC_MORERES_CTRL->SetValue(OptionsDisplay::instance()->getMoreRes());
	IDC_MORERES_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getMoreResToolTip(), wxConvUTF8));
	IDC_SWAPYAXIS_CTRL->SetValue(OptionsDisplay::instance()->getSwapYAxis());
	IDC_SWAPYAXIS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getSwapYAxisToolTip(), wxConvUTF8));
	IDC_LOGGING_CTRL->SetValue(OptionsDisplay::instance()->getClientLogToFile());
	IDC_LOGGING_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getClientLogToFileToolTip(), wxConvUTF8));

	for (int i=2; i<=64; i+=2)
	{
		IDC_SOUNDCHANNELS_CTRL->Append(
			wxString(formatString("%i", i), wxConvUTF8));
	}
	IDC_SOUNDCHANNELS_CTRL->SetValue(
		wxString(formatString("%i", OptionsDisplay::instance()->getSoundChannels()),
			wxConvUTF8));

	refreshResolutions();

	IDC_TINYDIALOGS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDialogSizeToolTip(), wxConvUTF8));
	IDC_SMALLDIALOGS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDialogSizeToolTip(), wxConvUTF8));
	IDC_MEDIUMDIALOGS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDialogSizeToolTip(), wxConvUTF8));
	IDC_LARGEDIALOGS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDialogSizeToolTip(), wxConvUTF8));
	IDC_HUGEDIALOGS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getDialogSizeToolTip(), wxConvUTF8));
	switch (OptionsDisplay::instance()->getDialogSize())
	{
	case 0:
		IDC_TINYDIALOGS_CTRL->SetValue(true);
		break;
	case 1:
		IDC_SMALLDIALOGS_CTRL->SetValue(true);
		break;
	case 2:
		IDC_MEDIUMDIALOGS_CTRL->SetValue(true);
		break;
	case 3:
		IDC_LARGEDIALOGS_CTRL->SetValue(true);
		break;
	case 4:
		IDC_HUGEDIALOGS_CTRL->SetValue(true);
		break;
	}

	IDC_SMALLTEX_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTexSizeToolTip(), wxConvUTF8));
	IDC_MEDIUMTEX_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTexSizeToolTip(), wxConvUTF8));
	IDC_LARGETEX_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTexSizeToolTip(), wxConvUTF8));
	switch (OptionsDisplay::instance()->getTexSize())
	{
	case 0:
		IDC_SMALLTEX_CTRL->SetValue(true);
		break;
	case 1:
		IDC_MEDIUMTEX_CTRL->SetValue(true);
		break;
	default:
		IDC_LARGETEX_CTRL->SetValue(true);
		break;
	};

	IDC_LOWEFFECTS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getEffectsDetailToolTip(), wxConvUTF8));
	IDC_MEDIUMEFFECTS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getEffectsDetailToolTip(), wxConvUTF8));
	IDC_HIGHEFFECTS_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getEffectsDetailToolTip(), wxConvUTF8));
	switch (OptionsDisplay::instance()->getEffectsDetail())
	{
	case 0:
		IDC_LOWEFFECTS_CTRL->SetValue(true);
		break;
	case 1:
		IDC_MEDIUMEFFECTS_CTRL->SetValue(true);
		break;
	default:
		IDC_HIGHEFFECTS_CTRL->SetValue(true);
		break;
	};

	IDC_LOWTANK_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTankDetailToolTip(), wxConvUTF8));
	IDC_MEDIUMTANK_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTankDetailToolTip(), wxConvUTF8));
	IDC_HIGHTANK_CTRL->SetToolTip(wxString(OptionsDisplay::instance()->getTankDetailToolTip(), wxConvUTF8));
	switch (OptionsDisplay::instance()->getTankDetail())
	{
	case 0:
		IDC_LOWTANK_CTRL->SetValue(true);
		break;
	case 1:
		IDC_MEDIUMTANK_CTRL->SetValue(true);
		break;
	default:
		IDC_HIGHTANK_CTRL->SetValue(true);
		break;
	};

	refreshKeysControls();

	IDOK_CTRL->SetDefault();
}

void DisplayFrame::refreshResolutions()
{
	IDC_DISPLAY_CTRL->Clear();

	std::set<std::string> displaySet;
	char string[256];
	SDL_Rect **modes = SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
	if((modes != (SDL_Rect **)0) && (modes != (SDL_Rect **)-1))
	{
		for(int i=0;modes[i];++i)
		{
			sprintf(string, "%i x %i", 
				modes[i]->w, modes[i]->h);

			std::string newDisplay(string);
			if (displaySet.find(newDisplay) == displaySet.end())
			{
				IDC_DISPLAY_CTRL->Append(wxString(string, wxConvUTF8));
				displaySet.insert(newDisplay);
			}
		}
	}

	if (IDC_MORERES_CTRL->GetValue())
	{
		const char *extraModes[] = 
			{ "320 x 200", "320 x 240", "512 x 384", 
			"640 x 480", "800 x 600", "1024 x 768" };
		for (int i=0; i<sizeof(extraModes)/sizeof(const char *); i++)
		{
			std::string newDisplay(extraModes[i]);
			if (displaySet.find(newDisplay) == displaySet.end())
			{
				IDC_DISPLAY_CTRL->Append(wxString(newDisplay.c_str(), wxConvUTF8));
				displaySet.insert(newDisplay);
			}
		}
	}

	sprintf(string, "%i x %i", 
		OptionsDisplay::instance()->getScreenWidth(),
		OptionsDisplay::instance()->getScreenHeight());
	IDC_DISPLAY_CTRL->SetValue(wxString(string, wxConvUTF8));
}

bool DisplayFrame::TransferDataFromWindow()
{
	OptionsDisplay::instance()->setFullClear(IDC_FULLCLEAR_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLTexSubImage(IDC_NOLANDSCAPESCORCH_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLExt(IDC_NOEXT_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLMultiTex(IDC_NOMULTITEX_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLCompiledArrays(IDC_NOCOMPILEDARRAYS_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLEnvCombine(IDC_NOENVCOMBINE_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLCubeMap(IDC_NOCUBEMAP_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLSphereMap(IDC_NOSPHEREMAP_CTRL->GetValue());
	OptionsDisplay::instance()->setNoVBO(IDC_NOVBO_CTRL->GetValue());
	OptionsDisplay::instance()->setNoGLHardwareMipmaps(IDC_NOMIPMAPS_CTRL->GetValue());
	OptionsDisplay::instance()->setNoSound(IDC_NOSOUND_CTRL->GetValue());
	OptionsDisplay::instance()->setNoAmbientSound(IDC_NOAMBIENTSOUND_CTRL->GetValue());
	OptionsDisplay::instance()->setNoBoidSound(IDC_NOBOIDSOUND_CTRL->GetValue());
	OptionsDisplay::instance()->setSoundChannels(atoi(IDC_SOUNDCHANNELS_CTRL->GetValue().mb_str(wxConvUTF8)));
	OptionsDisplay::instance()->setNoSkins(IDC_NOSKINS_CTRL->GetValue());
	OptionsDisplay::instance()->setNoTrees(IDC_NOTREES_CTRL->GetValue());
	OptionsDisplay::instance()->setNoPrecipitation(IDC_NOPRECIPITATION_CTRL->GetValue());
	OptionsDisplay::instance()->setNoWaves(IDC_NOWAVES_CTRL->GetValue());
	OptionsDisplay::instance()->setNoDepthSorting(IDC_NODEPTHSORT_CTRL->GetValue());
	OptionsDisplay::instance()->setFullScreen(IDC_FULLSCREEN_CTRL->GetValue());
	OptionsDisplay::instance()->setNoSkyLayers(IDC_SINGLESKYLAYER_CTRL->GetValue());
	OptionsDisplay::instance()->setNoSkyMovement(IDC_NOSKYANI_CTRL->GetValue());
	OptionsDisplay::instance()->setNoWaterMovement(IDC_NOWATERANI_CTRL->GetValue());
	OptionsDisplay::instance()->setBrightness(IDC_SLIDER1_CTRL->GetValue());
	OptionsDisplay::instance()->setSoundVolume(IDC_VOLUME_CTRL->GetValue());
	OptionsDisplay::instance()->setDrawWater(!IDC_NOWATER_CTRL->GetValue());
	OptionsDisplay::instance()->setDrawSurround(!IDC_NOSURROUND_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertElevation(IDC_INVERT_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertMouse(IDC_INVERTMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setSoftwareMouse(IDC_SMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setShowContextHelp(IDC_TOOLTIP_CTRL->GetValue());
	OptionsDisplay::instance()->setFrameTimer(IDC_TIMER_CTRL->GetValue());
	OptionsDisplay::instance()->setSideScroll(IDC_SIDESCROLL_CTRL->GetValue());
	OptionsDisplay::instance()->setValidateServerIp(IDC_VALIDATESERVER_CTRL->GetValue());
	OptionsDisplay::instance()->setOnlineUserName(IDC_USERNAME_CTRL->GetValue().mb_str(wxConvUTF8));
	OptionsDisplay::instance()->setOnlineTankModel(IDC_TANKMODEL_CTRL->GetValue().mb_str(wxConvUTF8));
	OptionsDisplay::instance()->setHostDescription(IDC_HOSTDESC_CTRL->GetValue().mb_str(wxConvUTF8));
	OptionsDisplay::instance()->setDetailTexture(!IDC_NODETAILTEX_CTRL->GetValue());
	OptionsDisplay::instance()->setNoBOIDS(IDC_NOBOIDS_CTRL->GetValue());
	OptionsDisplay::instance()->setMoreRes(IDC_MORERES_CTRL->GetValue());
	OptionsDisplay::instance()->setSwapYAxis(IDC_SWAPYAXIS_CTRL->GetValue());
	OptionsDisplay::instance()->setClientLogToFile(IDC_LOGGING_CTRL->GetValue());

	wxString buffer = IDC_DISPLAY_CTRL->GetValue();
	int windowWidth, windowHeight;
	if (sscanf(buffer.mb_str(wxConvUTF8), 
		"%i x %i", 
		&windowWidth,
		&windowHeight) == 2)
	{
		OptionsDisplay::instance()->setScreenWidth(windowWidth);
		OptionsDisplay::instance()->setScreenHeight(windowHeight);
	}

	int dialogSize = 0;
	if (IDC_SMALLDIALOGS_CTRL->GetValue()) dialogSize = 1;
	if (IDC_MEDIUMDIALOGS_CTRL->GetValue()) dialogSize = 2;
	if (IDC_LARGEDIALOGS_CTRL->GetValue()) dialogSize = 3;
	if (IDC_HUGEDIALOGS_CTRL->GetValue()) dialogSize = 4;
	OptionsDisplay::instance()->setDialogSize(dialogSize);

	int texSize = 1;
	if (IDC_SMALLTEX_CTRL->GetValue()) texSize = 0;
	if (IDC_LARGETEX_CTRL->GetValue()) texSize = 2;
	OptionsDisplay::instance()->setTexSize(texSize);

	int effectsDetail = 1;
	if (IDC_LOWEFFECTS_CTRL->GetValue()) effectsDetail = 0;
	if (IDC_HIGHEFFECTS_CTRL->GetValue()) effectsDetail = 2;
	OptionsDisplay::instance()->setEffectsDetail(effectsDetail);

	int tankDetail = 1;
	if (IDC_LOWTANK_CTRL->GetValue()) tankDetail = 0;
	if (IDC_HIGHTANK_CTRL->GetValue()) tankDetail = 2;
	OptionsDisplay::instance()->setTankDetail(tankDetail);

	// Save display options to file
	OptionsDisplay::instance()->writeOptionsToFile();

	// Save keyboard keys to file
	Keyboard::instance()->saveKeyFile();

	return true;
}

void DisplayFrame::onExportMod(wxCommandEvent &event)
{
	int selectionNo = modbox->GetSelection();
	if (selectionNo < 0) return;
	wxString selection = modbox->GetString(selectionNo);
	if (selection.empty()) return;

	wxString file = ::wxFileSelector(wxT("Please choose the export file to save"),
		wxString(getSettingsFile(""), wxConvUTF8), // default path
		wxString(formatString("%s.s3m", (const char *) selection.mb_str(wxConvUTF8)), wxConvUTF8), // default filename
		wxT(""), // default extension
		wxT("*.s3m"),
		wxSAVE);
	if (file.empty()) return;
	ModFiles files;
	if (!files.loadModFiles(selection.mb_str(wxConvUTF8), false))
	{
		dialogMessage("Export Mod", "Failed to load mod files");
		return;
	}
	if (!files.exportModFiles(
		selection.mb_str(wxConvUTF8), 
		file.mb_str(wxConvUTF8)))
	{
		dialogMessage("Export Mod", "Failed to write mod export file");
		return;
	}
}

void DisplayFrame::onImportMod(wxCommandEvent &event)
{
	wxString file = ::wxFileSelector(wxT("Please choose the import file to open"),
		wxString(getSettingsFile(""),wxConvUTF8), // default path
		wxT(""), // default filename
		wxT(""), // default extension
		wxT("*.s3m"),
		wxOPEN | wxFILE_MUST_EXIST);
	if (file.empty()) return;
	ModFiles files;
	const char *mod = 0;
	if (!files.importModFiles(&mod, file.mb_str(wxConvUTF8)))
	{
		dialogMessage("Import Mod", "Failed to read mod export file");
		return;
	}
	if (!files.writeModFiles(mod))
	{
		dialogMessage("Import Mod", "Failed to write mod files");
		return;
	}
	updateModList();
}

void showDisplayDialog()
{
	DisplayFrame frame;
	frame.ShowModal();
}
