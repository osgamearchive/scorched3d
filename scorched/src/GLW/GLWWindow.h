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

#if !defined(AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_)
#define AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_

#include <string>
#include <GLW/GLWVisiblePanel.h>
#include <GLW/GLWToolTip.h>
#include <GLEXT/GLTexture.h>

class GLWWindow : public GLWVisiblePanel
{
public:
	enum PossibleStates
	{
		eNoTitle = 1,
		eSmallTitle = 2,
		eTransparent = 4,
		eResizeable = 8,
		eCircle = 16
	};

	GLWWindow(const char *name, float x, float y, 
		float w, float h,
		unsigned int states, 
		const char *description);
	GLWWindow(const char *name, float w, float h,
		unsigned int states,
		const char *description);
	virtual ~GLWWindow();

	virtual void windowDisplay();
	virtual void windowHide();
	virtual void windowInit(const unsigned state);
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	const char *getName() { return name_.c_str(); }
	const char *getDescription() { return description_.c_str(); }
	GLWTip &getToolTip() { return toolTip_; }
	void needsCentered() { needCentered_ = true; }

METACLASSID

protected:
	enum
	{
		NoDrag,
		TitleDrag,
		SizeDrag
	} dragging_;

	GLWTip toolTip_;
	GLTexture moveTexture_;
	bool showTitle_;
	bool needCentered_;
	bool disabled_;
	unsigned int windowState_;
	float maxWindowSize_;
	std::string name_;
	std::string description_;

	virtual void drawWindowCircle(float x, float y, float w, float h);
	virtual void drawOutlinePoints(float x, float y, float w, float h);
	virtual void drawBackSurface(float x, float y, float w, float h);
	virtual void drawTitleBar(float x, float y, float w, float h);
	virtual void drawSurround(float x, float y, float w, float h);
	virtual void drawMaximizedWindow();
	virtual void drawInfoBox(float x, float y, float w);
	virtual void drawJoin(float x, float y);
	virtual void drawIconBox(float x, float y);

};

#endif // !defined(AFX_GLWWINDOW_H__DF296D0F_BC67_4A40_B8F9_3B70E8AC1F65__INCLUDED_)
