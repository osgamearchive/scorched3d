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

#include <GLEXT/GLState.h>
#include <GLW/GLWToolTip.h>
#include <GLW/GLWVisibleWidget.h>
#include <GLW/GLWFont.h>
#include <client/MainCamera.h>
#include <common/OptionsDisplay.h>
#include <string.h>

unsigned int GLWTip::nextId_ = 0;
static Vector color(0.2f, 0.2f, 0.2f);
static Vector selectedColor(0.0f, 0.0f, 0.0f);

GLWTip::GLWTip(const char *tit, const char *tex) 
	: id_(++nextId_), x(0), y(0), w(0), h(0)
{
	setText(tit, tex);
}

GLWTip::~GLWTip()
{
}

void GLWTip::populate()
{
}

void GLWTip::setText(const char *title, const char *text)
{
	texts_.clear();
	title_ = title; text_ = text;
	textWidth_ = 0.0f; textHeight_ = 24.0f;

	char *token = strtok((char *) text_.c_str(), "\n");
	while(token != NULL)
	{
		texts_.push_back(token);
		float width = float(strlen(token)) * 7.0f + 10.0f;
		if (width > textWidth_) textWidth_ = width;
		textHeight_ += 10.0f;
		token = strtok(NULL, "\n");
	}

	float width = float(strlen(title_.c_str())) * 7.0f + 10.0f;
	if (width > textWidth_) textWidth_ = width;
}

float GLWTip::getX()
{
	return x;
}

float GLWTip::getY()
{
	return y;
}

float GLWTip::getW()
{
	return w;
}

float GLWTip::getH()
{
	return h;
}

GLWToolTip *GLWToolTip::instance_ = 0;

GLWToolTip *GLWToolTip::instance()
{
	if (!instance_)
	{
		instance_ = new GLWToolTip;
	}
	return instance_;
}

GLWToolTip::GLWToolTip() : 
	lastTip_(0), currentTip_(0),
	timePasted_(0.0f)
{
}

GLWToolTip::~GLWToolTip()
{
}

void GLWToolTip::addToolTip(GLWTip *tip, float x, float y, float w, float h)
{
	if (!OptionsDisplay::instance()->getShowContextHelp()) return;

	int windowHeight = MainCamera::instance()->getCamera().getHeight();
	int mouseX = GameState::instance()->getMouseX();
	int mouseY = windowHeight - GameState::instance()->getMouseY();
	
	if (x < mouseX && mouseX < x + w &&
		y < mouseY && mouseY < y + h)
	{
		tip->x = x;
		tip->y = y;
		tip->w = w;
		tip->h = h;
		currentTip_ = tip;
	}
}

void GLWToolTip::simulate(const unsigned state, float frameTime)
{
	timePasted_ += frameTime;
	const float TimeStep = 2.0f;
	if (timePasted_ > TimeStep)
	{
		timePasted_ = 0.0f;
	}
}

void GLWToolTip::draw(const unsigned state)
{
	bool sameTip = (lastTip_ == currentTip_);
	lastTip_ = currentTip_;
	if (!currentTip_) return;

	if (!sameTip)
	{
		currentTip_->populate();
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	float posX = currentTip_->getX();
	float posY = currentTip_->getY();
	float posH = currentTip_->getTextHeight();
	float posW = currentTip_->getTextWidth();

	if (posX > MainCamera::instance()->getCamera().getWidth() / 2)
	{
		posX -= posW + 5.0f;
	}
	else
	{
		posX += currentTip_->w + 5.0f;
	}
	if (posY > MainCamera::instance()->getCamera().getHeight() / 2)
	{
		posY -= posH;
	}
	else
	{
		posY += 5.0f;
	}

	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(1.0f, 1.0, 0.0f, 0.7f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(posX + 10.0f, posY + 2.0f);
			glVertex2f(posX + 10.0f, posY);
			GLWVisibleWidget::drawRoundBox(
				posX, posY,
				posW, posH, false);
			glVertex2f(posX + 10.0f, posY);
		glEnd();
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);

		GLWVisibleWidget::drawRoundBox(
			posX, posY,
			posW, posH, false);
	glEnd();

	float pos = posY + posH - 16.0f;
	GLWFont::instance()->getFont()->draw(selectedColor, 11, posX + 3.0f, 
		pos, 0.0f, currentTip_->getTitle());
	pos -= 2.0f;

	std::list<char *> &texts = currentTip_->getTexts();
	std::list<char *>::iterator itor;
	std::list<char *>::iterator enditor = texts.end();
	for (itor = texts.begin(); itor != enditor; itor++)
	{
		pos -= 10.0f;

		GLWFont::instance()->getFont()->draw(color, 10, posX + 6.0f, 
			pos, 0.0f, (*itor));
	}

	currentTip_ = 0;
}
