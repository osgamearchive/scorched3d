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
	delete [] landscapes;
	landscapes = new wxCheckBox*[
		LandscapeDefinitions::instance()->getAllLandscapes().size()];

	wxSizer *sizer = new wxGridSizer(3, 3);
	topsizer->Add(sizer, 0, wxALL | wxALIGN_CENTER, 10);

	int i = 0;
	std::vector<LandscapeDefinition> &defns =
		LandscapeDefinitions::instance()->getAllLandscapes();
	std::vector<LandscapeDefinition>::iterator itor;
	for (itor = defns.begin();
		 itor != defns.end();
		 itor++, i++)
	{
		LandscapeDefinition &dfn = *itor;
		wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

		char fileName[256];
		strcpy(fileName, PKGDIR "data/landscapes/picture-none.bmp");
		if (::wxFileExists(dfn.picture.c_str()))
		{
			sprintf(fileName, PKGDIR "%s", dfn.picture.c_str());
		}

		wxImage image;
		if (image.LoadFile(_T(fileName), wxBITMAP_TYPE_BMP))
		{
			wxBitmap bitmap(image);
			wxStaticBitmap *staticBmp = new wxStaticBitmap(parent, -1, bitmap);
			staticBmp->SetToolTip(wxString(dfn.description.c_str()));
			boxSizer->Add(staticBmp, 0, wxALL, 2);
		}

		landscapes[i] = new wxCheckBox(parent, -1, dfn.name.c_str());
		boxSizer->Add(landscapes[i]);

		sizer->Add(boxSizer, 0, wxALL, 5);
	}
}
