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
	visible_(false)
{
}

GLWSelector::~GLWSelector()
{
}

void GLWSelector::draw()
{
	if (!visible_) return;
	GLState currentStateBlend(GLState::TEXTURE_OFF | 
		GLState::DEPTH_OFF | GLState::BLEND_ON);

	GLFont2d &font = *GLWFont::instance()->getLargePtFont();
	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();

	// Get the height and width of the selector
	float selectedHeight = 10.0f; // Padding
	float selectedWidth = 0.0f;
	float selectedX = drawX_;
	float selectedY = drawY_;
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
	selectedWidth_ = selectedWidth;
	selectedHeight_ = selectedHeight;

	// Draw the background
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(0.5f, 0.5f, 1.0f, 0.5f);	
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(selectedX + 20.0f, 
				selectedY - 25.0f + 5.0f);
			glVertex2f(selectedX + 20.0f, 
				selectedY - selectedHeight + 5.0f);
			GLWVisibleWidget::drawRoundBox(
				selectedX, selectedY - selectedHeight + 5.0f, 
				selectedWidth, selectedHeight, 20.0f);
			glVertex2f(selectedX + 20.0f, 
				selectedY - selectedHeight + 5.0f);
		glEnd();

		glColor4f(0.9f, 0.9f, 1.0f, 0.5f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWVisibleWidget::drawRoundBox(
				selectedX, selectedY - selectedHeight + 5.0f, 
				selectedWidth, selectedHeight, 20.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	// Draw the menu items
	float currentTop = selectedY;
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
				glVertex2f(selectedX + 5.0f, currentTop - 5.0f);
				glVertex2f(selectedX + selectedWidth - 5.0f, currentTop - 5.0f);
			glEnd();
			currentTop -= 8.0f;
		}
		else
		{
			// Draw the actual line
			bool selected = false;
			if (selectedX < mouseX && mouseX < selectedX + selectedWidth &&
				currentTop - 18.0f < mouseY && mouseY < currentTop)
			{
				selected = true;
			}

			if (item.getToolTip())
			{
				GLWToolTip::instance()->addToolTip(item.getToolTip(),
					selectedX, currentTop - 18.0f, selectedWidth, 18.0f);
			}

			static Vector color(0.9f, 0.9f, 1.0f);
			static Vector itemcolor(0.1f, 0.1f, 0.4f);
			if (item.getSelected())
			{
				font.draw(selected?color:itemcolor, 12, selectedX + 5.0f, 
					currentTop - 16.0f, 0.0f, "x");
			}
			font.draw(selected?color:itemcolor, 12, selectedX + indent + 10.0f, 
				currentTop - 16.0f, 0.0f, (char *) item.getText());
			currentTop -= 18.0f;
		}
	}
}

void GLWSelector::showSelector(GLWSelectorI *user,
							   float x, float y,
							   std::list<GLWSelectorEntry> &entries)
{
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
	if (visible_)
	{
		bool thisMenu = (mouseX > drawX_ && mouseX < drawX_ + selectedWidth_ && 
			mouseY > drawY_ && mouseY < drawY_ + selectedHeight_);
		if (thisMenu)
		{
			float selectedX = drawX_;
			float selectedY = drawY_;
			
			// Draw the menu items
			int position = 0;
			float currentTop = selectedY;
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
					position++;
					
					// Draw the actual line
					if (selectedX < mouseX && mouseX < selectedX + selectedWidth_ &&
						currentTop - 18.0f < mouseY && mouseY < currentTop)
					{
						if (user_) user_->itemSelected(&item, position);
					}
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
