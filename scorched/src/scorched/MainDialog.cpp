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
static char exeName[1024];
bool wxWindowExit = false;

enum
{
	ID_BUTTON_DISPLAY = 27270,
	ID_BUTTON_NETLAN,
	ID_BUTTON_SINGLE,
	ID_BUTTON_SERVER,
	ID_BUTTON_SCORCHED,
	ID_BUTTON_HELP,
	ID_MAIN_TIMER
};

void addTitleToWindow(
	wxWindow *parent,
	wxSizer *sizer)
{
	wxBitmap scorchedBitmap;
	if (scorchedBitmap.LoadFile(getDataFile("data/windows/scorched.bmp"), 
		wxBITMAP_TYPE_BMP) &&
		scorchedBitmap.Ok())
	{
		wxBitmapButton *button = new wxBitmapButton(
			parent, ID_BUTTON_SCORCHED, scorchedBitmap);
		wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
		boxSizer->Add(button, 0, wxALL, 5);
		sizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
	}
}

void setExeName(const char *name, bool allowExceptions)
{
	strcpy(exeName, name);
	if (allowExceptions)
	{
		strcat(exeName, " -allowexceptions");
	}
}

static SDL_mutex *messageMutex_ = 0;
static wxString messageString_;

class ScorchedProcess : public wxProcess
{
public:
	ScorchedProcess() : wxProcess(wxPROCESS_REDIRECT) { }
	
	virtual void OnTerminate(int pid, int status) 
	{
		if (status != 0)
		{
			SDL_LockMutex(messageMutex_);
			messageString_ = "The Scorched3d process "
				"terminated unexpectedly.\n"
				"The error given was :\n";
			while (IsInputAvailable())
			{
				wxTextInputStream tis(*GetInputStream());
				messageString_.append(tis.ReadLine());
				messageString_.append("\n");
			}
			SDL_UnlockMutex(messageMutex_);
		}
		Detach();
		wxProcess::OnTerminate(pid, status);
	}
};

void runScorched3D(const char *fmt, ...)
{
	char text[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	char path[1024];
	sprintf(path, "%s %s", exeName, text);

	ScorchedProcess *process = new ScorchedProcess();
	long result = ::wxExecute(path, wxEXEC_ASYNC, process);
	if (result == 0)
	{
		delete process;
		dialogMessage(scorched3dAppName,
			"Error: Failed to execute scorched3d using commandline :-\n"
			"%s",
			path);
	}
}

wxButton *addButtonToWindow(
	int id,
	char *text,
	char *bitmapName,
	wxWindow *parent,
	wxSizer *sizer)
{
	wxButton *button = 0;
	wxBitmap bitmap;
	if (bitmap.LoadFile(getDataFile(bitmapName), wxBITMAP_TYPE_BMP) &&
		bitmap.Ok())
	{
		button = new wxBitmapButton(parent, id, bitmap);
	}
	else
	{
		button = new wxButton(parent, id, "Select");
	}

	wxStaticText *staticText = new wxStaticText(
		parent, -1, 
		text);

	sizer->Add(button, 0, wxRIGHT, 5);
	sizer->Add(staticText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	return button;
}

class MainFrame: public wxFrame
{
public:
	MainFrame();

	void onTimer();
	void onDisplayButton();
	void onQuitButton();
	void onNetLanButton();
	void onSingleButton();
	void onServerButton();
	void onScorchedClick();
	void onHelpButton();

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
	EVT_BUTTON(ID_BUTTON_HELP,  MainFrame::onHelpButton)
END_EVENT_TABLE()

MainFrame::MainFrame() :
	wxFrame((wxFrame *)NULL, -1, scorched3dAppName, wxDefaultPosition, wxDefaultSize, 
		wxMINIMIZE_BOX | wxCAPTION)
{
	if (!messageMutex_) messageMutex_ = SDL_CreateMutex();

	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	// Set the backbround color to be that of the default
	// panel colour
	wxPanel panel;
	SetBackgroundColour(panel.GetBackgroundColour());

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Top Scorched Bitmap
	{
		addTitleToWindow(this, topsizer);
	}	

	wxFlexGridSizer *gridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	// Single Player Bitmap
	{
		wxButton *button =
			addButtonToWindow(ID_BUTTON_SINGLE,
				"Start a single or multi-player game.\n"
				"One or more people play against themselves or the computer.", 
				"data/windows/tank2.bmp", this, gridsizer);
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
				"data/windows/client.bmp", this, gridsizer);
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
			"data/windows/server.bmp", this, gridsizer);
	}

	// Display Settings
	{
		wxButton *button =
			addButtonToWindow(ID_BUTTON_DISPLAY,
				"Change game settings.\n"
				"Graphics, compatability and other options", 
				"data/windows/display.bmp", this, gridsizer);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Help Dialog
	{
		addButtonToWindow(ID_BUTTON_HELP,
			"Show help for Scorched3D",
			"data/windows/help.bmp", this, gridsizer);
	}

	topsizer->Add(gridsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);

	// Setup timer
	timer_.SetOwner(this, ID_MAIN_TIMER);
	timer_.Start(1000, false);

	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Quit"), 0, wxALL, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void MainFrame::onTimer()
{
	wxString newString;
	SDL_LockMutex(messageMutex_);
	if (!messageString_.empty())
	{
		newString = messageString_;
		messageString_ = "";
	}
	SDL_UnlockMutex(messageMutex_);

	if (!newString.empty())
	{
		newString.append("\n"
			"Would you like to load the failsafe "
			"scorched3d settings?\n"
			"This gives the best chance of working but "
			"at the cost of graphical detail.\n"
			"You can adjust this later in the Scorched3D "
			"display settings dialog.");
		int answer = ::wxMessageBox(
			newString,
			"Scorched3D Abnormal Termination",
			wxYES_NO | wxICON_ERROR);
		if (answer == wxYES)
		{
			OptionsDisplay::instance()->loadSafeValues();
			OptionsDisplay::instance()->writeOptionsToFile();
		}
	}
}

void MainFrame::onScorchedClick()
{
#ifdef _WIN32
	WinExec("explorer http://www.scorched3d.co.uk", SW_SHOWDEFAULT);
#endif
}

void MainFrame::onDisplayButton()
{
	showDisplayDialog();
}

void MainFrame::onNetLanButton()
{
	showNetLanDialog();
}

void MainFrame::onSingleButton()
{
	showSingleDialog();
}

void MainFrame::onServerButton()
{
	showServerSDialog();
}

void MainFrame::onQuitButton()
{
	wxWindowExit = true;
	Close();
}

void MainFrame::onHelpButton()
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
