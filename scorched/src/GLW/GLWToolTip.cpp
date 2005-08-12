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
#include <GLEXT/GLViewPort.h>
#include <GLW/GLWToolTip.h>
#include <GLW/GLWidget.h>
#include <GLW/GLWFont.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <string.h>

unsigned int GLWTip::nextId_ = 0;
static Vector color(0.1f, 0.1f, 0.4f);
static Vector selectedColor(0.9f, 0.9f, 1.0f);

GLWTipI::~GLWTipI()
{

}

GLWTip::GLWTip(const char *tit, const char *tex) 
	: id_(++nextId_), x(0), y(0), w(0), h(0), handler_(0)
{
	setText(tit, tex);
}

GLWTip::~GLWTip()
{
}

void GLWTip::populate()
{
	if (handler_) handler_->populateCalled(id_);
}

void GLWTip::setText(const char *title, const char *fmt, ...)
{
	const int TEXT_SIZE = 10000;
	static char text[TEXT_SIZE];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	texts_.clear();
	title_ = title; text_ = text;
	textWidth_ = 0.0f; textHeight_ = 24.0f;

	char *token = strtok((char *) text_.c_str(), "\n");
	while(token != NULL)
	{
		texts_.push_back(token);
		textHeight_ += 10.0f;
		token = strtok(NULL, "\n");
	}
}

void GLWTip::calcWidth()
{
	if (textWidth_ != 0.0f) return;

	std::list<char *> &texts = getTexts();
	std::list<char *>::iterator itor;
	std::list<char *>::iterator enditor = texts.end();
	for (itor = texts.begin(); itor != enditor; itor++)
	{
		float width = float(GLWFont::instance()->getSmallPtFont()->
			getWidth(9,(*itor))) + 10.0f;
		if (width > textWidth_) textWidth_ = width;
	}

	float width = float(GLWFont::instance()->getSmallPtFont()->
		getWidth(11,title_.c_str())) + 10.0f; 
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
	timeDrawn_(0.0f), timeSeen_(0.0),
	refreshTime_(100.0f)
{
}

GLWToolTip::~GLWToolTip()
{
}

bool GLWToolTip::addToolTip(GLWTip *tip, float x, float y, float w, float h)
{
	if (!OptionsDisplay::instance()->getShowContextHelp()) return false;

	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();
	
	bool result = false;
	if (x < mouseX && mouseX < x + w &&
		y < mouseY && mouseY < y + h)
	{
		tip->x = x;
		tip->y = y;
		tip->w = w;
		tip->h = h;
		currentTip_ = tip;
		result = true;
	}
	return result;
}

void GLWToolTip::clearToolTip(float x, float y, float w, float h)
{
	if (!OptionsDisplay::instance()->getShowContextHelp()) return;

	int mouseX = ScorchedClient::instance()->getGameState().getMouseX();
	int mouseY = ScorchedClient::instance()->getGameState().getMouseY();
	
	if (x < mouseX && mouseX < x + w &&
		y < mouseY && mouseY < y + h)
	{
		currentTip_ = 0;
	}
}

void GLWToolTip::simulate(const unsigned state, float frameTime)
{
	timeDrawn_ += frameTime;
}

void GLWToolTip::draw(const unsigned state)
{
	bool sameTip = (lastTip_ == currentTip_);
	lastTip_ = currentTip_;
	currentTip_ = 0;

	if (sameTip && lastTip_)
	{
		timeSeen_ += timeDrawn_;
		refreshTime_ += timeDrawn_;
	}
	else
	{
		timeSeen_ = 0.0f;
		refreshTime_ = 100.0f;
	}
	timeDrawn_ = 0.0f;

	float showTime = 
		float(OptionsDisplay::instance()->getToolTipTime()) / 1000.0f;
	if (!lastTip_ || (timeSeen_ < showTime))
	{
		return;
	}

	float alpha = MIN(1.0f, (timeSeen_ - showTime) * 
		float(OptionsDisplay::instance()->getToolTipSpeed()));

	if (refreshTime_ > 1.0f)
	{
		lastTip_->populate();
		refreshTime_ = 0.0f;
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::DEPTH_OFF);

	lastTip_->calcWidth();
	float posX = lastTip_->getX();
	float posY = lastTip_->getY();
	float posH = lastTip_->getTextHeight();
	float posW = lastTip_->getTextWidth();

	int camWidth = GLViewPort::getWidth();
	if (posX > camWidth / 2)
	{
		posX -= posW + 5.0f;
	}
	else
	{
		posX += lastTip_->w + 5.0f;
	}
	int camHeight = GLViewPort::getHeight();
	if (posY > camHeight / 2)
	{
		posY -= posH;
	}
	else
	{
		posY += 5.0f;
	}

	if (posX < 0) posX = 0;
	else if (posX + posW > camWidth) posX -= posX + posW - camWidth;

	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glColor4f(0.5f, 0.5f, 1.0f, 0.8f * alpha);	
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(posX + 10.0f, posY + 2.0f);
			glVertex2f(posX + 10.0f, posY);
			GLWidget::drawRoundBox(
				posX, posY,
				posW, posH, 10.0f);
			glVertex2f(posX + 10.0f, posY);
		glEnd();
		glColor4f(0.9f, 0.9f, 1.0f, 0.5f * alpha);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWidget::drawRoundBox(
				posX, posY,
				posW, posH, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	float pos = posY + posH - 16.0f;
	GLWFont::instance()->getSmallPtFont()->drawA(selectedColor, alpha, 11, posX + 3.0f, 
		pos, 0.0f, lastTip_->getTitle());
	pos -= 2.0f;

	std::list<char *> &texts = lastTip_->getTexts();
	std::list<char *>::iterator itor;
	std::list<char *>::iterator enditor = texts.end();
	for (itor = texts.begin(); itor != enditor; itor++)
	{
		pos -= 10.0f;

		GLWFont::instance()->getSmallPtFont()->drawA(color, alpha, 9, posX + 6.0f, 
			pos, 0.0f, (*itor));
	}
}
