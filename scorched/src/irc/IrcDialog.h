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

#ifdef _NO_SERVER_ASE_

#if !defined(__INCLUDE_NetDialogh_INCLUDE__)
#define __INCLUDE_NetDialogh_INCLUDE__

#include <common/Defines.h>
#include <common/Logger.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/image.h>
#include <windows.h>

class NetListControl : public wxListCtrl
{
public:
	NetListControl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~NetListControl();

	virtual wxString OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const;
	virtual int OnGetItemImage(long WXUNUSED(item)) const;
};


class NetLanFrame: public wxDialog , public LoggerI
{
public:
	NetLanFrame();
	~NetLanFrame();

	virtual bool TransferDataToWindow();
	virtual bool TransferDataFromWindow();
	virtual void logMessage(const char *time, const char *message,
				unsigned int playerId);

	void onRefreshLanButton();
	void onRefreshNETButton();
	void onClearButton();
	void onSelectServer();
	void onTimer();
	void onServerChanged();

private:
	DECLARE_EVENT_TABLE()
	wxTimer timer_;
	int entries_;

};

bool showNetLanDialog();

#endif
#endif /* _NO_SERVER_ASE_ */