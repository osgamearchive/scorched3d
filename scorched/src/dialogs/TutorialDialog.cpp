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
#include <XML/XMLFile.h>
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
	GLWWindow("Tutorial", 160.0f, -140.0f, 470.0f, 120.0f, eTransparent |eSmallTitle,
		"The ingame tutorial.")
{
	Vector listColor(0.0f, 0.0f, 0.0f);
	listView_ = new GLWListView(0.0f, 0.0f, 470.0f, 95.0f, -1, 12.0f, true);
	listView_->setColor(listColor);
	addWidget(listView_, 0, SpaceAll, 10.0f);

	XMLFile file(true);
	if (!file.readFile(getDataFile("data/tutorial.xml")) ||
		!file.getRootNode())
	{
		dialogMessage("Scorched Tutorial", formatString(
					  "Failed to parse \"data/tutorial.xml\"\n%s", 
					  file.getParserError()));
	}
	else
	{
		std::string start;
		if (file.getRootNode()->getNamedParameter("start", start, false))
		{
			XMLNode *startNode;
			if (file.getRootNode()->getNamedChild(start.c_str(), startNode, false))
			{
				listView_->addXML(startNode);
			}
		}
	}

	setLayout(GLWPanel::LayoutVerticle);
	layout();
}

TutorialDialog::~TutorialDialog()
{

}

void TutorialDialog::display()
{
	GLWWindow::display();
	listView_->resetPosition();
}

void TutorialDialog::buttonDown(unsigned int id)
{
}

