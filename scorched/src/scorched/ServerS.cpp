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

#include "ServerS-def.cpp"

static void createControls(wxWindow *parent,
						   wxSizer *sizer)
{
	wxStaticBox *servernameBox = 
		new wxStaticBox(parent, -1, "Server Settings");
	wxStaticBoxSizer *servernameSizer = 
		new wxStaticBoxSizer(servernameBox, wxVERTICAL);
	wxFlexGridSizer *servernameSizer2 = new wxFlexGridSizer(4, 2, 5, 5);
	IDC_SERVER_NAME_CTRL = 
		new wxTextCtrl(parent, IDC_SERVER_NAME,
		"",
		wxDefaultPosition, wxSize((int) 226.5, -1));
	servernameSizer2->Add(new wxStaticText(parent, -1, "Server Name :"), 
						  0, wxALIGN_CENTER_VERTICAL);
	servernameSizer2->Add(IDC_SERVER_NAME_CTRL);
	IDC_SERVER_PORT_CTRL = 
		new wxTextCtrl(parent, IDC_SERVER_PORT,
		"",
		wxDefaultPosition, wxSize((int) 72, -1));
	servernameSizer2->Add(new wxStaticText(parent, -1, "Port Number :"),
						  0, wxALIGN_CENTER_VERTICAL);
	servernameSizer2->Add(IDC_SERVER_PORT_CTRL);		
	servernameSizer->Add(servernameSizer2);
	IDC_PUBLISHIP_CTRL_TEXT = new wxStaticText(parent, -1, "Published IP :");
	servernameSizer2->Add(IDC_PUBLISHIP_CTRL_TEXT,
						  0, wxALIGN_CENTER_VERTICAL);

	wxSizer *hori = new wxBoxSizer(wxHORIZONTAL);
	IDC_PUBLISHIP_CTRL = 
		new wxTextCtrl(parent, IDC_PUBLISHIP,
		"",
		wxDefaultPosition, wxSize((int) 150, -1));
	hori->Add(IDC_PUBLISHIP_CTRL);
	IDOK_PUBLISHAUTO_CTRL =
		new wxButton(parent, IDC_PUBLISHAUTO,
		"AutoDetect");
	hori->Add(IDOK_PUBLISHAUTO_CTRL);
	servernameSizer2->Add(hori);		

	IDC_PUBLISH_CTRL = 
		new wxCheckBox(parent, IDC_PUBLISH,
		"Allow ALL other internet users to see and use this server");	
	servernameSizer->Add(IDC_PUBLISH_CTRL);
	sizer->Add(servernameSizer, 0, wxALL, 5);
	
	wxStaticBox *modBox = 
		new wxStaticBox(parent, -1, "Mod Settings");
	wxStaticBoxSizer *modSizer = 
		new wxStaticBoxSizer(modBox, wxHORIZONTAL);
	modSizer->Add(new wxStaticText(parent, -1,
		"Use Mod :"), 0, wxALL, 5);
	IDC_SERVER_MOD_CTRL = 
		new wxComboBox(parent, -1,
		"",
		wxDefaultPosition, wxSize((int) 150, (int) 124.5),
		0, 0, wxCB_READONLY);
	modSizer->Add(IDC_SERVER_MOD_CTRL, 0, wxALL, 5);
	sizer->Add(modSizer, 0, wxGROW | wxALIGN_RIGHT | wxALL, 5);

	wxStaticBox *advBox = 
		new wxStaticBox(parent, -1, "Advanced Settings");
	wxStaticBoxSizer *advSizer = 
		new wxStaticBoxSizer(advBox, wxVERTICAL);
	IDC_BUTTON_SETTINGS_CTRL = 
		new wxButton(parent, IDC_BUTTON_SETTINGS,
		"Edit Advanced Settings");
	advSizer->Add(IDC_BUTTON_SETTINGS_CTRL, 0, wxALL, 5);
	sizer->Add(advSizer, 0, wxGROW | wxALL, 5);

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(parent, wxID_OK, "Start Server");
	IDCANCEL_CTRL = new wxButton(parent, wxID_CANCEL, "Cancel");
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxALL, 10);
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 10);
	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
}

