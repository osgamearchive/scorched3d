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

#include <scorched/MainDialog.h>
#include <scorched/SettingsDialog.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <engine/ModFiles.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include "SingleS.cpp"

extern char scorched3dAppName[128];

class SingleSFrame: public wxDialog
{
public:
	SingleSFrame(OptionsGame &options);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onSettingsButton();

private:
	DECLARE_EVENT_TABLE()

	OptionsGame &options_;
};

BEGIN_EVENT_TABLE(SingleSFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_SETTINGS,  SingleSFrame::onSettingsButton)
END_EVENT_TABLE()

SingleSFrame::SingleSFrame(OptionsGame &options) :
	wxDialog(getMainDialog(), -1, scorched3dAppName,
			 wxDefaultPosition, wxDefaultSize),
	 options_(options)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controlls
	createControls(this, topsizer);
	IDOK_CTRL->SetDefault();

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SingleSFrame::onSettingsButton()
{
	TransferDataFromWindow();
	// Don't save until the whole options have been choosen
	showSettingsDialog(false, options_);
}

bool SingleSFrame::TransferDataToWindow()
{
	int i;
	char string[20];
	for (i=24; i>1; i--)
	{
		char string[20];
		sprintf(string, "%i", i);
		IDC_CLIENT_PLAYERS_CTRL->Append(string);
	}
	sprintf(string, "%i", options_.getNoMaxPlayers());
	IDC_CLIENT_PLAYERS_CTRL->SetValue(string);
	IDC_CLIENT_PLAYERS_CTRL->SetToolTip(
		wxString("The number of players that will play in this game.\n"
			"This number should include computer players"));

	ModDirs modDirs;
	if (!modDirs.loadModDirs()) dialogExit("ModFiles", "Failed to load mod files");
	IDC_CLIENT_MOD_CTRL->Append("none");
	std::list<std::string>::iterator itor;
	for (itor = modDirs.getDirs().begin();
		itor != modDirs.getDirs().end();
		itor++)
	{
		IDC_CLIENT_MOD_CTRL->Append((*itor).c_str());
	}

	if (IDC_CLIENT_MOD_CTRL->FindString(options_.getMod()) != -1)
		IDC_CLIENT_MOD_CTRL->SetValue(options_.getMod());
	else 
		IDC_CLIENT_MOD_CTRL->SetValue("none");
	IDC_CLIENT_MOD_CTRL->SetToolTip(
		wxString("The Scorched3D mod to use for this game."));

	return true;
}

bool SingleSFrame::TransferDataFromWindow()
{
	int noPlayers = 2;
	sscanf(IDC_CLIENT_PLAYERS_CTRL->GetValue(), "%i", &noPlayers);
	options_.setNoMaxPlayers(noPlayers);
	options_.setNoMinPlayers(noPlayers);
	wxString value = IDC_CLIENT_MOD_CTRL->GetValue();
	if (strcmp(value.c_str(), "none"))
		options_.setMod(IDC_CLIENT_MOD_CTRL->GetValue());
	else
		options_.setMod("");

	return true;
}

bool showSingleSDialog()
{
	OptionsGame tmpOptions;
	wxString customFilePathSrc = getDataFile("data/singlecustom.xml");
	wxString customFilePathDest = getSettingsFile("singlecustom.xml");
	if (::wxFileExists(customFilePathDest))
	{
		tmpOptions.readOptionsFromFile((char *) customFilePathDest.c_str());
	}
	else
	{
		tmpOptions.readOptionsFromFile((char *) customFilePathSrc.c_str());
	}

	SingleSFrame frame(tmpOptions);
	if (frame.ShowModal() == wxID_OK)
	{
		tmpOptions.writeOptionsToFile((char *) customFilePathDest.c_str());
		runScorched3D("-startclient \"%s\"", customFilePathDest.c_str());
		return true;
	}
	return false;
}

