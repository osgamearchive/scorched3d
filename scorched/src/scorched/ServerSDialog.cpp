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
#include <server/ScorchedServer.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include "ServerS.cpp"

extern char scorched3dAppName[128];

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
	wxDialog(getMainDialog(), -1, scorched3dAppName,
			 wxDefaultPosition, wxDefaultSize)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controlls
	createControls(this, topsizer);
	IDOK_CTRL->SetDefault();

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void ServerSFrame::onSettingsButton()
{
	// Don't save until the whole options have been choosen
	showSettingsDialog(true, ScorchedServer::instance()->getOptionsGame());
}

bool ServerSFrame::TransferDataToWindow()
{
	// Load the server settings
	wxString serverFileSrc = getDataFile("data/server.xml");
	wxString serverFileDest = getSettingsFile("server.xml");
	if (::wxFileExists(serverFileDest))
	{
		ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
			(char *) serverFileDest.c_str());
	}
	else
	{
		ScorchedServer::instance()->getOptionsGame().readOptionsFromFile(
			(char *) serverFileSrc.c_str());
	}

	char buffer[256];
	sprintf(buffer, "%i", ScorchedServer::instance()->getOptionsGame().getPortNo());
	IDC_SERVER_PORT_CTRL->SetValue(buffer);
	IDC_SERVER_PORT_CTRL->SetToolTip(
		ScorchedServer::instance()->getOptionsGame().getPortNoToolTip());
	IDC_SERVER_NAME_CTRL->SetValue(
		ScorchedServer::instance()->getOptionsGame().getServerName());
	IDC_SERVER_NAME_CTRL->SetToolTip(
		ScorchedServer::instance()->getOptionsGame().getServerNameToolTip());
	IDC_PUBLISH_CTRL->SetValue(
		ScorchedServer::instance()->getOptionsGame().getPublishServer());
	IDC_PUBLISH_CTRL->SetToolTip(
		ScorchedServer::instance()->getOptionsGame().getPublishServerToolTip());
	IDC_PUBLISHIP_CTRL->SetValue(
		ScorchedServer::instance()->getOptionsGame().getPublishAddress());
	IDC_PUBLISHIP_CTRL->SetToolTip(
		ScorchedServer::instance()->getOptionsGame().getPublishAddressToolTip());
	return true;
}

bool ServerSFrame::TransferDataFromWindow()
{
	ScorchedServer::instance()->getOptionsGame().setPortNo(atoi(IDC_SERVER_PORT_CTRL->GetValue()));
	ScorchedServer::instance()->getOptionsGame().setServerName(IDC_SERVER_NAME_CTRL->GetValue());
	ScorchedServer::instance()->getOptionsGame().setPublishServer(IDC_PUBLISH_CTRL->GetValue());
	ScorchedServer::instance()->getOptionsGame().setPublishAddress(IDC_PUBLISHIP_CTRL->GetValue());

	return true;
}

bool showServerSDialog()
{
	ServerSFrame frame;
	if (frame.ShowModal() == wxID_OK)
	{
		wxString serverFileDest = getSettingsFile("server.xml");
		ScorchedServer::instance()->getOptionsGame().writeOptionsToFile(
			(char *) serverFileDest.c_str());

		runScorched3D("-startserver \"%s\"", serverFileDest.c_str());
		return true;
	}
	return false;
}

