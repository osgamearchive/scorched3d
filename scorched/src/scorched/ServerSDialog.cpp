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
#include <wx/utils.h>
#include "ServerS.cpp"

extern char scorched3dAppName[128];

class ServerSFrame: public wxDialog
{
public:
	ServerSFrame(OptionsGame &options);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onSettingsButton();

private:
	DECLARE_EVENT_TABLE()

	OptionsGame &options_;
};

BEGIN_EVENT_TABLE(ServerSFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_SETTINGS,  ServerSFrame::onSettingsButton)
END_EVENT_TABLE()

ServerSFrame::ServerSFrame(OptionsGame &options) :
	wxDialog(getMainDialog(), -1, scorched3dAppName,
			 wxDefaultPosition, wxDefaultSize),
	 options_(options)
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
	showSettingsDialog(true, options_);
}

bool ServerSFrame::TransferDataToWindow()
{
	// Load the server settings
	wxString serverFileSrc = getDataFile("data/server.xml");
	wxString serverFileDest = getSettingsFile("server.xml");
	if (::wxFileExists(serverFileDest))
	{
		options_.readOptionsFromFile((char *) serverFileDest.c_str());
	}
	else
	{
		options_.readOptionsFromFile((char *) serverFileSrc.c_str());
	}

	char buffer[256];
	sprintf(buffer, "%i", options_.getPortNo());
	IDC_SERVER_PORT_CTRL->SetValue(buffer);
	IDC_SERVER_PORT_CTRL->SetToolTip(options_.getPortNoToolTip());
	IDC_SERVER_NAME_CTRL->SetValue(options_.getServerName());
	IDC_SERVER_NAME_CTRL->SetToolTip(options_.getServerNameToolTip());
	IDC_PUBLISH_CTRL->SetValue(options_.getPublishServer());
	IDC_PUBLISH_CTRL->SetToolTip(options_.getPublishServerToolTip());
	IDC_PUBLISHIP_CTRL->SetValue(options_.getPublishAddress());
	IDC_PUBLISHIP_CTRL->SetToolTip(options_.getPublishAddressToolTip());
	return true;
}

bool ServerSFrame::TransferDataFromWindow()
{
	options_.setPortNo(atoi(IDC_SERVER_PORT_CTRL->GetValue()));
	options_.setServerName(IDC_SERVER_NAME_CTRL->GetValue());
	options_.setPublishServer(IDC_PUBLISH_CTRL->GetValue());
	options_.setPublishAddress(IDC_PUBLISHIP_CTRL->GetValue());

	return true;
}

bool showServerSDialog()
{
	OptionsGame tmpOptions;
	ServerSFrame frame(tmpOptions);
	if (frame.ShowModal() == wxID_OK)
	{
		wxString serverFileDest = getSettingsFile("server.xml");
		tmpOptions.writeOptionsToFile((char *) serverFileDest.c_str());

		runScorched3D("-startserver \"%s\"", serverFileDest.c_str());
		return true;
	}
	return false;
}

