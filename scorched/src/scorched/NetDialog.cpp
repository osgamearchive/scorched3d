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
#include <client/ServerBrowser.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include <windows.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/image.h>
#include "NetLan.cpp"

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
	if ((item != -1) && 
		(item < ServerBrowser::instance()->getServerList().getNoEntries()))
	{
		char *name = "";
		switch (column)
		{
		case 0: name = "servername"; break;
		case 1: name = "password"; break;
		case 2:
		{
			std::string clients = 
				ServerBrowser::instance()->getServerList().getEntryValue(item, "noplayers");
			std::string maxclients = 
				ServerBrowser::instance()->getServerList().getEntryValue(item, "maxplayers");
			static char text[256];
			sprintf(text, "%s/%s", clients.c_str(), maxclients.c_str());

			return text;
		}
		case 3: name = "address"; break;
		case 4: name = "fullversion"; break;
		case 5: name = "gametype"; break;
		}
		return ServerBrowser::instance()->getServerList().getEntryValue(item, name);
	}
	return "";
}

static NetListControl *IDC_SERVER_LIST_CTRL = 0;
static wxListCtrl *IDC_PLAYER_LIST_CTRL = 0;

static unsigned int invalidateId = 0;
extern char scorched3dAppName[128];

void refreshLANWindow(bool lanRefresh)
{
	ServerBrowser::instance()->refresh(lanRefresh);

	IDC_BUTTON_LAN_CTRL->Disable();
	IDC_BUTTON_NET_CTRL->Disable();
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
	void onClearPasswordButton();
	void onSelectServer();
	void onTimer();
	void onServerChanged();

private:
	DECLARE_EVENT_TABLE()
	wxTimer timer_;
};

BEGIN_EVENT_TABLE(NetLanFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_LAN,  NetLanFrame::onRefreshLanButton)
	EVT_BUTTON(IDC_BUTTON_NET,  NetLanFrame::onRefreshNETButton)
	EVT_BUTTON(IDC_CLEAR,  NetLanFrame::onClearButton)
	EVT_BUTTON(IDC_CLEAR_PASSWORD,  NetLanFrame::onClearPasswordButton)
	EVT_TIMER(1001, NetLanFrame::onTimer)
	EVT_LIST_ITEM_SELECTED(IDC_SERVER_LIST, NetLanFrame::onSelectServer)
	EVT_TEXT(IDC_EDIT_SERVER, NetLanFrame::onServerChanged)
END_EVENT_TABLE()

NetLanFrame::NetLanFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(542, 411))
{
	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	IDC_SERVER_LIST_CTRL = 
		new NetListControl(this, IDC_SERVER_LIST,
		wxPoint((int) 10.5, (int) 55.5), wxSize((int) 516, (int) 170.5));

	IDC_PLAYER_LIST_CTRL = 
		new wxListCtrl(this, IDC_PLAYER_LIST,
		wxPoint((int) 10.5, (int) 226.5), wxSize((int) 516, (int) 83),
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL );

	// Create a timer
	timer_.SetOwner(this, 1001);
	timer_.Start(3000, false);

	// Create all the display controlls
	createControls(this);
	onTimer();
}

void NetLanFrame::onClearButton()
{
	IDC_EDIT_SERVER_CTRL->SetValue("");
	onServerChanged();
}

void NetLanFrame::onClearPasswordButton()
{
	IDC_EDIT_PASSWORD_CTRL->SetValue("");
}

void NetLanFrame::onSelectServer()
{
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();

    long item = -1;
    for ( ;; )
    {
        item = IDC_SERVER_LIST_CTRL->GetNextItem(
			item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1) break;

		std::string text =
			ServerBrowser::instance()->getServerList().getEntryValue(item, "address");
		if (text.size())
		{
			std::string protocolVersion = 
				ServerBrowser::instance()->getServerList().getEntryValue(item, "protocolversion");
			std::string version =
				ServerBrowser::instance()->getServerList().getEntryValue(item, "version");
			if (protocolVersion.size() > 0 &&
				stricmp(protocolVersion.c_str(), ScorchedProtocolVersion) != 0)
			{
				dialogMessage("Scorched 3D", 
					"Warning: This server is running a incompatable version of Scorched3D.\n"
					"You cannot connect to this server.\n\n"
					"This server is running Scorched build %s (%s).\n"
					"You are running Scorched build %s (%s).\n\n"					
					"The latest version of Scorched3D can be downloaded from http://www.scorched3d.co.uk\n",
					version.c_str(), protocolVersion.c_str(),
					ScorchedVersion, ScorchedProtocolVersion);
			}
			IDC_EDIT_SERVER_CTRL->SetValue(text.c_str());

			int noplayers =
				atoi(ServerBrowser::instance()->getServerList().getEntryValue(item, "noplayers"));
			for (int i=0; i<noplayers; i++)
			{
				static char tmp[128];
				sprintf(tmp, "pn%i", i);
				long index = IDC_PLAYER_LIST_CTRL->InsertItem(0, 
					ServerBrowser::instance()->getServerList().getEntryValue(item, tmp));

				sprintf(tmp, "ps%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 1, 
					ServerBrowser::instance()->getServerList().getEntryValue(item, tmp));

				sprintf(tmp, "pt%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 2, 
					ServerBrowser::instance()->getServerList().getEntryValue(item, tmp));
			}
			
			onServerChanged();
		}
    }
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
	if (!ServerBrowser::instance()->getRefreshing())
	{
		IDC_BUTTON_LAN_CTRL->Enable();
		IDC_BUTTON_NET_CTRL->Enable();
	}
	else
	{
		IDC_BUTTON_LAN_CTRL->Disable();
		IDC_BUTTON_NET_CTRL->Disable();
	}

	if (invalidateId != ServerBrowser::instance()->getServerList().getRefreshId())
	{
		invalidateId = ServerBrowser::instance()->getServerList().getRefreshId();
		IDC_SERVER_LIST_CTRL->SetItemCount(
			ServerBrowser::instance()->getServerList().getNoEntries());
		IDC_PLAYER_LIST_CTRL->DeleteAllItems();
	}
}

bool NetLanFrame::TransferDataToWindow()
{
	// Set the ok button to disabled
	IDC_EDIT_SERVER_CTRL->SetValue(
		OptionsParam::instance()->getConnect());
	IDC_EDIT_PASSWORD_CTRL->SetValue(
		OptionsParam::instance()->getPassword());

	// Setup the server list control
	struct ListItem
	{
		char *name;
		int size;
	} mainListItems[] =
	{
		{ "Server Name", 200 },
		{ "Pswd", 40 },
		{ "Plyrs", 50 },
		{ "Server IP Address", 140 },
		{ "Version", 60 },
		{ "Game Type", 80 }
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
	OptionsParam::instance()->setConnect(
		IDC_EDIT_SERVER_CTRL->GetValue().c_str());
	OptionsParam::instance()->setPassword(
		IDC_EDIT_PASSWORD_CTRL->GetValue().c_str());
		
	return true;
}

bool showNetLanDialog()
{
	NetLanFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
