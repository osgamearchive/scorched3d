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

	return true;
}

bool SingleSFrame::TransferDataFromWindow()
{
	int noPlayers = 2;
	sscanf(IDC_CLIENT_PLAYERS_CTRL->GetValue(), "%i", &noPlayers);
	options_.setNoMaxPlayers(noPlayers);
	options_.setNoMinPlayers(noPlayers);

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

