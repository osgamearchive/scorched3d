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


#ifndef _GLWBUTTON_H_
#define _GLWBUTTON_H_
// GLWButton.h: interface for the GLWButton class.
//
//////////////////////////////////////////////////////////////////////



#include <GLW/GLWVisibleWidget.h>

class GLWButtonI
{
public:
	virtual ~GLWButtonI();

	virtual void buttonDown(unsigned int id) = 0;
};

class GLWButton : public GLWVisibleWidget
{
public:
	GLWButton(float x, float y, float w, float h, GLWButtonI *handler = 0,
			  bool ok = false, bool cancel = false);
	virtual ~GLWButton();

	virtual void draw();	
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);
	bool &getPressed() { return pressed_; }

	virtual void setHandler(GLWButtonI *handler);

METACLASSID

protected:
	GLWButtonI *handler_;
	bool cancel_, ok_;
	bool startdrag_, pressed_;

};

#endif /* _GLWBUTTON_H_ */
