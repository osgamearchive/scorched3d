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

#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/image.h>
#include <wx/filedlg.h>
#include <scorched/MainDialog.h>
#include <scorched/SingleSDialog.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>

extern char scorched3dAppName[128];

enum
{
	ID_BUTTON_EASY = 1,
	ID_BUTTON_NORMAL,
	ID_BUTTON_HARD,
	ID_BUTTON_CUSTOM,
	ID_BUTTON_TARGET,
	ID_BUTTON_LOAD
};

class SingleFrame: public wxDialog
{
public:
	SingleFrame();

	void onEasyButton();
	void onNormalButton();
	void onHardButton();
	void onCustomButton();
	void onTargetButton();
	void onLoadButton();

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SingleFrame, wxDialog)
    EVT_BUTTON(ID_BUTTON_EASY,  SingleFrame::onEasyButton)
	EVT_BUTTON(ID_BUTTON_NORMAL,  SingleFrame::onNormalButton)
	EVT_BUTTON(ID_BUTTON_HARD,  SingleFrame::onHardButton)
	EVT_BUTTON(ID_BUTTON_CUSTOM,  SingleFrame::onCustomButton)
	EVT_BUTTON(ID_BUTTON_TARGET, SingleFrame::onTargetButton)
	EVT_BUTTON(ID_BUTTON_LOAD, SingleFrame::onLoadButton)
END_EVENT_TABLE()

SingleFrame::SingleFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxDefaultPosition, wxDefaultSize)
{
	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	wxFlexGridSizer *gridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	{
		addButtonToWindow(ID_BUTTON_TARGET, 
			"Start an target practice game.\n"
			"Play a practice level against targets that don't fire back.", 
			"data/windows/target.bmp", this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_EASY, 
			"Start an easy single player game.\n"
			"Play a quick game against easy computer players.", 
			"data/windows/tank-easy.bmp", this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_NORMAL, 
			"Start an normal single player game.\n"
			"Play a quick game against normal strength computer players.", 
			"data/windows/tank-med.bmp", this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_HARD,
			"Start an hard single player game.\n"
			"Play a quick game against hard computer players.", 
			"data/windows/tank-hard.bmp", this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_CUSTOM,
			"Start an custom single or multi-player game.\n"
			"Choose the opponents to play against.", 
			"data/windows/tank-cus.bmp", this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_LOAD,
			"Load a previously saved single or multi-player game.", 
			"data/windows/save.bmp", this, gridsizer);
	}
	topsizer->Add(gridsizer, 0, wxALIGN_CENTER | wxALL, 5);

	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Close"), 0, wxALL, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SingleFrame::onLoadButton()
{
	EndModal(wxID_OK);

	wxString file = ::wxFileSelector("Please choose the saved game to load",
									 getSaveFile(""), // default path
									 "", // default filename
									 "", // default extension
									 "*.s3d",
									 wxOPEN | wxFILE_MUST_EXIST);
	if (!file.empty())
	{
		runScorched3D("-loadsave \"%s\"", file.c_str());
	}
}

void SingleFrame::onTargetButton()
{
	const char *targetFilePath = getDataFile("data/singletarget.xml");
	EndModal(wxID_OK);

	runScorched3D("-startclient \"%s\"", targetFilePath);
}

void SingleFrame::onEasyButton()
{
	const char *easyFilePath = getDataFile("data/singleeasy.xml");
	EndModal(wxID_OK);

	runScorched3D("-startclient \"%s\"", easyFilePath);
}

void SingleFrame::onNormalButton()
{
	const char *normalFilePath = getDataFile("data/singlenormal.xml");
	EndModal(wxID_OK);

	runScorched3D("-startclient \"%s\"", normalFilePath);
}

void SingleFrame::onHardButton()
{
	const char *hardFilePath = getDataFile("data/singlehard.xml");
	EndModal(wxID_OK);

	runScorched3D("-startclient \"%s\"", hardFilePath);
}

void SingleFrame::onCustomButton()
{
	if (showSingleSDialog())
	{
		EndModal(wxID_OK);
	}
}

bool showSingleDialog()
{
	SingleFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
