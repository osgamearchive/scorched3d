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

#include <scorched/ListDialog.h>
#include <common/Defines.h>

ListDialog::ListDialog(wxWindow *parent, const char *title) :
	wxDialog(parent, -1, wxString(title, wxConvUTF8), 
		wxDefaultPosition, wxDefaultSize)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	list_ = new wxListBox(this, -1, wxDefaultPosition, wxSize(400, 200), 0, 0, wxLB_SORT);
	topsizer->Add(list_, 0, wxALL, 10);

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_OK, wxT("Ok")), 0, wxALL, 10);
	topsizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	CentreOnScreen();
}

void ListDialog::addItem(const char *item)
{
	list_->Append(wxString(item, wxConvUTF8));
}

