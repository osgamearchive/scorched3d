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
#include <GLEXT/GLViewPort.h>
#include <client/ScorchedClient.h>
#include <common/Defines.h>

GLWSelectorEntry::GLWSelectorEntry(const char *text, 
			GLWTip *tooltip, 
			bool selected,
			GLTexture *icon,
			void *userData) : 
	text_(text),
	tip_(tooltip),
	selected_(selected),
	icon_(icon),
	userData_(userData)
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
	visible_(false), showState_(0)
{
}

GLWSelector::~GLWSelector()
{
}

void GLWSelector::draw()
{
	if (showState_ != 0 &&
		ScorchedClient::instance()->getGameState().getState() != showState_)
	{
		hideSelector();
	}
	if (!visible_) return;

	// Get the height and width of the selector
	GLFont2d &font = *GLWFont::instance()->getLargePtFont();
	float selectedHeight = 10.0f; // Padding
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
		if (item.getSelected()) indent = MAX(indent, 10.0f);
		if (item.getIcon()) indent = MAX(indent, 16.0f);	

		if (currentwidth > selectedWidth) selectedWidth = currentwidth;
	}
	selectedWidth += indent;

	float selectedX = drawX_;
	float selectedY = drawY_;
	if (selectedX + selectedWidth > GLViewPort::getWidth())
	{
		selectedX -= (selectedX + selectedWidth) - GLViewPort::getWidth();
	}
	else if (selectedX < 0.0f) selectedX = 0.0f;
	if (selectedY - selectedHeight < 0)
	{
		selectedY -= (selectedY - selectedHeight);
	}

	selectedWidth_ = selectedWidth;
	selectedHeight_ = selectedHeight;
	selectedX_ = selectedX;
	selectedY_ = selectedY;

	drawMain(indent);
}

void GLWSelector::drawMain(float indent)
{
	GLState currentStateBlend(GLState::TEXTURE_OFF | 
		GLState::DEPTH_OFF | GLState::BLEND_ON);
	GLFont2d &font = *GLWFont::instance()->getLargePtFont();
	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();

	// Draw the background
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(selectedX_ + 20.0f, 
				selectedY_ - 25.0f + 5.0f);
			glVertex2f(selectedX_ + 20.0f, 
				selectedY_ - selectedHeight_ + 5.0f);
			GLWVisibleWidget::drawRoundBox(
				selectedX_, selectedY_ - selectedHeight_ + 5.0f, 
				selectedWidth_, selectedHeight_, 10.0f);
			glVertex2f(selectedX_ + 20.0f, 
				selectedY_ - selectedHeight_ + 5.0f);
		glEnd();

		glColor4f(0.9f, 0.9f, 1.0f, 0.5f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWVisibleWidget::drawRoundBox(
				selectedX_, selectedY_ - selectedHeight_ + 5.0f, 
				selectedWidth_, selectedHeight_, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	// Draw the menu items
	float currentTop = selectedY_;
	std::list<GLWSelectorEntry>::iterator itor;
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
				glVertex2f(selectedX_ + 5.0f, currentTop - 5.0f);
				glVertex2f(selectedX_ + selectedWidth_ - 5.0f, currentTop - 5.0f);
			glEnd();
			currentTop -= 8.0f;
		}
		else
		{
			// Draw the actual line
			bool selected = false;
			if (selectedX_ < mouseX && mouseX < selectedX_ + selectedWidth_ &&
				currentTop - 18.0f <= mouseY && mouseY < currentTop)
			{
				selected = true;
			}

			if (item.getToolTip())
			{
				GLWToolTip::instance()->addToolTip(item.getToolTip(),
					selectedX_, currentTop - 18.0f, selectedWidth_, 18.0f);
			}

			if (item.getIcon())
			{
				glColor3f(1.0f, 1.0f, 1.0f);
				GLState textureOn(GLState::TEXTURE_ON);
				item.getIcon()->draw();
				drawIconBox(selectedX_ + 5.0f, currentTop - 19.0f);
			}

			static Vector color(0.9f, 0.9f, 1.0f);
			static Vector itemcolor(0.1f, 0.1f, 0.4f);
			if (item.getSelected())
			{
				font.draw(selected?color:itemcolor, 12, selectedX_ + 5.0f, 
					currentTop - 16.0f, 0.0f, "x");
			}
			font.draw(selected?color:itemcolor, 12, selectedX_ + indent + 10.0f, 
				currentTop - 16.0f, 0.0f, (char *) item.getText());
			currentTop -= 18.0f;
		}
	}
}

void GLWSelector::showSelector(GLWSelectorI *user,
							   float x, float y,
							   std::list<GLWSelectorEntry> &entries,
							   unsigned int showState)
{
	showState_ = showState;
	selectedHeight_ = 0;
	selectedWidth_ = 0;
	drawX_ = x;
	drawY_ = y;
	user_ = user;
	entries_ = entries;
	visible_ = true;
	w_ = 100000.0f;
	h_ = 100000.0f;
}

void GLWSelector::hideSelector()
{
	visible_ = false;
	w_ = 0;
	h_ = 0; 
}

void GLWSelector::mouseDown(float mouseX, float mouseY, bool &hitMenu)
{
	// Override default window behaviour
	if (visible_ && ((showState_ == 0) ||
		(ScorchedClient::instance()->getGameState().getState() == showState_)))
	{
		bool thisMenu = (mouseX > selectedX_ && mouseX < selectedX_ + selectedWidth_ && 
			mouseY < selectedY_ && mouseY > selectedY_ - selectedHeight_);
		if (thisMenu)
		{
			// Draw the menu items
			int position = 0;
			float currentTop = selectedY_;
			std::list<GLWSelectorEntry>::iterator itor;
			for (itor =	entries_.begin();
				 itor != entries_.end();
				 itor++)
			{
				GLWSelectorEntry &item = (*itor);
			
				// Check if the item is a seperator
				if (item.getText()[0] == '-')
				{
					currentTop -= 8.0f;
				}
				else
				{
					// Check if the item is selected
					if (selectedX_ < mouseX && mouseX < selectedX_ + selectedWidth_ &&
						currentTop - 18.0f <= mouseY && mouseY < currentTop)
					{
						if (user_) user_->itemSelected(&item, position);
					}

					position++;
					currentTop -= 18.0f;
				}
			}
		}

		hideSelector();
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
