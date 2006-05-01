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

#include <GLW/GLWListView.h>
#include <GLW/GLWFont.h>
#include <common/Defines.h>
#include <stdio.h>

static const float BorderWidth = 20.0f;

REGISTER_CLASS_SOURCE(GLWListView);

GLWListView::GLWListView(float x, float y, float w, float h, 
	int maxLen, float textSize, bool scrollText) :
	GLWidget(x, y, w, h), 
	scroll_(x + w - 17, y, h - 1, 0, 1), 
	maxLen_(maxLen), textSize_(textSize),
	scrollText_(scrollText)
{
	color_ = GLWFont::widgetFontColor;
	scroll_.setMax((int) lines_.size());
	scroll_.setSee((int) (h_ / 12.0f));
	scroll_.setCurrent(scroll_.getMax());
}

GLWListView::~GLWListView()
{
}

void GLWListView::draw()
{
	glBegin(GL_LINE_LOOP);
		drawShadedRoundBox(x_, y_, w_, h_, 6.0f, false);
	glEnd();

	{
		// Stops each font draw from changing state every time
		GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
		GLFont2d *font = GLWFont::instance()->getSmallPtFont();

		// Calculate current position
		float posY = y_ + h_ - (textSize_ + 4.0f);
		int pos = (scroll_.getMax() - scroll_.getSee()) - scroll_.getCurrent();

		// Figure out how many characters we are not drawing because
		// they are off the top of the scrolling window
		int charCount = 0;
		for (int i=0; i<pos; i++)
		{
			LineEntry &lineEntry = lines_[i];

			for (int j=0; j<(int) lineEntry.words_.size(); j++)
			{
				WordEntry &wordEntry = lineEntry.words_[j];
				charCount += (int) wordEntry.word_.size();
			}
		}

		// For each line
		for (int i=pos; i<(int) scroll_.getMax(); i++)
		{
			// Check the line is valid
			if (i >= 0 && i < (int) lines_.size())
			{
				float width = w_ - BorderWidth;
				float widthUsed = 0.0f;

				// For each word
				LineEntry &lineEntry = lines_[i];
				for (int j=0; j<(int) lineEntry.words_.size(); j++)
				{
					WordEntry &wordEntry = lineEntry.words_[j];

					// Check if there is space left to draw this word
					int possibleChars = font->getChars(textSize_, wordEntry.word_.c_str(), width - widthUsed);
					int drawChars = possibleChars;
					if (scrollText_)
					{
						if (possibleChars + charCount > (int) currentPosition_)
						{
							drawChars = int(currentPosition_) - charCount;
						}
					}

					// Draw this word
					Vector red(1.0f, 0.0f, 0.0f);
					font->drawSubStr(0, drawChars,
						(wordEntry.bold_?red:color_), 
						textSize_,
						x_ + 5.0f + widthUsed, posY, 0.0f, 
						formatString("%s", wordEntry.word_.c_str()));

					// Add this word to the space used
					widthUsed += font->getWidth(textSize_, wordEntry.word_.c_str());
					charCount += (int) wordEntry.word_.size();
				}

				// Move down a line
				posY -= (textSize_ + 2.0f);
			}

			// Check if we have drawn more than we should see
			if (scrollText_ && (charCount > int(currentPosition_))) break;
			if (posY < y_) 
			{
				if (scrollText_)
				{
					scroll_.setCurrent(scroll_.getCurrent() - 1);
				}
				break;
			}
		}
	}

	scroll_.draw();
}

void GLWListView::simulate(float frameTime)
{
	currentPosition_ += (frameTime * 8.0f);
	scroll_.simulate(frameTime);
}

void GLWListView::mouseDown(float x, float y, bool &skipRest)
{
	scroll_.mouseDown(x, y, skipRest);
}

void GLWListView::mouseUp(float x, float y, bool &skipRest)
{
	scroll_.mouseUp(x, y, skipRest);
}

void GLWListView::mouseDrag(float mx, float my, float x, float y, bool &skipRest)
{
	scroll_.mouseDrag(mx, my, x, y, skipRest);
}

void GLWListView::clear()
{
	lines_.clear();
	scroll_.setCurrent(0);
}

void GLWListView::addWordEntry(std::string &word, XMLNode *parentNode)
{
	WordEntry wordEntry(word.c_str());
	if (0 == strcmp("b", parentNode->getName()))
	{
		wordEntry.bold_ = true;
	}
	word = "";

	lines_.back().words_.push_back(wordEntry);
}

void GLWListView::addXML(XMLNode *node, float &lineLen)
{
	// For each child XML node
	std::list<XMLNode *>::iterator childrenItor;
	std::list<XMLNode *> children = node->getChildren();
	for (childrenItor = children.begin();
		childrenItor != children.end();
		childrenItor++)
	{
		XMLNode *child = (*childrenItor);

		// Get the child node type
		if (child->getType() == XMLNode::XMLNodeType)
		{
			// Its another node, recurse over its children too
			addXML(child, lineLen);
		}
		else
		{
			// Its a text type, add the words from the text
			std::string word;
			for (const char *t=child->getContent(); *t; t++)
			{
				if (*t == '\n')
				{
					// Add the current word
					addWordEntry(word, node);

					// Add a new line
					LineEntry lineEntry;
					lines_.push_back(lineEntry);
					lineLen = 0.0f;
				}
				else
				{
					word += *t;

					// Check if weve run out of space on the current line
					float wordLen = 
						GLWFont::instance()->getSmallPtFont()->
						getWidth(textSize_, word.c_str());
					if (wordLen + lineLen > w_ - BorderWidth)
					{
						// Add a new line
						LineEntry lineEntry;
						lines_.push_back(lineEntry);
						lineLen = 0.0f;
					}

					// A word break
					if (*t == ' ')
					{
						// Add a new word
						addWordEntry(word, node);
						lineLen += wordLen;
					}
				}
			}

			// Add any words we've got left over
			float wordLen = 
				GLWFont::instance()->getSmallPtFont()->
				getWidth(textSize_, word.c_str());
			addWordEntry(word, node);
			lineLen += wordLen;
		}
	}
}

void GLWListView::addXML(XMLNode *node)
{
	float lineLen = 0.0f;

	// Add a blank line to start with
	LineEntry lineEntry;
	lines_.push_back(lineEntry);

	// Recurse over the document adding the words
	addXML(node, lineLen);

	// Setup the current scroll position
	setScroll();
}

void GLWListView::addLine(const char *text)
{
	// Remove extra lines
	if (maxLen_ > 0)
	{
		if (lines_.size() > (unsigned int) maxLen_) lines_.clear();
	}

	// Generate the line to add (add a single word)
	WordEntry wordEntry(text);
	LineEntry lineEntry;
	lineEntry.words_.push_back(wordEntry);
	lines_.push_back(lineEntry);

	// Setup the current scroll position
	setScroll();
}

void GLWListView::setScroll()
{
	int view = (int) (h_ / (textSize_ + 2.0f));
	scroll_.setMax((int) lines_.size());
	scroll_.setSee(view);
	scroll_.setCurrent(scroll_.getMax() - scroll_.getSee());
}
