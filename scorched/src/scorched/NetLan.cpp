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
		"Server browsing brought to you\nby The All-Seeing Eye :\nhttp://www.udpsoft.com/eye",
		wxPoint((int) 315, (int) 20));
	IDC_BUTTON_LAN_CTRL = 
		new wxButton(parent, IDC_BUTTON_LAN,
		"Refresh LAN Games",
		wxPoint((int) 265.5, (int) 328.5), wxSize((int) 133.5, (int) 21));
	IDC_BUTTON_NET_CTRL = 
		new wxButton(parent, IDC_BUTTON_NET,
		"Refresh Internet Games",
		wxPoint((int) 403.5, (int) 328.5), wxSize((int) 123, (int) 21));
	new wxStaticText(parent, -1,
		"Connect To Name/IP : ",
		wxPoint((int) 37.5, (int) 88.5), wxSize((int) 111, (int) 12));
	IDC_PROGRESS1_CTRL = 
		new wxGauge(parent, IDC_PROGRESS1, 0,
		wxPoint((int) 9, (int) 334.5), wxSize((int) 249, (int) 10.5));
	IDC_EDIT_SERVER_CTRL = 
		new wxTextCtrl(parent, IDC_EDIT_SERVER,
		"",
		wxPoint((int) 147, (int) 84), wxSize((int) 285, (int) 21));

	wxImage image;
	if (image.LoadFile(PKGDIR "data/windows/eye1.bmp", wxBITMAP_TYPE_BMP))
	{
		wxBitmap eyeBitmap(image);
		IDC_BUTTON_EYE_CTRL = 
		  new wxBitmapButton(parent, IDC_BUTTON_EYE,
				     eyeBitmap,
				     wxPoint((int) 54, (int) 0.0));
	}
	IDC_CLEAR_CTRL = 
		new wxButton(parent, IDC_CLEAR,
		"Clear",
		wxPoint((int) 436.5, (int) 87), wxSize((int) 46.5, (int) 15));
	IDOK_CTRL = 
		new wxButton(parent, wxID_OK,
		"Join Game",
		wxPoint((int) 451.5, (int) 364.5), wxSize((int) 75, (int) 21));
	IDCANCEL_CTRL = 
		new wxButton(parent, wxID_CANCEL,
		"Cancel",
		wxPoint((int) 370.5, (int) 364.5), wxSize((int) 75, (int) 21));
}

