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
#include <scorched/SettingsDialog.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <common/OptionsParam.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/dir.h>
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
	TransferDataFromWindow();
	// Don't save until the whole options have been choosen
	showSettingsDialog(true, options_);
}

bool ServerSFrame::TransferDataToWindow()
{
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

	IDC_SERVER_MOD_CTRL->Append("None");
	{
		wxDir dir(getModFile(""));
		if (dir.IsOpened())
		{
			wxString filename;
			bool cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
			while (cont)
			{
				if (IDC_SERVER_MOD_CTRL->FindString(filename) == -1)
					IDC_SERVER_MOD_CTRL->Append(filename);
				cont = dir.GetNext(&filename);
			}
		}
	}
	{
		wxDir dir(getGlobalModFile(""));
		if (dir.IsOpened())
		{
			wxString filename;
			bool cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
			while (cont)
			{
				if (IDC_SERVER_MOD_CTRL->FindString(filename) == -1)
					IDC_SERVER_MOD_CTRL->Append(filename);
				cont = dir.GetNext(&filename);
			}
		}
	}
	if (IDC_SERVER_MOD_CTRL->FindString(options_.getMod()) != -1)
		IDC_SERVER_MOD_CTRL->SetValue(options_.getMod());
	else 
		IDC_SERVER_MOD_CTRL->SetValue("None");
	IDC_SERVER_MOD_CTRL->SetToolTip(
		wxString("The Scorched3D mod to use for this game."));

	return true;
}

bool ServerSFrame::TransferDataFromWindow()
{
	options_.setPortNo(atoi(IDC_SERVER_PORT_CTRL->GetValue()));
	options_.setServerName(IDC_SERVER_NAME_CTRL->GetValue());
	options_.setPublishServer(IDC_PUBLISH_CTRL->GetValue());
	options_.setPublishAddress(IDC_PUBLISHIP_CTRL->GetValue());
	wxString value = IDC_SERVER_MOD_CTRL->GetValue();
	if (strcmp(value.c_str(), "None"))
		options_.setMod(IDC_SERVER_MOD_CTRL->GetValue());
	else
		options_.setMod("");

	return true;
}

bool showServerSDialog()
{
	OptionsGame tmpOptions;
	wxString serverFileSrc = getDataFile("data/server.xml");
	wxString serverFileDest = getSettingsFile("server.xml");
	if (::wxFileExists(serverFileDest))
	{
		tmpOptions.readOptionsFromFile((char *) serverFileDest.c_str());
	}
	else
	{
		tmpOptions.readOptionsFromFile((char *) serverFileSrc.c_str());
	}

	ServerSFrame frame(tmpOptions);
	if (frame.ShowModal() == wxID_OK)
	{
		tmpOptions.writeOptionsToFile((char *) serverFileDest.c_str());
		runScorched3D("-startserver \"%s\"", serverFileDest.c_str());
		return true;
	}
	return false;
}

