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

#include <stdio.h> 
#include <GLEXT/GLFontBanner.h>
#include <GLW/GLWFont.h>

static const float visibleTimeLimit = 10.0f;

GLFontBannerEntry::GLFontBannerEntry()
{
	
}

GLFontBannerEntry::~GLFontBannerEntry()
{

}

bool GLFontBannerEntry::decrementTime(float time)
{
	timeRemaining_ -= time;
	return (timeRemaining_ > 0.0f);
}

void GLFontBannerEntry::setText(char *text)
{
	strncpy(textLine_, text, 999);
	len_ = strlen(text);
	textLine_[999] = '\0';
	timeRemaining_ = visibleTimeLimit;
}

Vector GLFontBanner::defaultColor = Vector(0.7f, 0.7f, 0.7f);

REGISTER_CLASS_SOURCE(GLFontBanner);

GLFontBanner::GLFontBanner(GLfloat x, GLfloat y, GLfloat w, int lines)
	: x_(x), y_(y), w_(w), totalLines_(lines),
	  startLine_(0), usedLines_(0)
{
	textLines_ = new GLFontBannerEntry[lines];
}

GLFontBanner::~GLFontBanner()
{
	delete [] textLines_;
}

void GLFontBanner::addLine(Vector &color, const char *fmt, ...)
{
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

	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);	

	textLines_[pos].setText(text);
	textLines_[pos].setColor(color);
}

void GLFontBanner::simulate(float frameTime)
{
	int pos = startLine_;
	int used = usedLines_;
	for (int i=0; i<used; i++)
	{
		if (!textLines_[pos].decrementTime(frameTime)) 
		{
			if (++startLine_ >= totalLines_) startLine_ = 0;
			--usedLines_;
		}

		if (++pos >= totalLines_) pos = 0;
	}
}

void GLFontBanner::draw()
{
	const int lineDepth = 15;

	if (usedLines_ > 0)
	{
		GLState currentStateBlend(GLState::BLEND_ON | GLState::DEPTH_OFF);

		Vector black(0.0f, 0.0f, 0.0f);
		const int fontWidth = 10;
		const int outlineFontWidth = 14;

		float start = y_ + lineDepth * usedLines_;
		{
			int pos = startLine_;
			int used = usedLines_;
			for (int i=0; i<used; i++)
			{
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontWidth, textLines_[pos].getText()) / 2.0f;

				GLWFont::instance()->getLargePtFont()->
					drawOutline(black, outlineFontWidth, fontWidth,
						x_ - minus - 2, start - i * lineDepth - 2.0f, 0.0f, 
						textLines_[pos].getText());
				/*GLWFont::instance()->getLargePtFont()->
					drawOutline(black, outlineFontWidth, fontWidth,
						x_ - minus - 0, start - i * lineDepth - 0.0f, 0.0f, 
						textLines_[pos].getText());*/

				if (++pos >= totalLines_) pos = 0;
			}
		}

		{
			int pos = startLine_;
			int used = usedLines_;
			for (int i=0; i<used; i++)
			{		
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontWidth, textLines_[pos].getText()) / 2.0f;
				
				GLWFont::instance()->getLargePtFont()->
					draw(textLines_[pos].getColor(), fontWidth,
						x_ - minus, start - i * lineDepth, 0.0f, 
						textLines_[pos].getText());
				if (++pos >= totalLines_) pos = 0;
			}
		}
	}
}
