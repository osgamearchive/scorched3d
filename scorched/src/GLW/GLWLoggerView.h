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

#if !defined(__INCLUDE_GLWLoggerViewh_INCLUDE__)
#define __INCLUDE_GLWLoggerViewh_INCLUDE__

#include <GLW/GLWidget.h>
#include <common/LoggerI.h>

class GLWLoggerViewEntry
{
public:
	LoggerInfo info;
	float timeRemaining;
};

class GLWLoggerView : 
	public GLWidget,
	public LoggerI
{
public:
	GLWLoggerView();
	virtual ~GLWLoggerView();

	// GLWidget
	virtual void draw();
	virtual void simulate(float frameTime);
	virtual bool initFromXML(XMLNode *node);
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	// LoggerI
	virtual void logMessage(LoggerInfo &info);

	REGISTER_CLASS_HEADER(GLWLoggerView);

protected:
	int mask_;
	bool oldStyle_;
	bool init_;
	bool alignTop_;
	bool parentSized_;
	int lineDepth_;
	float displayTime_;
	float fontSize_, outlineFontSize_;

	int totalLines_;
	int startLine_, usedLines_;
	GLWLoggerViewEntry *textLines_;
};

#endif // __INCLUDE_GLWLoggerViewh_INCLUDE__
