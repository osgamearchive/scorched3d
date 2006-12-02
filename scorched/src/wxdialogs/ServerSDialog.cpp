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

#include <wxdialogs/MainDialog.h>
#include <wxdialogs/SettingsDialog.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <engine/ModDirs.h>
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

	void onSettingsButton(wxCommandEvent &event);
	void onPublishAutoButton(wxCommandEvent &event);

private:
	DECLARE_EVENT_TABLE()

	OptionsGame &options_;
};

BEGIN_EVENT_TABLE(ServerSFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_SETTINGS,  ServerSFrame::onSettingsButton)
	EVT_BUTTON(IDC_PUBLISHAUTO,  ServerSFrame::onPublishAutoButton)
END_EVENT_TABLE()

ServerSFrame::ServerSFrame(OptionsGame &options) :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8),
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

void ServerSFrame::onPublishAutoButton(wxCommandEvent &event)
{
	IDC_PUBLISHIP_CTRL->SetValue(wxT("AutoDetect"));
}

void ServerSFrame::onSettingsButton(wxCommandEvent &event)
{
	TransferDataFromWindow();
	// Don't save until the whole options have been choosen
	showSettingsDialog(true, options_);
}

bool ServerSFrame::TransferDataToWindow()
{
	IDC_SERVER_PORT_CTRL->SetValue(
		wxString(formatString("%i", options_.getPortNo()), wxConvUTF8));
	IDC_SERVER_PORT_CTRL->SetToolTip(
		wxString(options_.getPortNoEntry().getDescription(), wxConvUTF8));
	IDC_SERVERMANAGEMENT_PORT_CTRL->SetValue(
		wxString(formatString("%i", options_.getManagementPortNo()), wxConvUTF8));
	IDC_SERVERMANAGEMENT_PORT_CTRL->SetToolTip(
		wxString(options_.getManagementPortNoEntry().getDescription(), wxConvUTF8));
	IDC_SERVER_NAME_CTRL->SetValue(
		wxString(options_.getServerName(), wxConvUTF8));
	IDC_SERVER_NAME_CTRL->SetToolTip(
		wxString(options_.getServerNameEntry().getDescription(), wxConvUTF8));
	IDC_PUBLISH_CTRL->SetValue(options_.getPublishServer());
	IDC_PUBLISH_CTRL->SetToolTip(
		wxString(options_.getPublishServerEntry().getDescription(), wxConvUTF8));
	IDC_PUBLISHIP_CTRL->SetValue(
		wxString(options_.getPublishAddress(), wxConvUTF8));
	IDC_PUBLISHIP_CTRL->SetToolTip(
		wxString(options_.getPublishAddressEntry().getDescription(), wxConvUTF8));
	IDC_ALLOWSAME_CTRL->SetValue(options_.getAllowSameIP());
	IDC_ALLOWSAME_CTRL->SetToolTip(
		wxString(options_.getAllowSameIPEntry().getDescription(), wxConvUTF8));
	IDC_ALLOWSAMEID_CTRL->SetValue(options_.getAllowSameUniqueId());
	IDC_ALLOWSAMEID_CTRL->SetToolTip(
		wxString(options_.getAllowSameUniqueIdEntry().getDescription(), wxConvUTF8));

	ModDirs modDirs;
	if (!modDirs.loadModDirs()) dialogExit("ModFiles", "Failed to load mod files");
	std::list<ModInfo>::iterator itor;
	for (itor = modDirs.getDirs().begin();
		itor != modDirs.getDirs().end();
		itor++)
	{
		ModInfo &info = *itor;
		IDC_SERVER_MOD_CTRL->Append(wxString(info.getName(), wxConvUTF8));
	}
	if (IDC_SERVER_MOD_CTRL->FindString(wxString(options_.getMod(), wxConvUTF8)) != -1)
		IDC_SERVER_MOD_CTRL->SetValue(wxString(options_.getMod(), wxConvUTF8));
	else 
		IDC_SERVER_MOD_CTRL->SetValue(wxT("none"));
	IDC_SERVER_MOD_CTRL->SetToolTip(
		wxString("The Scorched3D mod to use for this game.", wxConvUTF8));

	return true;
}

bool ServerSFrame::TransferDataFromWindow()
{
	options_.getPortNoEntry().setValue(atoi(IDC_SERVER_PORT_CTRL->GetValue().mb_str(wxConvUTF8)));
	options_.getManagementPortNoEntry().setValue(atoi(IDC_SERVERMANAGEMENT_PORT_CTRL->GetValue().mb_str(wxConvUTF8)));
	options_.getServerNameEntry().setValue(IDC_SERVER_NAME_CTRL->GetValue().mb_str(wxConvUTF8));
	options_.getPublishServerEntry().setValue(IDC_PUBLISH_CTRL->GetValue());
	options_.getPublishAddressEntry().setValue(IDC_PUBLISHIP_CTRL->GetValue().mb_str(wxConvUTF8));
	options_.getAllowSameIPEntry().setValue(IDC_ALLOWSAME_CTRL->GetValue());
	options_.getAllowSameUniqueIdEntry().setValue(IDC_ALLOWSAMEID_CTRL->GetValue());
	options_.getModEntry().setValue(IDC_SERVER_MOD_CTRL->GetValue().mb_str(wxConvUTF8));
	return true;
}

bool showServerSDialog()
{
	OptionsGame tmpOptions;
	std::string serverFileSrc = getDataFile("data/server.xml");
	std::string serverFileDest = getSettingsFile("server.xml");
	if (s3d_fileExists(serverFileDest.c_str()))
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
		runScorched3D(formatString("-startserver \"%s\"", serverFileDest.c_str()), true);
		return true;
	}
	return false;
}

