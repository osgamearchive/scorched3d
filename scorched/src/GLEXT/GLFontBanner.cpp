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
#include <GLEXT/GLTexture.h>
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

void GLFontBanner::addLine(Vector &color, GLTexture *texture, 
	const char *fmt, ...)
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
	textLines_[pos].setTexture(texture);
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
	const int lineDepth = 18;

	if (usedLines_ > 0)
	{
		GLState currentStateBlend(GLState::TEXTURE_ON | 
			GLState::BLEND_ON | GLState::DEPTH_OFF);

		Vector black(0.0f, 0.0f, 0.0f);
		const float fontWidth = 12;
		const float outlineFontWidth = 14;
		const float outlineFontWidth2 = 8;

		float start = y_ + lineDepth * usedLines_;
		{
			int pos = startLine_;
			int used = usedLines_;
			for (int i=0; i<used; i++)
			{
				GLFontBannerEntry &entry = textLines_[pos];

				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontWidth, entry.getText()) / 2.0f;
 				GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutline(black, outlineFontWidth, fontWidth,
 						x_ - minus - 1, start - i * lineDepth - 1, 0.0f, 
 						textLines_[pos].getText());
 				/*GLWFont::instance()->getSmallPtFontOutline()->
 					drawOutline(black, outlineFontWidth2, fontWidth,
 						x_ - minus + 3, start - i * lineDepth - 0.0f, 0.0f, 
 						textLines_[pos].getText());*/

				/*float left = minus;
				GLState currentStateBlend(GLState::TEXTURE_OFF);
				if (entry.getTexture())
				{
					left = minus + fontWidth + 8.0f;
				}
				glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
				glBegin(GL_QUADS);
					glVertex2f(x_ + minus + 5, start - i * lineDepth + lineDepth - 2.0f);
					glVertex2f(x_ - left - 3, start - i * lineDepth + lineDepth - 2.0f);
					glVertex2f(x_ - left - 3, start - i * lineDepth - 5.0f);
					glVertex2f(x_ + minus + 5, start - i * lineDepth - 5.0f);
				glEnd();*/

				if (++pos >= totalLines_) pos = 0;
			}
		}

		{
			int pos = startLine_;
			int used = usedLines_;
			for (int i=0; i<used; i++)
			{
				GLFontBannerEntry &entry = textLines_[pos];

				// Figure texture width
				float minus = GLWFont::instance()->getLargePtFont()->
					getWidth(fontWidth, entry.getText()) / 2.0f;
				float x = x_ - minus;
				float y = start - i * lineDepth;

				// Draw icon
				if (entry.getTexture())
				{
					entry.getTexture()->draw();
					glPushMatrix();
						glTranslatef(x - fontWidth - 8.0f, y - 3.0f, 0.0f);
						glColor3f(1.0f, 1.0f, 1.0f);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 0.0f);
							glVertex2f(0.0f, 0.0f);
							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(outlineFontWidth, 0.0f);
							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(outlineFontWidth, outlineFontWidth);
							glTexCoord2f(0.0f, 1.0f);
							glVertex2f(0.0f, outlineFontWidth);
						glEnd();
					glPopMatrix();
				}

				// Draw Text
				GLWFont::instance()->getLargePtFont()->
					draw(entry.getColor(), fontWidth,
						x, y, 0.0f, 
						entry.getText());
				if (++pos >= totalLines_) pos = 0;
			}
		}
	}
}
