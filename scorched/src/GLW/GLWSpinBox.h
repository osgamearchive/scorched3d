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


#ifndef _GLWSPINBOX_H_
#define _GLWSPINBOX_H_
// GLWSpinBox.h: interface for the GLWSpinBox class.
//
//////////////////////////////////////////////////////////////////////



#include <GLW/GLWButton.h>

class GLWSpinBox : public GLWVisibleWidget, 
					public GLWButtonI
{
public:
	GLWSpinBox(float x, float y, float w, 
		int start, int minRange, int maxRange, int step=1);
	virtual ~GLWSpinBox();

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, int bufferCount, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	virtual void buttonDown(unsigned int id);
	int getValue() { return value_; }

METACLASSID

protected:
	bool keyDown_;
	bool dragging_;
	int step_;
	int value_;
	int minRange_, maxRange_;
	GLWButton top_, bottom_;

};

#endif /* _GLWSPINBOX_H_ */
