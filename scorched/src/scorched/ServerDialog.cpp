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


#include <scorched/ServerDialog.h>
#include <scorched/MainDialog.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <coms/NetBufferUtil.h>
#include <coms/ComsTextMessage.h>
#include <coms/ComsMessageSender.h>
#include <coms/NetServer.h>
#include <tank/TankContainer.h>
#include <engine/GameState.h>
#include <server/ServerState.h>
#include <server/ServerMain.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/listctrl.h>

enum 
{
	IDC_PLAYER_LIST = 550,
	IDC_LOG_LIST,
	IDC_TIMER1,
	IDC_TIMER2,
	IDC_MENU_EXIT,
	IDC_MENU_SHOWOPTIONS,
	IDC_MENU_PLAYERTALK,
	IDC_MENU_PLAYERTALKALL,
	IDC_MENU_PLAYERKICK
};

class ServerPlayerListControl : public wxListCtrl
{
public:
	ServerPlayerListControl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~ServerPlayerListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
};

ServerPlayerListControl::ServerPlayerListControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
	wxListCtrl(parent, id, pos, size, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_SINGLE_SEL )
{
}

ServerPlayerListControl::~ServerPlayerListControl()
{
}

int ServerPlayerListControl::OnGetItemImage(long item) const
{
	return -1;
}

wxString ServerPlayerListControl::OnGetItemText(long item, long column) const
{
	if ((item != -1) && (item < TankContainer::instance()->getNoOfTanks()))
	{
		Tank *tank = 
			TankContainer::instance()->getTankByPos((unsigned int) item);
		switch (column)
		{
		case 0:
			return (char *) tank->getName();
			break;
		case 1:
			{
				static char buffer[256];
				sprintf(buffer, "%u", tank->getPlayerId());
				return buffer;
			}
			break;
		case 2:
			return (char *) tank->getScore().getTimePlayedString();
			break;
		case 3:
			return (char *) tank->getScore().getScoreString();
			break;
		case 4:
			return (char *) tank->getState().getStateString();
			break;
		}
	}
	return "";
}

class ServerFrame: public wxFrame
{
public:
	ServerFrame(const char *name);

	void OnSize(wxSizeEvent& event);
	void onTimer();
	void onTimerMain();
	void onMenuExit();
	void onShowOptions();
	void onPlayerTalk();
	void onPlayerTalkAll();
	void onPlayerKick();

	ServerPlayerListControl *playerList_;
	wxListCtrl *logList_;
	wxStatusBar *statusBar_;

protected:
	wxTimer timer_;
	wxTimer timerMain_;

private:
    DECLARE_EVENT_TABLE()
};

static ServerFrame *frame = 0;

BEGIN_EVENT_TABLE(ServerFrame, wxFrame)
    EVT_SIZE(ServerFrame::OnSize)
	EVT_TIMER(IDC_TIMER1, ServerFrame::onTimer)
	EVT_TIMER(IDC_TIMER2, ServerFrame::onTimerMain)
	EVT_MENU(IDC_MENU_EXIT, ServerFrame::onMenuExit)
	EVT_MENU(IDC_MENU_SHOWOPTIONS, ServerFrame::onShowOptions)
	EVT_MENU(IDC_MENU_PLAYERTALK, ServerFrame::onPlayerTalk)
	EVT_MENU(IDC_MENU_PLAYERTALKALL, ServerFrame::onPlayerTalkAll)
	EVT_MENU(IDC_MENU_PLAYERKICK, ServerFrame::onPlayerKick)
END_EVENT_TABLE()

ServerFrame::ServerFrame(const char *name) :
	wxFrame(NULL, -1, name, wxPoint(0,0), wxSize(630, 470))
{
	CentreOnScreen();

	// Create timers
	timerMain_.SetOwner(this, IDC_TIMER2);
	timerMain_.Start(10, false);
	timer_.SetOwner(this, IDC_TIMER1);
	timer_.Start(3000, false);

	// Set the backbround color to be that of the default
	// panel colour
	wxPanel panel;
	SetBackgroundColour(panel.GetBackgroundColour());

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create all the display controlls
	playerList_ = 
		new ServerPlayerListControl(this, IDC_PLAYER_LIST,
		wxDefaultPosition, wxDefaultSize);
	logList_ =
		new wxListCtrl(this, IDC_LOG_LIST, wxDefaultPosition, wxDefaultSize, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL );

	// Setup list collumns
	// Setup the list control
	struct ListItem
	{
		char *name;
		int size;
	};
	int i;
	ListItem playerListItems[] =
		{
			{ "Player Name", 100 },
			{ "Player Id", 80 },
			{ "Played Time", 100 },
			{ "Player Score", 175 },
			{ "Player State", 175 }
		};
	for (i=0; i<sizeof(playerListItems)/sizeof(ListItem); i++)
	{
		playerList_->InsertColumn(
			i,
			playerListItems[i].name,
			wxLIST_FORMAT_LEFT,
			playerListItems[i].size);
	}
	ListItem mainListItems[] =
		{
			{ "Log Message", 600 }
		};
	for (i=0; i<sizeof(mainListItems)/sizeof(ListItem); i++)
	{
		logList_->InsertColumn(
			i,
			mainListItems[i].name,
			wxLIST_FORMAT_LEFT,
			mainListItems[i].size);
	}

	// Add menu items
	wxMenu *menuFile = new wxMenu;
    menuFile->Append(IDC_MENU_SHOWOPTIONS, "Display &Options");
    menuFile->AppendSeparator();
    menuFile->Append(IDC_MENU_EXIT, "E&xit");
	wxMenu *menuPlayer = new wxMenu;
	menuPlayer->Append(IDC_MENU_PLAYERTALK, "Talk to selected players");
	menuPlayer->Append(IDC_MENU_PLAYERTALKALL, "Talk to all players");
	menuPlayer->Append(IDC_MENU_PLAYERKICK, "Kick selected players");
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
	menuBar->Append(menuPlayer, "&Players");
    SetMenuBar( menuBar );

#ifdef __WXMSW__
	SendSizeEvent();
#endif

	// Create the bottom status bar
	statusBar_ = CreateStatusBar(4);
}

void ServerFrame::onMenuExit()
{
	Close();
}

void ServerFrame::OnSize(wxSizeEvent& event)
{
	wxSize size = GetClientSize();
	wxCoord y = (2*size.y)/3;
	logList_->SetSize(0, 0, size.x, y);
	playerList_->SetSize(0, y + 1, size.x, size.y - y);
	event.Skip();
}

void ServerFrame::onTimerMain()
{
	serverLoop();
}

void ServerFrame::onTimer()
{
	// Update the player list
	if (frame->playerList_->GetItemCount() !=
		TankContainer::instance()->getNoOfTanks())
	{
		frame->playerList_->SetItemCount(
			TankContainer::instance()->getNoOfTanks());
	}
	frame->playerList_->Refresh();

	// Update the status bar
	char buffer[256];
	std::map<unsigned int, Tank *> &tanks = 
		TankContainer::instance()->getPlayingTanks();
	sprintf(buffer, "%i/%i Players", tanks.size(), 
		OptionsGame::instance()->getNoMaxPlayers());
	frame->statusBar_->SetStatusText(buffer, 0);
	frame->statusBar_->SetStatusText(
		(GameState::instance()->getState() == ServerState::ServerStateWaitingForPlayers?"Not Playing":"Playing"), 1);
	sprintf(buffer, "Round %i/%i, %i/%i Moves",
		OptionsGame::instance()->getNoRounds() - OptionsTransient::instance()->getNoRoundsLeft(),
		OptionsGame::instance()->getNoRounds(),
		OptionsTransient::instance()->getCurrentGameNo(),
		OptionsGame::instance()->getNoMaxRoundTurns());
	frame->statusBar_->SetStatusText(buffer, 2);
	sprintf(buffer, "%i Bytes in, %i Bytes out",
		NetBufferUtil::getBytesIn(),
		NetBufferUtil::getBytesOut());
	frame->statusBar_->SetStatusText(buffer, 3);
}

void sendString(NetPlayerID dest, const char *fmt, ...)
{
	static char text[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	ComsTextMessage message(text);
	if (dest == 0)
	{
		ComsMessageSender::sendToAllConnectedClients(message);
	}
	else
	{
		ComsMessageSender::sendToSingleClient(message, dest);
	}
}

void kickPlayer(NetPlayerID id)
{
	unsigned int tankId = (unsigned int) id;
	Tank *tank = TankContainer::instance()->getTankById(tankId);
	if (tank)
	{
		sendString(0,
			"Player \"%s\" has been kicked from the server",
			tank->getName(), id);
		Logger::log(tank, "Kicking client \"%s\" \"%i\"", 
			tank->getName(), id);
	}
	else
	{
		sendString(0,
			"%i has been kicked from the server",
			id);
		Logger::log(0, "Kicking client \"%i\"", id);
	}

	NetServer::instance()->destroyClient(id);
}

void ServerFrame::onPlayerTalkAll()
{
	wxTextEntryDialog entryDialog(
		frame, "Message to players");
	if (entryDialog.ShowModal() == wxID_OK)
	{
		Logger::log(0, "Says \"%s\"", 
			entryDialog.GetValue().GetData());
		sendString(0, entryDialog.GetValue());
	}
}

void ServerFrame::onPlayerTalk()
{
	wxTextEntryDialog entryDialog(
		frame, "Message to players");
	if (entryDialog.ShowModal() == wxID_OK)
	{
		Logger::log(0, "Says \"%s\"", 
			entryDialog.GetValue().GetData());

		long item = -1;
		while ((item = frame->playerList_->GetNextItem(
			item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED)) != -1)
		{
			if ((item != -1) && (item < TankContainer::instance()->getNoOfTanks()))
			{
				Tank *tank = 
					TankContainer::instance()->getTankByPos((unsigned int) item);
				sendString((NetPlayerID)tank->getPlayerId(), entryDialog.GetValue());
			}
		}
	}
}

void ServerFrame::onPlayerKick()
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < TankContainer::instance()->getNoOfTanks()))
		{
			Tank *tank = 
				TankContainer::instance()->getTankByPos((unsigned int) item);
			kickPlayer((NetPlayerID)tank->getPlayerId());
		}		
    }
}

void ServerFrame::onShowOptions()
{
	char allOptionsStr[2048];
	allOptionsStr[0] = '\0';
	std::list<OptionEntry *> &allOptions = OptionsGame::instance()->getOptions();
	std::list<OptionEntry *>::iterator itor;
	for (itor = allOptions.begin();
		itor != allOptions.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		strcat(allOptionsStr, entry->getName());
		strcat(allOptionsStr, " = ");
		strcat(allOptionsStr, entry->getValueAsString());
		strcat(allOptionsStr, "\n");
	}
	dialogMessage("Scorched 3D Server Options", allOptionsStr);
}

static FileLogger serverFileLogger("ServerLog");
static class ServerLogger : public LoggerI
{
public:
	virtual void logMessage(
		const char *time,
		const char *message,
		Tank *source)
	{
		static char text[1024];
		sprintf(text, "%s - %s", time, message);
		long index = frame->logList_->InsertItem(0, text);

		if (frame->logList_->GetItemCount() > 100)
		{
			for (int j=0; j<20; j++)
			{
				frame->logList_->DeleteItem(0);
			}
		}
	};
} serverLogger;

void showServerDialog()
{
	char serverName[1024];
	sprintf(serverName, "Scorched 3D Dedicated Server [ %s : %i ] [%s]",
			OptionsGame::instance()->getServerName(), 
			OptionsGame::instance()->getPortNo(), 
			(OptionsGame::instance()->getPublishServer()?"Published":"Not Published"));

	frame = new ServerFrame(serverName);
	frame->Show();

	Logger::addLogger(&serverFileLogger);
	Logger::addLogger(&serverLogger);
}
