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

	void onLoadDefaultsButton();
	void onLoadSafeButton();
	void onMoreRes();

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

	// Create all the display controlls
	createControls(this, topsizer);

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void DisplayFrame::onLoadDefaultsButton()
{
	OptionsDisplay::instance()->loadDefaultValues();
	refreshScreen();
}

void DisplayFrame::onLoadSafeButton()
{
	OptionsDisplay::instance()->loadSafeValues();
	refreshScreen();
}

void DisplayFrame::onMoreRes()
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
	IDC_NOEXT_CTRL->SetValue(OptionsDisplay::instance()->getNoGLExt());
	IDC_NOLANDSCAPESCORCH_CTRL->SetValue(OptionsDisplay::instance()->getNoGLTexSubImage());
	IDC_NOMULTITEX_CTRL->SetValue(OptionsDisplay::instance()->getNoGLMultiTex());
	IDC_NOCOMPILEDARRAYS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCompiledArrays());
	IDC_NOENVCOMBINE_CTRL->SetValue(OptionsDisplay::instance()->getNoGLEnvCombine());
	IDC_NOCUBEMAP_CTRL->SetValue(OptionsDisplay::instance()->getNoGLCubeMap());
	IDC_NOMIPMAPS_CTRL->SetValue(OptionsDisplay::instance()->getNoGLHardwareMipmaps());
	IDC_NOSOUND_CTRL->SetValue(OptionsDisplay::instance()->getNoSound());
	IDC_NOSKINS_CTRL->SetValue(OptionsDisplay::instance()->getNoSkins());
	IDC_FULLSCREEN_CTRL->SetValue(OptionsDisplay::instance()->getFullScreen());
	IDC_SINGLESKYLAYER_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyLayers());
	IDC_NOSKYANI_CTRL->SetValue(OptionsDisplay::instance()->getNoSkyMovement());
	IDC_NOWATERANI_CTRL->SetValue(OptionsDisplay::instance()->getNoWaterMovement());
	IDC_NOWATER_CTRL->SetValue(!OptionsDisplay::instance()->getDrawWater());
	IDC_NOTREES_CTRL->SetValue(OptionsDisplay::instance()->getNoTrees());
	IDC_NOWAVES_CTRL->SetValue(OptionsDisplay::instance()->getNoWaves());
	IDC_NODEPTHSORT_CTRL->SetValue(OptionsDisplay::instance()->getNoDepthSorting());
	IDC_INVERT_CTRL->SetValue(OptionsDisplay::instance()->getInvertElevation());
	IDC_INVERTMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getInvertMouse());
	IDC_SMOUSE_CTRL->SetValue(OptionsDisplay::instance()->getSoftwareMouse());
	IDC_TOOLTIP_CTRL->SetValue(OptionsDisplay::instance()->getShowContextHelp());
	IDC_TIMER_CTRL->SetValue(OptionsDisplay::instance()->getFrameTimer());
	IDC_SIDESCROLL_CTRL->SetValue(OptionsDisplay::instance()->getSideScroll());
	IDC_SLIDER1_CTRL->SetRange(3, 40);
	IDC_SLIDER1_CTRL->SetValue(OptionsDisplay::instance()->getBrightness());
	IDC_USERID_CTRL->SetValue(OptionsDisplay::instance()->getUniqueUserId());
	IDC_HOSTDESC_CTRL->SetValue(OptionsDisplay::instance()->getHostDescription());
	IDC_NODETAILTEX_CTRL->SetValue(!OptionsDisplay::instance()->getDetailTexture());
	IDC_MORERES_CTRL->SetValue(OptionsDisplay::instance()->getMoreRes());

	refreshResolutions();

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
			{ "640 x 480", "800 x 600", "1024 x 768" };
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
	OptionsDisplay::instance()->setDrawWater(!IDC_NOWATER_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertElevation(IDC_INVERT_CTRL->GetValue());
	OptionsDisplay::instance()->setInvertMouse(IDC_INVERTMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setSoftwareMouse(IDC_SMOUSE_CTRL->GetValue());
	OptionsDisplay::instance()->setShowContextHelp(IDC_TOOLTIP_CTRL->GetValue());
	OptionsDisplay::instance()->setFrameTimer(IDC_TIMER_CTRL->GetValue());
	OptionsDisplay::instance()->setSideScroll(IDC_SIDESCROLL_CTRL->GetValue());
	OptionsDisplay::instance()->setUniqueUserId(IDC_USERID_CTRL->GetValue());
	OptionsDisplay::instance()->setHostDescription(IDC_HOSTDESC_CTRL->GetValue());
	OptionsDisplay::instance()->setDetailTexture(!IDC_NODETAILTEX_CTRL->GetValue());
	OptionsDisplay::instance()->setMoreRes(IDC_MORERES_CTRL->GetValue());

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
