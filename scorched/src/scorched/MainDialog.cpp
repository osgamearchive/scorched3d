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
#include <scorched/NetDialog.h>
#include <scorched/SingleDialog.h>
#include <scorched/DisplayDialog.h>
#include <scorched/ServerSDialog.h>
#include <irc/IrcDialog.h>
#include <server/ServerMain.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <windows.h>

extern char scorched3dAppName[128];
static wxFrame *mainDialog = 0;

wxBitmapButton *addButtonToWindow(
	int id, int X, int Y,
	char *text,
	char *bitmapName,
	wxWindow *parent)
{
	wxBitmapButton *button = 0;
	wxImage image;
	if (image.LoadFile(_T(bitmapName), wxBITMAP_TYPE_BMP))
	{
		wxBitmap bitmap(image);
		button = new wxBitmapButton(
			parent, id, bitmap, wxPoint(X, Y));
		new wxStaticText(
			parent, -1, 
			text, wxPoint(X + 60, Y + 5));
	}
	return button;
}

enum
{
	ID_BUTTON_DISPLAY = 1,
	ID_BUTTON_NETLAN,
	ID_BUTTON_SINGLE,
	ID_BUTTON_SERVER,
	ID_BUTTON_SCORCHED
};

class MainFrame: public wxFrame
{
public:
	MainFrame();

	void onDisplayButton();
	void onQuitButton();
	void onNetLanButton();
	void onSingleButton();
	void onServerButton();
	void onScorchedClick();

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_BUTTON_DISPLAY,  MainFrame::onDisplayButton)
	EVT_BUTTON(ID_BUTTON_NETLAN,  MainFrame::onNetLanButton)
	EVT_BUTTON(ID_BUTTON_SINGLE,  MainFrame::onSingleButton)
	EVT_BUTTON(ID_BUTTON_SERVER,  MainFrame::onServerButton)
	EVT_BUTTON(wxID_CANCEL,  MainFrame::onQuitButton)
	EVT_BUTTON(ID_BUTTON_SCORCHED,  MainFrame::onScorchedClick)
END_EVENT_TABLE()

MainFrame::MainFrame() :
	wxFrame((wxFrame *)NULL, -1, scorched3dAppName, wxPoint(0,0), wxSize(418, 345), 
		wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION)
{
	CentreOnScreen();

	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	// Set the backbround color to be that of the default
	// panel colour
	wxPanel panel;
	SetBackgroundColour(panel.GetBackgroundColour());

	// Top Scorched Bitmap
	{
		wxImage image;

		if (image.LoadFile(_T(PKGDIR "data/windows/scorcheds.bmp"), wxBITMAP_TYPE_BMP))
		{
			wxBitmap scorchedBitmap(image);
			wxBitmapButton *button = new wxBitmapButton(
				this, ID_BUTTON_SCORCHED, scorchedBitmap, wxPoint(10, 10));

		}
	}

	// Single Player Bitmap
	{
		wxBitmapButton *button =
			addButtonToWindow(ID_BUTTON_SINGLE, 30, 85,
				"Start a single or multi-player player game.\n"
				"One or more people play against themselves or the computer.", 
				PKGDIR "data/windows/tank2.bmp", this);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Client Player Bitmap
	{
		wxBitmapButton *button =
			addButtonToWindow(ID_BUTTON_NETLAN, 30, 140,
				"Join a game over the internet or LAN.\n"
				"Connect to a server and play with others over the internet.", 
				PKGDIR "data/windows/client.bmp", this);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	// Server Player Bitmap
	{
		addButtonToWindow(ID_BUTTON_SERVER, 30, 195,
			"Start a multiplayer LAN or internet server.\n"
			"Allow other people to connect to your computer to play.", 
			PKGDIR "data/windows/server.bmp", this);
	}

	// Display Settings
	{
		wxBitmapButton *button =
			addButtonToWindow(ID_BUTTON_DISPLAY, 30, 250,
				"Change the display settings.\n"
				"Change graphics and compatability options", 
				PKGDIR "data/windows/display.bmp", this);
		if (button && !OptionsParam::instance()->getSDLInitVideo())
		{
			button->Disable();
		}
	}

	new wxButton(this, wxID_CANCEL,
		"Quit",
		wxPoint((int) 330, (int) 285), wxSize((int) 75, (int) 21));
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
	if (showNetLanDialog())
	{
		Close();
	}
}

void MainFrame::onSingleButton()
{
	if (showSingleDialog())
	{
		Close();
	}
}

void MainFrame::onServerButton()
{
	if (showServerSDialog())
	{
		Close();
	}
}

void MainFrame::onQuitButton()
{
	Close();
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
