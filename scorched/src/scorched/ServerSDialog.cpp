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


#include <scorched/MainDialog.h>
#include <scorched/ServerDialog.h>
#include <scorched/SettingsDialog.h>
#include <server/ServerMain.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <wx/wx.h>
#include "ServerS.cpp"

extern char scorched3dAppName[128];
static char *serverFilePath = PKGDIR "data/server.xml";

class ServerSFrame: public wxDialog
{
public:
	ServerSFrame();

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onSettingsButton();

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ServerSFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_SETTINGS,  ServerSFrame::onSettingsButton)
END_EVENT_TABLE()

ServerSFrame::ServerSFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(365, 265))
{
	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create all the display controlls
	createControls(this);
	IDOK_CTRL->SetDefault();
}

void ServerSFrame::onSettingsButton()
{
	if (showSettingsDialog(true))
	{
		OptionsGame::instance()->writeOptionsToFile(serverFilePath);
	}
}

bool ServerSFrame::TransferDataToWindow()
{
	// Load the server settings
	OptionsGame::instance()->readOptionsFromFile(serverFilePath);

	char buffer[256];
	sprintf(buffer, "%i", OptionsGame::instance()->getPortNo());
	IDC_SERVER_PORT_CTRL->SetValue(buffer);
	IDC_SERVER_NAME_CTRL->SetValue(OptionsGame::instance()->getServerName());

	// Disable the publish options for non-win 32 clients
#ifdef _NO_SERVER_ASE_
	if (OptionsGame::instance()->getPublishServer())
	{
		dialogMessage("Scorched3D Server", 
			"The publish server option is currently only available for\n"
			"officially built and Win32 versions of Scorched");
	}
	IDC_PUBLISH_CTRL->Enable(false);
	IDC_PUBLISHIP_CTRL->Enable(false);
	IDC_PUBLISHIP_CTRL_TEXT->Enable(false);
#else
	IDC_PUBLISH_CTRL->SetValue(OptionsGame::instance()->getPublishServer());
	IDC_PUBLISHIP_CTRL->SetValue(OptionsGame::instance()->getPublishAddress());
#endif

	return true;
}

bool ServerSFrame::TransferDataFromWindow()
{
	OptionsGame::instance()->setPortNo(atoi(IDC_SERVER_PORT_CTRL->GetValue()));
	OptionsGame::instance()->setServerName(IDC_SERVER_NAME_CTRL->GetValue());
	OptionsGame::instance()->setPublishServer(IDC_PUBLISH_CTRL->GetValue());
	OptionsGame::instance()->setPublishAddress(IDC_PUBLISHIP_CTRL->GetValue());

	// Save the server settings
	OptionsGame::instance()->writeOptionsToFile(serverFilePath);

	// Start up the server
	showServerDialog();
	serverMain();

	return true;
}

bool showServerSDialog()
{
	ServerSFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
