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


// GLWPanel.h: interface for the GLWPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_)
#define AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_

#pragma warning(disable: 4786)


#include <list>
#include <GLW/GLWidget.h>

class GLWPanel : public GLWidget
{
public:
	GLWPanel();
	virtual ~GLWPanel();

	virtual void simulate(float frameTime);
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	virtual void clear();

	GLWidget *addWidget(GLWidget *widget);
	std::list<GLWidget *> &getWidgets() { return widgets_; }

METACLASSID

protected:
	std::list<GLWidget *> widgets_;

};

#endif // !defined(AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_)
