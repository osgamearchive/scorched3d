////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <GLW/GLWLoggerView.h>
#include <GLW/GLWFont.h>
#include <GLW/GLWPanel.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>
#include <XML/XMLParser.h>
#include <common/Logger.h>
#include <common/Keyboard.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>

static Vector defaultColor(0.7f, 0.7f, 0.7f);
static Vector otherDefaultColor(0.85f, 0.85f, 0.85f);
static Vector black(0.0f, 0.0f, 0.0f);

REGISTER_CLASS_SOURCE(GLWLoggerView);

GLWLoggerView::GLWLoggerView() : 
	init_(false),
	visibleLines_(5), totalLines_(50),
	displayTime_(10.0f),
	oldStyle_(false), mask_(255),
	fontSize_(12.0f), outlineFontSize_(14.0f), lineDepth_(18),
	currentVisible_(0),
	alignTop_(false), parentSized_(false), splitLargeLines_(false),
	scrollPosition_(-1), allowScroll_(false),
	upButton_(x_ + 2.0f, y_ + 2.0f, 10.0f, 10.0f),
	downButton_(x_ + 2.0f, y_ + 2.0f, 10.0f, 10.0f),
	resetButton_(x_ + 2.0f, y_ + 2.0f, 10.0f, 10.0f),
	scrollUpKey_(0), scrollDownKey_(0), scrollResetKey_(0)
{
	upButton_.setHandler(this);
	downButton_.setHandler(this);
	resetButton_.setHandler(this);

	upButton_.setToolTip(new GLWTip("Log", "Show previous log entry"));
	downButton_.setToolTip(new GLWTip("Log", "Show next log entry"));
	resetButton_.setToolTip(new GLWTip("Log", "Go to the end of the log\n"
		"and hide all elapsed log entries"));
}

GLWLoggerView::~GLWLoggerView()
{
}

void GLWLoggerView::logMessage(LoggerInfo &info)
{
	if (!(info.getType() & mask_)) return;

	if (!splitLargeLines_)
	{
		addInfo(info);
		return;
	}

	bool firstTime = true;
	int currentLen = 0;
	int totalLen = (int) strlen(info.getMessage());
	while (currentLen < totalLen)
	{
		std::string result;
		int partLen = splitLine(&info.getMessage()[currentLen], result);
		currentLen += partLen;

		LoggerInfo newInfo = info;
		newInfo.setMessage(result.c_str());
		if (firstTime)
		{
			firstTime = false;
			newInfo.setInfoLen(MIN((int)newInfo.getInfoLen(), partLen));
		}
		else
		{
			newInfo.setInfoLen(0);
			newInfo.setIcon(0);
		}
		addInfo(newInfo);
	}
}

void GLWLoggerView::buttonDown(unsigned int id)
{
	if (!allowScroll_) return;

	if (id == upButton_.getId())
	{
		scrollPosition_ ++;
		if (scrollPosition_ > (int) textLines_.size() - 1)
			scrollPosition_ = (int) textLines_.size() - 1;
	}
	else if (id == downButton_.getId())
	{
		scrollPosition_ --;
		if (scrollPosition_ < 0) scrollPosition_ = 0;
	}
	else
	{
		scrollPosition_ = -1;
	}
}

int GLWLoggerView::splitLine(const char *message, std::string &result)
{
	int lastSpace = 0;
	int totalLen = (int) strlen(message);
	for (int len=1; len<totalLen; len++)
	{
		float width = GLWFont::instance()->getLargePtFont()->
			getWidth(outlineFontSize_, message, len);
		if (width > w_)
		{
			if (lastSpace)
			{
				result.append(message, lastSpace);
				return lastSpace;
			}
			else
			{
				result.append(message, len-1);
				return len-1;
			}
		}

		if (message[len] == ' ')
		{
			lastSpace = len;
		}
	}

	result.append(message);
	return totalLen;
}

void GLWLoggerView::addInfo(LoggerInfo &info)
{
	GLWLoggerViewEntry entry;
	entry.info = info;
	entry.timeRemaining = displayTime_;
	textLines_.push_front(entry);

	if (scrollPosition_ > 0)
	{
		scrollPosition_ ++;
		if (scrollPosition_ > (int) textLines_.size() - 1)
			scrollPosition_ = (int) textLines_.size() - 1;
	}

	if (textLines_.size() > (unsigned int) totalLines_)
	{
		textLines_.pop_back();
	}
}

void GLWLoggerView::simulate(float frameTime)
{
	currentVisible_ = 0;
	int count = 0;
	std::list<GLWLoggerViewEntry>::iterator itor;
	for (itor = textLines_.begin();
		itor != textLines_.end() && count < visibleLines_;
		itor++, count++)
	{
		GLWLoggerViewEntry &entry = (*itor);
		entry.timeRemaining -= frameTime;
		if (entry.timeRemaining > 0.0f) currentVisible_ ++;
	}
}

void GLWLoggerView::draw()
{
	GLWidget::draw();

	if (allowScroll_)
	{
		upButton_.draw();
		downButton_.draw();
		resetButton_.draw();
	}

	if (!init_)
	{
		Logger::addLogger(this);
		init_ = true;
		if (parent_ && parentSized_)
		{
			w_ = parent_->getW();
			h_ = parent_->getH();
		}

		upButton_.setX(x_ + 2.0f);
		upButton_.setY(y_ + 36.0f);
		downButton_.setX(x_ + 2.0f);
		downButton_.setY(y_ + 24.0f);
		resetButton_.setX(x_ + 2.0f);
		resetButton_.setY(y_ + 6.0f);

	}

	if (textLines_.empty()) return;

	GLState currentStateBlend(GLState::TEXTURE_ON | 
		GLState::BLEND_ON | GLState::DEPTH_OFF);
	float start = y_ + 8.0f; //lineDepth_;
	if (alignTop_)
	{
		start = y_ + h_ - float(currentVisible_) * lineDepth_;
	}

	std::list<GLWLoggerViewEntry>::iterator startingPos = textLines_.begin();
	{
		int count = 0;
		for (;
			startingPos != textLines_.end() && count < scrollPosition_;
			startingPos++, count++)
		{
		}
	}

	{
		int count = 0;
		std::list<GLWLoggerViewEntry>::iterator itor;
		for (itor = startingPos;
			itor != textLines_.end() && count < visibleLines_;
			itor++, count++)
		{
			GLWLoggerViewEntry &entry = (*itor);

			float alpha = 1.0f;
			if (scrollPosition_ < 0)
			{
				if (entry.timeRemaining <= 0.0f) continue;
				if (entry.timeRemaining < 1.0f) alpha = entry.timeRemaining;
			}

			if (oldStyle_ || !(entry.info.getType() & LoggerInfo::TypeDeath))
			{
				// Figure texture width
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, entry.info.getMessage()) / 2.0f;
				float x = x_ + w_ / 2.0f - minus - 1;
				float y = start + count * lineDepth_ - 1;

				// Draw outline
 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutlineA(black, alpha, outlineFontSize_, fontSize_,
 						x, y, 0.0f, 
 						entry.info.getMessage());
			}
			else
			{
				Tank *source = ScorchedClient::instance()->
					getTankContainer().getTankById(entry.info.getPlayerId());
				Tank *dest =  ScorchedClient::instance()->
					getTankContainer().getTankById(entry.info.getOtherPlayerId());

				float x = x_ + w_ / 2.0f;
				float y = start + count * lineDepth_;
				float minusSource = (float) GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, source->getName());

 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutlineA(black, alpha, outlineFontSize_, fontSize_,
 						x - minusSource - outlineFontSize_ / 2.0f - 2.0f, y, 0.0f, 
						source->getName());
 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutlineA(black, alpha, outlineFontSize_, fontSize_,
 						x + outlineFontSize_ / 2.0f + 2.0f, y, 0.0f, 
						dest->getName());
			}
		}
	}

	{
		int count = 0;
		std::list<GLWLoggerViewEntry>::iterator itor;
		for (itor = startingPos;
			itor != textLines_.end() && count < visibleLines_;
			itor++, count++)
		{
			GLWLoggerViewEntry &entry = (*itor);

			float alpha = 1.0f;
			if (scrollPosition_ < 0)
			{
				if (entry.timeRemaining <= 0.0f) continue;
				if (entry.timeRemaining < 1.0f) alpha = entry.timeRemaining;
			}

			if (oldStyle_ || !(entry.info.getType() & LoggerInfo::TypeDeath))
			{
				// Figure texture width
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, entry.info.getMessage()) / 2.0f;
				float x = x_ + w_ / 2.0f  - minus;
				float y = start + count * lineDepth_;

				// Draw icon
				if (entry.info.getIcon())
				{
					entry.info.getIcon()->draw();
					glPushMatrix();
						glTranslatef(x - fontSize_ - 8.0f, y - 3.0f, 0.0f);
						glColor4f(1.0f, 1.0f, 1.0f, alpha);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 0.0f);
							glVertex2f(0.0f, 0.0f);
							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(outlineFontSize_, 0.0f);
							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(outlineFontSize_, outlineFontSize_);
							glTexCoord2f(0.0f, 1.0f);
							glVertex2f(0.0f, outlineFontSize_);
						glEnd();
					glPopMatrix();
				}

				// Draw Text
				Tank *source = ScorchedClient::instance()->
					getTankContainer().getTankById(entry.info.getPlayerId());

				Vector dColor = defaultColor;
				if ((entry.info.getType() & LoggerInfo::TypeTalk) ||
					(entry.info.getType() & LoggerInfo::TypeDeath))
				{
					dColor = otherDefaultColor;
				}

				if (entry.info.getInfoLen() > 0)
				{
					GLWFont::instance()->getLargePtFont()->
						drawSubStrA(0, entry.info.getInfoLen(),
							source?source->getColor():dColor, alpha, fontSize_,
							x, y, 0.0f, 
							entry.info.getMessage());
					GLWFont::instance()->getLargePtFont()->
						drawSubStrA(entry.info.getInfoLen(), entry.info.getMessageLen(),
							dColor, alpha, fontSize_,
							x, y, 0.0f, 
							entry.info.getMessage());
				}
				else
				{
					GLWFont::instance()->getLargePtFont()->
						drawA(dColor, alpha, fontSize_,
							x, y, 0.0f, 
							entry.info.getMessage());
				}
			}
			else
			{
				Tank *source = ScorchedClient::instance()->
					getTankContainer().getTankById(entry.info.getPlayerId());
				Tank *dest =  ScorchedClient::instance()->
					getTankContainer().getTankById(entry.info.getOtherPlayerId());

				float x = x_ + w_ / 2.0f;
				float y = start + count * lineDepth_;
				float minusSource = (float) GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, source->getName());

				GLWFont::instance()->getLargePtFont()->
					drawA(source?source->getColor():defaultColor, alpha, fontSize_,
						x - minusSource - outlineFontSize_ / 2.0f - 4.0f, y, 0.0f, 
						source->getName());
				GLWFont::instance()->getLargePtFont()->
					drawA(dest?dest->getColor():defaultColor, alpha, fontSize_,
						x + outlineFontSize_ / 2.0f + 4.0f, y, 0.0f, 
						dest->getName());

				if (entry.info.getIcon())
				{
					float iconSize = fontSize_ * 2.0f;

					entry.info.getIcon()->draw();
					glPushMatrix();
						glTranslatef(x - iconSize / 2.0f, y - 3.0f, 0.0f);
						glColor4f(1.0f, 1.0f, 1.0f, alpha);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 0.0f);
							glVertex2f(0.0f, 0.0f);
							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(iconSize, 0.0f);
							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(iconSize, iconSize);
							glTexCoord2f(0.0f, 1.0f);
							glVertex2f(0.0f, iconSize);
						glEnd();
					glPopMatrix();
				}
			}
		}
	}
}

void GLWLoggerView::mouseDown(int button, float x, float y, bool &skipRest)
{
	skipRest = false;
	upButton_.mouseDown(button, x, y, skipRest);
	downButton_.mouseDown(button, x, y, skipRest);
	resetButton_.mouseDown(button, x, y, skipRest);
}

void GLWLoggerView::mouseUp(int button, float x, float y, bool &skipRest)
{
	skipRest = false;
	upButton_.mouseUp(button, x, y, skipRest);
	downButton_.mouseUp(button, x, y, skipRest);
	resetButton_.mouseUp(button, x, y, skipRest);
}

void GLWLoggerView::mouseDrag(int button, float mx, float my, float x, float y, bool &skipRest)
{
	skipRest = false;
	upButton_.mouseDrag(button, mx, my, x, y, skipRest);
	downButton_.mouseDrag(button, mx, my, x, y, skipRest);
	resetButton_.mouseDrag(button, mx, my, x, y, skipRest);
}

void GLWLoggerView::keyDown(char *buffer, unsigned int keyState, 
	KeyboardHistory::HistoryElement *history, int hisCount, 
	bool &skipRest)
{
	if (scrollUpKey_ && scrollUpKey_->keyDown(buffer, keyState, false))
	{
		buttonDown(upButton_.getId());
		skipRest = true;
	}
	if (scrollDownKey_ && scrollDownKey_->keyDown(buffer, keyState, false))
	{
		buttonDown(downButton_.getId());
		skipRest = true;
	}
	if (scrollResetKey_ && scrollResetKey_->keyDown(buffer, keyState, false))
	{
		buttonDown(resetButton_.getId());
		skipRest = true;
	}
}

bool GLWLoggerView::initFromXML(XMLNode *node)
{
	if (!GLWidget::initFromXML(node)) return false;

	if (!node->getNamedChild("mask", mask_)) return false;
	if (!node->getNamedChild("oldstyle", oldStyle_)) return false;
	if (!node->getNamedChild("displaytime", displayTime_)) return false;
	if (!node->getNamedChild("totallines", totalLines_)) return false;
	if (!node->getNamedChild("visiblelines", visibleLines_)) return false;
	if (!node->getNamedChild("linedepth", lineDepth_)) return false;
	if (!node->getNamedChild("fontsize", fontSize_)) return false;
	if (!node->getNamedChild("outlinefontsize", outlineFontSize_)) return false;
	if (!node->getNamedChild("textaligntop", alignTop_)) return false;
	if (!node->getNamedChild("parentsized", parentSized_)) return false;
	if (!node->getNamedChild("splitlargelines", splitLargeLines_)) return false;
	if (!node->getNamedChild("allowscroll", allowScroll_)) return false;

	std::string scrollUpKey, scrollDownKey, scrollResetKey;
	if (node->getNamedChild("scrollupkey", scrollUpKey, false))
	{
		scrollUpKey_ = Keyboard::instance()->getKey(scrollUpKey.c_str());
	}
	if (node->getNamedChild("scrolldownkey", scrollDownKey, false))
	{
		scrollDownKey_ = Keyboard::instance()->getKey(scrollDownKey.c_str());
	}
	if (node->getNamedChild("scrollresetkey", scrollResetKey, false))
	{
		scrollResetKey_ = Keyboard::instance()->getKey(scrollResetKey.c_str());
	}
		
	return true;
}

