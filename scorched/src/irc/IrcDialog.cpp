#ifdef _NO_SERVER_ASE_

#include <irc/IrcDialog.h>
#include <irc/ClientGameInfo.h>
#include <scorched/MainDialog.h>
#include <common/OptionsParam.h>

enum
{
	IDC_SERVER_LIST = 1,
	IDC_BUTTON_LAN,
	IDC_BUTTON_NET,
	IDC_EDIT_SERVER,
	IDC_CLEAR,
};

extern char scorched3dAppName[128];

static wxTextCtrl *IDC_EDIT_SERVER_CTRL = 0;
static wxListCtrl *IDC_LOG_CTRL=0;
static wxButton *IDC_BUTTON_LAN_CTRL = 0;
static wxButton *IDC_BUTTON_NET_CTRL = 0;
static wxButton *IDC_CLEAR_CTRL = 0;
static wxButton *IDOK_CTRL = 0;
static wxButton *IDCANCEL_CTRL = 0;
static NetListControl *IDC_SERVER_LIST_CTRL = 0;

BEGIN_EVENT_TABLE(NetLanFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_LAN,  NetLanFrame::onRefreshLanButton)
	EVT_BUTTON(IDC_BUTTON_NET,  NetLanFrame::onRefreshNETButton)
	EVT_BUTTON(IDC_CLEAR,  NetLanFrame::onClearButton)
	EVT_TIMER(1001, NetLanFrame::onTimer)
	EVT_LIST_ITEM_SELECTED(IDC_SERVER_LIST, NetLanFrame::onSelectServer)
	EVT_TEXT(IDC_EDIT_SERVER, NetLanFrame::onServerChanged)
END_EVENT_TABLE()


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
	GameInfoEntry entry;
	entry = ClientGameInfo::instance()->GetEntry(item);
	switch(column) {
	case 0:	return entry.getDescription();
	case 1: return string( 
			string(entry.getNoClients()) + "/" + 
			string(entry.getMaxClients())
		).c_str(); 
	case 2: return entry.getHost();
	case 3: return entry.getPort();
	case 4: return entry.getProtocol();
	default :
		break;
	}
	return "unknown";
}

NetLanFrame::NetLanFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(545,430)) , entries_(0)
{

	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif
	new wxStaticText(this, -1,
		"Connect To Name/IP : ",
		wxPoint(10, 20), wxSize(140,20));
	IDC_EDIT_SERVER_CTRL = 
		new wxTextCtrl(this, IDC_EDIT_SERVER,
		"",
		wxPoint(150, 20), wxSize(295, 20));
	IDC_CLEAR_CTRL = 
		new wxButton(this, IDC_CLEAR,
		"Clear",
		wxPoint(450, 20), wxSize(85, 20));
	IDC_SERVER_LIST_CTRL = 
		new NetListControl(this, IDC_SERVER_LIST,
		wxPoint(10,50), wxSize(525,220));
	IDC_BUTTON_LAN_CTRL = 
		new wxButton(this, IDC_BUTTON_LAN,
		"Refresh LAN Games",
		wxPoint(10, 280), wxSize(130,20));
	IDC_BUTTON_NET_CTRL = 
		new wxButton(this, IDC_BUTTON_NET,
		"Refresh Internet Games",
		wxPoint(145,280), wxSize(150,20));
	IDOK_CTRL = 
		new wxButton(this, wxID_OK,
		"Join Game",
		wxPoint(300,280), wxSize(115,20));
	IDCANCEL_CTRL = 
		new wxButton(this, wxID_CANCEL,
		"Cancel",
		wxPoint(420,280), wxSize(115,20));
	IDC_LOG_CTRL = new wxListCtrl(this, -1,
                           wxPoint(10,310),
			   wxSize(525,110),
			   wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL );
	IDC_LOG_CTRL->InsertColumn(0,"Logging",wxLIST_FORMAT_LEFT,525);

	// Create a timer
	timer_.SetOwner(this, 1001);
	timer_.Start(3000, false);

	Logger::addLogger(this);
}

NetLanFrame::~NetLanFrame()
{
	Logger::remLogger(this);
}

void NetLanFrame::onClearButton()
{
	IDC_EDIT_SERVER_CTRL->SetValue("");
	onServerChanged();
}

void NetLanFrame::onSelectServer()
{
    long item = -1;
    for ( ;; )
    {
	GameInfoEntry entry;

        item = IDC_SERVER_LIST_CTRL->GetNextItem(
			item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        
	if ( item == -1) break;

	entry = ClientGameInfo::instance()->GetEntry(item);
	if ( strcmp(entry.getProtocol(),ScorchedProtocolVersion))
	{
		dialogMessage("Scorched 3D", 
			"Warning: This server is running a different version of Scorched3D\n"
			"which may may not work with your version.\n\n"
			"The latest version of Scorched3D can be downloaded from http://www.scorched3d.co.uk\n");						
	}
	IDC_EDIT_SERVER_CTRL->SetValue(entry.getAddress());
	onServerChanged();
    }
}

void NetLanFrame::onRefreshLanButton()
{
	IDC_BUTTON_LAN_CTRL->Disable();
	IDC_BUTTON_NET_CTRL->Disable();

 	ClientGameInfo::instance()->LanQuery();
}

void NetLanFrame::onRefreshNETButton()
{
	IDC_BUTTON_LAN_CTRL->Disable();
	IDC_BUTTON_NET_CTRL->Disable();
	ClientGameInfo::instance()->IrcQuery();
}

void NetLanFrame::onServerChanged()
{
	bool enabled = IDC_EDIT_SERVER_CTRL->GetValue().c_str()[0] != '\0';
	IDOK_CTRL->Enable(enabled);
}

void NetLanFrame::onTimer()
{
	static int new_entries_=0;
	entries_ = ClientGameInfo::instance()->Servers();
	IDC_SERVER_LIST_CTRL->SetItemCount(entries_);
	if (new_entries_!=entries_) {
		IDC_SERVER_LIST_CTRL->EnsureVisible(entries_);
		new_entries_=entries_;
	}
	IDC_BUTTON_LAN_CTRL->Enable();		
	IDC_BUTTON_NET_CTRL->Enable();
	Logger::processLogEntries();
}

bool NetLanFrame::TransferDataToWindow()
{
	// Set the ok button to disabled
	IDC_EDIT_SERVER_CTRL->SetValue(OptionsParam::instance()->getConnect());
	
	// Setup the list control
	struct ListItem
	{
		char *name;
		int size;
	} mainListItems[] =
	{
		{ "Server", 200 },
		{ "Players", 60 },
		{ "IP Address", 140 },
		{ "Port",    60 },
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

	onServerChanged();
	return true;
}

bool NetLanFrame::TransferDataFromWindow()
{
	OptionsParam::instance()->setConnect(
		IDC_EDIT_SERVER_CTRL->GetValue().c_str());
	
	return true;
}

void NetLanFrame::logMessage(
		const char *time,
		const char *message,
		unsigned int playerId)
{
	long index;
	static char text[1024];
	sprintf(text, "%s - %s", time, message);
	while (IDC_LOG_CTRL->GetItemCount() > 100) {
		IDC_LOG_CTRL->DeleteItem(0);	
	}
	index=IDC_LOG_CTRL->InsertItem(IDC_LOG_CTRL->GetItemCount(), text);
	IDC_LOG_CTRL->EnsureVisible(index);
};


bool showNetLanDialog()
{
	NetLanFrame frame;
	return (frame.ShowModal() == wxID_OK);
}

#endif /* _NO_SERVER_ASE_ */