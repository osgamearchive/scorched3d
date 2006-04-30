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

#include <dialogs/TutorialDialog.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWLabel.h>
#include <GLW/GLWWindowManager.h>
#include <GLW/GLWListView.h>
#include <client/MessageDisplay.h>
#include <client/ScorchedClient.h>
#include <common/Logger.h>
#include <common/LoggerI.h>
#include <common/Defines.h>
#include <time.h>

TutorialDialog *TutorialDialog::instance_ = 0;

TutorialDialog *TutorialDialog::instance()
{
	if (!instance_)
	{
		instance_ = new TutorialDialog;
	}
	return instance_;
}

TutorialDialog::TutorialDialog() : 
	GLWWindow("Tutorial", 440.0f, 120.0f, eTransparent |eSmallTitle,
		"The ingame tutorial.")
{
	GLWListView *listView = new GLWListView(0.0f, 0.0f, 440.0f, 95.0f, -1, 12.0f);
	addWidget(listView, 0, SpaceAll, 10.0f);

	/*GLWPanel *buttonPanel = new GLWPanel(0.0f, 0.0f, 0.0f, 0.0f, false, false);
	GLWButton *okButton = new GLWTextButton("Next", 10, 45, 105, this, 
		GLWButton::ButtonFlagOk | GLWButton::ButtonFlagCenterX);
	okId_ = okButton->getId();
	buttonPanel->addWidget(okButton);
	buttonPanel->setLayout(GLWPanel::LayoutHorizontal);
	addWidget(buttonPanel, 0, SpaceAll | AlignRight, 10.0f);*/

	listView->addText("Test viewer capibility to handle basic use of 'textLength' and 'lengthAdjust' attributes. "
		"There are four pairs of sub-tests. Each pair of sub-tests consists of the same two strings: \"Line to Stretch\" on the left, and \"this is a line to squeeze\" on the right."
		"The first (topmost) pair contains no occurrences of the textLength and lengthAdjust attributes in the 'text' elements. The red reference line under each of the top two strings indicates the approximate length of the strings. Since the lengths are not constrained by the 'textLength' attribute, small variations of the lengths are permissible. ");

	setLayout(GLWPanel::LayoutVerticle);
	layout();
}

TutorialDialog::~TutorialDialog()
{

}

void TutorialDialog::display()
{
	GLWWindow::display();
}

void TutorialDialog::buttonDown(unsigned int id)
{
	if (id == okId_)
	{

	}
}

