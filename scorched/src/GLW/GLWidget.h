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


// GLWidget.h: interface for the GLWidget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLWIDGET_H__3F7BC394_576B_4ADF_8771_7D97EB3AF314__INCLUDED_)
#define AFX_GLWIDGET_H__3F7BC394_576B_4ADF_8771_7D97EB3AF314__INCLUDED_

#include <GLEXT/GLState.h>
#include <common/KeyboardHistory.h>

#define METACLASSID virtual unsigned int getMetaClassId() \
	{ static unsigned int metaClassID = nextMetaClassId_++; return metaClassID; }

class GLWPanel;
class GLWidget
{
public:
	GLWidget();
	virtual ~GLWidget();

	virtual void setParent(GLWPanel *parent);

	virtual void draw() = 0;
	virtual void simulate(float frameTime);
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	unsigned int getId() { return id_; }
	virtual unsigned int getMetaClassId() = 0;

protected:
	static unsigned int nextMetaClassId_;
	static unsigned int nextId_;
	unsigned int id_;
	GLWPanel *parent_;

};

#endif // !defined(AFX_GLWIDGET_H__3F7BC394_576B_4ADF_8771_7D97EB3AF314__INCLUDED_)
