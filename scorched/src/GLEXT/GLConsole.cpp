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


// GLConsole.cpp: implementation of the GLConsole class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Keyboard.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLConsole.h>
#include <GLW/GLWFont.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLConsole *GLConsole::instance_ = 0;

GLConsole *GLConsole::instance()
{
	if (!instance_)
	{
		instance_ = new GLConsole;
	}
	return instance_;
}

GLConsole::GLConsole() : 
	height_(0.0f), opening_(false), lines_(1000), 
	methods_(rules_, lines_), showCursor_(true)
{
	font_ = GLWFont::instance()->getFont();

	// Some default stuff
	lines_.addLine("GL_VENDOR:", false);
	lines_.addLine((const char *) glGetString(GL_VENDOR), false);
	lines_.addLine("GL_RENDERER:", false);
	lines_.addLine((const char *) glGetString(GL_RENDERER), false);
	lines_.addLine("GL_VERSION:", false);
	lines_.addLine((const char *) glGetString(GL_VERSION), false);
	lines_.addLine("GL_EXTENSIONS:", false);
	lines_.addLine((const char *) glGetString(GL_EXTENSIONS), false);
}

GLConsole::~GLConsole()
{

}

void GLConsole::keyboardCheck(const unsigned state, float frameTime, 
							   char *buffer, unsigned int keyState,
							   KeyboardHistory::HistoryElement *history, int hisCount, 
							   bool &skipRest)
{
	// Use the history to ensure that no keystrokes are missed
	// regardless of the framerate.
	KEYBOARDKEY("CONSOLE", consoleKey);
	for (int i=0; i<hisCount; i++)
	{
		unsigned int dik = history[i].sdlKey;
		if (consoleKey->keyMatch(dik))
		{
			if (currentLine_.empty()) opening_ = !opening_;
			else currentLine_ = "";
			skipRest = true;
		}
	}

	if (height_ > 0.0f)
	{
		// Use the history to ensure that no keystrokes are missed
		// regardless of the framerate.
		if (!skipRest)
		for (int i=0; i<hisCount; i++)
		{
			char c = history[i].representedKey;
			DWORD dik = history[i].sdlKey;

			if (c >= ' ')
			{
				currentLine_ += c;
			}
			else 
			{
				switch (dik)
				{
				case SDLK_TAB:
					{
						std::list<GLConsoleRule *> matches;
						const char *result = rules_.matchRule(currentLine_.c_str(), matches);
						if (result) currentLine_ = result;
						if (matches.size() > 1)
						{
							addLine(false, "-------------------");
							std::list<GLConsoleRule *>::iterator itor;
							for (itor = matches.begin();
								itor != matches.end();
								itor++)
							{
								addLine(false, (*itor)->getName());
							}
						}
					}
					break;
				case SDLK_BACKSPACE:
				case SDLK_DELETE:
					currentLine_ = currentLine_.substr(0, currentLine_.length() - 1);
					break;
				case SDLK_RETURN:
					if (!currentLine_.empty())
					{
						parseLine(currentLine_.c_str());
						currentLine_.erase();
					}	
					break;
				case SDLK_UP:
					currentLine_ = lines_.getItem(-1);
					break;
				case SDLK_DOWN:
					currentLine_ = lines_.getItem(+1);
					break;
				case SDLK_PAGEUP:
					lines_.scroll(-5);
					break;
				case SDLK_PAGEDOWN:
					lines_.scroll(+5);
					break;				
				}
			}
		}
		skipRest = true;
	}
}

void GLConsole::simulate(const unsigned state, float frameTime)
{
	const GLfloat dropSpeed = 600.0f;
	if (opening_)
	{
		height_ += frameTime * dropSpeed;
	}
	else
	{
		height_ -= frameTime * dropSpeed;
		if (height_ < 0.0f) height_ = 0.0f;
	}

	static float ctime = 0;
	ctime += frameTime;
	if (ctime > 0.5f)
	{
		ctime = 0.0f;
		showCursor_ = !showCursor_;
	}
}

void GLConsole::draw(const unsigned state)
{
	if (height_ <= 0.0f) return;

	GLState currentState(GLState::DEPTH_OFF | GLState::TEXTURE_OFF | GLState::BLEND_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLfloat width = (GLfloat) viewport[2];
	GLfloat top = (GLfloat) viewport[3];

	drawBackdrop(width, top);
	drawText(width, top);
}

void GLConsole::drawBackdrop(float width, float top)
{
	if (height_ > top * .75f) height_ = top * .75f;

	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
	glBegin(GL_QUADS);
		glVertex2f(0.0f, top - height_ + 10.0f);
		glVertex2f(width, top - height_ + 10.0f);
		glVertex2f(width, top);
		glVertex2f(0.0f, top);

		glVertex2f(10.0f, top - height_);
		glVertex2f(width - 10.0f, top - height_);
		glVertex2f(width, top - height_ + 10.0f);
		glVertex2f(0.0f, top - height_ + 10.0f);
	glEnd();

	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0.0f, top - height_ + 10.0f);
		glVertex2f(10.0f, top - height_);
		glVertex2f(width - 10.0f, top - height_);
		glVertex2f(width, top - height_ + 10.0f);
	glEnd();
}

void GLConsole::drawText(float width, float top)
{
	static Vector color(0.8f, 0.8f, 0.8f);
	font_->draw(color, 16,
		10.0f, top - (height_ - 14.0f), 0.0f, "> %s%c", 
		currentLine_.c_str(),
		(showCursor_?'_':' '));

	lines_.drawLines(font_, top - (height_ - 14.0f), top, width);
}

void GLConsole::parseLine(const char *line)
{
	std::list<std::string> linesToAdd;
	std::string result;
	rules_.addLine(line, result, linesToAdd);

	lines_.addLine(result.c_str(), true);
	std::list<std::string>::iterator iter;
	for (iter = linesToAdd.begin();
		iter != linesToAdd.end();
		iter++)
	{
		lines_.addLine(iter->c_str(), false);
	}
}

void GLConsole::addLine(bool parse, const char *fmt, ...)
{
	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	if (parse)
	{
		parseLine(text);
	}
	else
	{
		lines_.addLine(text, false);
	}
}

bool GLConsole::addFunction(const char *name, 
							GLConsoleRuleFnI *user, 
							GLConsoleRuleType type, 
							GLConsoleRuleAccessType access)
{
	GLConsoleRuleFn *rule = new GLConsoleRuleFn(name, user, type, access);
	if (!rules_.addRule(rule))
	{
		delete rule;
		return false;
	}
	return true;
}

bool GLConsole::addMethod(const char *name,
						  GLConsoleRuleMethodI *user)
{
	GLConsoleRuleMethod *rule = new GLConsoleRuleMethod(name, user);
	if (!rules_.addRule(rule))
	{
		delete rule;
		return false;
	}
	return true;
}
