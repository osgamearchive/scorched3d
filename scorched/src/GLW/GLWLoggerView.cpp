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
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>

static Vector defaultColor(0.7f, 0.7f, 0.7f);
static Vector black(0.0f, 0.0f, 0.0f);

REGISTER_CLASS_SOURCE(GLWLoggerView);

GLWLoggerView::GLWLoggerView() : 
	init_(false), textLines_(0),
	startLine_(0), usedLines_(0), totalLines_(5), 
	displayTime_(10.0f),
	oldStyle_(false), mask_(255),
	fontSize_(12.0f), outlineFontSize_(14.0f), lineDepth_(18),
	alignTop_(false), parentSized_(false)
{
}

GLWLoggerView::~GLWLoggerView()
{
	delete [] textLines_;
}

void GLWLoggerView::logMessage(LoggerInfo &info)
{
	if (!(info.getType() & mask_)) return;

	bool textRemoved = false;
	if (usedLines_ < totalLines_)
	{
		usedLines_++;
	}
	else
	{
		textRemoved = true;
		if (++startLine_ >= totalLines_)
		{
			startLine_ = 0;
		}
	}

	int pos = startLine_ + usedLines_ - 1;
	if (pos >= totalLines_)
	{
		pos -= totalLines_;
	}

	textLines_[pos].info = info;
	textLines_[pos].timeRemaining = displayTime_;
}

void GLWLoggerView::simulate(float frameTime)
{
	if (!textLines_) return;

	int pos = startLine_;
	int used = usedLines_;
	for (int i=0; i<used; i++)
	{
		textLines_[pos].timeRemaining -= frameTime;
		if (textLines_[pos].timeRemaining < 0.0f)
		{
			if (++startLine_ >= totalLines_) startLine_ = 0;
			--usedLines_;
		}

		if (++pos >= totalLines_) pos = 0;
	}
}

void GLWLoggerView::draw()
{
	GLWidget::draw();

	if (!init_)
	{
		Logger::addLogger(this);
		init_ = true;
		textLines_ = new GLWLoggerViewEntry[totalLines_];
		if (parent_ && parentSized_)
		{
			w_ = parent_->getW();
			h_ = parent_->getH();
		}
	}

	if (usedLines_ == 0) return;

	GLState currentStateBlend(GLState::TEXTURE_ON | 
		GLState::BLEND_ON | GLState::DEPTH_OFF);
	float start = y_ + lineDepth_ * usedLines_;
	if (alignTop_)
	{
		start = y_ + h_ - lineDepth_;
	}

	{
		int pos = startLine_;
		int used = usedLines_;
		for (int i=0; i<used; i++)
		{
			GLWLoggerViewEntry &entry = textLines_[pos];

			if (oldStyle_ || !(entry.info.getType() & LoggerInfo::TypeDeath))
			{
				// Figure texture width
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, entry.info.getMessage()) / 2.0f;
				float x = x_ + w_ / 2.0f - minus - 1;
				float y = start - i * lineDepth_ - 1;

				// Draw outline
 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutline(black, outlineFontSize_, fontSize_,
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
				float y = start - i * lineDepth_;
				float minusSource = (float) GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, source->getName());

 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutline(black, outlineFontSize_, fontSize_,
 						x - minusSource - outlineFontSize_ / 2.0f - 2.0f, y, 0.0f, 
						source->getName());
 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutline(black, outlineFontSize_, fontSize_,
 						x + outlineFontSize_ / 2.0f + 2.0f, y, 0.0f, 
						dest->getName());
			}

			if (++pos >= totalLines_) pos = 0;
		}
	}

	{
		int pos = startLine_;
		int used = usedLines_;
		for (int i=0; i<used; i++)
		{
			GLWLoggerViewEntry &entry = textLines_[pos];

			if (oldStyle_ || !(entry.info.getType() & LoggerInfo::TypeDeath))
			{
				// Figure texture width
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, entry.info.getMessage()) / 2.0f;
				float x = x_ + w_ / 2.0f  - minus;
				float y = start - i * lineDepth_;

				// Draw icon
				if (entry.info.getIcon())
				{
					entry.info.getIcon()->draw();
					glPushMatrix();
						glTranslatef(x - fontSize_ - 8.0f, y - 3.0f, 0.0f);
						glColor3f(1.0f, 1.0f, 1.0f);
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
				GLWFont::instance()->getLargePtFont()->
					draw(source?source->getColor():defaultColor, fontSize_,
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
				float y = start - i * lineDepth_;
				float minusSource = (float) GLWFont::instance()->getLargePtFont()->
					getWidth(fontSize_, source->getName());

				GLWFont::instance()->getLargePtFont()->
					draw(source?source->getColor():defaultColor, fontSize_,
						x - minusSource - outlineFontSize_ / 2.0f - 4.0f, y, 0.0f, 
						source->getName());
				GLWFont::instance()->getLargePtFont()->
					draw(dest?dest->getColor():defaultColor, fontSize_,
						x + outlineFontSize_ / 2.0f + 4.0f, y, 0.0f, 
						dest->getName());

				if (entry.info.getIcon())
				{
					float iconSize = fontSize_ * 2.0f;

					entry.info.getIcon()->draw();
					glPushMatrix();
						glTranslatef(x - iconSize / 2.0f, y - 3.0f, 0.0f);
						glColor3f(1.0f, 1.0f, 1.0f);
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

			if (++pos >= totalLines_) pos = 0;
		}
	}
}

void GLWLoggerView::mouseDown(float x, float y, bool &skipRest)
{
	skipRest = false;
}

void GLWLoggerView::mouseUp(float x, float y, bool &skipRest)
{
	skipRest = false;
}

void GLWLoggerView::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	skipRest = false;
}

void GLWLoggerView::keyDown(char *buffer, unsigned int keyState, 
	KeyboardHistory::HistoryElement *history, int hisCount, 
	bool &skipRest)
{
	skipRest = false;
}

bool GLWLoggerView::initFromXML(XMLNode *node)
{
	if (!GLWidget::initFromXML(node)) return false;

	if (!node->getNamedChild("mask", mask_)) return false;
	if (!node->getNamedChild("oldstyle", oldStyle_)) return false;
	if (!node->getNamedChild("displaytime", displayTime_)) return false;
	if (!node->getNamedChild("totallines", totalLines_)) return false;
	if (!node->getNamedChild("linedepth", lineDepth_)) return false;
	if (!node->getNamedChild("fontsize", fontSize_)) return false;
	if (!node->getNamedChild("outlinefontsize", outlineFontSize_)) return false;
	if (!node->getNamedChild("textaligntop", alignTop_)) return false;
	if (!node->getNamedChild("parentsized", parentSized_)) return false;
		
	return true;
}

