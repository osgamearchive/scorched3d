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

#include <scorched/KeyDialog.h>
#include <common/Defines.h>
#include <common/Keyboard.h>
#include <wx/wx.h>

extern char scorched3dAppName[128];
extern char *displayOptions;

class KeyFrame: public wxDialog
{
public:
	KeyFrame(wxDialog *);

	void onKeyDown(wxKeyEvent &event);

protected:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(KeyFrame, wxDialog)
    EVT_KEY_DOWN(KeyFrame::onKeyDown)
END_EVENT_TABLE()

KeyFrame::KeyFrame(wxDialog *dialog) :
	wxDialog(dialog, -1, wxString(scorched3dAppName))
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	topsizer->Add(new wxStaticText(this, -1, "Press desired key"));

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void KeyFrame::onKeyDown(wxKeyEvent &event)
{
	EndModal(0);
}

void showKeyDialog(wxDialog *dialog)
{
	KeyFrame frame(dialog);
	frame.ShowModal();
}

