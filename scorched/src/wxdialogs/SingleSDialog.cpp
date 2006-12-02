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
#include <wxdialogs/OptionEntrySetter.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>
#include <scorched/ScorchedParams.h>
#include <engine/ModDirs.h>
#include <wx/wx.h>
#include <wx/utils.h>

extern char scorched3dAppName[128];

enum
{
	IDC_BUTTON_SETTINGS
};

class SingleSFrame: public wxDialog
{
public:
	SingleSFrame(OptionsGame &options);

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();

	void onSettingsButton(wxCommandEvent &event);

private:
	DECLARE_EVENT_TABLE()

	wxComboBox *IDC_CLIENT_MOD_CTRL;
	wxButton *IDOK_CTRL;
	wxButton *IDC_BUTTON_SETTINGS_CTRL;
	wxButton *IDCANCEL_CTRL;

	std::list<OptionEntrySetter> setters_;
	OptionsGame &options_;
};

BEGIN_EVENT_TABLE(SingleSFrame, wxDialog)
    EVT_BUTTON(IDC_BUTTON_SETTINGS,  SingleSFrame::onSettingsButton)
END_EVENT_TABLE()

SingleSFrame::SingleSFrame(OptionsGame &options) :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8),
			 wxDefaultPosition, wxDefaultSize),
	 options_(options)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(wxString(getDataFile("data/windows/tank2.ico"), wxConvUTF8), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	// Create all the display controlls
	{
		wxStaticBox *settingsbox = 
			new wxStaticBox(this, -1, wxT("Client Settings"));
		wxStaticBoxSizer *settingsboxsizer = 
			new wxStaticBoxSizer(settingsbox, wxVERTICAL);

		wxSizer *settingssizer = new wxFlexGridSizer(2, 2);
		setters_.push_back(
			OptionEntrySetterUtil::createOtherSetter(
				this, settingssizer, options.getNoMaxPlayersEntry()));
		setters_.push_back(
			OptionEntrySetterUtil::createOtherSetter(
			this, settingssizer, options.getTeamsEntry()));
		setters_.push_back(
			OptionEntrySetterUtil::createOtherSetter(
			this, settingssizer, options.getTurnTypeEntry()));

		settingssizer->Add(new wxStaticText(this, -1,
			wxT("Use Mod")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		IDC_CLIENT_MOD_CTRL = 
			new wxComboBox(this, -1,
			wxT(""),
			wxDefaultPosition, wxSize(160, -1),
			0, 0, wxCB_READONLY);
		settingssizer->Add(IDC_CLIENT_MOD_CTRL, 0, wxALIGN_LEFT);

		settingsboxsizer->Add(settingssizer);
		topsizer->Add(settingsboxsizer, 0, wxALL, 5);

		IDC_BUTTON_SETTINGS_CTRL = 
			new wxButton(this, IDC_BUTTON_SETTINGS,
			wxT("Edit Advanced Settings"));
		settingsboxsizer->Add(IDC_BUTTON_SETTINGS_CTRL, 0, wxALL, 5);
	}

	ModDirs modDirs;
	if (!modDirs.loadModDirs()) dialogExit("ModFiles", "Failed to load mod files");
	std::list<ModInfo>::iterator itor;
	for (itor = modDirs.getDirs().begin();
		itor != modDirs.getDirs().end();
		itor++)
	{
		ModInfo &info = (*itor);
		IDC_CLIENT_MOD_CTRL->Append(wxString(info.getName(), wxConvUTF8));
	}

	// Ok and cancel boxes
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	IDOK_CTRL = new wxButton(this, wxID_OK, wxT("Start Client"));
	IDCANCEL_CTRL = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	buttonSizer->Add(IDCANCEL_CTRL, 0, wxALL, 10);
	buttonSizer->Add(IDOK_CTRL, 0, wxALL, 10);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	IDOK_CTRL->SetDefault();

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SingleSFrame::onSettingsButton(wxCommandEvent &event)
{
	TransferDataFromWindow();
	// Don't save until the whole options have been choosen
	showSettingsDialog(false, options_);
	TransferDataToWindow();
}

bool SingleSFrame::TransferDataToWindow()
{
	OptionEntrySetterUtil::updateControls(setters_);

	if (IDC_CLIENT_MOD_CTRL->FindString(wxString(options_.getMod(), wxConvUTF8)) != -1)
		IDC_CLIENT_MOD_CTRL->SetValue(wxString(options_.getMod(), wxConvUTF8));
	else 
		IDC_CLIENT_MOD_CTRL->SetValue(wxT("none"));
	IDC_CLIENT_MOD_CTRL->SetToolTip(
		wxString("The Scorched3D mod to use for this game.", wxConvUTF8));

	return true;
}

bool SingleSFrame::TransferDataFromWindow()
{
	OptionEntrySetterUtil::updateEntries(setters_);
	options_.getNoMinPlayersEntry().setValue(options_.getNoMaxPlayers());
	options_.getModEntry().setValue(IDC_CLIENT_MOD_CTRL->GetValue().mb_str(wxConvUTF8));

	return true;
}

bool showSingleSDialog()
{
	OptionsGame tmpOptions;
	std::string customFilePathSrc = getDataFile("data/singlecustom.xml");
	std::string customFilePathDest = getSettingsFile("singlecustom.xml");
	if (s3d_fileExists(customFilePathDest.c_str()))
	{
		tmpOptions.readOptionsFromFile((char *) customFilePathDest.c_str());
	}
	else
	{
		tmpOptions.readOptionsFromFile((char *) customFilePathSrc.c_str());
	}

	SingleSFrame frame(tmpOptions);
	if (frame.ShowModal() == wxID_OK)
	{
		tmpOptions.writeOptionsToFile((char *) customFilePathDest.c_str());
		runScorched3D(formatString("-startclient \"%s\"", customFilePathDest.c_str()), false);
		return true;
	}
	return false;
}

