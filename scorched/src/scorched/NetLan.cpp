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

static void createControls(wxWindow *parent)
{
	new wxStaticText(parent, -1,
		"Connect To Name/IP : ",
		wxPoint((int) 30, (int) 10.5));
	new wxStaticText(parent, -1,
		"Password : ",
		wxPoint((int) 30, (int) 34.5));
	IDC_EDIT_SERVER_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_SERVER,
		"",
		wxPoint((int) 147, (int) 6), wxSize((int) 285, (int) 21));
	IDC_EDIT_PASSWORD_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_PASSWORD,
		"",
		wxPoint((int) 147, (int) 30), wxSize((int) 285, (int) 21));
	IDC_CLEAR_CTRL = 
		new wxButton(parent, IDC_CLEAR,
		"Clear",
		wxPoint((int) 436.5, (int) 9), wxSize((int) 46.5, (int) 15));
	IDC_CLEAR_PASSWORD_CTRL = 
		new wxButton(parent, IDC_CLEAR_PASSWORD,
		"Clear",
		wxPoint((int) 436.5, (int) 33), wxSize((int) 46.5, (int) 15));
	IDOK_CTRL = 
		new wxButton(parent, wxID_OK,
		"Join Game",
		wxPoint((int) 451.5, (int) 346.5), wxSize((int) 75, (int) 21));
	IDCANCEL_CTRL = 
		new wxButton(parent, wxID_CANCEL,
		"Cancel",
		wxPoint((int) 370.5, (int) 346.5), wxSize((int) 75, (int) 21));
	IDC_BUTTON_LAN_CTRL = 
		new wxButton(parent, IDC_BUTTON_LAN,
		"Refresh LAN Games",
		wxPoint((int) 265.5, (int) 316), wxSize((int) 133.5, (int) 21));
	IDC_BUTTON_NET_CTRL = 
		new wxButton(parent, IDC_BUTTON_NET,
		"Refresh Internet Games",
		wxPoint((int) 403.5, (int) 316), wxSize((int) 123, (int) 21));
}

