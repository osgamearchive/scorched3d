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

#if !defined(AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_)
#define AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_

#include <list>
#include <GLW/GLWidget.h>

/**
A container widget for other widgets.
Also controls how widgets are layed out.
**/
class GLWPanel : public GLWidget
{
public:
	enum LayoutFlags
	{
		SpaceRight = 1,
		SpaceLeft = 2,
		SpaceTop = 4,
		SpaceBottom = 8,
		AlignLeft = 16,
		AlignRight = 32,
		AlignCenterLeftRight = 64,
		AlignTop = 128,
		AlignBottom = 256,
		AlignCenterTopBottom = 512
	};
	enum LayoutType
	{
		LayoutNone,
		LayoutHorizontal,
		LayoutVerticle,
		LayoutGrid
	};

	struct GLWPanelEntry
	{
		GLWPanelEntry(GLWidget *widget, GLWCondition *con,
			unsigned int flags, float width); 

		GLWidget *widget;
		GLWCondition *condition;
		float leftSpace;
		float rightSpace;
		float topSpace;
		float bottomSpace;
		unsigned flags;
	};

	GLWPanel(float x = 0.0f, float y = 0.0f, 
		float w = 0.0f, float h = 0.0f, 
		bool depressed = false,
		bool visible = true);
	virtual ~GLWPanel();

	virtual void simulate(float frameTime);
	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	virtual bool initFromXML(XMLNode *node);
	virtual void clear();
	virtual void layout();

	virtual void setLayout(unsigned int layout);
	virtual unsigned int getLayout();
	virtual void setGridWidth(unsigned int grid);
	virtual unsigned int getGridWidth();

	GLWidget *addWidget(GLWidget *widget, GLWCondition *condition = 0, 
		unsigned int flags = AlignLeft | AlignTop, 
		float width = 0.0f);
	std::list<GLWPanelEntry> &getWidgets() { return widgets_; }

	REGISTER_CLASS_HEADER(GLWPanel);

	// Accessors
	bool &getDepressed() { return depressed_; }
	bool &getDrawPanel() { return drawPanel_; }

protected:
	std::list<GLWPanelEntry> widgets_;
	bool depressed_;
	bool drawPanel_;
	unsigned int layout_;
	unsigned int gridWidth_;

};

#endif // !defined(AFX_GLWPANEL_H__6619410E_0A6B_459B_8A38_11024F49A6E3__INCLUDED_)
