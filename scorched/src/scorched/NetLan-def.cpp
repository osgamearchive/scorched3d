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

enum
{
	IDC_SERVER_LIST = 1,
	IDC_PLAYER_LIST,
	IDC_BUTTON_REFRESH,
	IDC_BUTTON_FAVOURITES,
	IDC_BUTTON_JOIN,
	IDC_EDIT_NAME,
	IDC_EDIT_SERVER,
	IDC_EDIT_PASSWORD,
	IDC_CLEAR,
	IDC_CLEAR_NAME,
	IDC_CLEAR_PASSWORD,
	IDC_RADIO_LAN,
	IDC_RADIO_NET,
	IDC_RADIO_FAV
};

static wxImageList *netLanImageList = 0;
static wxTextCtrl *IDC_EDIT_SERVER_CTRL = 0;
static wxTextCtrl *IDC_EDIT_NAME_CTRL = 0;
static wxTextCtrl *IDC_EDIT_PASSWORD_CTRL = 0;
static wxButton *IDC_BUTTON_REFRESH_CTRL = 0;
static wxButton *IDC_BUTTON_FAVOURITES_CTRL = 0;
static wxButton *IDC_CLEAR_CTRL = 0;
static wxButton *IDC_CLEAR_NAME_CTRL = 0;
static wxButton *IDC_CLEAR_PASSWORD_CTRL = 0;
static wxButton *IDOK_CTRL = 0;
static wxButton *IDCANCEL_CTRL = 0;
static wxRadioButton *IDC_RADIO_NET_CTRL = 0;
static wxRadioButton *IDC_RADIO_LAN_CTRL = 0;
static wxRadioButton *IDC_RADIO_FAV_CTRL = 0;

