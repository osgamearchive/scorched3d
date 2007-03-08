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
#include <wxdialogs/NetDialog.h>
#include <wxdialogs/MainDialog.h>
#include <wxdialogs/SingleSDialog.h>
#include <wxdialogs/SingleChoiceDialog.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <scorched/ScorchedParams.h>
#include <engine/ModDirs.h>

extern char scorched3dAppName[128];

enum
{
	ID_BUTTON_EASY = 1,
	ID_BUTTON_CUSTOM,
	ID_BUTTON_LOAD,
	ID_BUTTON_NETLAN,
	ID_BUTTON_TUTORIAL,
	ID_BUTTON_SCORCHED3D,
	ID_BUTTON_GAME = 100
};

class SingleFrameData : public wxObjectRefData
{
public:
	SingleFrameData(ModInfo &m) : mod(m) { }
	virtual ~SingleFrameData() { }

	ModInfo mod;
};

class SingleFrame: public wxDialog
{
public:
	SingleFrame();

	void onGameButton(wxCommandEvent &event);
	void onCustomButton(wxCommandEvent &event);
	void onLoadButton(wxCommandEvent &event);
	void onScorchedButton(wxCommandEvent &event);
	void onNetLanButton(wxCommandEvent &event);
	void onTutorialButton(wxCommandEvent &event);

protected:
	void addModButton(
		int &count,
		ModInfo &mod, 
		wxSizer *sizer);

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SingleFrame, wxDialog)
	EVT_BUTTON(ID_BUTTON_CUSTOM,  SingleFrame::onCustomButton)
	EVT_BUTTON(ID_BUTTON_LOAD, SingleFrame::onLoadButton)
	EVT_BUTTON(ID_BUTTON_SCORCHED3D, SingleFrame::onScorchedButton)
	EVT_BUTTON(ID_BUTTON_NETLAN, SingleFrame::onNetLanButton)
	EVT_BUTTON(ID_BUTTON_TUTORIAL, SingleFrame::onTutorialButton)
	EVT_BUTTON(ID_BUTTON_GAME,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 1,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 2,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 3,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 4,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 5,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 6,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 7,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 8,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 9,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 10,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 11,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 12,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 13,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 14,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 15,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 16,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 17,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 18,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 19,  SingleFrame::onGameButton)
	EVT_BUTTON(ID_BUTTON_GAME + 20,  SingleFrame::onGameButton)
END_EVENT_TABLE()

SingleFrame::SingleFrame() :
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

	wxFlexGridSizer *topgridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	{
		addButtonToWindow(ID_BUTTON_TUTORIAL,
			"Start the single player tutorial.\n"
			"Learn how to play Scorched 3D.", 
			getDataFile("data/windows/book.bmp"), this, topgridsizer);
	}
	topsizer->Add(topgridsizer, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 5);

	// Add all of the mods
	wxStaticBox *modbox = 
		new wxStaticBox(this, -1, wxT("Start a single player game"));
	wxStaticBoxSizer *modboxsizer = 
		new wxStaticBoxSizer(modbox, wxVERTICAL);
	wxFlexGridSizer *modgridsizer = new wxFlexGridSizer(4, 8, 5, 5);
	
	int count = ID_BUTTON_GAME;
	ModDirs modDirs;
	if (!modDirs.loadModDirs()) dialogExit("SingleFrame", "Failed to load mod dirs");	
	std::list<ModInfo>::iterator itor;
	for (itor = modDirs.getDirs().begin();
		itor != modDirs.getDirs().end();
		itor++)
	{
		ModInfo &info = (*itor);
		const char *modName = info.getName();
			
		if (!info.getMenuEntries().empty())
		{
			wxObjectRefData *refData = new SingleFrameData(info);
			wxButton *button = 0;
			wxBitmap bitmap;
			if (bitmap.LoadFile(wxString((char *) info.getIcon(), 
				wxConvUTF8), wxBITMAP_TYPE_BMP) &&
				bitmap.Ok())
			{
				button = new wxBitmapButton(this, count++, bitmap);
			}
			else
			{
				button = new wxButton(this, count++, wxT("Select"));
			}
			button->SetRefData(refData);

			wxStaticText *staticText = new wxStaticText(
				this, -1, 
				wxString(modName, wxConvUTF8));

			wxBoxSizer *tmpsizer = new wxBoxSizer(wxVERTICAL);
			tmpsizer->Add(button, 0, wxCENTER);
			tmpsizer->Add(staticText, 0, wxCENTER);

			modgridsizer->Add(tmpsizer);
		}
	}

	modboxsizer->Add(modgridsizer);
	topsizer->Add(modboxsizer, 1, wxGROW | wxALL, 5);

	wxFlexGridSizer *gridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	{
		addButtonToWindow(ID_BUTTON_CUSTOM,
			"Select your own game settings.\n"
			"Choose to play against other local players, \n"
			"or change the game settings and the mod.",
			getDataFile("data/windows/tank-cus.bmp"), this, gridsizer);
	}

	{
		addButtonToWindow(ID_BUTTON_NETLAN,
			"Join a game over the internet or LAN.\n"
			"Connect to a server and play with others over the internet.", 
			getDataFile("data/windows/client.bmp"), this, gridsizer);
	}
	{
		addButtonToWindow(ID_BUTTON_LOAD,
			"Load a previously saved single or multi-player game.", 
			getDataFile("data/windows/save.bmp"), this, gridsizer);
	}
	topsizer->Add(gridsizer, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 5);

	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(
		new wxButton(this, wxID_CANCEL, wxT("Close")), 0, wxALL, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SingleFrame::addModButton(
	int &count,
	ModInfo &info,
	wxSizer *sizer)
{
	wxObjectRefData *refData = new SingleFrameData(info);
	addButtonToWindow(count++, 
		(char *) info.getDescription(), 
		(char *) info.getIcon(), 
		this, sizer, refData);
}

void SingleFrame::onLoadButton(wxCommandEvent &event)
{
	wxString file = ::wxFileSelector(wxT("Please choose the saved game to load"),
		wxString(getSaveFile(""), wxConvUTF8), // default path
		wxT(""), // default filename
		wxT(""), // default extension
		wxT("*.s3d"),
		wxOPEN | wxFILE_MUST_EXIST);
	if (!file.empty())
	{
		EndModal(wxID_OK);
		runScorched3D(formatString("-loadsave \"%s\"", (const char *) file.mb_str(wxConvUTF8)), false);
	}
}

void SingleFrame::onGameButton(wxCommandEvent &event)
{
	wxObject *object = event.GetEventObject();
	if (!object) return;
	SingleFrameData *data = 
		(SingleFrameData *) object->GetRefData();
	if (!data) return;

	if (showSingleChoiceDialog(data->mod))
	{
		EndModal(wxID_OK);
	}
}

void SingleFrame::onNetLanButton(wxCommandEvent &event)
{
	showNetLanDialog();
	EndModal(wxID_OK);
}

void SingleFrame::onTutorialButton(wxCommandEvent &event)
{
	const char *targetFilePath = getDataFile("data/singletutorial.xml");
	runScorched3D(formatString("-startclient \"%s\"", targetFilePath), false);
	EndModal(wxID_OK);
}

void SingleFrame::onCustomButton(wxCommandEvent &event)
{
	if (showSingleSDialog())
	{
		EndModal(wxID_OK);
	}
}

void SingleFrame::onScorchedButton(wxCommandEvent &event)
{
	showURL("http://www.scorched3d.co.uk");
}

bool showSingleDialog()
{
	SingleFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
