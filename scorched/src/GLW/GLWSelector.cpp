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

#include <GLW/GLWSelector.h>
#include <GLW/GLWFont.h>
#include <GLEXT/GLState.h>
#include <client/ScorchedClient.h>

GLWSelectorEntry::GLWSelectorEntry(const char *text, 
			GLWTip *tooltip, 
			bool selected,
			GLTexture *icon) : 
	text_(text),
	tip_(tooltip),
	selected_(selected),
	icon_(icon)
{

}

GLWSelector *GLWSelector::instance_ = 0;

GLWSelector *GLWSelector::instance()
{
	if (!instance_)
	{
		instance_ = new GLWSelector();
	}
	return instance_;
}

GLWSelector::GLWSelector() :
	GLWWindow("", 0.0f, 0.0f, 0.0f, 0.0f, 0, ""), 
	visible_(false)
{
}

GLWSelector::~GLWSelector()
{
}

void GLWSelector::draw()
{
	if (!visible_) return;

	GLFont2d &font = *GLWFont::instance()->getLargePtFont();
	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();

	// Get the height and width of the selector
	float selectedHeight = 0.0f;
	float selectedWidth = 0.0f;
	float indent = 0.0f;
	std::list<GLWSelectorEntry>::iterator itor;
	for (itor =	entries_.begin();
		itor != entries_.end();
		itor++)
	{
		// Get height
		GLWSelectorEntry &item = (*itor);
		if (item.getText()[0] == '-') selectedHeight += 8.0f;
		else selectedHeight += 18.0f;

		// Get width
		float currentwidth = 
			(float) font.getWidth(12, (char *) item.getText()) + 20.0f;
		if (item.getSelected()) indent = 10.0f;
		if (currentwidth > selectedWidth) selectedWidth = currentwidth;
	}
	selectedWidth += indent;
	drawW_ = selectedWidth;
	drawH_ = selectedHeight;

	// Draw the background
	{
		float drop = 12.0f;
		GLState currentStateBlend(GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);	
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(drawX_ + 20.0f, 
				drawY_ - 25.0f - drop);
			glVertex2f(drawX_ + 20.0f, 
				drawY_ - drawH_ - drop);
			GLWVisibleWidget::drawRoundBox(
				drawX_, drawY_ - drawH_ - drop, 
				drawW_, drawH_ - drop, 20.0f);
			glVertex2f(drawX_ + 20.0f, 
				drawY_ - drawH_ - drop);
		glEnd();

		glColor4f(0.9f, 0.9f, 1.0f, 0.5f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWVisibleWidget::drawRoundBox(
				drawX_, drawY_ - drawH_  - drop, 
				drawW_, drawH_ - drop, 20.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	// Draw the menu items
	float currentTop = drawY_;
	for (itor =	entries_.begin();
		itor != entries_.end();
		itor++)
	{
		GLWSelectorEntry &item = (*itor);

		// Check if the item is a seperator
		if (item.getText()[0] == '-')
		{
			// Draw a seperator
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex2f(drawX_ + 5.0f, currentTop - 5.0f);
				glVertex2f(drawX_ + drawW_ - 5.0f, currentTop - 5.0f);
			glEnd();
			currentTop -= 8.0f;
		}
		else
		{
			// Draw the actual line
			bool selected = false;
			if (drawX_ < mouseX && mouseX < drawX_ + drawW_ &&
				currentTop - 18.0f < mouseY && mouseY < currentTop)
			{
				selected = true;
			}

			if (item.getToolTip())
			{
				GLWToolTip::instance()->addToolTip(item.getToolTip(),
					drawX_, currentTop - 18.0f, drawW_, 18.0f);
			}

			static Vector color(0.9f, 0.9f, 1.0f);
			static Vector itemcolor(0.1f, 0.1f, 0.4f);
			if (item.getSelected())
			{
				font.draw(selected?color:itemcolor, 12, drawX_ + 5.0f, 
					currentTop - 16.0f, 0.0f, "x");
			}
			font.draw(selected?color:itemcolor, 12, drawX_ + indent + 10.0f, 
				currentTop - 16.0f, 0.0f, (char *) item.getText());
			currentTop -= 18.0f;
		}
	}
}

void GLWSelector::showSelector(GLWSelectorI *user,
							   float x, float y,
							   std::list<GLWSelectorEntry> &entries)
{
	drawX_ = x;
	drawY_ = y;
	drawW_ = 0;
	drawH_ = 0;
	user_ = user;
	entries_ = entries;
	visible_ = true;
	w_ = 100000.0f;
	h_ = 100000.0f;
}

void GLWSelector::hideSelector()
{
	visible_ = false;
}


void GLWSelector::mouseDown(float x, float y, bool &hitMenu)
{
	// Override default window behaviour
	if (visible_)
	{
		// If visible hide this window
		visible_ = false;
		hitMenu = true;
	}
}

void GLWSelector::mouseUp(float x, float y, bool &skipRest)
{
	// Override and disable default window behaviour
}

void GLWSelector::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	// Override and disable default window behaviour
}

void GLWSelector::keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest)
{
	// Override and disable default window behaviour
}
