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

#include "SettingsLand-def.cpp"

static void createControls(wxWindow *parent, wxSizer *topsizer)
{
	// Re-read just in case a new mod has been loaded
	LandscapeDefinitions::instance()->clearLandscapeDefinitions();
	DIALOG_ASSERT(LandscapeDefinitions::instance()->readLandscapeDefinitions());

	delete [] landscapes;
	landscapes = new wxCheckBox*[
		LandscapeDefinitions::instance()->getAllLandscapes().size()];

	wxScrolledWindow *scrolledWindow = new wxScrolledWindow(parent, -1,
		wxDefaultPosition, wxSize(340, 320));

	wxSizer *sizer = new wxFlexGridSizer(3, 3);
	int i = 0;
	std::list<LandscapeDefinitionsEntry> &defns =
		LandscapeDefinitions::instance()->getAllLandscapes();
	std::list<LandscapeDefinitionsEntry>::iterator itor;
	for (itor = defns.begin();
		 itor != defns.end();
		 itor++, i++)
	{
		LandscapeDefinitionsEntry &dfn = *itor;
		wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

		const char *fileName = getDataFile("data/landscapes/%s", dfn.picture.c_str());
		if (!::wxFileExists(fileName))
		{
			fileName = getDataFile("data/landscapes/picture-none.bmp");
		}

		wxImage image;
		if (image.LoadFile(fileName, wxBITMAP_TYPE_BMP))
		{
			wxBitmap bitmap(image);
			wxStaticBitmap *staticBmp = new wxStaticBitmap(scrolledWindow, -1, bitmap);
			staticBmp->SetToolTip(wxString(dfn.description.c_str()));
			boxSizer->Add(staticBmp, 0, wxALL, 2);
		}

		landscapes[i] = new wxCheckBox(scrolledWindow, -1, dfn.name.c_str());
		boxSizer->Add(landscapes[i]);

		sizer->Add(boxSizer, 0, wxALL, 5);
	}

	scrolledWindow->SetAutoLayout(TRUE);
	scrolledWindow->SetSizer(sizer);
	wxSize minSize = sizer->CalcMin();
	scrolledWindow->SetScrollbars(0, 10, 50, minSize.GetHeight() / 10);
	topsizer->Add(scrolledWindow, 0, wxALL | wxALIGN_CENTER, 10);
}

