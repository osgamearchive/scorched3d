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

#include <client/ScorchedClient.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLMenuEntry.h>
#include <GLW/GLWidget.h>

static Vector color(0.9f, 0.9f, 1.0f);
static Vector itemcolor(0.1f, 0.1f, 0.4f);
static const float menuItemHeight = 20.0f;

GLMenuEntry::GLMenuEntry(
	char *name, float width, 
	unsigned int state,
	GLMenuI *selectFn, 
	GLMenuI *textFn, 
	GLMenuI *subMenuFn,
	GLMenuI *enabledFn) :
	left_(0.0f), width_(width), height_(0.0f),
	selectFn_(selectFn), textFn_(textFn), 
	subMenuFn_(subMenuFn), enabledFn_(enabledFn),
	menuName_(name), state_(state), selected_(false)
{
}

GLMenuEntry::~GLMenuEntry()
{
}

void GLMenuEntry::draw(GLFont2d &font, float currentTop, float currentLeft)
{
	left_ = currentLeft;
	height_ = 0.0f;

	// Draw the menu backdrops
	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glColor4f(0.4f, 0.6f, 0.8f, 0.6f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(currentLeft + 10.0f, currentTop - 10.0f);
			glVertex2f(currentLeft + 10.0f, currentTop - menuItemHeight);
			GLWidget::drawRoundBox(currentLeft, currentTop - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
			glVertex2f(currentLeft + 10.0f, currentTop - menuItemHeight);
		glEnd();

		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWidget::drawRoundBox(currentLeft, currentTop - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	// Get and print the menu title text
	char *menuTitle = 0;
	if (textFn_)
	{
		// If there is a fn defined to draw the menu title use it
		menuTitle = (char *) textFn_->getMenuText(menuName_.c_str());
	}
	if (!menuTitle)
	{
		// Else default to the name of the menu
		menuTitle = (char *) menuName_.c_str();
	}

	font.draw((selected_?color:itemcolor), 12, currentLeft + 5.0f, 
		currentTop - 15.0f, 0.0f, menuTitle);
}

bool GLMenuEntry::click(float currentTop, int x, int y)
{
	if (y > currentTop - menuItemHeight &&
		x>left_ && x<left_ + width_) 
	{
		selected_ = true;
		
		// Get the contents of the menu
		if (subMenuFn_)
		{
			menuItems_.clear();
			subMenuFn_->getMenuItems(menuName_.c_str(), menuItems_);
		}
	
		// Show the menu
		std::list<GLWSelectorEntry> entries;
		std::list<GLMenuItem>::iterator itor;
		for (itor = menuItems_.begin();
			itor != menuItems_.end();
			itor++)
		{
			GLMenuItem &item = (*itor);
			entries.push_back(
				GLWSelectorEntry(
					item.getText(),
					item.getToolTip(),
					item.getSelected()
					)
				);
		}
		GLWSelector::instance()->showSelector(
			this, 
			left_, currentTop - (menuItemHeight + 10.0f), 
			entries,
			state_);
	}

	return false;
}

void GLMenuEntry::addMenuItem(GLMenuItem &item)
{
	menuItems_.push_back(item);
}

void GLMenuEntry::noItemSelected()
{
	selected_ = false;
}

void GLMenuEntry::itemSelected(GLWSelectorEntry *entry, int position)
{
	selected_ = false;
	selectFn_->menuSelection(menuName_.c_str(), position, entry->getText());
}

