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
#include <GLW/GLWVisibleWidget.h>

static Vector color(0.9f, 0.9f, 1.0f);
static Vector itemcolor(0.1f, 0.1f, 0.4f);
static const float menuItemHeight = 20.0f;

GLMenuEntry::GLMenuEntry(char *name, float width, 
						 GLMenuI *selectFn, GLMenuI *textFn, 
						 GLMenuI *subMenuFn, GLMenuI *enabledFn) :
	depressed_(false), left_(0.0f), width_(width), height_(0.0f),
	selectFn_(selectFn), textFn_(textFn), 
	subMenuFn_(subMenuFn), enabledFn_(enabledFn),
	menuName_(name), selectedWidth_(0.0f)
{

}

GLMenuEntry::~GLMenuEntry()
{

}

void GLMenuEntry::collapse()
{
	depressed_ = false;
	left_ = 0.0f;
	height_ = 0.0f;
}

void GLMenuEntry::draw(GLFont2d &font, float currentTop, float currentLeft)
{
	left_ = currentLeft;
	height_ = 0.0f;

	// Draw the menu backdrops
	if (depressed_)
	{
		drawDepressed(font, currentTop, currentLeft);
	}
	drawNonDepressed(currentTop, currentLeft);

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

	font.draw(depressed_?color:itemcolor, 12, currentLeft + 5.0f, 
		currentTop - 15.0f, 0.0f, menuTitle);
}

void GLMenuEntry::drawNonDepressed(float currentTop, float currentLeft)
{
	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(0.4f, 0.6f, 0.8f, 0.6f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(currentLeft + 10.0f, currentTop - 10.0f);
			glVertex2f(currentLeft + 10.0f, currentTop - menuItemHeight);
			GLWVisibleWidget::drawRoundBox(currentLeft, currentTop - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
			glVertex2f(currentLeft + 10.0f, currentTop - menuItemHeight);
		glEnd();

		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWVisibleWidget::drawRoundBox(currentLeft, currentTop - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}
}

void GLMenuEntry::drawDepressed(GLFont2d &font, float currentTop, float currentLeft)
{
	// Get the contents of the menu
	if (subMenuFn_)
	{
		menuItems_.clear();
		subMenuFn_->getMenuItems(menuName_.c_str(), menuItems_);
	}

	// Get the height of the menu
	selectedWidth_ = 0.0f;
	bool oneSelected = false;
	GLfloat lowerHeight = 0.0f;
	std::list<GLMenuItem>::iterator itor;
	for (itor =	menuItems_.begin();
		itor != menuItems_.end();
		itor++)
	{
		GLMenuItem &item = (*itor);
		if (item.getSelected()) oneSelected = true;
		if (item.getText()[0] == '-') lowerHeight += 8.0f;
		else lowerHeight += 18.0f;

		float currentwidth = 
			(float) font.getWidth(12, (char *) item.getText()) + 10.0f;
		if (currentwidth > selectedWidth_) selectedWidth_ = currentwidth;
	}
	float side = (oneSelected?20.0f:10.0f);
	selectedWidth_ += side + 5.0f;

	lowerHeight = lowerHeight + menuItemHeight + 5.0f;
	height_ = currentTop - lowerHeight;

	{
		float drop = 12.0f;
		GLState currentStateBlend(GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(0.4f, 0.6f, 0.8f, 0.6f);	
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(currentLeft + 20.0f, currentTop - 25.0f - drop);
			glVertex2f(currentLeft + 20.0f, currentTop - lowerHeight - drop);
			GLWVisibleWidget::drawRoundBox(currentLeft, currentTop - lowerHeight - drop, 
				selectedWidth_, lowerHeight - drop, 10.0f);
			glVertex2f(currentLeft + 20.0f, currentTop - lowerHeight - drop);
		glEnd();

		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWVisibleWidget::drawRoundBox(currentLeft, currentTop - lowerHeight  - drop, 
				selectedWidth_, lowerHeight - drop, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();

	// Draw the menu items
	currentTop -= menuItemHeight + 7.0f;
	for (itor = menuItems_.begin();
		itor != menuItems_.end();
		itor++)
	{
		GLMenuItem &item = (*itor);
		if (item.getText()[0] == '-')
		{
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex2f(currentLeft + 5.0f, currentTop - 5.0f);
				glVertex2f(currentLeft + selectedWidth_ - 5.0f, currentTop - 5.0f);
			glEnd();
			currentTop -= 8.0f;
		}
		else
		{
			bool selected = false;
			if (currentLeft < mouseX && mouseX < currentLeft + selectedWidth_ &&
				currentTop - 18.0f < mouseY && mouseY < currentTop)
			{
				selected = true;
			}

			if (item.getToolTip())
			{
				GLWToolTip::instance()->addToolTip(item.getToolTip(),
					currentLeft, currentTop - 18.0f, selectedWidth_, 18.0f);
			}
			if (item.getSelected())
			{
				font.draw(selected?color:itemcolor, 12, currentLeft + 5.0f, 
					currentTop - 16.0f, 0.0f, "x");
			}
			font.draw(selected?color:itemcolor, 12, currentLeft + side, 
				currentTop - 16.0f, 0.0f, (char *) item.getText());
			currentTop -= 18.0f;
		}
	}
}

bool GLMenuEntry::click(float currentTop, int x, int y)
{
	if (y > currentTop - menuItemHeight) 
	{
		return clickTitle(currentTop, x);
	}
	else
	{
		return clickMenu(currentTop, x, y);
	}

	return false;
}

bool GLMenuEntry::clickTitle(float currentTop, int x)
{
	bool thisMenu = (x>left_ && x<left_ + width_);
	if (thisMenu && !depressed_)
	{
		depressed_ = true;
	}
	else
	{
		collapse();
	}

	return thisMenu;
}

bool GLMenuEntry::clickMenu(float currentTop, int x, int y)
{
	if (!depressed_) return false;

	bool thisMenu = (x>left_ && x<left_ + selectedWidth_ && y>height_);
	if (thisMenu)
	{
		GLfloat lowerHeight = currentTop - menuItemHeight - 7.0f;
		std::list<GLMenuItem> items = menuItems_;
		std::list<GLMenuItem>::iterator itor;
		int i = 0;
		for (itor =	items.begin();
			itor != items.end();
			itor++, i++)
		{
			GLMenuItem &item = (*itor);
			if (item.getText()[0] == '-') lowerHeight -= 8.0f;
			else
			{
				lowerHeight -= 18.0f;
				if (y > lowerHeight) 
				{
					selectFn_->menuSelection(menuName_.c_str(), i, item.getText());
					collapse();
					return true;
				}
			}
		}
	}
	collapse();
	return true;
}

void GLMenuEntry::addMenuItem(GLMenuItem &item)
{
	menuItems_.push_back(item);
}
