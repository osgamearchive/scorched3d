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


// GLWVisiblePanel.h: interface for the GLWVisiblePanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWVISIBLEPANEL_H__813A99E5_8077_4BFD_9C49_81CF68526962__INCLUDED_)
#define AFX_GLWVISIBLEPANEL_H__813A99E5_8077_4BFD_9C49_81CF68526962__INCLUDED_


#include <GLW/GLWVisibleWidget.h>
#include <GLW/GLWPanel.h>

class GLWVisiblePanel : public GLWVisibleWidget
{
public:
	GLWVisiblePanel(float x, float y, float w, float h, bool depressed = false);
	virtual ~GLWVisiblePanel();

	virtual void simulate(float frameTime);
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	virtual void clear() { panel_.clear(); }

	GLWidget *addWidget(GLWVisibleWidget *widget) { return panel_.addWidget(widget); }
	std::list<GLWidget *> &getWidgets() { return panel_.getWidgets(); }

	GLWPanel &getPanel() { return panel_; }
	bool &getDepressed() { return depressed_; }
	bool &getDrawPanel() { return drawPanel_; }

METACLASSID

protected:
	bool depressed_;
	bool drawPanel_;
	GLWPanel panel_;

};

#endif // !defined(AFX_GLWVISIBLEPANEL_H__813A99E5_8077_4BFD_9C49_81CF68526962__INCLUDED_)
