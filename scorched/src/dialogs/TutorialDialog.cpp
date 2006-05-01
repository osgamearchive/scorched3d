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
	GLWWindow("Tutorial", 160.0f, -120.0f, 
		470.0f, 120.0f, eTransparent | eNoTitle,
		"The ingame tutorial."),
	file_(true)
{
	Vector listColor(0.0f, 0.0f, 0.0f);
	listView_ = new GLWListView(0.0f, 0.0f, 470.0f, 95.0f, -1, 12.0f, 16.0f);
	listView_->setColor(listColor);
	listView_->setHandler(this);
	addWidget(listView_, 0, SpaceAll, 10.0f);

	if (!file_.readFile(getDataFile("data/tutorial.xml")) ||
		!file_.getRootNode())
	{
		dialogMessage("Scorched Tutorial", formatString(
					  "Failed to parse \"data/tutorial.xml\"\n%s", 
					  file_.getParserError()));
	}
	else
	{
		std::string start;
		if (file_.getRootNode()->getNamedParameter("start", start, false))
		{
			url(start.c_str());
		}
	}

	setLayout(GLWPanel::LayoutVerticle);
	layout();
}

TutorialDialog::~TutorialDialog()
{

}

void TutorialDialog::showPage(XMLNode *node)
{
	listView_->clear();
	listView_->addXML(node);
}

void TutorialDialog::display()
{
	GLWWindow::display();
	listView_->resetPosition();
}

void TutorialDialog::buttonDown(unsigned int id)
{
}

void TutorialDialog::url(const char *url)
{
	XMLNode *startNode;
	if (file_.getRootNode()->getNamedChild(url, startNode, false))
	{
		showPage(startNode);
	}
}
