////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/image.h>
#include <wx/filedlg.h>
#include <wxdialogs/MainDialog.h>
#include <engine/ModInfo.h>
#include <common/Defines.h>
#include <common/OptionsGame.h>

extern char scorched3dAppName[128];

enum
{
	ID_BUTTON_SCORCHED3D = 999,
	ID_BUTTON_CHOICE
};

class SingleChoiceFrameData : public wxObjectRefData
{
public:
	SingleChoiceFrameData(const char *f) : file(f) { }
	virtual ~SingleChoiceFrameData() { }

	std::string file;
};

class SingleChoiceFrame: public wxDialog
{
public:
	SingleChoiceFrame(ModInfo &info);

protected:
	void onChoiceButton(wxCommandEvent &event);
	void onScorchedButton(wxCommandEvent &event);

	ModInfo info_;

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SingleChoiceFrame, wxDialog)
    EVT_BUTTON(ID_BUTTON_SCORCHED3D,  SingleChoiceFrame::onScorchedButton)
    EVT_BUTTON(ID_BUTTON_CHOICE,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 1,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 2,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 3,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 4,  SingleChoiceFrame::onChoiceButton)
    EVT_BUTTON(ID_BUTTON_CHOICE + 5,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 6,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 7,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 8,  SingleChoiceFrame::onChoiceButton)
	EVT_BUTTON(ID_BUTTON_CHOICE + 9,  SingleChoiceFrame::onChoiceButton)
END_EVENT_TABLE()

SingleChoiceFrame::SingleChoiceFrame(ModInfo &info) :
	wxDialog(getMainDialog(), -1, wxString(scorched3dAppName, wxConvUTF8), 
	wxDefaultPosition, wxDefaultSize),
	info_(info)
{
	// Create the positioning sizer
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif
	setDataFileMod(info_.getName());
	addTitleToWindow(this, topsizer, 
		getDataFile("data/windows/scorched.bmp"),
		ID_BUTTON_SCORCHED3D);
	setDataFileMod("none");

	int useId = ID_BUTTON_CHOICE;
	wxFlexGridSizer *gridsizer = new wxFlexGridSizer(4, 2, 5, 5);
	std::list<ModInfo::MenuEntry>::iterator itor;
	for (itor = info_.getMenuEntries().begin();
		itor != info_.getMenuEntries().end();
		itor++)
	{
		ModInfo::MenuEntry &entry = (*itor);
		char *icon = (char *) entry.icon.c_str();
		char *desc = (char *) entry.description.c_str();

		wxObjectRefData *refData = new SingleChoiceFrameData(entry.gamefile.c_str());
		addButtonToWindow(useId++, 
			desc, icon, 
			this, gridsizer, refData);
	}


	topsizer->Add(gridsizer, 0, wxALIGN_CENTER | wxALL, 5);

	// Quit button
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, 
		wxT("Close")), 0, wxALL, 5);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// use the sizer for layout
	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void SingleChoiceFrame::onChoiceButton(wxCommandEvent &event)
{
	wxObject *object = event.GetEventObject();
	if (!object) return;
	SingleChoiceFrameData *data = 
		(SingleChoiceFrameData *) object->GetRefData();
	if (!data) return;
	const char *targetFilePath = data->file.c_str();

	EndModal(wxID_OK);
	runScorched3D(formatString("-startclient \"%s\"", targetFilePath), false);
}

void SingleChoiceFrame::onScorchedButton(wxCommandEvent &event)
{
	showURL(info_.getUrl());
}

bool showSingleChoiceDialog(ModInfo &mod)
{
	SingleChoiceFrame frame(mod);
	return (frame.ShowModal() == wxID_OK);
}
