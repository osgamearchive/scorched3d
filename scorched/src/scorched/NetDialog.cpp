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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/utils.h>
#include <client/ServerBrowser.h>
#include <common/OptionsParam.h>
#include <common/Defines.h>
#include "NetLan.cpp"

static bool serverCompatable(std::string pversion, std::string version)
{
	if (pversion.size() > 0 && 
		0 != strcmp(pversion.c_str(), ScorchedProtocolVersion))
	{
		return false;
	}

	return true;
}

class NetListControl : public wxListCtrl
{
public:
	NetListControl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~NetListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
	
	void onDClickServer(wxMouseEvent&);
	
private:
	DECLARE_EVENT_TABLE()
};

NetListControl::NetListControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
	wxListCtrl(parent, id, pos, size, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_SINGLE_SEL )
{
	SetImageList(netLanImageList, wxIMAGE_LIST_SMALL);
	SetImageList(netLanImageList, wxIMAGE_LIST_NORMAL);
}

NetListControl::~NetListControl()
{
}

int NetListControl::OnGetItemImage(long item) const
{
	if ((item != -1) && 
		(item < ServerBrowser::instance()->getServerList().getNoEntries()))
	{
		std::string pversion =
			ServerBrowser::instance()->getServerList().
				getEntryValue(item, "protocolversion");
		std::string version =
			ServerBrowser::instance()->getServerList().
				getEntryValue(item, "version");
		if (!serverCompatable(pversion, version))
		{
			return 3;
		}

		std::string clients = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(item, "noplayers");
		std::string maxclients = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(item, "maxplayers");
		if (clients.size() > 0 &&
			0 == strcmp(clients.c_str(), maxclients.c_str()))
		{
			return 3;
		}
		
		std::string state = 
			ServerBrowser::instance()->getServerList().
				getEntryValue(item, "state");
		if (0 == strcmp(state.c_str(), "Waiting")) return 2;
		if (0 == strcmp(state.c_str(), "Started")) return 1;
	}
	return 0;
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
				ServerBrowser::instance()->getServerList().
					getEntryValue(item, "noplayers");
			std::string maxclients = 
				ServerBrowser::instance()->getServerList().
					getEntryValue(item, "maxplayers");
			char text[256];
			sprintf(text, "%s/%s", clients.c_str(), maxclients.c_str());

			return wxString(text, wxConvUTF8);
		}
		case 3: name = "round"; break;
		case 4: name = "mod"; break;
		case 5: name = "fullversion"; break;
		case 6: name = "gametype"; break;
		case 7: name = "os"; break;
		case 8: name = "address"; break;
		}
		return wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, name), wxConvUTF8);
	}
	return wxT("");
}

BEGIN_EVENT_TABLE(NetListControl, wxListCtrl)
	EVT_LEFT_DCLICK(NetListControl::onDClickServer)
END_EVENT_TABLE()

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

	void onJoinButton(wxCommandEvent &event);
	void onRefreshLanButton(wxCommandEvent &event);
	void onRefreshNETButton(wxCommandEvent &event);
	void onClearButton(wxCommandEvent &event);
	void onClearPasswordButton(wxCommandEvent &event);
	void onSelectServer(wxListEvent &event);
	void onTimer(wxTimerEvent &event);
	void onServerChanged(wxCommandEvent &event);

private:
	DECLARE_EVENT_TABLE()
	wxTimer timer_;
};

BEGIN_EVENT_TABLE(NetLanFrame, wxDialog)
	EVT_BUTTON(IDC_BUTTON_LAN,  NetLanFrame::onRefreshLanButton)
	EVT_BUTTON(IDC_BUTTON_NET,  NetLanFrame::onRefreshNETButton)
	EVT_BUTTON(IDC_BUTTON_JOIN,  NetLanFrame::onJoinButton)
	EVT_BUTTON(IDC_CLEAR,  NetLanFrame::onClearButton)
	EVT_BUTTON(IDC_CLEAR_PASSWORD,  NetLanFrame::onClearPasswordButton)
	EVT_TIMER(1001, NetLanFrame::onTimer)
	EVT_LIST_ITEM_SELECTED(IDC_SERVER_LIST, NetLanFrame::onSelectServer)
	EVT_TEXT(IDC_EDIT_SERVER, NetLanFrame::onServerChanged)
END_EVENT_TABLE()

NetLanFrame::NetLanFrame() :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8),
		wxDefaultPosition, wxDefaultSize,
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controls
	createControlsPre(this, topsizer);

	IDC_SERVER_LIST_CTRL = 
		new NetListControl(this, IDC_SERVER_LIST,
		wxDefaultPosition, wxSize(550, 170));
	topsizer->Add(IDC_SERVER_LIST_CTRL, 3, wxALL | wxGROW, 5);

	IDC_PLAYER_LIST_CTRL = 
		new wxListCtrl(this, IDC_PLAYER_LIST,
		wxDefaultPosition, wxSize(550, 100),
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL );
	topsizer->Add(IDC_PLAYER_LIST_CTRL, 2, wxALL | wxGROW, 5);

	// Create all the display controls
	createControlsPost(this, topsizer);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();

	// Create a timer
	timer_.SetOwner(this, 1001);
	timer_.Start(3000, false);
	wxTimerEvent timerEvent;
	onTimer(timerEvent);
}

static void onJoinButtonExec(NetLanFrame *frame)
{
	wxString value = IDC_EDIT_SERVER_CTRL->GetValue();
	wxString password = IDC_EDIT_PASSWORD_CTRL->GetValue();
	wxString username = IDC_EDIT_NAME_CTRL->GetValue();

	std::string hostPart;
	const char *host = value.mb_str(wxConvUTF8);
	char *colon = strchr(host, ':');
	if (colon) 
	{
		*colon = '\0';
		hostPart = host;
		*colon = ':';
	}
	else
	{
		hostPart = host;
	}
	
	if (!password.empty() && !username.empty())
	{
		IPaddress officialaddress;
		IPaddress thisaddress;
		if (SDLNet_ResolveHost(&officialaddress, "scorched3d.game-host.org", 0) == 0 &&
			SDLNet_ResolveHost(&thisaddress, (char *) hostPart.c_str(), 0) == 0)
		{
			if (officialaddress.host != thisaddress.host)
			{
				wxString wxText(wxT("Warning: You are about to send username and password\n")
					wxT("information to a non-official server.\n")
					wxT("\n")
					wxT("Do you wish to continue?"));
				wxString wxHeader(wxT("Scorched3D"));
				int answer = ::wxMessageBox(wxText, wxHeader, wxYES_NO | wxCENTRE);
				if (answer != wxYES) return;
			}
		}
	}

	if (hostPart.c_str()[0])
	{
		frame->EndModal(wxID_OK);
	}
}

void NetLanFrame::onJoinButton(wxCommandEvent &event)
{
	onJoinButtonExec(this);
}

void NetLanFrame::onClearButton(wxCommandEvent &event)
{
	IDC_EDIT_SERVER_CTRL->SetValue(wxT(""));
	onServerChanged(event);
}

void NetLanFrame::onClearPasswordButton(wxCommandEvent &event)
{
	IDC_EDIT_PASSWORD_CTRL->SetValue(wxT(""));
}

void NetLanFrame::onSelectServer(wxListEvent &event)
{
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();
	wxCommandEvent ev;
	onClearButton(ev);

    long item = -1;
    for ( ;; )
    {
        item = IDC_SERVER_LIST_CTRL->GetNextItem(
			item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (item == -1 || 
			item >= ServerBrowser::instance()->getServerList().getNoEntries()) break;

		std::string text =
			ServerBrowser::instance()->getServerList().getEntryValue(item, "address");
		if (text.size())
		{
			std::string protocolVersion = 
				ServerBrowser::instance()->getServerList().getEntryValue(item, "protocolversion");
			std::string version =
				ServerBrowser::instance()->getServerList().getEntryValue(item, "version");
			if (!serverCompatable(protocolVersion, version))
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
			IDC_EDIT_SERVER_CTRL->SetValue(wxString(text.c_str(), wxConvUTF8));

			int noplayers =
				atoi(ServerBrowser::instance()->getServerList().getEntryValue(item, "noplayers"));
			for (int i=0; i<noplayers; i++)
			{
				static char tmp[128];
				sprintf(tmp, "pn%i", i);
				long index = IDC_PLAYER_LIST_CTRL->InsertItem(0, 
					wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, tmp), wxConvUTF8));

				sprintf(tmp, "ps%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 1, 
					wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, tmp), wxConvUTF8));

				sprintf(tmp, "pt%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 2, 
					wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, tmp), wxConvUTF8));

				sprintf(tmp, "pm%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 3, 
					wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, tmp), wxConvUTF8));

				sprintf(tmp, "pr%i", i);
				IDC_PLAYER_LIST_CTRL->
					SetItem(index, 4, 
					wxString(ServerBrowser::instance()->getServerList().getEntryValue(item, tmp), wxConvUTF8));
			}
		
			wxCommandEvent event;	
			onServerChanged(event);
		}
    }
}

void NetLanFrame::onRefreshLanButton(wxCommandEvent &event)
{
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();
	refreshLANWindow(true);
}

void NetLanFrame::onRefreshNETButton(wxCommandEvent &event)
{
	IDC_PLAYER_LIST_CTRL->DeleteAllItems();
	refreshLANWindow(false);
}

void NetLanFrame::onServerChanged(wxCommandEvent &event)
{
	wxString value = IDC_EDIT_SERVER_CTRL->GetValue();
	bool enabled = !value.empty();
	IDOK_CTRL->Enable(enabled);
	IDOK_CTRL->SetDefault();
}

void NetLanFrame::onTimer(wxTimerEvent &event)
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
		//IDC_PLAYER_LIST_CTRL->DeleteAllItems();
	}
}

bool NetLanFrame::TransferDataToWindow()
{
	// Set the ok button to disabled
	IDC_EDIT_SERVER_CTRL->SetValue(
		wxString(OptionsParam::instance()->getConnect(), wxConvUTF8));
	IDC_EDIT_NAME_CTRL->SetValue(
		wxString(OptionsParam::instance()->getUserName(), wxConvUTF8));
	IDC_EDIT_PASSWORD_CTRL->SetValue(
		wxString(OptionsParam::instance()->getPassword(), wxConvUTF8));

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
		{ "Round", 50 },
		{ "Mod", 70 },
		{ "Ver", 50 },
		{ "Game Type", 160 },
		{ "OS", 160 },
		{ "Server IP Address", 140 }
	};
	for (int i=0; i<sizeof(mainListItems)/sizeof(ListItem); i++)
	{
		IDC_SERVER_LIST_CTRL->InsertColumn(
			i,
			wxString(mainListItems[i].name, wxConvUTF8),
			wxLIST_FORMAT_LEFT,
			mainListItems[i].size);
	}

	// Setup the player list control	
	ListItem playerListItems[] =
	{
		{ "Player Name", 200 },
		{ "Player Score", 100 },
		{ "Player Time", 100 },
		{ "Player Team", 80 },
		{ "Player Rank", 80 }
	};
	for (int i=0; i<sizeof(playerListItems)/sizeof(ListItem); i++)
	{
		IDC_PLAYER_LIST_CTRL->InsertColumn(
			i,
			wxString(playerListItems[i].name, wxConvUTF8),
			wxLIST_FORMAT_LEFT,
			playerListItems[i].size);
	}

	wxCommandEvent evt;
	onServerChanged(evt);
	return true;
}

bool NetLanFrame::TransferDataFromWindow()
{
	return true;
}

void NetListControl::onDClickServer(wxMouseEvent& event)
{
	wxString value = IDC_EDIT_SERVER_CTRL->GetValue();
	bool enabled = !value.empty();
	if (enabled)
	{
		NetLanFrame *parent = (NetLanFrame *) GetParent();
		onJoinButtonExec(parent);
	}
}

bool showNetLanDialog()
{
	NetLanFrame frame;
	if (frame.ShowModal() == wxID_OK)
	{
		wxString value = IDC_EDIT_SERVER_CTRL->GetValue();
		wxString password = IDC_EDIT_PASSWORD_CTRL->GetValue();
		wxString username = IDC_EDIT_NAME_CTRL->GetValue();

		char buffer[1024];
		sprintf(buffer, "-connect \"%s\"", (const char *) value.mb_str(wxConvUTF8));

		if (!password.empty())
		{
			strcat(buffer, " -password ");
			strcat(buffer, password.mb_str(wxConvUTF8));
		}
		if (!username.empty())
		{
			strcat(buffer, " -username ");
			strcat(buffer, username.mb_str(wxConvUTF8));
		}
		runScorched3D(buffer);
	}
	return false;
}
