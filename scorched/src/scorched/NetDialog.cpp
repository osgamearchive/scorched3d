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


#include <scorched/NetDialog.h>
#include <scorched/MainDialog.h>
#include <windows.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/image.h>
#include "NetLan.cpp"
#ifndef _NO_SERVER_ASE_
extern "C" {
#include <ASE/ingame.h>
}
#endif

class NetListControl : public wxListCtrl
{
public:
	NetListControl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~NetListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
};

NetListControl::NetListControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
	wxListCtrl(parent, id, pos, size, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_SINGLE_SEL )
{
}

NetListControl::~NetListControl()
{
}

int NetListControl::OnGetItemImage(long item) const
{
	return -1;
}

wxString NetListControl::OnGetItemText(long item, long column) const
{
#ifndef _NO_SERVER_ASE_
	if ((item != -1) && (item < ASEBrowser_servers))
	{
		if (column == 2)
		{
			char *clients = ASEBrowser_getinfo(item, "clients");
			char *maxclients = ASEBrowser_getinfo(item, "maxclients");
			static char text[256];
			sprintf(text, "%s/%s", clients, maxclients);

			return text;
		}
		else
		{
			char *name = "ping";
			switch (column)
			{
			case 0: name = "hostname"; break;
			case 1: name = "ping"; break;
			case 2: name = "clients"; break;
			case 3: name = "address"; break;
			case 4: name = "version"; break;
			}
			return ASEBrowser_getinfo(item, name);
		}
	}
#endif
	return "";
}

static NetListControl *IDC_SERVER_LIST_CTRL = 0;
static wxListCtrl *IDC_PLAYER_LIST_CTRL = 0;

static bool invalidate = false;
extern char scorched3dAppName[128];

void redrawListView()
{
#ifndef _NO_SERVER_ASE_
	invalidate = false;
	IDC_SERVER_LIST_CTRL->SetItemCount(ASEBrowser_servers);
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();
#endif
}

void ASEBrowser_invalidate(void)
{
	invalidate = true;
}

void ASEBrowser_progress(void)
{
#ifndef _NO_SERVER_ASE_
	IDC_PROGRESS1_CTRL->SetValue(ASEBrowser_progresspos);
#endif
}

void refreshLANWindow(bool lanRefresh)
{
#ifndef _NO_SERVER_ASE_
	static bool init = false;
	if (!init)
	{
		WORD WSAVerReq = (MAKEWORD((1),(1)));
		WSADATA WSAData;
		if (WSAStartup(WSAVerReq, &WSAData) != 0)
		{
			dialogMessage("Scorched 3D LAN/NET", 
				"Failed to initialize windows sockets");
			return;
		}

		if (ASEBrowser_init()==0)
		{
			dialogMessage("Scorched 3D LAN/NET", 
				"Failed to initialize the browser");
			return;
		}

		ASEBrowser_setconfig(28800, 57600, 40, 59);
		init = true;
	}

	if (lanRefresh)
	{
		if (ASEBrowser_refreshlan(ScorchedPort) == 0)
		{
			dialogMessage("Scorched 3D LAN/NET", 
				"Failed to initialize this request");
			return;
		}
	}
	else
	{
		if (ASEBrowser_refresh("") == 0)
		{
			dialogMessage("Scorched 3D LAN/NET", 
				"Failed to initialize this request");
			return;
		}
	}

	IDC_BUTTON_LAN_CTRL->Disable();
	IDC_BUTTON_NET_CTRL->Disable();
#endif
}

class NetLanFrame: public wxDialog
{
public:
	NetLanFrame();

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onRefreshLanButton();
	void onRefreshNETButton();
	void onClearButton();
	void onSelectServer();
	void onTimer();
	void onServerChanged();
	void onEyeClick();

private:
	DECLARE_EVENT_TABLE()
	wxTimer timer_;
};

BEGIN_EVENT_TABLE(NetLanFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_LAN,  NetLanFrame::onRefreshLanButton)
	EVT_BUTTON(IDC_BUTTON_NET,  NetLanFrame::onRefreshNETButton)
	EVT_BUTTON(IDC_CLEAR,  NetLanFrame::onClearButton)
	EVT_BUTTON(IDC_BUTTON_EYE, NetLanFrame::onEyeClick)
	EVT_BUTTON(IDC_BUTTON_EYE2, NetLanFrame::onEyeClick)
	EVT_TIMER(1001, NetLanFrame::onTimer)
	EVT_LIST_ITEM_SELECTED(IDC_SERVER_LIST, NetLanFrame::onSelectServer)
	EVT_TEXT(IDC_EDIT_SERVER, NetLanFrame::onServerChanged)
END_EVENT_TABLE()

NetLanFrame::NetLanFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(542, 465))
{
	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	IDC_SERVER_LIST_CTRL = 
		new NetListControl(this, IDC_SERVER_LIST,
		wxPoint((int) 10.5, (int) 109.5), wxSize((int) 516, (int) 170.5));

	IDC_PLAYER_LIST_CTRL = 
		new wxListCtrl(this, IDC_PLAYER_LIST,
		wxPoint((int) 10.5, (int) 280.5), wxSize((int) 516, (int) 83),
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL );

	// Create a timer
	timer_.SetOwner(this, 1001);
	timer_.Start(3000, false);

	// Create all the display controlls
	createControls(this);

	ID_EYEURL->SetForegroundColour(wxColour(0, 0, 255));
}

void NetLanFrame::onEyeClick()
{
#ifdef _WIN32
	WinExec("explorer http://www.udpsoft.com/eye", SW_SHOWDEFAULT);
#endif
}

void NetLanFrame::onClearButton()
{
	IDC_EDIT_SERVER_CTRL->SetValue("");
	onServerChanged();
}

void NetLanFrame::onSelectServer()
{
#ifndef _NO_SERVER_ASE_
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();

    long item = -1;
    for ( ;; )
    {
        item = IDC_SERVER_LIST_CTRL->GetNextItem(
			item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1) break;

		char *text = ASEBrowser_getinfo(item, "address");
		if (text)
		{
			char *protocolVersion = ASEBrowser_getinfo(item, "ProtocolVersion");
			char *version = ASEBrowser_getinfo(item, "version");
			if (!protocolVersion || (stricmp(protocolVersion, ScorchedProtocolVersion) != 0))
			{
				dialogMessage("Scorched 3D", 
					"Warning: This server is running a incompatable version of Scorched3D.\n"
					"You cannot connect to this server.\n\n"
					"This server is running Scorched build %s.\n"
					"You are running Scorched build %s (%s).\n\n"					
					"The latest version of Scorched3D can be downloaded from http://www.scorched3d.co.uk\n",
					version?version:"??",
					ScorchedVersion, ScorchedProtocolVersion);
			}
			IDC_EDIT_SERVER_CTRL->SetValue(text);

			if (ASEBrowser_getfirstplayer(item))
			{
				do
				{
					long index = IDC_PLAYER_LIST_CTRL->InsertItem(0, 
						ASEBrowser_getplayerinfo("name"));
					IDC_PLAYER_LIST_CTRL->
						SetItem(index, 1, ASEBrowser_getplayerinfo("score"));
					IDC_PLAYER_LIST_CTRL->
						SetItem(index, 2, ASEBrowser_getplayerinfo("time"));
				} while (ASEBrowser_getnextplayer());
			}
			
			onServerChanged();
		}
    }
#endif
}

void NetLanFrame::onRefreshLanButton()
{
	refreshLANWindow(true);
}

void NetLanFrame::onRefreshNETButton()
{
	refreshLANWindow(false);
}

void NetLanFrame::onServerChanged()
{
	bool enabled = IDC_EDIT_SERVER_CTRL->GetValue().c_str()[0] != '\0';
	IDOK_CTRL->Enable(enabled);
	IDOK_CTRL->SetDefault();
}

void NetLanFrame::onTimer()
{
#ifndef _NO_SERVER_ASE_
	if (!ASEBrowser_pinging)
	{
		IDC_BUTTON_LAN_CTRL->Enable();
		IDC_BUTTON_NET_CTRL->Enable();
	}
	if (invalidate)
	{
		redrawListView();
	}
#endif
}

bool NetLanFrame::TransferDataToWindow()
{
	// Set the ok button to disabled
	IDC_EDIT_SERVER_CTRL->SetValue(OptionsParam::instance()->getConnect());
	IDC_PROGRESS1_CTRL->SetRange(4096);
	
	// Disable server browsing on non-ase capable machines
#ifdef _NO_SERVER_ASE_
	IDC_BUTTON_LAN_CTRL->Disable();
	IDC_BUTTON_NET_CTRL->Disable();
#endif

	// Setup the server list control
	struct ListItem
	{
		char *name;
		int size;
	} mainListItems[] =
	{
		{ "Server Name", 200 },
		{ "Ping", 40 },
		{ "Plyrs", 50 },
		{ "Server IP Address", 140 },
		{ "Version", 60 }
	};
	for (int i=0; i<sizeof(mainListItems)/sizeof(ListItem); i++)
	{
		IDC_SERVER_LIST_CTRL->InsertColumn(
			i,
			mainListItems[i].name,
			wxLIST_FORMAT_LEFT,
			mainListItems[i].size);
	}

	// Setup the player list control	
	ListItem playerListItems[] =
	{
		{ "Player Name", 200 },
		{ "Player Score", 200 },
		{ "Player Time", 100 }
	};
	for (int i=0; i<sizeof(playerListItems)/sizeof(ListItem); i++)
	{
		IDC_PLAYER_LIST_CTRL->InsertColumn(
			i,
			playerListItems[i].name,
			wxLIST_FORMAT_LEFT,
			playerListItems[i].size);
	}

	onServerChanged();
	return true;
}

bool NetLanFrame::TransferDataFromWindow()
{
#ifndef _NO_SERVER_ASE_
	ASEBrowser_cancel();
#endif

	OptionsParam::instance()->setConnect(
		IDC_EDIT_SERVER_CTRL->GetValue().c_str());
	
	return true;
}

bool showNetLanDialog()
{
	NetLanFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
