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
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <set>
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
	void onMoreRes(wxCommandEvent &event);

	wxNotebook *book_;
	wxPanel *mainPanel_;
	wxPanel *troublePanel_;
	wxPanel *identPanel_;

protected:
	DECLARE_EVENT_TABLE()

	void refreshScreen();
	void refreshResolutions();
};

BEGIN_EVENT_TABLE(DisplayFrame, wxDialog)
    EVT_BUTTON(ID_LOADDEFAULTS,  DisplayFrame::onLoadDefaultsButton)
    EVT_BUTTON(ID_LOADSAFE,  DisplayFrame::onLoadSafeButton)
	EVT_CHECKBOX(ID_MORERES, DisplayFrame::onMoreRes) 
END_EVENT_TABLE()

DisplayFrame::DisplayFrame() :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName))
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
	book_->AddPage(mainPanel_, "Main");
	wxSizer *mainPanelSizer = new wxBoxSizer(wxVERTICAL);
	createMainControls(mainPanel_, mainPanelSizer);
	mainPanel_->SetAutoLayout(TRUE);
	mainPanel_->SetSizer(mainPanelSizer);
	
	// Trouble Panel
	troublePanel_ = new wxPanel(book_, -1);
	wxSizer *troublePanelSizer = new wxBoxSizer(wxVERTICAL);
	createTroubleControls(troublePanel_, troublePanelSizer);
	book_->AddPage(troublePanel_, "Troubleshooting / Detail");
	troublePanel_->SetAutoLayout(TRUE);
	troublePanel_->SetSizer(troublePanelSizer);

	// Ident Panel
	identPanel_ = new wxPanel(book_, -1);
	wxSizer *identPanelSizer = new wxBoxSizer(wxVERTICAL);
	createIdentControls(identPanel_, identPanelSizer);
	book_->AddPage(identPanel_, "Identity");
	identPanel_->SetAutoLayout(TRUE);
	identPanel_->SetSizer(identPanelSizer);
	topsizer->Add(nbs, 0, wxALL, 10);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(this, wxID_OK, "Ok");
	IDCANCEL_CTRL = new wxButton(this, wxID_CANCEL, "Cancel");
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

void DisplayFrame::onMoreRes(wxCommandEvent &event)
{
	refreshResolutions();
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
	IDC_FULLCLEAR_CTRL->SetToolTip(OptionsDisplay::instance()->getFullClearToolTip());
	IDC_NOEXT_CTRL->SetValue(OptionsDisplay::instance()->getNoGLExt());
	IDC_NOEXT_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLExtToolTip());
	IDC_NOLANDSCAPESCORCH_CTRL->SetValue(OptionsDisplay::instance()->getNoGLTexSubImage());
	IDC_NOLANDSCAPESCORCH_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLTexSubImageToolTip());
	IDC_NOMULTITEX_CTRL->SetValue(OptionsDisplay::instance()->getNoGLMultiTex());
	IDC_NOMULTITEX_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLMultiTexToolTip());
	IDC_NOCOMPILEDARRAYS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCompiledArrays());
	IDC_NOCOMPILEDARRAYS_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLCompiledArraysToolTip());
	IDC_NOENVCOMBINE_CTRL->SetValue(OptionsDisplay::instance()->getNoGLEnvCombine());
	IDC_NOENVCOMBINE_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLEnvCombineToolTip());
	IDC_NOCUBEMAP_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCubeMap());
	IDC_NOCUBEMAP_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLCubeMapToolTip());
	IDC_NOSPHEREMAP_CTRL->SetValue(OptionsDisplay::instance()->getNoGLSphereMap());
	IDC_NOSPHEREMAP_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLSphereMapToolTip());
	IDC_NOVBO_CTRL->SetValue(OptionsDisplay::instance()->getNoVBO());
	IDC_NOVBO_CTRL->SetToolTip(OptionsDisplay::instance()->getNoVBOToolTip());
	IDC_NOMIPMAPS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLHardwareMipmaps());
	IDC_NOMIPMAPS_CTRL->SetToolTip(OptionsDisplay::instance()->getNoGLHardwareMipmapsToolTip());
	IDC_NOSOUND_CTRL->SetValue(OptionsDisplay::instance()->getNoSound());
	IDC_NOSOUND_CTRL->SetToolTip(OptionsDisplay::instance()->getNoSoundToolTip());
	IDC_NOSKINS_CTRL->SetValue(OptionsDisplay::instance()->getNoSkins());
	IDC_NOSKINS_CTRL->SetToolTip(OptionsDisplay::instance()->getNoSkinsToolTip());
	IDC_FULLSCREEN_CTRL->SetValue(OptionsDisplay::instance()->getFullScreen());
	IDC_FULLSCREEN_CTRL->SetToolTip(OptionsDisplay::instance()->getFullScreenToolTip());
	IDC_SINGLESKYLAYER_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyLayers());
	IDC_SINGLESKYLAYER_CTRL->SetToolTip(OptionsDisplay::instance()->getNoSkyLayersToolTip());
	IDC_NOSKYANI_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyMovement());
	IDC_NOSKYANI_CTRL->SetToolTip(OptionsDisplay::instance()->getNoSkyMovementToolTip());
	IDC_NOWATERANI_CTRL->SetValue(OptionsDisplay::instance()->getNoWaterMovement());
	IDC_NOWATERANI_CTRL->SetToolTip(OptionsDisplay::instance()->getNoWaterMovementToolTip());
	IDC_NOWATER_CTRL->SetValue(!OptionsDisplay::instance()->getDrawWater());
	IDC_NOWATER_CTRL->SetToolTip(OptionsDisplay::instance()->getDrawWaterToolTip());
	IDC_NOTREES_CTRL->SetValue(OptionsDisplay::instance()->getNoTrees());
	IDC_NOTREES_CTRL->SetToolTip(OptionsDisplay::instance()->getNoTreesToolTip());
	IDC_NOWAVES_CTRL->SetValue(OptionsDisplay::instance()->getNoWaves());
	IDC_NOWAVES_CTRL->SetToolTip(OptionsDisplay::instance()->getNoWavesToolTip());
	IDC_NODEPTHSORT_CTRL->SetValue(OptionsDisplay::instance()->getNoDepthSorting());
	IDC_NODEPTHSORT_CTRL->SetToolTip(OptionsDisplay::instance()->getNoDepthSortingToolTip());
	IDC_INVERT_CTRL->SetValue(OptionsDisplay::instance()->getInvertElevation());
	IDC_INVERT_CTRL->SetToolTip(OptionsDisplay::instance()->getInvertElevationToolTip());
	IDC_INVERTMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getInvertMouse());
	IDC_INVERTMOUSE_CTRL->SetToolTip(OptionsDisplay::instance()->getInvertMouseToolTip());
	IDC_SMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getSoftwareMouse());
	IDC_SMOUSE_CTRL->SetToolTip(OptionsDisplay::instance()->getSoftwareMouseToolTip());
	IDC_TOOLTIP_CTRL->SetValue(OptionsDisplay::instance()->getShowContextHelp());
	IDC_TOOLTIP_CTRL->SetToolTip(OptionsDisplay::instance()->getShowContextHelpToolTip());
	IDC_TIMER_CTRL->SetValue(OptionsDisplay::instance()->getFrameTimer());
	IDC_TIMER_CTRL->SetToolTip(OptionsDisplay::instance()->getFrameTimerToolTip());
	IDC_SIDESCROLL_CTRL->SetValue(OptionsDisplay::instance()->getSideScroll());
	IDC_SIDESCROLL_CTRL->SetToolTip(OptionsDisplay::instance()->getSideScrollToolTip());
	IDC_SLIDER1_CTRL->SetRange(3, 40);
	IDC_SLIDER1_CTRL->SetValue(OptionsDisplay::instance()->getBrightness());
	IDC_SLIDER1_CTRL->SetToolTip(OptionsDisplay::instance()->getBrightnessToolTip());
	IDC_VOLUME_CTRL->SetRange(0, 128);
	IDC_VOLUME_CTRL->SetValue(OptionsDisplay::instance()->getSoundVolume());
	IDC_VOLUME_CTRL->SetToolTip(OptionsDisplay::instance()->getSoundVolumeToolTip());
	IDC_USERNAME_CTRL->SetValue(OptionsDisplay::instance()->getOnlineUserName());
	IDC_USERNAME_CTRL->SetToolTip(OptionsDisplay::instance()->getOnlineUserNameToolTip());
	IDC_USERID_CTRL->SetValue(OptionsDisplay::instance()->getUniqueUserId());
	IDC_USERID_CTRL->SetToolTip(OptionsDisplay::instance()->getUniqueUserIdToolTip());
	IDC_HOSTDESC_CTRL->SetValue(OptionsDisplay::instance()->getHostDescription());
	IDC_HOSTDESC_CTRL->SetToolTip(OptionsDisplay::instance()->getHostDescriptionToolTip());
	IDC_NODETAILTEX_CTRL->SetValue(!OptionsDisplay::instance()->getDetailTexture());
	IDC_NODETAILTEX_CTRL->SetToolTip(OptionsDisplay::instance()->getDetailTextureToolTip());
	IDC_MORERES_CTRL->SetValue(OptionsDisplay::instance()->getMoreRes());
	IDC_MORERES_CTRL->SetToolTip(OptionsDisplay::instance()->getMoreResToolTip());
	IDC_SWAPYAXIS_CTRL->SetValue(OptionsDisplay::instance()->getSwapYAxis());
	IDC_SWAPYAXIS_CTRL->SetToolTip(OptionsDisplay::instance()->getSwapYAxisToolTip());

	refreshResolutions();

	IDC_TINYDIALOGS_CTRL->SetToolTip(OptionsDisplay::instance()->getDialogSizeToolTip());
	IDC_SMALLDIALOGS_CTRL->SetToolTip(OptionsDisplay::instance()->getDialogSizeToolTip());
	IDC_MEDIUMDIALOGS_CTRL->SetToolTip(OptionsDisplay::instance()->getDialogSizeToolTip());
	IDC_LARGEDIALOGS_CTRL->SetToolTip(OptionsDisplay::instance()->getDialogSizeToolTip());
	IDC_HUGEDIALOGS_CTRL->SetToolTip(OptionsDisplay::instance()->getDialogSizeToolTip());
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

	IDC_SMALLTEX_CTRL->SetToolTip(OptionsDisplay::instance()->getTexSizeToolTip());
	IDC_MEDIUMTEX_CTRL->SetToolTip(OptionsDisplay::instance()->getTexSizeToolTip());
	IDC_LARGETEX_CTRL->SetToolTip(OptionsDisplay::instance()->getTexSizeToolTip());
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

	IDC_LOWEFFECTS_CTRL->SetToolTip(OptionsDisplay::instance()->getEffectsDetailToolTip());
	IDC_MEDIUMEFFECTS_CTRL->SetToolTip(OptionsDisplay::instance()->getEffectsDetailToolTip());
	IDC_HIGHEFFECTS_CTRL->SetToolTip(OptionsDisplay::instance()->getEffectsDetailToolTip());
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

	IDC_LOWTANK_CTRL->SetToolTip(OptionsDisplay::instance()->getTankDetailToolTip());
	IDC_MEDIUMTANK_CTRL->SetToolTip(OptionsDisplay::instance()->getTankDetailToolTip());
	IDC_HIGHTANK_CTRL->SetToolTip(OptionsDisplay::instance()->getTankDetailToolTip());
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
				IDC_DISPLAY_CTRL->Append(string);
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
				IDC_DISPLAY_CTRL->Append(newDisplay.c_str());
				displaySet.insert(newDisplay);
			}
		}
	}

	sprintf(string, "%i x %i", 
		OptionsDisplay::instance()->getScreenWidth(),
		OptionsDisplay::instance()->getScreenHeight());
	IDC_DISPLAY_CTRL->SetValue(string);
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
	OptionsDisplay::instance()->setNoSkins(IDC_NOSKINS_CTRL->GetValue());
	OptionsDisplay::instance()->setNoTrees(IDC_NOTREES_CTRL->GetValue());
	OptionsDisplay::instance()->setNoWaves(IDC_NOWAVES_CTRL->GetValue());
	OptionsDisplay::instance()->setNoDepthSorting(IDC_NODEPTHSORT_CTRL->GetValue());
	OptionsDisplay::instance()->setFullScreen(IDC_FULLSCREEN_CTRL->GetValue());
	OptionsDisplay::instance()->setNoSkyLayers(IDC_SINGLESKYLAYER_CTRL->GetValue());
	OptionsDisplay::instance()->setNoSkyMovement(IDC_NOSKYANI_CTRL->GetValue());
	OptionsDisplay::instance()->setNoWaterMovement(IDC_NOWATERANI_CTRL->GetValue());
	OptionsDisplay::instance()->setBrightness(IDC_SLIDER1_CTRL->GetValue());
	OptionsDisplay::instance()->setSoundVolume(IDC_VOLUME_CTRL->GetValue());
	OptionsDisplay::instance()->setDrawWater(!IDC_NOWATER_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertElevation(IDC_INVERT_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertMouse(IDC_INVERTMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setSoftwareMouse(IDC_SMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setShowContextHelp(IDC_TOOLTIP_CTRL->GetValue());
	OptionsDisplay::instance()->setFrameTimer(IDC_TIMER_CTRL->GetValue());
	OptionsDisplay::instance()->setSideScroll(IDC_SIDESCROLL_CTRL->GetValue());
	OptionsDisplay::instance()->setOnlineUserName(IDC_USERNAME_CTRL->GetValue());
	OptionsDisplay::instance()->setUniqueUserId(IDC_USERID_CTRL->GetValue());
	OptionsDisplay::instance()->setHostDescription(IDC_HOSTDESC_CTRL->GetValue());
	OptionsDisplay::instance()->setDetailTexture(!IDC_NODETAILTEX_CTRL->GetValue());
	OptionsDisplay::instance()->setMoreRes(IDC_MORERES_CTRL->GetValue());
	OptionsDisplay::instance()->setSwapYAxis(IDC_SWAPYAXIS_CTRL->GetValue());

	wxString buffer = IDC_DISPLAY_CTRL->GetValue();
	int windowWidth, windowHeight;
	if (sscanf(buffer.c_str(), 
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

	return true;
}

void showDisplayDialog()
{
	DisplayFrame frame;
	frame.ShowModal();
}
