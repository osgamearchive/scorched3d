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

#include <scorched/HtmlHelpDialog.h>
#include <scorched/MainDialog.h>
#include <common/Defines.h>
#include <wx/wx.h>
#include <wx/minifram.h>
#include <wx/wxhtml.h>
#include <wx/event.h>

enum
{
	ID_BUTTON_OK = 111,
	ID_BUTTON_BACK,
	ID_BUTTON_FORWARD,
	ID_BUTTON_HOME
};

extern char scorched3dAppName[128];

class HelpFrame: public wxFrame
{
public:
	HelpFrame();

	void onExit(wxCommandEvent &event);
	void onBack(wxCommandEvent &event);
	void onForward(wxCommandEvent &event);
	void onHome(wxCommandEvent &event);
	void onKeyDown(wxKeyEvent &event);

	wxHtmlWindow *html_;

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(HelpFrame, wxFrame)
	EVT_KEY_DOWN(HelpFrame::onKeyDown) 
	EVT_BUTTON(ID_BUTTON_OK, HelpFrame::onExit)
	EVT_BUTTON(ID_BUTTON_BACK, HelpFrame::onBack)
	EVT_BUTTON(ID_BUTTON_FORWARD, HelpFrame::onForward)
	EVT_BUTTON(ID_BUTTON_HOME, HelpFrame::onHome)
END_EVENT_TABLE()

HelpFrame::HelpFrame() :
	wxFrame(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8),
		wxDefaultPosition, wxDefaultSize, 
		wxSTAY_ON_TOP | wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Set the backbround color to be that of the default
	// panel colour
	wxPanel panel;
	SetBackgroundColour(panel.GetBackgroundColour());

	// Create the size for the layout
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Ok Button
	wxBoxSizer *buttonSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxButton *IDBACK_CTRL = new wxButton(this, ID_BUTTON_BACK, wxT("<< Back"),
		wxDefaultPosition, wxSize(-1, 18));
	buttonSizer2->Add(IDBACK_CTRL, 0, wxALL, 3);
	wxButton *IDHOME_CTRL = new wxButton(this, ID_BUTTON_HOME, wxT("Home"),
		wxDefaultPosition, wxSize(-1, 18));
	buttonSizer2->Add(IDHOME_CTRL, 0, wxALL, 3);
	wxButton *IDFORWARD_CTRL = new wxButton(this, ID_BUTTON_FORWARD, wxT("Forward >>"),
		wxDefaultPosition, wxSize(-1, 18));
	buttonSizer2->Add(IDFORWARD_CTRL, 0, wxALL, 3);
	topsizer->Add(buttonSizer2, 0, wxALIGN_CENTER);

	// Html Window
	html_ = new wxHtmlWindow(this, -1, 
		wxDefaultPosition, wxSize(500, 300));
	wxCommandEvent event;
	onHome(event);
	topsizer->Add(html_, 1, wxLEFT | wxRIGHT | wxGROW, 5);

	// Ok Button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *IDOK_CTRL = new wxButton(this, ID_BUTTON_OK, wxT("Close"),
		wxDefaultPosition, wxSize(-1, 18));
	IDOK_CTRL->SetDefault();
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 3);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void HelpFrame::onKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_ESCAPE) Close();
}

void HelpFrame::onExit(wxCommandEvent &event)
{
	Close();
}

void HelpFrame::onForward(wxCommandEvent &event)
{
	html_->HistoryForward();
}

void HelpFrame::onBack(wxCommandEvent &event)
{
	html_->HistoryBack();
}

void HelpFrame::onHome(wxCommandEvent &event)
{
	html_->HistoryClear();
	html_->LoadPage(wxString(getDocFile("documentation/html/index.html"), wxConvUTF8));
}

void showHtmlHelpDialog()
{
	// For the moment just show the documentation in the default web browser
	showURL(formatString("file://%s", getDocFile("documentation/html/index.html")));

	//HelpFrame *frame = new HelpFrame();
	//frame->Show();
}
