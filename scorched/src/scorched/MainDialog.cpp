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

#include <stdlib.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include <scorched/MainDialog.h>
#include <scorched/HtmlHelpDialog.h>
#include <scorched/NetDialog.h>
#include <scorched/SingleDialog.h>
#include <scorched/DisplayDialog.h>
#include <scorched/ServerSDialog.h>
#include <server/ServerMain.h>
#include <common/OptionsParam.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>

extern char scorched3dAppName[128];
static wxFrame *mainDialog = 0;
bool wxWindowExit = false;

enum
{
	ID_BUTTON_DISPLAY = 27270,
	ID_BUTTON_NETLAN,
	ID_BUTTON_SINGLE,
	ID_BUTTON_SERVER,
	ID_BUTTON_SCORCHED,
	ID_BUTTON_DONATE,
	ID_BUTTON_HELP,
	ID_MAIN_TIMER
};

void addTitleToWindow(
	wxWindow *parent,
	wxSizer *sizer,
	const char *fileName,
	int buttonId)
{
	wxBitmap scorchedBitmap;
	if (scorchedBitmap.LoadFile(wxString(fileName, wxConvUTF8), 
		wxBITMAP_TYPE_BMP) &&
		scorchedBitmap.Ok())
	{
		wxBitmapButton *button = new wxBitmapButton(
			parent, buttonId, scorchedBitmap);
		wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
		boxSizer->Add(button, 0, wxALL, 5);
		sizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
	}
}

static SDL_mutex *messageMutex_ = 0;
static std::string messageString_;
static int exitCode_ = 0;

class ScorchedProcess : public wxProcess
{
public:
	ScorchedProcess() : wxProcess(wxPROCESS_REDIRECT) { }
	
	virtual void OnTerminate(int pid, int status) 
	{
		if (status != 0)
		{
			SDL_LockMutex(messageMutex_);
			exitCode_ = status;
			if (exitCode_ != 64)
			{
				messageString_ = "The Scorched3d process "
					"terminated unexpectedly.\n";
			}
			else
			{
				messageString_ = "The Scorched3d process "
					"terminated due to configuration errors.\n";
			}
			while (IsInputAvailable())
			{
				wxTextInputStream tis(*GetInputStream());
				wxString line = tis.ReadLine();
				messageString_.append((const char *) line.mb_str(wxConvUTF8));
				messageString_.append("\n");
			}
			SDL_UnlockMutex(messageMutex_);
		}
		Detach();
		wxProcess::OnTerminate(pid, status);
	}
};

void showURL(const char *url)
{
#ifdef _WIN32
	WinExec(formatString("explorer %s", url) ,SW_SHOWDEFAULT);
#else
#ifdef __DARWIN__
	system(formatString("open %s", url));
#else
	system(formatString("mozilla %s", url));
	dialogMessage("Web site location", formatString("%s", url));
#endif // __DARWIN__
#endif // _WIN32
}

void runScorched3D(const char *text)
{
	char path[1024];
	snprintf(path, 1024, "%s %s -settingsdir %s %s", 
		getExeName(), 
		(OptionsParam::instance()->getAllowExceptions()?" -allowexceptions":""),
		OptionsParam::instance()->getSettingsDir(), 
		text);

	ScorchedProcess *process = new ScorchedProcess();
	long result = ::wxExecute(wxString(path, wxConvUTF8), wxEXEC_ASYNC, process);
	if (result == 0)
	{
		delete process;
		dialogMessage(scorched3dAppName, formatString(
			"Error: Failed to execute scorched3d using commandline :-\n"
			"%s",
			path));
	}
}

wxButton *addButtonToWindow(
	int id,
	const char *text,
	const char *bitmapName,
	wxWindow *parent,
	wxSizer *sizer,
	wxObjectRefData *data)
{
	wxButton *button = 0;
	wxBitmap bitmap;
	if (bitmap.LoadFile(wxString(bitmapName, wxConvUTF8), wxBITMAP_TYPE_BMP) &&
		bitmap.Ok())
	{
		button = new wxBitmapButton(parent, id, bitmap);
	}
	else
	{
		button = new wxButton(parent, id, wxT("Select"));
	}
	if (data) button->SetRefData(data);

	wxStaticText *staticText = new wxStaticText(
		parent, -1, 
		wxString(text, wxConvUTF8));

	sizer->Add(button, 0, wxRIGHT, 5);
	sizer->Add(staticText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	return button;
}

class MainFrame: public wxFrame
{
public:
	MainFrame();

	void onTimer(wxTimerEvent &event);
	void onDisplayButton(wxCommandEvent &event);
	void onQuitButton(wxCommandEvent &event);
	void onNetLanButton(wxCommandEvent &event);
	void onSingleButton(wxCommandEvent &event);
	void onServerButton(wxCommandEvent &event);
	void onScorchedClick(wxCommandEvent &event);
	void onDonateClick(wxCommandEvent &event);
	void onHelpButton(wxCommandEvent &event);

private:
	DECLARE_EVENT_TABLE()
	wxTimer timer_;
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_TIMER(ID_MAIN_TIMER, MainFrame::onTimer)
	EVT_BUTTON(ID_BUTTON_DISPLAY,  MainFrame::onDisplayButton)
	EVT_BUTTON(ID_BUTTON_NETLAN,  MainFrame::onNetLanButton)
	EVT_BUTTON(ID_BUTTON_SINGLE,  MainFrame::onSingleButton)
	EVT_BUTTON(ID_BUTTON_SERVER,  MainFrame::onServerButton)
	EVT_BUTTON(wxID_CANCEL,  MainFrame::onQuitButton)
	EVT_BUTTON(ID_BUTTON_SCORCHED,  MainFrame::onScorchedClick)
	EVT_BUTTON(ID_BUTTON_DONATE, MainFrame::onDonateClick)
	EVT_BUTTON(ID_BUTTON_HELP,  MainFrame::onHelpButton)
END_EVENT_TABLE()

MainFrame::MainFrame() :
	wxFrame((wxFrame *)NULL, -1, wxString(scorched3dAppName, wxConvUTF8), 
		wxDefaultPosition, wxDefaultSize, 
		wxMINIMIZE_BOX | wxCAPTION)
{
	if (!messageMutex_) messageMutex_ = SDL_CreateMutex();

	// Set the frame's icon
	wxString iconName(getDataFile("data/windows/tank2.ico"), wxConvUTF8);
	wxIcon icon(iconName, wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	// Set the backbround color to be that of the default
	// panel colour
	wxPanel panel;
	SetBackgroundColour(panel.GetBackgroundColour());

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Top Scorched Bitmap
	{
		addTitleToWindow(this, topsizer, 
			getDataFile("data/windows/scorched.bmp"),
			ID_BUTTON_SCORCHED);
	}	

	wxFlexGridSizer *gridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	// Single Player Bitmap
	{
		wxButton *button =
			addButtonToWindow(ID_BUTTON_SINGLE,
				"Start a single or multi-player game.\n"
				"One or more people play against themselves or the computer.", 
				getDataFile("data/windows/tank2.bmp"), this, gridsizer);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Client Player Bitmap
	{
		wxButton *button =
			addButtonToWindow(ID_BUTTON_NETLAN,
				"Join a game over the internet or LAN.\n"
				"Connect to a server and play with others over the internet.", 
				getDataFile("data/windows/client.bmp"), this, gridsizer);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Server Player Bitmap
	{
		addButtonToWindow(ID_BUTTON_SERVER,
			"Start a multi-player LAN or internet server.\n"
			"Allow other people to connect to your computer to play.", 
			getDataFile("data/windows/server.bmp"), this, gridsizer);
	}

	// Display Settings
	{
		wxButton *button =
			addButtonToWindow(ID_BUTTON_DISPLAY,
				"Change game settings.\n"
				"Graphics, compatability and other options", 
				getDataFile("data/windows/display.bmp"), this, gridsizer);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Help Dialog
	{
		addButtonToWindow(ID_BUTTON_HELP,
			"Show help for Scorched3D",
			getDataFile("data/windows/help.bmp"), this, gridsizer);
	}

	topsizer->Add(gridsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);
	
	// Donatations
	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	{
		wxBitmap scorchedBitmap;
		if (scorchedBitmap.LoadFile(wxString(getDataFile("data/windows/donate.bmp"), wxConvUTF8), 
			wxBITMAP_TYPE_BMP) &&
			scorchedBitmap.Ok())
		{
			wxBitmapButton *button = new wxBitmapButton(
				this, ID_BUTTON_DONATE, scorchedBitmap);
			buttonSizer->Add(button, 0, wxALIGN_LEFT | wxALL, 10);
		}
	}
	buttonSizer->Add(new wxBoxSizer(wxHORIZONTAL), 1, wxGROW);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, wxT("Quit")), 0, 
		wxALIGN_RIGHT | wxALIGN_BOTTOM | wxALL, 10);
	topsizer->Add(buttonSizer, 1, wxGROW);

	// Setup timer
	timer_.SetOwner(this, ID_MAIN_TIMER);
	timer_.Start(1000, false);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void MainFrame::onTimer(wxTimerEvent &event)
{
	std::string newString;
	SDL_LockMutex(messageMutex_);
	if (!messageString_.empty())
	{
		newString = messageString_;
		messageString_ = "";
	}
	SDL_UnlockMutex(messageMutex_);

	if (!newString.empty())
	{
		if (exitCode_ != 64)
		{
			newString.append("\n"
				"Would you like to load the failsafe "
				"scorched3d settings?\n"
				"This gives the best chance of working but "
				"at the cost of graphical detail.\n"
				"You can adjust this later in the Scorched3D "
				"display settings dialog.\n"
				"Note: Most problems can be fixed by using "
				"the very latest drivers\n"
				"for your graphics card.");
			int answer = ::wxMessageBox(
				wxString(newString.c_str(), wxConvUTF8),
				wxT("Scorched3D Abnormal Termination"),
				wxYES_NO | wxICON_ERROR);
			if (answer == wxYES)
			{
				OptionsDisplay::instance()->loadSafeValues();
				OptionsDisplay::instance()->writeOptionsToFile();
			}
		}
		else
		{
			::wxMessageBox(
				wxString(newString.c_str(), wxConvUTF8),
				wxT("Scorched3D Termination"),
				wxICON_ERROR);
		}
	}
}

void MainFrame::onScorchedClick(wxCommandEvent &event)
{
	showURL("http://www.scorched3d.co.uk");
}

void MainFrame::onDonateClick(wxCommandEvent &event)
{
	const char *exec = 
		"\"https://www.paypal.com/xclick/business=donations%40"
		"scorched3d.co.uk&item_name=Scorched3D&no_note=1&tax=0&currency_code=GBP\"";
	showURL(exec);
}

void MainFrame::onDisplayButton(wxCommandEvent &event)
{
	showDisplayDialog();
}

void MainFrame::onNetLanButton(wxCommandEvent &event)
{
	showNetLanDialog();
}

void MainFrame::onSingleButton(wxCommandEvent &event)
{
	showSingleDialog();
}

void MainFrame::onServerButton(wxCommandEvent &event)
{
	showServerSDialog();
}

void MainFrame::onQuitButton(wxCommandEvent &event)
{
	wxWindowExit = true;
	Close();
}

void MainFrame::onHelpButton(wxCommandEvent &event)
{
	showHtmlHelpDialog();
}

void showMainDialog()
{
	mainDialog = new MainFrame;
	mainDialog->Show(TRUE);
}

wxFrame *getMainDialog()
{
	return mainDialog;
}
