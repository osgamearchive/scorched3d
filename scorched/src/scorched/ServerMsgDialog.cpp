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

#include <scorched/ServerMsgDialog.h>
#include <server/ServerTimedMessage.h>
#include <common/Defines.h>
#include <wx/wx.h>
#include <wx/image.h>

extern char scorched3dAppName[128];

class ServerMsgFrame: public wxDialog
{
public:
	ServerMsgFrame();

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

protected:
	wxTextCtrl *message_;
	wxChoice *time_;
};

ServerMsgFrame::ServerMsgFrame() :
	wxDialog(0, -1, wxString(scorched3dAppName))
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the size for the layout
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	message_ = new wxTextCtrl(this, -1, 
		ServerTimedMessage::instance()->getMessage(),
		wxDefaultPosition, wxSize(300, -1));
	topsizer->Add(message_, 0, wxALIGN_CENTER | wxALL, 10);

	time_ = new wxChoice(this, -1,
		wxDefaultPosition, wxSize(200, -1));
	topsizer->Add(time_, 0, wxALIGN_CENTER | wxBOTTOM, 10);
	for (int i=0; i<900; i+=30)
	{
		time_->Append(formatString("%i Seconds", i));
	}
	time_->SetStringSelection(
		formatString("%i Seconds", ServerTimedMessage::instance()->getTime()));

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *IDOK_CTRL = new wxButton(this, wxID_OK, "Ok");
	wxButton *IDCANCEL_CTRL = new wxButton(this, wxID_CANCEL, "Cancel");
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxRIGHT, 5);
	buttonSizer->Add(IDOK_CTRL, 0, wxLEFT, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	SetSizer(topsizer); // use the sizer for layout
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

bool ServerMsgFrame::TransferDataToWindow()
{
	return true;
}

bool ServerMsgFrame::TransferDataFromWindow()
{
	unsigned int time = 0;
	sscanf(time_->GetStringSelection().c_str(), "%i", &time);

	ServerTimedMessage::instance()->setMessage(
		message_->GetValue().c_str(),
		time);

	return true;
}

void showServerMsgDialog()
{
	ServerMsgFrame frame;
	frame.ShowModal();
}

