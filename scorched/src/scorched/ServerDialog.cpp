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
#include <scorched/SettingsDialog.h>
#include <scorched/ListDialog.h>
#include <tankai/TankAIStore.h>
#include <tankai/TankAIAdder.h>
#include <tank/TankContainer.h>
#include <tank/TankColorGenerator.h>
#include <engine/ModFiles.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <common/StatsLogger.h>
#include <common/Defines.h>
#include <coms/NetBufferUtil.h>
#include <coms/NetInterface.h>
#include <server/ServerState.h>
#include <server/ServerMain.h>
#include <server/ServerMessageHandler.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerCommon.h>
#include <server/ServerLog.h>
#include <server/ServerTooFewPlayersStimulus.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>

enum 
{
	IDC_PLAYER_LIST = 550,
	IDC_LOG_LIST,
	IDC_TIMER1,
	IDC_TIMER2,
	IDC_MENU_EXIT,
	IDC_MENU_SHOWMODFILES,
	IDC_MENU_SHOWBANNED,
	IDC_MENU_SHOWOPTIONS,
	IDC_MENU_EDITOPTIONS,
	IDC_MENU_SAVEOPTIONS,
	IDC_MENU_LOADOPTIONS,
	IDC_MENU_TIMEDMSG,
	IDC_MENU_PLAYERTALK,
	IDC_MENU_PLAYERTALKALL,
	IDC_MENU_PLAYERKICK,
	IDC_MENU_PLAYERSLAP25,
	IDC_MENU_PLAYERKILLALL,
	IDC_MENU_PLAYERBAN,
	IDC_MENU_PLAYERPERMMUTE,
	IDC_MENU_PLAYERUNPERMMUTE,
	IDC_MENU_PLAYERMUTE,
	IDC_MENU_PLAYERUNMUTE,
	IDC_MENU_PLAYERADD,
	IDC_MENU_PLAYERADD_1,
	IDC_MENU_PLAYERADD_2,
	IDC_MENU_PLAYERADD_3,
	IDC_MENU_PLAYERADD_4,
	IDC_MENU_PLAYERADD_5,
	IDC_MENU_PLAYERADD_6,
	IDC_MENU_PLAYERADD_7,
	IDC_MENU_PLAYERADD_8,
	IDC_MENU_PLAYERADD_9,
	IDC_MENU_STARTNEWGAME,
	IDC_MENU_COMSMESSAGELOGGING,
	IDC_MENU_STATELOGGING
};

class ServerPlayerListControl : public wxListCtrl
{
public:
	ServerPlayerListControl(wxWindow* parent, wxWindowID id);
	virtual ~ServerPlayerListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
};

ServerPlayerListControl::ServerPlayerListControl(wxWindow* parent, wxWindowID id) :
	wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, 
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
	if ((item != -1) && (item < ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
	{
		Tank *tank = 
			ScorchedServer::instance()->getTankContainer().getTankByPos((unsigned int) item);
		switch (column)
		{
		case 0:
			return wxString((char *) tank->getName(), wxConvUTF8);
			break;
		case 1:
			{
				const char *result = "Human";
				if (tank->getTankAI())
				{
					result = tank->getTankAI()->getName();
				}

				return wxString(result, wxConvUTF8);
			}
			break;
		case 2:
			{
				char buffer[256];
				snprintf(buffer, 256, "dest=%i ip=%s id=%i", 
					tank->getDestinationId(), 
					NetInterface::getIpName(tank->getIpAddress()), 
					tank->getPlayerId());
				return wxString(buffer, wxConvUTF8);
			}
			break;
		case 3:
			return wxString((char *) tank->getScore().getTimePlayedString(), wxConvUTF8);
			break;
		case 4:
			return wxString((char *) tank->getScore().getScoreString(), wxConvUTF8);
			break;
		case 5:
			return wxString((char *) tank->getState().getStateString(), wxConvUTF8);
			break;
		case 6:
			return wxString((char *) TankColorGenerator::getTeamName(tank->getTeam()), wxConvUTF8);
			break;
		case 7:
			return wxString(tank->getHostDesc(), wxConvUTF8);
			break;
		case 8:
			return wxString(tank->getScore().getStatsRank(), wxConvUTF8);
			break;
		}
	}
	return wxT("");
}

class ServerLogListControl : public wxListCtrl
{
public:
	ServerLogListControl(wxWindow* parent, wxWindowID id);
	virtual ~ServerLogListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
};

ServerLogListControl::ServerLogListControl(wxWindow* parent, wxWindowID id) :
	wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, 
		wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_SINGLE_SEL )
{
}

ServerLogListControl::~ServerLogListControl()
{
}

int ServerLogListControl::OnGetItemImage(long item) const
{
	return -1;
}

wxString ServerLogListControl::OnGetItemText(long item, long column) const
{
	if ((item != -1) && (item < (long) ServerLog::instance()->getEntries().size()))
	{
		return wxString(ServerLog::instance()->getEntries()[item].text.c_str(), wxConvUTF8);
	}
	return wxT("");
}

class ServerFrame: public wxFrame
{
public:
	ServerFrame(const char *name);

	void OnSize(wxSizeEvent& event);
	void OnClose(wxCloseEvent& event);
	void onTimer(wxTimerEvent &event);
	void onTimerMain(wxTimerEvent &event);
	void onMenuExit(wxCommandEvent &event);
	void onShowOptions(wxCommandEvent &event);
	void onShowModFiles(wxCommandEvent &event);
	void onShowBanned(wxCommandEvent &event);
	void onEditOptions(wxCommandEvent &event);
	void onLoadOptions(wxCommandEvent &event);
	void onSaveOptions(wxCommandEvent &event);
	void onPlayerTalk(wxCommandEvent &event);
	void onPlayerTalkAll(wxCommandEvent &event);
	void onPlayerKick(wxCommandEvent &event);
	void onPlayerSlap25(wxCommandEvent &event);
	void onKillAll(wxCommandEvent &event);
	void onStartNewGame(wxCommandEvent &event);
	void onPlayerBan(wxCommandEvent &event);
	void onPlayerPermMute(wxCommandEvent &event);
	void onPlayerUnPermMute(wxCommandEvent &event);
	void onPlayerMute(wxCommandEvent &event);
	void onPlayerUnMute(wxCommandEvent &event);
	void onPlayerAdd(int i);
	void onPlayerAdd1(wxCommandEvent &event);
	void onPlayerAdd2(wxCommandEvent &event);
	void onPlayerAdd3(wxCommandEvent &event);
	void onPlayerAdd4(wxCommandEvent &event);
	void onPlayerAdd5(wxCommandEvent &event);
	void onPlayerAdd6(wxCommandEvent &event);
	void onPlayerAdd7(wxCommandEvent &event);
	void onPlayerAdd8(wxCommandEvent &event);
	void onPlayerAdd9(wxCommandEvent &event);
	void onComsMessageLogging(wxCommandEvent &event);
	void onStateLogging(wxCommandEvent &event);

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
	EVT_CLOSE(ServerFrame::OnClose) 
	EVT_TIMER(IDC_TIMER1, ServerFrame::onTimer)
	EVT_TIMER(IDC_TIMER2, ServerFrame::onTimerMain)
	EVT_MENU(IDC_MENU_EXIT, ServerFrame::onMenuExit)
	EVT_MENU(IDC_MENU_SHOWMODFILES, ServerFrame::onShowModFiles)
	EVT_MENU(IDC_MENU_SHOWBANNED, ServerFrame::onShowBanned)
	EVT_MENU(IDC_MENU_SHOWOPTIONS, ServerFrame::onShowOptions)
	EVT_MENU(IDC_MENU_EDITOPTIONS, ServerFrame::onEditOptions)
	EVT_MENU(IDC_MENU_LOADOPTIONS, ServerFrame::onLoadOptions)
	EVT_MENU(IDC_MENU_SAVEOPTIONS, ServerFrame::onSaveOptions)
	EVT_MENU(IDC_MENU_PLAYERBAN, ServerFrame::onPlayerBan)
	EVT_MENU(IDC_MENU_PLAYERPERMMUTE, ServerFrame::onPlayerPermMute)
	EVT_MENU(IDC_MENU_PLAYERUNPERMMUTE, ServerFrame::onPlayerUnPermMute)
	EVT_MENU(IDC_MENU_PLAYERTALK, ServerFrame::onPlayerTalk)
	EVT_MENU(IDC_MENU_PLAYERTALKALL, ServerFrame::onPlayerTalkAll)
	EVT_MENU(IDC_MENU_PLAYERKICK, ServerFrame::onPlayerKick)
	EVT_MENU(IDC_MENU_PLAYERMUTE, ServerFrame::onPlayerMute)
	EVT_MENU(IDC_MENU_PLAYERUNMUTE, ServerFrame::onPlayerUnMute)
	EVT_MENU(IDC_MENU_PLAYERSLAP25, ServerFrame::onPlayerSlap25)
	EVT_MENU(IDC_MENU_PLAYERADD_1, ServerFrame::onPlayerAdd1)
	EVT_MENU(IDC_MENU_PLAYERADD_2, ServerFrame::onPlayerAdd2)
	EVT_MENU(IDC_MENU_PLAYERADD_3, ServerFrame::onPlayerAdd3)
	EVT_MENU(IDC_MENU_PLAYERADD_4, ServerFrame::onPlayerAdd4)
	EVT_MENU(IDC_MENU_PLAYERADD_5, ServerFrame::onPlayerAdd5)
	EVT_MENU(IDC_MENU_PLAYERADD_6, ServerFrame::onPlayerAdd6)
	EVT_MENU(IDC_MENU_PLAYERADD_7, ServerFrame::onPlayerAdd7)
	EVT_MENU(IDC_MENU_PLAYERADD_8, ServerFrame::onPlayerAdd8)
	EVT_MENU(IDC_MENU_PLAYERADD_9, ServerFrame::onPlayerAdd9)
	EVT_MENU(IDC_MENU_PLAYERKILLALL, ServerFrame::onKillAll)
	EVT_MENU(IDC_MENU_STARTNEWGAME, ServerFrame::onStartNewGame)
	EVT_MENU(IDC_MENU_COMSMESSAGELOGGING, ServerFrame::onComsMessageLogging)
	EVT_MENU(IDC_MENU_STATELOGGING, ServerFrame::onStateLogging)
END_EVENT_TABLE()

ServerFrame::ServerFrame(const char *name) :
	wxFrame(NULL, -1, wxString(name, wxConvUTF8), wxPoint(0,0), wxSize(630, 470))
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
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create all the display controlls
	playerList_ = 
		new ServerPlayerListControl(this, IDC_PLAYER_LIST);
	logList_ =
		new ServerLogListControl(this, IDC_LOG_LIST);

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
			{ "Player Type", 80 },
			{ "Player Id", 80 },
			{ "Played Time", 100 },
			{ "Player Score", 100 },
			{ "Player State", 140 },
			{ "Player Team", 100 },
			{ "Player OS", 140 },
			{ "Player Rank", 100 }
		};
	for (i=0; i<sizeof(playerListItems)/sizeof(ListItem); i++)
	{
		playerList_->InsertColumn(
			i,
			wxString(playerListItems[i].name, wxConvUTF8),
			wxLIST_FORMAT_LEFT,
			playerListItems[i].size);
	}
	ListItem mainListItems[] =
		{
			{ "Server Log Messages", 600 }
		};
	for (i=0; i<sizeof(mainListItems)/sizeof(ListItem); i++)
	{
		logList_->InsertColumn(
			i,
			wxString(mainListItems[i].name, wxConvUTF8),
			wxLIST_FORMAT_LEFT,
			mainListItems[i].size);
	}

	// Add menu items
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(IDC_MENU_COMSMESSAGELOGGING, wxT("Toggle Coms Messa&ge logging"));
	menuFile->Append(IDC_MENU_STATELOGGING, wxT("Toggle S&tate Logging"));
 	menuFile->AppendSeparator();
 	menuFile->Append(IDC_MENU_EXIT, wxT("E&xit"));

	wxMenu *menuOptions = new wxMenu;
	menuOptions->Append(IDC_MENU_SHOWOPTIONS, wxT("&Display Options"));
	menuOptions->Append(IDC_MENU_EDITOPTIONS, wxT("&Edit Options"));
	menuOptions->Append(IDC_MENU_LOADOPTIONS, wxT("&Load Options"));
	menuOptions->Append(IDC_MENU_SAVEOPTIONS, wxT("&Save Options"));
	menuOptions->AppendSeparator();
	menuOptions->Append(IDC_MENU_SHOWMODFILES, wxT("Show &Mod Files"));

	wxMenu *menuAdmin = new wxMenu;
	menuAdmin->Append(IDC_MENU_PLAYERKICK, wxT("Kick selected players"));
	menuAdmin->Append(IDC_MENU_PLAYERBAN, wxT("Ban selected players"));
	menuAdmin->Append(IDC_MENU_PLAYERPERMMUTE, wxT("Perminantly mutes selected players"));
	menuAdmin->Append(IDC_MENU_PLAYERUNPERMMUTE, wxT("Un-perminantly mutes selected players"));
	menuAdmin->Append(IDC_MENU_PLAYERSLAP25, wxT("Slap selected players (25 pts)"));
	menuAdmin->Append(IDC_MENU_PLAYERMUTE, wxT("Mute selected players"));
	menuAdmin->Append(IDC_MENU_PLAYERUNMUTE, wxT("Unmute selected players"));
	menuAdmin->AppendSeparator();
	menuAdmin->Append(IDC_MENU_SHOWBANNED, wxT("Show &Banned Users"));
	
	wxMenu *menuChat = new wxMenu;
	menuChat->Append(IDC_MENU_PLAYERTALK, wxT("Talk to selected players"));
	menuChat->Append(IDC_MENU_PLAYERTALKALL, wxT("Talk to all players"));

	wxMenu *menuAddPlayer = new wxMenu;
	int aicount = 0;
	std::list<TankAI *> &ais = ScorchedServer::instance()->getTankAIs().getAis();
	std::list<TankAI *>::iterator aiitor;
	for (aiitor = ais.begin();
		aiitor != ais.end();
		aiitor++, aicount++)
	{
		TankAI *ai = (*aiitor);
		char buffer[256];
		snprintf(buffer, 256, "Add %s", ai->getName());
		menuAddPlayer->Append(IDC_MENU_PLAYERADD_1 + aicount, wxString(buffer, wxConvUTF8));
	}

	wxMenu *menuPlayer = new wxMenu;
	menuPlayer->Append(IDC_MENU_STARTNEWGAME, wxT("Start a new game"));
	menuPlayer->Append(IDC_MENU_PLAYERKILLALL, wxT("Kill all players"));
	menuPlayer->Append(IDC_MENU_PLAYERADD, wxT("Add a new player"), menuAddPlayer);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, wxT("&File"));
	menuBar->Append(menuOptions, wxT("&Options"));
	menuBar->Append(menuAdmin, wxT("&Admin"));
	menuBar->Append(menuChat, wxT("&Chat"));
	menuBar->Append(menuPlayer, wxT("&Players"));
    SetMenuBar( menuBar );

#ifdef __WXMSW__
	SendSizeEvent();
#endif

	// Create the bottom status bar
	statusBar_ = CreateStatusBar(4);
}

void ServerFrame::onPlayerAdd(int i)
{
	if (ScorchedServer::instance()->getTankContainer().getNoOfTanks() <
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers())
	{
		int aicount = 1;
		std::list<TankAI *> &ais = ScorchedServer::instance()->getTankAIs().getAis();
		std::list<TankAI *>::iterator aiitor;
		for (aiitor = ais.begin();
			aiitor != ais.end();
			aiitor++, aicount++)
		{
			TankAI *ai = (*aiitor);
			if (aicount == i)
			{
				TankAIAdder::addTankAI(*ScorchedServer::instance(), ai->getName());
			}
		}
	}
}

void ServerFrame::onPlayerAdd1(wxCommandEvent &event) { onPlayerAdd(1); }
void ServerFrame::onPlayerAdd2(wxCommandEvent &event) { onPlayerAdd(2); }
void ServerFrame::onPlayerAdd3(wxCommandEvent &event) { onPlayerAdd(3); }
void ServerFrame::onPlayerAdd4(wxCommandEvent &event) { onPlayerAdd(4); }
void ServerFrame::onPlayerAdd5(wxCommandEvent &event) { onPlayerAdd(5); }
void ServerFrame::onPlayerAdd6(wxCommandEvent &event) { onPlayerAdd(6); }
void ServerFrame::onPlayerAdd7(wxCommandEvent &event) { onPlayerAdd(7); }
void ServerFrame::onPlayerAdd8(wxCommandEvent &event) { onPlayerAdd(8); }
void ServerFrame::onPlayerAdd9(wxCommandEvent &event) { onPlayerAdd(9); }

void ServerFrame::onComsMessageLogging(wxCommandEvent &event)
{
	ScorchedServer::instance()->getComsMessageHandler().getMessageLogging() =
		!ScorchedServer::instance()->getComsMessageHandler().getMessageLogging();
	Logger::log( "Server Coms Message Logging %s",
		(ScorchedServer::instance()->getComsMessageHandler().getMessageLogging()?"On":"Off"));
}

void ServerFrame::onStateLogging(wxCommandEvent &event)
{
	ScorchedServer::instance()->getGameState().getStateLogging() = 
		!ScorchedServer::instance()->getGameState().getStateLogging();
	Logger::log( "Server State Logging %s",
		(ScorchedServer::instance()->getGameState().getStateLogging()?"On":"Off"));
}

void ServerFrame::onMenuExit(wxCommandEvent &event)
{
	Close();
}

void ServerFrame::OnClose(wxCloseEvent& event)
{
	Destroy();
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	std::map<unsigned int, Tank *>::iterator itor;
	for (itor = tanks.begin();
			itor != tanks.end();
			itor++)
	{
		Tank *tank = (*itor).second;
		StatsLogger::instance()->tankDisconnected(tank);
	}
}

void ServerFrame::OnSize(wxSizeEvent& event)
{
	wxSize size = GetClientSize();
	wxCoord y = (2*size.y)/3;
	logList_->SetSize(0, 0, size.x, y);
	playerList_->SetSize(0, y + 1, size.x, size.y - y);
	event.Skip();
}

void ServerFrame::onTimerMain(wxTimerEvent &event)
{
	serverLoop();
}

void ServerFrame::onTimer(wxTimerEvent &event)
{
	// Update the player list
	if (frame->playerList_->GetItemCount() !=
		ScorchedServer::instance()->getTankContainer().getNoOfTanks())
	{
		frame->playerList_->SetItemCount(
			ScorchedServer::instance()->getTankContainer().getNoOfTanks());
	}
	frame->playerList_->Refresh();

	// Update the log list
	if (frame->logList_->GetItemCount() !=
		ServerLog::instance()->getEntries().size())
	{
		frame->logList_->SetItemCount(
			ServerLog::instance()->getEntries().size());
		frame->logList_->EnsureVisible(
			frame->logList_->GetItemCount() - 1);
	}
	frame->logList_->Refresh();

	// Update the status bar
	char buffer[256];
	std::map<unsigned int, Tank *> &tanks = 
		ScorchedServer::instance()->getTankContainer().getPlayingTanks();
	snprintf(buffer, 256, "%i/%i Players", tanks.size(), 
		ScorchedServer::instance()->getOptionsGame().getNoMaxPlayers());
	frame->statusBar_->SetStatusText(wxString(buffer, wxConvUTF8), 0);
	frame->statusBar_->SetStatusText(
		(ServerTooFewPlayersStimulus::instance()->acceptStateChange(0, 0, 0.0f)?
			wxT("Not Playing"):wxT("Playing")), 1);
	snprintf(buffer, 256, "Round %i/%i, %i/%i Moves",
		ScorchedServer::instance()->getOptionsTransient().getCurrentRoundNo(),
		ScorchedServer::instance()->getOptionsGame().getNoRounds(),
		ScorchedServer::instance()->getOptionsTransient().getCurrentGameNo(),
		ScorchedServer::instance()->getOptionsGame().getNoMaxRoundTurns());
	frame->statusBar_->SetStatusText(wxString(buffer, wxConvUTF8), 2);
	snprintf(buffer, 256, "BI:%i BO:%i P:%i C:%i",
		NetInterface::getBytesIn(),
		NetInterface::getBytesOut(),
		NetInterface::getPings(),
		NetInterface::getConnects());
	frame->statusBar_->SetStatusText(wxString(buffer, wxConvUTF8), 3);
}

void ServerFrame::onKillAll(wxCommandEvent &event)
{
	ServerCommon::killAll();
}

void ServerFrame::onStartNewGame(wxCommandEvent &event)
{
	ServerCommon::startNewGame();
}

void ServerFrame::onPlayerTalkAll(wxCommandEvent &event)
{
	wxTextEntryDialog entryDialog(
		frame, wxT("Message to players"));
	if (entryDialog.ShowModal() == wxID_OK)
	{
		Logger::log( "Says \"%s\"", 
			entryDialog.GetValue().GetData());
		ServerCommon::sendString(0, entryDialog.GetValue().mb_str(wxConvUTF8));
	}
}

void ServerFrame::onPlayerTalk(wxCommandEvent &event)
{
	wxTextEntryDialog entryDialog(
		frame, wxT("Message to players"));
	if (entryDialog.ShowModal() == wxID_OK)
	{
		Logger::log( "Says \"%s\"", 
			entryDialog.GetValue().GetData());

		long item = -1;
		while ((item = frame->playerList_->GetNextItem(
			item,
			wxLIST_NEXT_ALL,
			wxLIST_STATE_SELECTED)) != -1)
		{
			if ((item != -1) && 
				(item < ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
			{
				Tank *tank = 
					ScorchedServer::instance()->getTankContainer().getTankByPos((unsigned int) item);
				if (!tank->getTankAI())
				{
					ServerCommon::sendString(tank->getDestinationId(), 
						entryDialog.GetValue().mb_str(wxConvUTF8));
				}
			}
		}
	}
}

void ServerFrame::onPlayerKick(wxCommandEvent &event)
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
		{
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().
				getTankByPos((unsigned int) item);
			ServerCommon::kickPlayer(tank->getPlayerId());
		}		
    }
}

void ServerFrame::onPlayerMute(wxCommandEvent &event)
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
		{
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().
				getTankByPos((unsigned int) item);
			if (tank) tank->getState().setMuted(true);
		}		
    }
}

void ServerFrame::onPlayerUnMute(wxCommandEvent &event)
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
		{
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().
				getTankByPos((unsigned int) item);
			if (tank) tank->getState().setMuted(false);
		}		
    }
}

static void banPlayers(ServerBanned::BannedType type)
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
		{
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().
				getTankByPos((unsigned int) item);
			ServerCommon::banPlayer(tank->getPlayerId(), type);
		}		
    }
}

void ServerFrame::onPlayerBan(wxCommandEvent &event)
{
	banPlayers(ServerBanned::Banned);
}

void ServerFrame::onPlayerPermMute(wxCommandEvent &event)
{
	banPlayers(ServerBanned::Muted);
	onPlayerMute(event);
}

void ServerFrame::onPlayerUnPermMute(wxCommandEvent &event)
{
	banPlayers(ServerBanned::NotBanned);
	onPlayerUnMute(event);
}

void ServerFrame::onPlayerSlap25(wxCommandEvent &event)
{
	long item = -1;
    while ((item = frame->playerList_->GetNextItem(
		item,
		wxLIST_NEXT_ALL,
        wxLIST_STATE_SELECTED)) != -1)
    {
		if ((item != -1) && (item < 
			ScorchedServer::instance()->getTankContainer().getNoOfTanks()))
		{
			Tank *tank = 
				ScorchedServer::instance()->getTankContainer().
				getTankByPos((unsigned int) item);
			ServerCommon::slapPlayer(tank->getPlayerId(), 25.0f);
		}		
    }
}

void ServerFrame::onEditOptions(wxCommandEvent &event)
{
	if(showSettingsDialog(true, ScorchedServer::instance()->
		getOptionsGame().getChangedOptions()))
	{
		dialogMessage("Server",
			"These changes will only take place at the start of the next round (map).\n"
			"Be careful, some changes may not work without restarting server.");
	}
}

void ServerFrame::onLoadOptions(wxCommandEvent &event)
{
	wxString file = ::wxFileSelector(wxT("Please choose the options file to open"),
									 wxString(getSettingsFile(""), wxConvUTF8), // default path
									 wxT("server.xml"), // default filename
									 wxT(""), // default extension
									 wxT("*.xml"),
									 wxOPEN | wxFILE_MUST_EXIST);
	if (!file.empty())
	{
		std::string fileName = (const char *) file.mb_str(wxConvUTF8);
		if(ScorchedServer::instance()->getOptionsGame().getChangedOptions().
		   readOptionsFromFile((char *) fileName.c_str()))
		{
			dialogMessage("Server",
						  "These changes will only take place at the start of the next round (map).\n"
						  "Be careful, some changes may not work without restarting server.");
		}
	}
}

void ServerFrame::onSaveOptions(wxCommandEvent &event)
{
	wxString file = ::wxFileSelector(wxT("Please choose the options file to save"),
									 wxString(getSettingsFile(""), wxConvUTF8), // default path
									 wxT("server.xml"), // default filename
									 wxT(""), // default extension
									 wxT("*.xml"),
									 wxSAVE);
	if (!file.empty())
	{
		std::string fileName = (const char *) file.mb_str(wxConvUTF8);
		ScorchedServer::instance()->getOptionsGame().getChangedOptions().
		   readOptionsFromFile((char *) fileName.c_str());
	}
}

void ServerFrame::onShowModFiles(wxCommandEvent &event)
{
	ListDialog listDialog(this, "Scorched 3D Server Mod Files");
	std::map<std::string, ModFileEntry *> &modFiles = 
		ScorchedServer::instance()->getModFiles().getFiles();
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = modFiles.begin();
		itor != modFiles.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;

		listDialog.addItem(formatString("%s - %u bytes - %u", 
			entry->getFileName(), 
			entry->getCompressedSize(),
			entry->getCompressedCrc()));
	}
	listDialog.ShowModal();
}

void ServerFrame::onShowBanned(wxCommandEvent &event)
{
	ListDialog listDialog(this, "Scorched 3D Server Banned Users");
	std::list<ServerBanned::BannedRange> &bannedIps = 
		ScorchedServerUtil::instance()->bannedPlayers.getBannedIps();
	std::list<ServerBanned::BannedRange>::iterator itor;
	for (itor = bannedIps.begin();
		itor != bannedIps.end();
		itor++)
	{
		ServerBanned::BannedRange &range = (*itor);

		std::string mask = NetInterface::getIpName(range.mask);

		std::map<unsigned int, ServerBanned::BannedEntry>::iterator ipitor;
		for (ipitor = range.ips.begin();
			ipitor != range.ips.end();
			ipitor++)
		{
			unsigned int ip = (*ipitor).first;
			ServerBanned::BannedEntry &entry = (*ipitor).second;
			std::string ipName = NetInterface::getIpName(ip);

			listDialog.addItem(formatString("\"%s:%s\" %s %s (%s) - %s",
				entry.name.c_str(),
				entry.uniqueid.c_str(),
				ServerBanned::getBannedTypeStr(entry.type),
				ipName.c_str(), mask.c_str(),
				entry.bantime?ctime(&entry.bantime):""));
		}
	}
	listDialog.ShowModal();
}

void ServerFrame::onShowOptions(wxCommandEvent &event)
{
	ListDialog listDialog(this, "Scorched 3D Server Options");
	std::list<OptionEntry *> &allOptions = 
		ScorchedServer::instance()->getOptionsGame().getOptions();
	std::list<OptionEntry *>::iterator itor;
	for (itor = allOptions.begin();
		itor != allOptions.end();
		itor++)
	{
		OptionEntry *entry = (*itor);
		listDialog.addItem(formatString("%s = %s", 
			entry->getName(), entry->getValueAsString()));
	}
	listDialog.ShowModal();
}

void showServerDialog()
{
	char serverName[1024];
	snprintf(serverName, 1024,
		"Scorched 3D Dedicated Server [ %s : %i ] [%s]",
		ScorchedServer::instance()->getOptionsGame().getServerName(), 
		ScorchedServer::instance()->getOptionsGame().getPortNo(), 
		(ScorchedServer::instance()->getOptionsGame().getPublishServer()?"Published":"Not Published"));

	frame = new ServerFrame(serverName);
	frame->Show();

	ServerCommon::startFileLogger();
}
