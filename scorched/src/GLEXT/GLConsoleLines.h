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


// GLConsoleLines.h: interface for the GLConsoleLines class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCONSOLELINES_H__2DA65C94_3E52_43C6_B75B_D0CEDBF6A9DE__INCLUDED_)
#define AFX_GLCONSOLELINES_H__2DA65C94_3E52_43C6_B75B_D0CEDBF6A9DE__INCLUDED_

#pragma warning(disable: 4786)


#include <GLEXT/GLFont2d.h>
#include <list>
#include <string>

class GLConsoleLine
{
public:
	GLConsoleLine();
	virtual ~GLConsoleLine();

	void set(const char *line, bool showPointer);
	void drawLine(float x, float y, GLFont2d *font);

	bool getShowPointer() { return showPointer_; }
	const char *getLine() { return line_.c_str(); }

protected:
	bool showPointer_;
	std::string line_;
	static unsigned nextLineNumber_;
	unsigned lineNumber_;

};

class GLConsoleLines  
{
public:
	GLConsoleLines(unsigned maxLines);
	virtual ~GLConsoleLines();

	void addLine(const char *line, bool showPointer);
	void drawLines(GLFont2d *font, float startHeight, float totalHeight, float totalWidth);

	void clear();

	void scroll(int lines);
	const char *getItem(int lines);

	std::list<GLConsoleLine *> &getLines() { return lines_; }

protected:
	std::list<GLConsoleLine *> lines_;
	std::list<GLConsoleLine *>::reverse_iterator linesIter_;
	std::list<GLConsoleLine *>::reverse_iterator selectIter_;
	unsigned maxLines_;

	void addSmallLine(const char *line, bool showPointer);
	bool getNextLine(std::list<GLConsoleLine *>::reverse_iterator &pos);
	bool getPrevLine(std::list<GLConsoleLine *>::reverse_iterator &pos);

};

#endif // !defined(AFX_GLCONSOLELINES_H__2DA65C94_3E52_43C6_B75B_D0CEDBF6A9DE__INCLUDED_)
