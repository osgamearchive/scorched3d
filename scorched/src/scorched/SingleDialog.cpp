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

#include <wx/wx.h>
#include <wx/image.h>
#include <client/ScorchedClient.h>
#include <scorched/MainDialog.h>
#include <scorched/SettingsDialog.h>
#include <common/Defines.h>
#include <common/OptionsParam.h>

extern char scorched3dAppName[128];
static char *easyFilePath = PKGDIR "data/singleeasy.xml";
static char *normalFilePath = PKGDIR "data/singlenormal.xml";
static char *hardFilePath = PKGDIR "data/singlehard.xml";
static char *customFilePath = PKGDIR "data/singlecustom.xml";

enum
{
	ID_BUTTON_EASY = 1,
	ID_BUTTON_NORMAL,
	ID_BUTTON_HARD,
	ID_BUTTON_CUSTOM
};

class SingleFrame: public wxDialog
{
public:
	SingleFrame();

	void onEasyButton();
	void onNormalButton();
	void onHardButton();
	void onCustomButton();

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(SingleFrame, wxDialog)
    EVT_BUTTON(ID_BUTTON_EASY,  SingleFrame::onEasyButton)
	EVT_BUTTON(ID_BUTTON_NORMAL,  SingleFrame::onNormalButton)
	EVT_BUTTON(ID_BUTTON_HARD,  SingleFrame::onHardButton)
	EVT_BUTTON(ID_BUTTON_CUSTOM,  SingleFrame::onCustomButton)
END_EVENT_TABLE()

SingleFrame::SingleFrame() :
	wxDialog(getMainDialog(), -1, scorched3dAppName, wxPoint(0,0), wxSize(390, 290))
{
	CentreOnScreen();

#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(PKGDIR "data/windows/tank2.ico", wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif

	{
		addButtonToWindow(ID_BUTTON_EASY, 20, 20,
			"Start an easy single player game.\n"
			"Play a quick game against easy computer players.", 
			PKGDIR "data/windows/tank-easy.bmp", this);
	}
	{
		addButtonToWindow(ID_BUTTON_NORMAL, 20, 75,
			"Start an normal single player game.\n"
			"Play a quick game against normal strength computer players.", 
			PKGDIR "data/windows/tank-med.bmp", this);
	}
	{
		addButtonToWindow(ID_BUTTON_HARD, 20, 130,
			"Start an hard single player game.\n"
			"Play a quick game against hard computer players.", 
			PKGDIR "data/windows/tank-hard.bmp", this);
	}
	{
		addButtonToWindow(ID_BUTTON_CUSTOM, 20, 185,
			"Start an custom single or multi-player game.\n"
			"Choose the opponents to play against.", 
			PKGDIR "data/windows/tank2.bmp", this);
	}

	new wxButton(this, wxID_CANCEL,
		"Cancel",
		wxPoint((int) 300, (int) 230), wxSize((int) 75, (int) 21));
}

void SingleFrame::onEasyButton()
{
	ScorchedClient::instance()->getOptionsGame().readOptionsFromFile(easyFilePath);
	ScorchedClient::instance()->getOptionsGame().writeOptionsToFile(easyFilePath);
	OptionsParam::instance()->getSinglePlayer() = true;
	EndModal(wxID_OK);
}

void SingleFrame::onNormalButton()
{
	ScorchedClient::instance()->getOptionsGame().readOptionsFromFile(normalFilePath);
	ScorchedClient::instance()->getOptionsGame().writeOptionsToFile(normalFilePath);
	OptionsParam::instance()->getSinglePlayer() = true;
	EndModal(wxID_OK);
}

void SingleFrame::onHardButton()
{
	ScorchedClient::instance()->getOptionsGame().readOptionsFromFile(hardFilePath);
	ScorchedClient::instance()->getOptionsGame().writeOptionsToFile(hardFilePath);
	OptionsParam::instance()->getSinglePlayer() = true;
	EndModal(wxID_OK);
}

void SingleFrame::onCustomButton()
{
	ScorchedClient::instance()->getOptionsGame().readOptionsFromFile(customFilePath);

	if (showSettingsDialog(false, ScorchedClient::instance()->getContext()))
	{
		ScorchedClient::instance()->getOptionsGame().writeOptionsToFile(customFilePath);
		OptionsParam::instance()->getSinglePlayer() = true;
		EndModal(wxID_OK);
	}
}

bool showSingleDialog()
{
	SingleFrame frame;
	return (frame.ShowModal() == wxID_OK);
}
