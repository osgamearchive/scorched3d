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

static void createControlsPre(wxWindow *parent, wxSizer *sizer)
{
	// Name/password
	wxGridSizer *nameSizer = new wxFlexGridSizer(3, 3, 5, 5);
	wxStaticText *nameText = 
		new wxStaticText(parent, -1, "Connect To Name/IP : ");
	wxStaticText *passText = 
		new wxStaticText(parent, -1, "Password : ");
	IDC_EDIT_SERVER_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_SERVER,
		"", wxDefaultPosition, wxSize(285, -1));
	IDC_EDIT_PASSWORD_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_PASSWORD,
		"", wxDefaultPosition, wxSize(285, -1), wxTE_PASSWORD);
	IDC_CLEAR_CTRL = 
		new wxButton(parent, IDC_CLEAR,
		"Clear");
	IDC_CLEAR_PASSWORD_CTRL = 
		new wxButton(parent, IDC_CLEAR_PASSWORD,
		"Clear");
	nameSizer->Add(nameText, 0, wxALIGN_CENTER_VERTICAL);
	nameSizer->Add(IDC_EDIT_SERVER_CTRL, 0, 0);
	nameSizer->Add(IDC_CLEAR_CTRL, 0, 0);
	nameSizer->Add(passText, 0, wxALIGN_CENTER_VERTICAL);
	nameSizer->Add(IDC_EDIT_PASSWORD_CTRL, 0, 0);
	nameSizer->Add(IDC_CLEAR_PASSWORD_CTRL, 0, 0);
	sizer->Add(nameSizer, 0, wxALIGN_CENTER | wxALL, 5);
}


static void createControlsPost(wxWindow *parent, wxSizer *sizer)
{
	// Lan/Internet refresh
	wxBoxSizer *refreshButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDC_BUTTON_LAN_CTRL = 
		new wxButton(parent, IDC_BUTTON_LAN, "Refresh LAN Games");
	IDC_BUTTON_NET_CTRL = 
		new wxButton(parent, IDC_BUTTON_NET, "Refresh Internet Games");
	refreshButtonSizer->Add(IDC_BUTTON_LAN_CTRL, 0, wxALL, 5);
	refreshButtonSizer->Add(IDC_BUTTON_NET_CTRL, 0, wxALL, 5);
	sizer->Add(refreshButtonSizer, 0, wxALIGN_RIGHT);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(parent, wxID_OK, "Ok");
	IDCANCEL_CTRL = new wxButton(parent, wxID_CANCEL, "Cancel");
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxALL, 5);
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 5);
	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
}

