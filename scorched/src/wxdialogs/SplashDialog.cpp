////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/image.h>
#include <wx/filedlg.h>
#include <wxdialogs/MainDialog.h>
#include <engine/ModInfo.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>

extern char scorched3dAppName[128];

enum
{
	ID_BUTTON_SCORCHED3D = 999,
	ID_BUTTON_CHOICE
};

class SplashFrame: public wxDialog
{
public:
	SplashFrame();

protected:

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SplashFrame, wxDialog)
END_EVENT_TABLE()

SplashFrame::SplashFrame() :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8), 
	wxDefaultPosition, wxDefaultSize)
{
	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif
	addTitleToWindow(this, topsizer, 
		getDataFile("data/windows/scorched.bmp"),
		ID_BUTTON_SCORCHED3D);
	setDataFileMod("none");

	wxStaticText *text = new wxStaticText(this, -1, wxT("Welcome to Scorched3D.\n"
			"News, updates, player stats and new mods are available at http://www.scorched3d.co.uk\n"
			"\n"
			"If the game runs slowly please ensure you are using the latest graphics drivers.\n"
			"Game speed can also be optimized by changing the Scorched3D game settings."));
	topsizer->Add(text);

	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, 
		wxT("Close")), 0, wxALL, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

bool showSplashDialog()
{
	SplashFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
