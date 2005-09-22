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

#include "NetLan-def.cpp"
#include <common/Defines.h>

static void createImageMap()
{
	wxBitmap questionBitmap;
	questionBitmap.LoadFile(
		wxString(getDataFile("data/windows/question.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap okBitmap;
	okBitmap.LoadFile(
		wxString(getDataFile("data/windows/ok.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap warnBitmap;
	warnBitmap.LoadFile(
		wxString(getDataFile("data/windows/warn.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap exlaimBitmap;
	exlaimBitmap.LoadFile(
		wxString(getDataFile("data/windows/exclaim.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	if (!netLanImageList)
	{
		netLanImageList = new wxImageList(16, 16, TRUE);
		netLanImageList->Add(questionBitmap);
		netLanImageList->Add(okBitmap);
		netLanImageList->Add(warnBitmap);
		netLanImageList->Add(exlaimBitmap);
	}
}

static void createControlsPre(wxWindow *parent, wxSizer *sizer)
{
	createImageMap();

	wxBitmap maskBitmap;
	maskBitmap.LoadFile(
		wxString(getDataFile("data/windows/mask.bmp"), wxConvUTF8),
		wxBITMAP_TYPE_BMP);

	wxBitmap questionBitmap;
	questionBitmap.LoadFile(
		wxString(getDataFile("data/windows/question.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap okBitmap;
	okBitmap.LoadFile(
		wxString(getDataFile("data/windows/ok.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap warnBitmap;
	warnBitmap.LoadFile(
		wxString(getDataFile("data/windows/warn.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);
	wxBitmap exlaimBitmap;
	exlaimBitmap.LoadFile(
		wxString(getDataFile("data/windows/exclaim.gif"), wxConvUTF8),
		wxBITMAP_TYPE_GIF);

	// Name/password
	wxGridSizer *nameSizer = new wxFlexGridSizer(3, 3, 5, 5);
	wxStaticText *nameText = 
		new wxStaticText(parent, -1, wxT("Connect To Name/IP : "));
	wxStaticText *userNameText = 
		new wxStaticText(parent, -1, wxT("User Name (Optional) : "));
	wxStaticText *passText = 
		new wxStaticText(parent, -1, wxT("Password (Optional) : "));
	IDC_EDIT_SERVER_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_SERVER,
		wxT(""), wxDefaultPosition, wxSize(285, -1));
	IDC_EDIT_NAME_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_NAME,
		wxT(""), wxDefaultPosition, wxSize(285, -1), 0);
	IDC_EDIT_PASSWORD_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_PASSWORD,
		wxT(""), wxDefaultPosition, wxSize(285, -1), wxTE_PASSWORD);
	IDC_CLEAR_CTRL = 
		new wxButton(parent, IDC_CLEAR,
		wxT("Clear"),
		wxDefaultPosition, wxSize(-1, 20));
	IDC_CLEAR_NAME_CTRL = 
		new wxButton(parent, IDC_CLEAR_NAME,
		wxT("Clear"),
		wxDefaultPosition, wxSize(-1, 20));
	IDC_CLEAR_PASSWORD_CTRL = 
		new wxButton(parent, IDC_CLEAR_PASSWORD,
		wxT("Clear"),
		wxDefaultPosition, wxSize(-1, 20));
	nameSizer->Add(nameText, 0, wxALIGN_CENTER_VERTICAL);
	nameSizer->Add(IDC_EDIT_SERVER_CTRL, 0, 0);
	nameSizer->Add(IDC_CLEAR_CTRL, 0, 0);
	nameSizer->Add(userNameText, 0, wxALIGN_CENTER_VERTICAL);
	nameSizer->Add(IDC_EDIT_NAME_CTRL, 0, 0);
	nameSizer->Add(IDC_CLEAR_NAME_CTRL, 0, 0);
	nameSizer->Add(passText, 0, wxALIGN_CENTER_VERTICAL);
	nameSizer->Add(IDC_EDIT_PASSWORD_CTRL, 0, 0);
	nameSizer->Add(IDC_CLEAR_PASSWORD_CTRL, 0, 0);
	sizer->Add(nameSizer, 0, wxALIGN_CENTER | wxALL, 5);
	
	// Key
	wxFlexGridSizer *keySizer = new wxFlexGridSizer(1, 8, 5, 5);
	keySizer->Add(new wxStaticBitmap(parent, -1, questionBitmap));
	keySizer->Add(new wxStaticText(parent, -1, wxT("State unknown")), 0, wxRIGHT, 10);
	keySizer->Add(new wxStaticBitmap(parent, -1, okBitmap));
	keySizer->Add(new wxStaticText(parent, -1, wxT("Game in progress")), 0, wxRIGHT, 10);
	keySizer->Add(new wxStaticBitmap(parent, -1, warnBitmap));
	keySizer->Add(new wxStaticText(parent, -1, wxT("No game in progress")), 0, wxRIGHT, 10);
	keySizer->Add(new wxStaticBitmap(parent, -1, exlaimBitmap));
	keySizer->Add(new wxStaticText(parent, -1, wxT("Cannot join")));	
	sizer->Add(keySizer, 0, wxALIGN_CENTER | wxALL, 5);
}


static void createControlsPost(wxWindow *parent, wxSizer *sizer)
{
	// Lan/Internet refresh
	wxBoxSizer *refreshButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDC_BUTTON_LAN_CTRL = 
		new wxButton(parent, IDC_BUTTON_LAN, wxT("Refresh LAN Games"));
	IDC_BUTTON_NET_CTRL = 
		new wxButton(parent, IDC_BUTTON_NET, wxT("Refresh Internet Games"));
	refreshButtonSizer->Add(IDC_BUTTON_LAN_CTRL, 0, wxALL, 5);
	refreshButtonSizer->Add(IDC_BUTTON_NET_CTRL, 0, wxALL, 5);
	sizer->Add(refreshButtonSizer, 0, wxALIGN_RIGHT);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(parent, IDC_BUTTON_JOIN, wxT("Join Game"));
	IDCANCEL_CTRL = new wxButton(parent, wxID_CANCEL, wxT("Cancel"));
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxALL, 5);
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 5);
	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
}

