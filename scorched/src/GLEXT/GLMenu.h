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

#if !defined(AFX_GLMENU_H__ED2E8B2C_46B3_400F_A3E2_FC1B53732D15__INCLUDED_)
#define AFX_GLMENU_H__ED2E8B2C_46B3_400F_A3E2_FC1B53732D15__INCLUDED_

#include <map>
#include <GLEXT/GLMenuEntry.h>
#include <GLW/GLWWindow.h>

class GLMenu : public GLWWindow
{
public:
	GLMenu();
	virtual ~GLMenu();

	bool addMenu(char *menuName, float width, unsigned int state,
		GLMenuI *selectFn,
		GLMenuI *textFn = 0, 
		GLMenuI *subMenuFn = 0,
		GLMenuI *enabledFn = 0);
	bool addMenuItem(char *menuName, const GLMenuItem item);

	virtual void draw();
	virtual void mouseDown(float x, float y, bool &skipRest);
	virtual void mouseUp(float x, float y, bool &skipRest);
	virtual void mouseDrag(float mx, float my, float x, float y, bool &skipRest);
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

protected:
	std::map<std::string, GLMenuEntry *> menuList_;

	GLMenuEntry *getMenu(char *menuItem);

};

#endif // !defined(AFX_GLMENU_H__ED2E8B2C_46B3_400F_A3E2_FC1B53732D15__INCLUDED_)
