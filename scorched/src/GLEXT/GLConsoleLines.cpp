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


// GLConsoleLines.cpp: implementation of the GLConsoleLines class.
//
//////////////////////////////////////////////////////////////////////

#include <GLEXT/GLConsoleLines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

unsigned GLConsoleLine::nextLineNumber_ = 0;

GLConsoleLine::GLConsoleLine() :
	showPointer_(false), lineNumber_(0)
{

}

GLConsoleLine::~GLConsoleLine()
{

}

void GLConsoleLine::set(const char *line, bool showPointer)
{
	line_ = std::string(line);
	showPointer_ = showPointer;
	if (showPointer_)
	{
		lineNumber_ = ++nextLineNumber_;
	}
}

void GLConsoleLine::drawLine(float x, float y, GLFont2d *font)
{
	static Vector color(0.6f, 0.6f, 0.6f);
	if (showPointer_)
	{
		// We show a line number of those lines with commands
		// on them
		font->draw(color, 14, x, y, 0.0f, "%4i : %s", lineNumber_, line_.c_str());
	}
	else
	{
		font->draw(color, 14, x, y, 0.0f,"      %s", line_.c_str());
	}
}

GLConsoleLines::GLConsoleLines(unsigned maxLines) :
	maxLines_(maxLines)
{
	linesIter_ = lines_.rbegin();
	selectIter_ = lines_.rbegin();
}

GLConsoleLines::~GLConsoleLines()
{

}

void GLConsoleLines::clear()
{
	lines_.clear();
	linesIter_ = lines_.rbegin();
	selectIter_ = lines_.rbegin();
}

void GLConsoleLines::scroll(int lines)
{
	// Move the lookat by the direction and number of lines
	while (lines < 0)
	{		
		if (linesIter_ != lines_.rend())
		{
			linesIter_++;
		}
		lines++;
	}
	while (lines > 0)
	{
		if (linesIter_ != lines_.rbegin())
		{
			linesIter_--;
		}
		lines--;
	}
}

bool GLConsoleLines::getNextLine(std::list<GLConsoleLine *>::reverse_iterator &pos)
{
	// Find the next line with an actual command on it
	while (pos != lines_.rend())
	{
		pos ++;
		if (pos == lines_.rend()) return false;
		if ((*pos)->getShowPointer()) return true;
	}

	return false;
}

bool GLConsoleLines::getPrevLine(std::list<GLConsoleLine *>::reverse_iterator &pos)
{
	// Find the prev line with an actual command on it
	while (pos != lines_.rbegin())
	{
		pos --;
		if (pos == lines_.rbegin()) return false;
		if ((*pos)->getShowPointer()) return true;
	}

	return false;
}

const char *GLConsoleLines::getItem(int lines)
{
	// Find the next or previous line with a command on it
	while (lines < 0)
	{		
		std::list<GLConsoleLine *>::reverse_iterator pos = selectIter_;
		if (getNextLine(pos))
		{
			selectIter_ = pos;
		}
		lines++;
	}
	while (lines > 0)
	{
		std::list<GLConsoleLine *>::reverse_iterator pos = selectIter_;
		if (getPrevLine(pos))
		{
			selectIter_ = pos;
		}
		lines--;
	}

	// Set the lookat to lookat the command
	linesIter_ = selectIter_;

	// Check the command is valid and return the line
	if (selectIter_ == lines_.rbegin())
	{
		return "";
	}
	return (*selectIter_)->getLine();
}

void GLConsoleLines::addLine(const char *text, bool showPointer)
{
	const int bufferSize = 80;
	if (strlen(text) < bufferSize)
	{
		addSmallLine(text, showPointer);
	}
	else
	{
		static char buffer[bufferSize + 2];
		int c = 0;
		const char *a;
		for (a=text, c=0; *a; a++, c++)
		{
			buffer[c] = *a;
			if ((c==bufferSize) || (c>bufferSize-15 && *a ==' '))
			{
				buffer[c+1] = '\0';
				c=-1;
				addSmallLine(buffer, showPointer);
			}
		}
		if (buffer[0]) 
		{
			buffer[c+1] = '\0';
			addSmallLine(buffer, showPointer);
		}
	}
}

void GLConsoleLines::addSmallLine(const char *text, bool showPointer)
{
	if (lines_.size() == maxLines_)
	{
		// We need to reuse the lines
		// reuse the first line
		GLConsoleLine *line = lines_.front();
		lines_.pop_front();
		line->set(text, showPointer);
		lines_.push_back(line);
	}
	else
	{
		// Add a new line
		GLConsoleLine *line = new GLConsoleLine;
		line->set(text, showPointer);
		lines_.push_back(line);
	}

	// Set the lookat and cursor to point at the new line
	linesIter_ = lines_.rbegin();
	selectIter_ = lines_.rbegin();
}

void GLConsoleLines::drawLines(GLFont2d *font, float startHeight, float totalHeight, float totalWidth)
{
	if (linesIter_ != lines_.rbegin())
	{
		// Draw arrows at top of screen if stuff off top
		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_TRIANGLES);
			for (float a=100.0f; a<totalWidth - 100.0f; a+=totalWidth / 25.0f)
			{
				glVertex2f(a + 20.0f, startHeight - 4.0f);
				glVertex2f(a + 24.0f, startHeight - 8.0f);
				glVertex2f(a + 28.0f, startHeight - 4.0f);
			}
		glEnd();
	}

	// Draw all of the text
	startHeight += 10.0f;
	std::list<GLConsoleLine *>::reverse_iterator iter;
	{
		// Stops each drawLine() from changing state
		GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);

		for (iter = linesIter_;
			iter != lines_.rend();
			iter++)
		{
			startHeight += 15.0f;
			if (startHeight > totalHeight - 20) break;

			(*iter)->drawLine(20.0f, startHeight, font);
		}
	}

	if (iter != lines_.rend())
	{
		// Draw arrows at the bottom of the screen if stuff off bottom
		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_TRIANGLES);
			for (float a=100.0f; a<totalWidth - 100.0f; a+=totalWidth / 25.0f)
			{
				glVertex2f(a + 24.0f, totalHeight - 4.0f);
				glVertex2f(a + 20.0f, totalHeight - 8.0f);
				glVertex2f(a + 28.0f, totalHeight - 8.0f);
			}
		glEnd();
	}
}
