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
#include <wx/wxhtml.h>

extern char scorched3dAppName[128];

class HelpFrame: public wxFrame
{
public:
	HelpFrame();

};

HelpFrame::HelpFrame() :
	wxFrame(getMainDialog(), -1, wxString(scorched3dAppName))
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the size for the layout
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Html Window
	wxHtmlWindow *window = new wxHtmlWindow(this, -1, 
		wxDefaultPosition, wxSize(600, 400));
	window->LoadPage(getDocFile("documentation/html/index.html"));
	topsizer->Add(window);

	// Ok Button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *IDOK_CTRL = new wxButton(this, wxID_OK, "Ok");
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 10);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void showHtmlHelpDialog()
{
	HelpFrame *frame = new HelpFrame();
	frame->Show();
}

