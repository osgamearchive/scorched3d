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


// GLWWindow.h: interface for the GLWWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_)
#define AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_

#include <string>
#include <GLW/GLWVisiblePanel.h>

class GLWWindow : public GLWVisiblePanel
{
public:
	enum PossibleStates
	{
		eNoTitle = 1,
		eSmallTitle = 2,
		eTransparent = 4,
		eResizeable = 8
	};

	GLWWindow(const char *name, float x, float y, 
		float w, float h,
		unsigned int states);
	GLWWindow(const char *name, float w, float h,
		unsigned int states);
	virtual ~GLWWindow();

	virtual void windowDisplay();
	virtual void windowInit(const unsigned state);
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	const char *getName() { return name_.c_str(); }
	void needsCentered() { needCentered_ = true; }

METACLASSID

protected:
	enum
	{
		NoDrag,
		TitleDrag,
		SizeDrag
	} dragging_;

	bool showTitle_;
	bool needCentered_;
	bool disabled_;
	unsigned int windowState_;
	std::string name_;

	virtual void drawOutlinePoints(float x, float y, float w, float h);

	virtual void drawBackSurface(float x, float y, float w, float h);
	virtual void drawTitleBar(float x, float y, float w, float h);
	virtual void drawSurround(float x, float y, float w, float h);
	virtual void drawMaximizedWindow();

};

#endif // !defined(AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_)
