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

#include <dialogs/HUDDialog.h>
#include <common/OptionsDisplay.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWWindowManager.h>

HUDDialog *HUDDialog::instance_ = 0;

HUDDialog *HUDDialog::instance()
{
	if (!instance_)
	{
		instance_ = new HUDDialog;
	}
	return instance_;
}

HUDDialog::HUDDialog() :
	GLWWindow("HUD Settings", 10.0f, 10.0f, 300.0f, 70.0f, eSmallTitle,
		"Allow the user to change hud settings")
{
	needCentered_ = true;

	nameBox_ = new GLWCheckBoxText(0.0f, 0.0f, "Show Name");
	addWidget(nameBox_, 0, SpaceLeft | SpaceRight | SpaceTop, 10.0f);
	sightBox_ = new GLWCheckBoxText(0.0f, 0.0f, "Show Sight");
	addWidget(sightBox_, 0, SpaceLeft | SpaceRight | SpaceTop, 10.0f);
	colorBox_ = new GLWCheckBoxText(0.0f, 0.0f, "Show Color");
	addWidget(colorBox_, 0, SpaceLeft | SpaceRight | SpaceTop, 10.0f);
	healthBox_ = new GLWCheckBoxText(0.0f, 0.0f, "Show Health");
	addWidget(healthBox_, 0, SpaceLeft | SpaceRight | SpaceTop, 10.0f);

	GLWPanel *buttonPanel = new GLWPanel(0.0f, 0.0f, 0.0f, 0.0f, false, false);
	GLWButton *cancelButton = new GLWTextButton("Cancel", 95, 10, 105, this, 
		GLWButton::ButtonFlagCancel | GLWButton::ButtonFlagCenterX);
	cancelId_ = cancelButton->getId();
	buttonPanel->addWidget(cancelButton, 0, SpaceRight, 10.0f);
	GLWButton *okButton = new GLWTextButton("Ok", 235, 10, 55, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX);
	okId_ = okButton->getId();
	buttonPanel->addWidget(okButton);
	buttonPanel->setLayout(GLWPanel::LayoutHorizontal);
	addWidget(buttonPanel, 0, SpaceAll, 10.0f);

	setLayout(GLWPanel::LayoutVerticle);
	layout();
}

HUDDialog::~HUDDialog()
{
}

void HUDDialog::windowDisplay()
{
	nameBox_->getCheckBox().setState(
		OptionsDisplay::instance()->getDrawPlayerNames());
	sightBox_->getCheckBox().setState(
		OptionsDisplay::instance()->getDrawPlayerSight());
	colorBox_->getCheckBox().setState(
		OptionsDisplay::instance()->getDrawPlayerColor());
	healthBox_->getCheckBox().setState(
		OptionsDisplay::instance()->getDrawPlayerHealth());
}

void HUDDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{
		OptionsDisplay::instance()->setDrawPlayerNames(
			nameBox_->getCheckBox().getState());
		OptionsDisplay::instance()->setDrawPlayerSight(
			sightBox_->getCheckBox().getState());
		OptionsDisplay::instance()->setDrawPlayerColor(
			colorBox_->getCheckBox().getState());
		OptionsDisplay::instance()->setDrawPlayerHealth(
			healthBox_->getCheckBox().getState());
	}
	GLWWindowManager::instance()->hideWindow(getId());
}
