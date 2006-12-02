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

#include <wxdialogs/ProgressBarDialog.h>

ProgressBarDialog::ProgressBarDialog(wxWindow *parent, const char *title) :
	wxDialog(parent, -1, wxString(title, wxConvUTF8), 
		wxDefaultPosition, wxDefaultSize)
{
#ifdef _WIN32
	// Set the frame's icon
	wxIcon icon(getDataFile("data/windows/tank2.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);
#endif
	wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

	text_ = new wxStaticText(this, -1, wxT(""));
	gauge_ = new wxGauge(this, -1, 100, wxDefaultPosition, wxSize(400, -1));

	topsizer->Add(text_, 0, wxLEFT | wxRIGHT | wxTOP, 10);
	topsizer->Add(gauge_, 0, wxALL, 10);

	SetSizer(topsizer); 
	topsizer->SetSizeHints(this); // set size hints to honour minimum size

	setUser(this);

	CentreOnScreen();
}

void ProgressBarDialog::progressChange(const char *op, const float percentage)
{
	text_->SetLabel(wxString(op, wxConvUTF8));
	gauge_->SetValue((int) percentage);
	::wxSafeYield();
}

ProgressBarDialog *createProgressBarDialog()
{
	ProgressBarDialog *frame = new ProgressBarDialog(0, "Scorched3D Server");
	frame->Show(true);
	return frame;
}
