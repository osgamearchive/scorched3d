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

#include <dialogs/SaveDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWLabel.h>
#include <GLW/GLWWindowManager.h>
#include <client/ScorchedClient.h>
#include <client/ClientSave.h>
#include <common/Logger.h>
#include <common/Defines.h>
#include <time.h>

SaveDialog *SaveDialog::instance_ = 0;

SaveDialog *SaveDialog::instance()
{
	if (!instance_)
	{
		instance_ = new SaveDialog;
	}
	return instance_;
}

SaveDialog::SaveDialog() : 
	GLWWindow("Save", 210.0f, 80.0f, 0,
		"Allows the player to save the game.")
{
	okId_ = addWidget(new GLWTextButton("Save", 95, 10, 105, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX))->getId();
	cancelId_ = addWidget(new GLWTextButton("Cancel", 10, 45, 190, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX))->getId();
}

SaveDialog::~SaveDialog()
{

}

void SaveDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		const char *saveFile = getSaveFile("saved-%i", time(0));
		if (saveClient(saveFile))
		{
			Logger::log(0, "Game saved as %s.", saveFile);
		}
		GLWWindowManager::instance()->hideWindow(id_);
	}
	else if (id == cancelId_)
	{
		GLWWindowManager::instance()->hideWindow(id_);
	}
}

