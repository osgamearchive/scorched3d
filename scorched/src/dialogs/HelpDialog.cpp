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


#include <dialogs/HelpDialog.h>
#include <GLW/GLWFileView.h>
#include <GLW/GLWTextButton.h>
#include <common/WindowManager.h>

HelpDialog *HelpDialog::instance_ = 0;

HelpDialog *HelpDialog::instance()
{
	if (!instance_)
	{
		instance_ = new HelpDialog;
	}
	return instance_;
}

HelpDialog::HelpDialog() : 
	GLWWindow("Help", 0.0f, 0.0f, 340.0f, 230.0f, 0)
{
	needCentered_ = true;
	addWidget(new GLWFileView(PKGDIR "README", 10, 40, 320, 180));
	okId_ = addWidget(new GLWTextButton(" Ok", 275, 10, 55, this, true))->getId();
}

HelpDialog::~HelpDialog()
{
}

void HelpDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		WindowManager::instance()->hideWindow(id_);
	}
}
